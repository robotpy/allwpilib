package frc.lib.AutoSequencer;

import java.util.ArrayList;

import edu.wpi.first.wpilibj.Timer;
import frc.lib.Util.CrashTracker;

/**
 * Autonomous mode event sequencer. Provides an infrastructure for defining
 * autonomous actions in a multi-layer state-machine like fashion. <br>
 * <br>
 * Events will be executed in the same order they are added. Once start() is
 * called, an event's update() method will be called repeatedly until isDone()
 * returns true. At this point, the next event in the primary timeline is run.
 * During each timeline event, in addition to repeatedly calling the update()
 * method, all child events are evaluated to see if they too need to be updated.
 */
public class AutoSequencer {

    ArrayList<AutoEvent> events = new ArrayList<AutoEvent>();

    AutoEvent activeEvent = null;

    long globalUpdateCount = 0;

    int globalEventIndex = 0;

    String name;

    public AutoSequencer() {
        commonConstructor("Auto");
    }

    public AutoSequencer(String name_in) {
        commonConstructor(name_in);
    }

    private void commonConstructor(String name_in) {
        name = name_in;
    }

    /**
     * Add sequential event to the primary timeline.
     * 
     * @param event_in
     */
    public void addEvent(AutoEvent event_in) {
        events.add(event_in);
        CrashTracker.logAndPrint("[" + name + "] New event registered - " + event_in.getClass().getName());
        if (event_in.childEvents.size() > 0) {
            CrashTracker.logAndPrint("[" + name + "] Child Events: ");
            for (AutoEvent child : event_in.childEvents) {
                CrashTracker.logAndPrint("[" + name + "]       " + child.getClass().getName());
            }
        }
    }

    public void clearAllEvents() {
        events.clear();
        CrashTracker.logAndPrint("[" + name + "] Cleared event list");
    }

    /**
     * Reset to the start of the autonomous sequence.
     */
    public void start() {
        globalEventIndex = 0;
        globalUpdateCount = 0;

        CrashTracker.logAndPrint("[" + name + "] Starting...");

        if (events.size() > 0) {
            activeEvent = events.get(globalEventIndex);
            startEvent(activeEvent);
        }
    }

    /**
     * Stop anything which might be running now. Will call the userForceStop() on
     * any presently running events.
     */
    public void stop() {
        // if something is running, we'll need to stop it.
        if (activeEvent != null) {

            // Force stop this event and its children
            activeEvent.forceStopAllChildren();
            activeEvent.userForceStop();
            CrashTracker.logAndPrint("[" + name + "] Stopped.");
        }

        // Set activeEvent to nothing running state.
        activeEvent = null;
    }

    public void update() {

        // Don't bother to do anything if there is no active event right now.
        if (activeEvent != null) {

            // Update the active event. This will probably set motors or stuff like that.
            activeEvent.update();

            // Check if there are any children to update.
            if (activeEvent.childEvents.size() > 0) {
                for (AutoEvent child : activeEvent.childEvents) {

                    // Evaluate if child needs to start running
                    if (!child.completed) {
                        if (child.isRunning == false & child.isTriggered()) {
                            child.isRunning = true;
                            CrashTracker.logAndPrint(
                                    "[" + name + "] Starting new child auto event " + child.getClass().getName());
                            child.userStart();
                        }
                        // Call update if the child is running
                        if (child.isRunning == true) {
                            child.update();
                        }
                        // Evaluate if the child needs to be stopped
                        if (child.isRunning == true & child.isDone()) {
                            child.isRunning = false;
                            child.completed = true;
                            CrashTracker.logAndPrint(
                                    "[" + name + "] Finished child auto event " + child.getClass().getName());
                        }
                    }

                }
            }

            // Check if active event has completed. Move on to the next one if this one is
            // done.
            // Note this sequence guarantees each event's update is called at least once.
            if (activeEvent.isDone() && allChildrenDone(activeEvent)) {
                // This event is done - determine if we are done with auto, or need to do the
                // next event.

                globalEventIndex++;

                // See what our new current event is.
                if (globalEventIndex >= events.size()) {
                    // terminal condition. we have no more states to run. Stop running things.
                    activeEvent = null;
                    CrashTracker.logAndPrint("[" + name + "] Finished all events in sequence.");
                    return;
                }

                activeEvent = events.get(globalEventIndex);
                startEvent(activeEvent);
            }

            if (globalUpdateCount % 50 == 0) {
                CrashTracker
                        .logAndPrint("[" + name + "] Running. timestep = " + Double.toString(globalUpdateCount * 0.02)
                                + "s | ActualTime = " + Double.toString(Timer.getFPGATimestamp()));
            }

        }
        globalUpdateCount++;

    }

    /**
     * Checks if all children of an event are no longer running
     * 
     * @param event
     * @return
     */
    private boolean allChildrenDone(AutoEvent event) {
        if (event.childEvents.size() > 0) {
            for (AutoEvent child : event.childEvents) {
                if (child.isRunning) {
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * Performs all actions required to start an event.
     * 
     * @param event
     */
    private void startEvent(AutoEvent event) {
        CrashTracker.logAndPrint("[" + name + "] Starting new auto event " + activeEvent.getClass().getName());
        activeEvent.userStart();

        if (event.childEvents.size() > 0) {
            for (AutoEvent child : event.childEvents) {
                child.completed = false;
                child.isRunning = false;
            }
        }
    }

    /**
     * Determine if the sequencer is active. As long as this is true, any calls to
     * update() will trigger various events run.
     * 
     * @return True if the auto sequencer is executing something, false otherwise
     */
    public boolean isRunning() {
        return activeEvent != null;
    }

    public int getEventIndex() {
        if (activeEvent != null) {
            return globalEventIndex;
        } else {
            return -1;
        }
    }

}