/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.controller;

import edu.wpi.first.hal.FRCNetComm.tResourceType;
import edu.wpi.first.hal.HAL;
import edu.wpi.first.wpilibj.SendableBase;
import edu.wpi.first.wpilibj.smartdashboard.SendableBuilder;

/**
 * Implements a PID control loop.
 */
@SuppressWarnings("PMD.TooManyFields")
public class PIDController extends SendableBase {
  private static int instances;

  // Factor for "proportional" control
  @SuppressWarnings("MemberName")
  private double m_Kp;

  // Factor for "integral" control
  @SuppressWarnings("MemberName")
  private double m_Ki;

  // Factor for "derivative" control
  @SuppressWarnings("MemberName")
  private double m_Kd;

  // The period (in seconds) of the loop that calls the controller
  private final double m_period;

  // |maximum output|
  private double m_maximumOutput = 1.0;

  // |minimum output|
  private double m_minimumOutput = -1.0;

  // Maximum input - limit setpoint to this
  private double m_maximumInput;

  // Minimum input - limit setpoint to this
  private double m_minimumInput;

  // input range - difference between maximum and minimum
  private double m_inputRange;

  // Do the endpoints wrap around? eg. Absolute encoder
  private boolean m_continuous;

  // The error at the time of the most recent call to calculate()
  private double m_currError;

  // The error at the time of the second-most-recent call to calculate() (used to compute velocity)
  private double m_prevError = Double.POSITIVE_INFINITY;

  // The sum of the errors for use in the integral calc
  private double m_totalError;

  enum Tolerance {
    kAbsolute, kPercent
  }

  private Tolerance m_toleranceType = Tolerance.kAbsolute;

  // The percentage or absolute error that is considered at setpoint.
  private double m_tolerance = 0.05;
  private double m_deltaTolerance = Double.POSITIVE_INFINITY;

  private double m_setpoint;
  private double m_output;

  /**
   * Allocates a PIDController with the given constants for Kp, Ki, and Kd and a default period of
   * 0.02 seconds.
   *
   * @param Kp The proportional coefficient.
   * @param Ki The integral coefficient.
   * @param Kd The derivative coefficient.
   */
  @SuppressWarnings("ParameterName")
  public PIDController(double Kp, double Ki, double Kd) {
    this(Kp, Ki, Kd, 0.02);
  }

  /**
   * Allocates a PIDController with the given constants for Kp, Ki, and Kd.
   *
   * @param Kp     The proportional coefficient.
   * @param Ki     The integral coefficient.
   * @param Kd     The derivative coefficient.
   * @param period The period between controller updates in seconds.
   */
  @SuppressWarnings("ParameterName")
  public PIDController(double Kp, double Ki, double Kd, double period) {
    m_Kp = Kp;
    m_Ki = Ki;
    m_Kd = Kd;

    m_period = period;

    instances++;
    setName("PIDController", instances);

    HAL.report(tResourceType.kResourceType_PIDController, instances);
  }

  /**
   * Sets the PID Controller gain parameters.
   *
   * <p>Set the proportional, integral, and differential coefficients.
   *
   * @param Kp The proportional coefficient.
   * @param Ki The integral coefficient.
   * @param Kd The derivative coefficient.
   */
  @SuppressWarnings("ParameterName")
  public void setPID(double Kp, double Ki, double Kd) {
    m_Kp = Kp;
    m_Ki = Ki;
    m_Kd = Kd;
  }

  /**
   * Sets the Proportional coefficient of the PID controller gain.
   *
   * @param Kp proportional coefficient
   */
  @SuppressWarnings("ParameterName")
  public void setP(double Kp) {
    m_Kp = Kp;
  }

  /**
   * Sets the Integral coefficient of the PID controller gain.
   *
   * @param Ki integral coefficient
   */
  @SuppressWarnings("ParameterName")
  public void setI(double Ki) {
    m_Ki = Ki;
  }

  /**
   * Sets the Differential coefficient of the PID controller gain.
   *
   * @param Kd differential coefficient
   */
  @SuppressWarnings("ParameterName")
  public void setD(double Kd) {
    m_Kd = Kd;
  }

  /**
   * Get the Proportional coefficient.
   *
   * @return proportional coefficient
   */
  public double getP() {
    return m_Kp;
  }

  /**
   * Get the Integral coefficient.
   *
   * @return integral coefficient
   */
  public double getI() {
    return m_Ki;
  }

  /**
   * Get the Differential coefficient.
   *
   * @return differential coefficient
   */
  public double getD() {
    return m_Kd;
  }

  /**
   * Returns the period of this controller.
   *
   * @return the period of the controller.
   */
  public double getPeriod() {
    return m_period;
  }

  /**
   * Returns the current controller output.
   *
   * <p>This is always centered around zero and constrained to the min and max outputs.
   *
   * @return The latest calculated output.
   */
  public double getOutput() {
    return m_output;
  }

  /**
   * Sets the setpoint for the PIDController.
   *
   * @param setpoint The desired setpoint.
   */
  public void setSetpoint(double setpoint) {
    if (m_maximumInput > m_minimumInput) {
      m_setpoint = clamp(setpoint, m_minimumInput, m_maximumInput);
    } else {
      m_setpoint = setpoint;
    }
  }

  /**
   * Returns the current setpoint of the PIDController.
   *
   * @return The current setpoint.
   */
  public double getSetpoint() {
    return m_setpoint;
  }

  /**
   * Returns true if the error is within the percentage of the total input range, determined by
   * SetTolerance. This asssumes that the maximum and minimum input were set using SetInput.
   *
   * <p>This will return false until at least one input value has been computed.
   *
   * @return Whether the error is within the acceptable bounds.
   */
  public boolean atSetpoint() {
    return atSetpoint(m_tolerance, m_deltaTolerance, m_toleranceType);
  }

  /**
   * Returns true if the error and change in error are below the specified tolerances.
   *
   * @param tolerance      The maximum allowable error.
   * @param deltaTolerance The maximum allowable change in error from the previous iteration.
   * @param toleranceType  Whether the given tolerance values are absolute, or percentages of the
   *                       total input range.
   * @return Whether the error is within the acceptable bounds.
   */
  public boolean atSetpoint(double tolerance, double deltaTolerance, Tolerance toleranceType) {
    double error = getError();
    double deltaError = (error - m_prevError) / getPeriod();

    if (toleranceType == Tolerance.kPercent) {
      return Math.abs(error) < tolerance / 100 * m_inputRange
          && Math.abs(deltaError) < deltaTolerance / 100 * m_inputRange;
    } else {
      return Math.abs(error) < tolerance && Math.abs(deltaError) < deltaTolerance;
    }
  }

  /**
   * Sets the PID controller to consider the input to be continuous.
   *
   * <p>Rather then using the max and min input range as constraints, it considers them to be the
   * same point and automatically calculates the shortest route to the setpoint.
   */
  public void setContinuous() {
    setContinuous(true);
  }

  /**
   * Sets the PID controller to consider the input to be continuous,
   *
   * <p>Rather then using the max and min input range as constraints, it considers them to be the
   * same point and automatically calculates the shortest route to the setpoint.
   *
   * @param continuous true turns on continuous, false turns off continuous
   */
  public void setContinuous(boolean continuous) {
    m_continuous = continuous;
  }

  /**
   * Sets the minimum and maximum values expected from the input.
   *
   * @param minimumInput The minimum value expected from the input.
   * @param maximumInput The maximum value expected from the input.
   */
  public void setInputRange(double minimumInput, double maximumInput) {
    m_minimumInput = minimumInput;
    m_maximumInput = maximumInput;
    m_inputRange = maximumInput - minimumInput;

    // Clamp setpoint to new input
    if (m_maximumInput > m_minimumInput) {
      m_setpoint = clamp(m_setpoint, m_minimumInput, m_maximumInput);
    }
  }

  /**
   * Sets the minimum and maximum values to write.
   *
   * @param minimumOutput the minimum value to write to the output
   * @param maximumOutput the maximum value to write to the output
   */
  public void setOutputRange(double minimumOutput, double maximumOutput) {
    m_minimumOutput = minimumOutput;
    m_maximumOutput = maximumOutput;
  }

  /**
   * Sets the absolute error which is considered tolerable for use with atSetpoint().
   *
   * @param tolerance Absolute error which is tolerable.
   */
  public void setAbsoluteTolerance(double tolerance) {
    setAbsoluteTolerance(tolerance, Double.POSITIVE_INFINITY);
  }

  /**
   * Sets the absolute error which is considered tolerable for use with atSetpoint().
   *
   * @param tolerance      Absolute error which is tolerable.
   * @param deltaTolerance Change in absolute error per second which is tolerable.
   */
  public void setAbsoluteTolerance(double tolerance, double deltaTolerance) {
    m_toleranceType = Tolerance.kAbsolute;
    m_tolerance = tolerance;
    m_deltaTolerance = deltaTolerance;
  }

  /**
   * Sets the percent error which is considered tolerable for use with atSetpoint().
   *
   * @param tolerance Percent error which is tolerable.
   */
  public void setPercentTolerance(double tolerance) {
    setPercentTolerance(tolerance, Double.POSITIVE_INFINITY);
  }

  /**
   * Sets the percent error which is considered tolerable for use with atSetpoint().
   *
   * @param tolerance      Percent error which is tolerable.
   * @param deltaTolerance Change in percent error per second which is tolerable.
   */
  public void setPercentTolerance(double tolerance, double deltaTolerance) {
    m_toleranceType = Tolerance.kPercent;
    m_tolerance = tolerance;
    m_deltaTolerance = deltaTolerance;
  }

  /**
   * Returns the difference between the setpoint and the measurement.
   *
   * @return The error.
   */
  public double getError() {
    return getContinuousError(m_currError);
  }

  /**
   * Returns the change in error per second.
   */
  public double getDeltaError() {
    double error = getError();
    return (error - m_prevError) / getPeriod();
  }

  /**
   * Returns the next output of the PID controller.
   *
   * @param measurement The current measurement of the process variable.
   * @param setpoint    The new setpoint of the controller.
   */
  public double calculate(double measurement, double setpoint) {
    // Set setpoint to provided value
    setSetpoint(setpoint);
    return calculate(measurement);
  }

  /**
   * Returns the next output of the PID controller.
   *
   * @param measurement The current measurement of the process variable.
   */
  public double calculate(double measurement) {
    m_prevError = m_currError;
    m_currError = getContinuousError(m_setpoint - measurement);

    if (m_Ki != 0) {
      m_totalError = clamp(m_totalError + m_currError * getPeriod(), m_minimumOutput / m_Ki,
          m_maximumOutput / m_Ki);
    }

    m_output = clamp(
        m_Kp * m_currError + m_Ki * m_totalError + m_Kd * (m_currError - m_prevError) / getPeriod(),
        m_minimumOutput, m_maximumOutput);

    return m_output;
  }

  /**
   * Resets the previous error and the integral term. Also disables the controller.
   */
  public void reset() {
    m_prevError = 0;
    m_totalError = 0;
    m_output = 0;
  }

  @Override
  public void initSendable(SendableBuilder builder) {
    builder.setSmartDashboardType("PIDController");
    builder.addDoubleProperty("p", this::getP, this::setP);
    builder.addDoubleProperty("i", this::getI, this::setI);
    builder.addDoubleProperty("d", this::getD, this::setD);
    builder.addDoubleProperty("setpoint", this::getSetpoint, this::setSetpoint);
  }

  /**
   * Wraps error around for continuous inputs. The original error is returned if continuous mode is
   * disabled.
   *
   * @param error The current error of the PID controller.
   * @return Error for continuous inputs.
   */
  protected double getContinuousError(double error) {
    if (m_continuous && m_inputRange > 0) {
      error %= m_inputRange;
      if (Math.abs(error) > m_inputRange / 2) {
        if (error > 0) {
          return error - m_inputRange;
        } else {
          return error + m_inputRange;
        }
      }
    }
    return error;
  }

  private static double clamp(double value, double low, double high) {
    return Math.max(low, Math.min(value, high));
  }
}
