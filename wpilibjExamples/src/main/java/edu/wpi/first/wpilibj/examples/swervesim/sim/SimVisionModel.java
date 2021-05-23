package frc.sim;

import org.photonvision.SimVisionSystem;
import org.photonvision.SimVisionTarget;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Transform2d;
import edu.wpi.first.wpilibj.util.Units;
import frc.Constants;

public class SimVisionModel{

    SimVisionSystem simVision;

    Pose2d fieldOrigin = new Pose2d();
    Pose2d closeTargetPose = fieldOrigin.transformBy(Constants.fieldToCloseVisionTargetTrans);
    Pose2d farTargetPose = fieldOrigin.transformBy(Constants.fieldToFarVisionTargetTrans);

    final double MAX_CAM_DISTANCE_M = Units.feetToMeters(20);
    final double CAM_DIAG_FOV_DEG = 79.0;

    public SimVisionModel(){
        String camName = Constants.PHOTON_CAM_NAME;
        double cameDiagFOV = CAM_DIAG_FOV_DEG; // degrees
        double camPitch = 10.0; // degrees
        Transform2d cameraToRobot = Constants.robotToCameraTrans.inverse();
        double camHeightOffGround = 0.85; // meters
        double maxLEDRange = MAX_CAM_DISTANCE_M; 
        int camResolutionWidth = 480;     // pixels
        int camResolutionHeight = 640;    // pixels
        double minTargetArea = 10;        // square pixels
        
        simVision = new SimVisionSystem(camName,
                                        cameDiagFOV,
                                        camPitch,
                                        cameraToRobot,
                                        camHeightOffGround,
                                        maxLEDRange,
                                        camResolutionWidth,
                                        camResolutionHeight,
                                        minTargetArea);    

        double targetHeightAboveGround = 2.3; // meters
        double targetWidth = 0.54;            // meters
        double targetHeight = 0.25;           // meters

        var newTgtClose = new SimVisionTarget(closeTargetPose,targetHeightAboveGround,targetWidth,targetHeight);
        var newTgtFar = new SimVisionTarget(farTargetPose,targetHeightAboveGround,targetWidth,targetHeight);

        simVision.addSimVisionTarget(newTgtClose);
        simVision.addSimVisionTarget(newTgtFar);
    }

    public void update(Pose2d robotPose){
        simVision.processFrame(robotPose);
    }

}