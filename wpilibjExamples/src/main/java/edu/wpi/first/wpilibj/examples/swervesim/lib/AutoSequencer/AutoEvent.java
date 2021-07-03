package frc.lib.AutoSequencer;

import java.util.ArrayList;

/**
 * AutoEvent is the abstract class which all things put onto the robot
 * autonomous timeline must implement. For example, A path-planned motion
 * profile is one type of event. Rotating in place, closed loop around a gyro is
 * another type of event. Raising an arm is another type of event. <br>
 * <br>
 * Events are fundamentally two types - parent events and child events. Parent
 * events are the primary ones run by the sequencer. Any event that is added
 * directly to the sequencer is a parent event. These events will be run in
 * order, each one starting as soon as the previous one declares it is finished.
 * <br>
 * <br>
 * Child events are assigned to a parent event. While a parent event is running,
 * each child event is evaluated individually to see if it is triggered (needs
 * to start running) or stopped (needs to stop running). As soon as a child
 * event reports it is triggered, at least once call to the child event's
 * update() method is guaranteed. The child event will no longer be run once it
 * declares itself as "done". <br>
 * <br>
 * All events on the timeline must implement the functions defined here.
 * Constructor is for performing all user-specific actions during sequencer
 * setup. User update is the periodic update function which the sequencer will
 * call as long as the event is active. isDone() signals the sequencer this
 * event no longer needs to run, while isTriggered() is used for child events to
 * signal they need to be run. Calculation of triggered/done conditions shoudl
 * be performed within those methods. Do not rely upon update() to be called to
 * evaluate those conditions. Finally, the forceStop method is required to be
 * implemented - ordinarily, the event should return the robot to a safe state
 * before declaring itself done. However, if the autonomous sequence is
 * interrupted before the event has a chance to finish naturally, this function
 * will be called to shut down the event properly. This means doing things like
 * stopping motors or resetting counters, whatever floats your boat (safely).
 * 
 * @author Chris Gerth
 *
 */

public abstract class AutoEvent {

    /**
     * Total number of times the update method has been called. Incremented AFTER
     * each update.
     */
    public long localUpdateCount;

    /** True when this event is active, false if not. */
    public boolean isRunning;

    /** True when this event has finished running once, false if not */
    public boolean completed;

    public ArrayList<AutoEvent> childEvents;

    public AutoEvent() {
        localUpdateCount = 0;
        isRunning = false;
        childEvents = new ArrayList<AutoEvent>(0);
    }

    /**
     * Assign a child event to this event. Whenever this event is active, the child
     * event will be checked for a trigger condition and will be updated in parallel
     * with its parent event.
     *
     * @param event_in The new event to add
     */

    public void addChildEvent(AutoEvent event_in) {
        childEvents.add(event_in);
    }

    public void forceStopAllChildren() {
        for (AutoEvent child : childEvents) {
            if (child.isRunning) {
                child.userForceStop();
                child.isRunning = false;
            }
        }
    }

    void update() {
        userUpdate();
        localUpdateCount++;
    }

    /** perform all actions needed at the start of the event */
    public abstract void userStart();

    /** Perform all actions needed during periodic update for this event */
    public abstract void userUpdate();

    /**
     * Perform all actions needed to restore robot to a safe state if the event is
     * prematurely stopped.
     */
    public abstract void userForceStop();

    /**
     * Determine if this event needs updating. The first time isTriggered() returns
     * true, the AutoSequencer begins updating this event.
     * 
     * @return True if the trigger condition has been hit, false otherwise
     */
    public abstract boolean isTriggered();

    /**
     * Determine if this event has completed. Might be that total time has expired
     * 
     * @return True if the event no longer needs updating, false otherwise.
     */
    public abstract boolean isDone();

}
