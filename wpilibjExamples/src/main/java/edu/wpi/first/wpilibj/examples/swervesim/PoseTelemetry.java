package frc;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.smartdashboard.Field2d;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

public class PoseTelemetry {

    public static Field2d field = new Field2d();

    // Desired Position says where path planning logic wants the
    // robot to be at any given time.
    Pose2d desiredPose = new Pose2d();

    // Estimated position says where you think your robot is at
    // Based on encoders, motion, vision, etc.
    Pose2d estimatedPose = new Pose2d();

    // Actual position defines wherever the robot is actually at
    // at any time. It is unknowable in real life. The simulation
    // generates this as its primary output.
    Pose2d actualPose = new Pose2d();

    public PoseTelemetry() {

        SmartDashboard.putData("Field", field);

    }

    public void setActualPose(Pose2d act) {
        actualPose = act;
    }

    public void setDesiredPose(Pose2d des) {
        desiredPose = des;
    }

    public void setEstimatedPose(Pose2d est) {
        estimatedPose = est;
    }

    public void update(double time) {

        field.getObject("DesPose").setPose(desiredPose);
        field.getObject("Robot").setPose(actualPose);
        field.getObject("EstPose").setPose(estimatedPose);
    }

}