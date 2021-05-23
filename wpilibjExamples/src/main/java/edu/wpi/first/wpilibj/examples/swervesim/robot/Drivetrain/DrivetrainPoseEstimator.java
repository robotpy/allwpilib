
package frc.robot.Drivetrain;

import org.photonvision.PhotonCamera;

import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.estimator.SwerveDrivePoseEstimator;
import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Rotation2d;
import edu.wpi.first.wpilibj.geometry.Transform2d;
import edu.wpi.first.wpilibj.kinematics.SwerveModuleState;
import edu.wpi.first.wpilibj.util.Units;
import edu.wpi.first.wpiutil.math.VecBuilder;
import frc.Constants;

public class DrivetrainPoseEstimator {

    /* Singleton infrastructure */
    private static DrivetrainPoseEstimator instance;

    public static DrivetrainPoseEstimator getInstance() {
        if (instance == null) {
            instance = new DrivetrainPoseEstimator();
        }
        return instance;
    }

    Pose2d curEstPose = new Pose2d(Constants.DFLT_START_POSE.getTranslation(), Constants.DFLT_START_POSE.getRotation());

    Pose2d fieldPose = new Pose2d(); // Field-referenced orign

    Pose2d visionEstPose = null; // Camera-reported raw pose. null if no pose available.

    PhotonCamera cam;

    WrapperedADXRS450 gyro;

    boolean pointedDownfield = false;

    SwerveDrivePoseEstimator m_poseEstimator;

    double curSpeed = 0;

    private DrivetrainPoseEstimator() {
        gyro = new WrapperedADXRS450();
        cam = new PhotonCamera(Constants.PHOTON_CAM_NAME);

        // Trustworthiness of the internal model of how motors should be moving
        // Measured in expected standard deviation (meters of position and degrees of
        // rotation)
        var stateStdDevs = VecBuilder.fill(0.05, 0.05, Units.degreesToRadians(5));

        // Trustworthiness of gyro in radians of standard deviation.
        var localMeasurementStdDevs = VecBuilder.fill(Units.degreesToRadians(0.1));

        // Trustworthiness of the vision system
        // Measured in expected standard deviation (meters of position and degrees of
        // rotation)
        var visionMeasurementStdDevs = VecBuilder.fill(0.01, 0.01, Units.degreesToRadians(0.1));

        m_poseEstimator = new SwerveDrivePoseEstimator(getGyroHeading(), Constants.DFLT_START_POSE,
                Constants.m_kinematics, stateStdDevs, localMeasurementStdDevs, visionMeasurementStdDevs,
                Constants.CTRLS_SAMPLE_RATE_SEC);

        setKnownPose(Constants.DFLT_START_POSE);

    }

    /**
     * Snap update the estimator to a known pose.
     * 
     * @param in known pose
     */
    public void setKnownPose(Pose2d in) {
        DrivetrainControl.getInstance().resetWheelEncoders();
        // No need to reset gyro, pose estimator does that.
        m_poseEstimator.resetPosition(in, getGyroHeading());
        updateDownfieldFlag();
        curEstPose = in;
    }

    public Pose2d getEstPose() {
        return curEstPose;
    }

    public Pose2d getVisionEstPose() {
        return visionEstPose;
    }

    public void update() {

        // Based on gyro and measured module speeds and positions, estimate where our
        // robot should have moved to.
        SwerveModuleState[] states = DrivetrainControl.getInstance().getModuleActualStates();
        Pose2d prevEstPose = curEstPose;
        curEstPose = m_poseEstimator.update(getGyroHeading(), states[0], states[1], states[2], states[3]);

        // If we see a vision target, adjust our pose estimate
        var res = cam.getLatestResult();
        if (res.hasTargets()) {

            double observationTime = Timer.getFPGATimestamp() - res.getLatencyMillis();

            Transform2d camToTargetTrans = res.getBestTarget().getCameraToTarget();
            Pose2d targetPose = fieldPose.transformBy(
                    pointedDownfield ? Constants.fieldToFarVisionTargetTrans : Constants.fieldToCloseVisionTargetTrans);
            Pose2d camPose = targetPose.transformBy(camToTargetTrans.inverse());
            visionEstPose = camPose.transformBy(Constants.robotToCameraTrans.inverse());

            m_poseEstimator.addVisionMeasurement(visionEstPose, observationTime);
        } else {
            visionEstPose = null;
        }

        // Calculate a "speedometer" velocity in ft/sec
        Transform2d deltaPose = new Transform2d(prevEstPose, curEstPose);
        curSpeed = Units.metersToFeet(deltaPose.getTranslation().getNorm()) / Constants.CTRLS_SAMPLE_RATE_SEC;

        updateDownfieldFlag();
    }

    public Rotation2d getGyroHeading() {
        return gyro.getAngle().times(-1.0);
    }

    public double getSpeedFtpSec() {
        return curSpeed;
    }

    public void updateDownfieldFlag() {
        double curRotDeg = curEstPose.getRotation().getDegrees();
        pointedDownfield = (curRotDeg > -90 && curRotDeg < 90);
    }

}