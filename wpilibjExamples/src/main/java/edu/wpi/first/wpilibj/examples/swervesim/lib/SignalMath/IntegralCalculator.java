package frc.lib.SignalMath;

import edu.wpi.first.wpilibj.Timer;

/**
 * Class which implements various calculations of the integral of a signal over
 * time. Useful for aproximating distance traveled from an accelerometer, or
 * pose angle from a gyroscope, or in an I term in a PID, or anything else
 * really! <br>
 * <br>
 *
 * The integral of a signal is pretty much just the sum of all previous values.
 * There is some scaling dependant on how far apart those previous values were.
 * With digital signals, it gets a bit more interesting because there's
 * different things you can assume about what the value of the signal does
 * between your known sample points. Often a rectangular or trapezoidal fit
 * works great, although for lower sample rates other methods can get you
 * further. this class implements many of these. <br>
 * <br>
 *
 * For the math-savvy, Wikipedia has a decent description of the various
 * <a href="https://en.wikipedia.org/wiki/Numerical_integration" target=
 * "_blank">numerical integration</a> methods this class implements for
 * real-time. <br>
 * <br>
 *
 */
public class IntegralCalculator {
    private double prev_time;

    /**
     * Present value of the accumulator. Use this if you want to know the value
     * without adding another sample to the buffer.
     */
    public double accumulator;
    private double[] point_num;
    private int choice;
    private double value;

    /**
     * Initalizes everything needed for an integral calculation.
     * 
     * This is the stage where you, the user, must select how to do the integration.
     * There are 5 methods available (named 0-4). As the number increases, so does
     * the accuracy. However, so does the time- and memory-complexity of the
     * calculation. Choose the lowest number that suits your appliaction needs. <br>
     * <br>
     * <br>
     * <br>
     * Integration Methods Available: <br>
     * <br>
     * 0: <i>Rectangular rule</i> Assumes the signal maintains the exact same value
     * from the previous sample to this sample, and then has a step change on the
     * new sample. "Sample and hold" is often used to refer to this assumption. <br>
     * <br>
     * 1: <i>Trapezoidal rule</i> Assumes the signal linearlly ramps between sample
     * points. <br>
     * <br>
     * 2: <i>Simpson's rule</i> Assumes the signal acts like a 2nd-degree polynomial
     * between points. Calculations are done by integrating a second degree
     * polynomial splined between the 3 most recent samples. <br>
     * <br>
     * 3: <i>Simpson's 3/8ths rule</i> Same as Simpson's rule, but presumes a 3rd
     * degree polynomial. <br>
     * <br>
     * 4: <i>Boole's rule</i> Same as Simpson's rule, but presumes a 4th degree
     * polynomial <br>
     * <br>
     * <br>
     * <br>
     * If you need more accuracy than this, you may wish to consider a faster sample
     * rate.
     *
     * @param choice_in An integer between 0 and 4 inclusive to indicate the type of
     *                  integration to do.
     */
    public IntegralCalculator(int choice_in) {
        prev_time = Timer.getFPGATimestamp();
        accumulator = 0;
        point_num = new double[5];
        choice = choice_in;
    }

    /**
     * Perform numerical integration by accounting for a new sample, and returning
     * the present integral value.
     *
     * @param in New sample from this control loop to add to the integral
     *           calculation
     * @return The present value of the integral
     */
    public double calcIntegral(double in) {
        double cur_time = Timer.getFPGATimestamp();

        // Shift the sample into the buffer.
        point_num[4] = point_num[3];
        point_num[3] = point_num[2];
        point_num[2] = point_num[1];
        point_num[1] = point_num[0];
        point_num[0] = in;

        if (choice == 0) /* rectangular rule */ {
            value += in * (cur_time - prev_time);
        } else if (choice == 1) /* trapezoid rule */ {
            value = ((cur_time - prev_time) / 2.0) * (point_num[0] + point_num[1]);
        } else if (choice == 2) /* simpson's rule */ {
            value = (1.0 / 2.0) * ((cur_time - prev_time) / 6.0) * (point_num[0] + (4.0 * point_num[1]) + point_num[2]);
        } else if (choice == 3) /* simpson's 3/8 rule */ {
            value = (1.0 / 3.0) * ((cur_time - prev_time) / 8.0)
                    * (point_num[0] + (3.0 * point_num[1]) + (3.0 * point_num[2]) + point_num[3]);
        } else if (choice == 4) /* boole's rule */ {
            value = (1.0 / 4.0) * ((cur_time - prev_time) / 90.0) * ((7.0 * point_num[0]) + (32.0 * point_num[1])
                    + (12.0 * point_num[2]) + (32.0 * point_num[3]) + (7.0 * point_num[4]));
        } else { // invalid number. Don't do any calculation
            System.out.println("Warning: invalid number choice in IntegralCalculator. No calculation will be done.");

        }

        /* Save values for next loop and return */
        accumulator = accumulator + value;
        prev_time = cur_time;
        return accumulator;
    }

    /**
     * Resets the present value of the integral to zero. Should be called during
     * times when the value of the integral is known to be zero. For example, if the
     * integral is tracking robot pose angle, this could be called when the robot is
     * backed up against a wall, since the angle is known.
     *
     */
    public void resetIntegral() {
        accumulator = 0;
        point_num[0] = 0;
        point_num[1] = 0;
        point_num[2] = 0;
        point_num[3] = 0;
        point_num[4] = 0;
    }

}
