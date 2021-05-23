package frc.sim;

import frc.sim.physics.Force2d;
import frc.sim.physics.Vector2d;
import frc.sim.physics.ForceAtPose2d;

import java.util.ArrayList;

import edu.wpi.first.wpilibj.RobotController;
import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Transform2d;
import edu.wpi.first.wpilibj.geometry.Translation2d;
import edu.wpi.first.wpilibj.geometry.Twist2d;
import edu.wpi.first.wpilibj.smartdashboard.Field2d;
import frc.Constants;
import frc.PoseTelemetry;

class DrivetrainModel {

    ArrayList<SwerveModuleModel> modules = new ArrayList<SwerveModuleModel>();

    SimGyroSensorModel gyro;
    SimVisionModel vision;

    Field2d field;
    Pose2d dtPoseForTelemetry;
    Pose2d endRobotRefFrame; //Previous sent pose, allows us to detect when the robot has been manually moved.

    Vector2d accel_prev = new Vector2d();
    Vector2d vel_prev   = new Vector2d();
    double   rotAccel_prev = 0;
    double   rotVel_prev   = 0;

    public DrivetrainModel(){

        modules.add(new SwerveModuleModel(Constants.FL_WHEEL_MOTOR_IDX,Constants.FL_AZMTH_MOTOR_IDX,Constants.FL_WHEEL_ENC_A_IDX,Constants.FL_AZMTH_ENC_A_IDX));
        modules.add(new SwerveModuleModel(Constants.FR_WHEEL_MOTOR_IDX,Constants.FR_AZMTH_MOTOR_IDX,Constants.FR_WHEEL_ENC_A_IDX,Constants.FR_AZMTH_ENC_A_IDX));
        modules.add(new SwerveModuleModel(Constants.BL_WHEEL_MOTOR_IDX,Constants.BL_AZMTH_MOTOR_IDX,Constants.BL_WHEEL_ENC_A_IDX,Constants.BL_AZMTH_ENC_A_IDX));
        modules.add(new SwerveModuleModel(Constants.BR_WHEEL_MOTOR_IDX,Constants.BR_AZMTH_MOTOR_IDX,Constants.BR_WHEEL_ENC_A_IDX,Constants.BR_AZMTH_ENC_A_IDX));

        gyro = new SimGyroSensorModel();
        vision = new SimVisionModel();

        field = PoseTelemetry.field;
        field.setRobotPose(Constants.DFLT_START_POSE);

        dtPoseForTelemetry = new Pose2d();
    }

    public void modelReset(Pose2d pose){
        field.setRobotPose(pose);
        accel_prev = new Vector2d();
        vel_prev   = new Vector2d();
        rotAccel_prev = 0;
        rotVel_prev   = 0;
        for(int idx = 0; idx < Constants.NUM_MODULES; idx++){
            modules.get(idx).reset(pose.transformBy(Constants.robotToModuleTF.get(idx)));
        }
        gyro.resetToPose(pose);
    }

    public void update(boolean isDisabled, double batteryVoltage){

        Pose2d fieldReferenceFrame = new Pose2d();// global origin
        Pose2d startRobotRefFrame = field.getRobotPose(); //origin on and aligned to robot's present position in the field
        Transform2d fieldToRobotTrans = new Transform2d(fieldReferenceFrame, startRobotRefFrame);

        ////////////////////////////////////////////////////////////////
        // Handle model state reset conditions

        if(!startRobotRefFrame.equals(endRobotRefFrame)){
            //Robot has been moved manually in the Field2D widget. Reset the sim model.
            endRobotRefFrame = startRobotRefFrame;
            modelReset(startRobotRefFrame);
        }

        ////////////////////////////////////////////////////////////////
        // Component-Force Calculations to populate the free-body diagram

        // Calculate each module's new position, and step it through simulation.
        for(int idx = 0; idx < Constants.NUM_MODULES; idx++){
            Pose2d modPose = fieldReferenceFrame.transformBy(fieldToRobotTrans).transformBy(Constants.robotToModuleTF.get(idx));
            modules.get(idx).setModulePose(modPose);
            modules.get(idx).update(isDisabled, batteryVoltage);
        }

        // Force on frame from wheel motive forces (along-tread)
        ArrayList<ForceAtPose2d> wheelMotiveForces = new ArrayList<ForceAtPose2d>(Constants.NUM_MODULES);
        for(int idx = 0; idx < Constants.NUM_MODULES; idx++){
            wheelMotiveForces.add(modules.get(idx).getWheelMotiveForce());
        }

        // First half of the somewhat-dubious friction model
        Force2d preFricNetForce = new Force2d();
        wheelMotiveForces.forEach((ForceAtPose2d mf) ->{
            preFricNetForce.accum(mf.getForceInRefFrame(startRobotRefFrame)); //Add up all the forces that friction gets a chance to fight against
        });

        Force2d sidekickForce = new Force2d(0, 0);
        if(RobotController.getUserButton()){
            //Kick the robot to the side
            sidekickForce = new Force2d(0, 700);
        }

        preFricNetForce.accum(sidekickForce);

        ForceAtPose2d preFricNetForceRobotCenter = new ForceAtPose2d(preFricNetForce, startRobotRefFrame);

        // Calculate the forces from cross-tread friction at each module
        ArrayList<ForceAtPose2d> netXtreadFricForces = new ArrayList<ForceAtPose2d>(Constants.NUM_MODULES);
        for(int idx = 0; idx < Constants.NUM_MODULES; idx++){
            SwerveModuleModel mod = modules.get(idx);
            double perWheelForceFrac = 1.0/Constants.NUM_MODULES; //Assume force evenly applied to all modules.
            Force2d preFricForceAtModule = preFricNetForceRobotCenter.getForceInRefFrame(mod.getModulePose()).times(perWheelForceFrac);
            netXtreadFricForces.add(mod.getCrossTreadFrictionalForce(preFricForceAtModule));
        }

        ////////////////////////////////////////////////////////////////
        // Combine forces in free-body diagram

        // Using all the above force components, do Sum of Forces
        Force2d forceOnRobotCenter = preFricNetForce;

        netXtreadFricForces.forEach((ForceAtPose2d f) -> {
            forceOnRobotCenter.accum(f.getForceInRefFrame(startRobotRefFrame));
        });
        
        ForceAtPose2d netForce = new ForceAtPose2d(forceOnRobotCenter, startRobotRefFrame);

        Force2d robotForceInFieldRefFrame = netForce.getForceInRefFrame(fieldReferenceFrame);

        robotForceInFieldRefFrame.accum(getWallCollisionForce(startRobotRefFrame));


        //Sum of Torques
        double netTorque = 0;

        for(int idx = 0; idx < Constants.NUM_MODULES; idx++){
            netTorque += wheelMotiveForces.get(idx).getTorque(startRobotRefFrame);
            netTorque += netXtreadFricForces.get(idx).getTorque(startRobotRefFrame);
        }


        ////////////////////////////////////////////////////////////////
        // Apply Newton's 2nd law to get motion from forces

        //a = F/m in field frame
        Vector2d accel = robotForceInFieldRefFrame.times(1/Constants.ROBOT_MASS_kg).vec;

        Vector2d velocity = new Vector2d( vel_prev.x + (accel.x + accel_prev.x)/2 * Constants.SIM_SAMPLE_RATE_SEC, //Trapezoidal integration
                                          vel_prev.y + (accel.y + accel_prev.y)/2 * Constants.SIM_SAMPLE_RATE_SEC);

        Translation2d posChange = new Translation2d( (velocity.x + vel_prev.x)/2 * Constants.SIM_SAMPLE_RATE_SEC, //Trapezoidal integration
                                                     (velocity.y + vel_prev.y)/2 * Constants.SIM_SAMPLE_RATE_SEC);
        
        vel_prev = velocity;
        accel_prev = accel;
        
        //alpha = T/I in field frame
        double rotAccel = netTorque / Constants.ROBOT_MOI_KGM2;
        double rotVel = rotVel_prev + (rotAccel + rotAccel_prev)/2 * Constants.SIM_SAMPLE_RATE_SEC;
        double rotPosChange = (rotVel + rotVel_prev)/2 * Constants.SIM_SAMPLE_RATE_SEC;

        rotVel_prev = rotVel;
        rotAccel_prev = rotAccel;

        posChange = posChange.rotateBy(startRobotRefFrame.getRotation().unaryMinus()); //Twist needs to be relative to robot reference frame

        Twist2d motionThisLoop = new Twist2d(posChange.getX(), posChange.getY(), rotPosChange);
        
        endRobotRefFrame = startRobotRefFrame.exp(motionThisLoop);


        field.setRobotPose(endRobotRefFrame);

        gyro.update(endRobotRefFrame, startRobotRefFrame);
        vision.update(endRobotRefFrame);

        dtPoseForTelemetry = endRobotRefFrame;
    }

    public double getCurrentDraw(){
        double retVal = 0;
        for(int idx = 0; idx < Constants.NUM_MODULES; idx++){
            retVal += modules.get(idx).getCurrentDraw_A();
        }
        return retVal;
    }

    // Very rough approximation of bumpers wacking into a wall.
    // Assumes wall is a very peculiar form of squishy.
    public Force2d getWallCollisionForce(Pose2d pos_in){
        final double WALL_PUSHY_FORCE_N = 5000; 

        Force2d netForce_in = new Force2d();

        if(pos_in.getX() > Constants.MAX_ROBOT_TRANSLATION.getX()){
            //Too far in the positive X direction
            netForce_in = new Force2d(-WALL_PUSHY_FORCE_N, 0);
        }else if(pos_in.getX() < Constants.MIN_ROBOT_TRANSLATION.getX()){
            //Too far in the negative X direction
            netForce_in = new Force2d(WALL_PUSHY_FORCE_N, 0);
        }

        if(pos_in.getY() > Constants.MAX_ROBOT_TRANSLATION.getY()){
            //Too far in the positive Y direction
            netForce_in = new Force2d(0, -WALL_PUSHY_FORCE_N);
        }else if(pos_in.getY() < Constants.MIN_ROBOT_TRANSLATION.getY()){
            //Too far in the negative Y direction
            netForce_in = new Force2d(0, WALL_PUSHY_FORCE_N);
        }

        return netForce_in;
    }

}