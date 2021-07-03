
package frc.lib.Util;

import java.lang.reflect.Method;

import edu.wpi.first.wpilibj.Timer;

public class ExecutionTimeTracker {

    String name_prefix = "";

    double threshold_sec;
    int num_slow_calls;

    public ExecutionTimeTracker(String name_prefix_in, double threshold_sec_in) {
        name_prefix = name_prefix_in;
        threshold_sec = threshold_sec_in;
    }

    public void reset() {
        num_slow_calls = 0;
    }

    public void logAndReset() {
        CrashTracker.logGenericMessage("[ExecTimeTracker]: " + name_prefix + ": " + Integer.toString(num_slow_calls)
                + " calls exceeded threshold.");
        reset();
    }

    public void run(Object objectToUpdate, Method methodToRun) {
        double startTime = Timer.getFPGATimestamp();
        try {
            methodToRun.invoke(objectToUpdate);
        } catch (Exception e) {
            CrashTracker.logThrowableCrash(e);
        }
        double endTime = Timer.getFPGATimestamp();
        double elapsedTime = endTime - startTime;

        if (elapsedTime > threshold_sec) {
            num_slow_calls++;
        }
    }
}