package frc;

/**
 * This class provides the single funneling point between user code and simulation
 * for values which are _not_ currently supported by WPILib HAL or 3rd party vendors 
 * in a manner which is condusive to our simulation architecture. As sim support expands,
 * the size of this file should shrink.
 */

public class SimDataInf{


    public static double [] gyro_rates = new double [5];
    public static double [] gyro_angles = new double [5];

}