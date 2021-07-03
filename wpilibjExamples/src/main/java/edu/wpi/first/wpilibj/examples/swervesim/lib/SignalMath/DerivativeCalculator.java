package frc.lib.SignalMath;

import edu.wpi.first.wpilibj.Timer;

/**
 * Class which implements a simple discrete-time derivative calculation. Output
 * is equal to the rate of change of the input signal in units of <signal> per
 * second.
 *
 */
public class DerivativeCalculator {
    private double prev_time;
    private double prev_signal_val;

    /**
     * Constructor for derivative calculator. Note that you should instantiate one
     * of these classes per signal we wish to take the derivative of.
     */
    public DerivativeCalculator() {
        prev_time = Timer.getFPGATimestamp();
        prev_signal_val = 0;
    }

    /**
     * Given a new input value, output its derivative based on the last time the
     * method was invoked. The idea is you would read a signal, then call this
     * method with this new signal value, so you have the derivative right there.
     * Then use it later on.
     * 
     * @param in input to take the derivative of
     * @return the derivative of the signal "in" in units of in/sec.
     */
    public double calcDeriv(double in) {
        double cur_time = Timer.getFPGATimestamp();
        double output = (in - prev_signal_val) / (cur_time - prev_time);
        prev_time = cur_time;
        prev_signal_val = in;
        return output;

    }

}
