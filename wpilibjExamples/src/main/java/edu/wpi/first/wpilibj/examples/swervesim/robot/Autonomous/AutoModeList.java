package frc.robot.Autonomous;

import java.util.ArrayList;
import java.util.TreeMap;

import frc.robot.Autonomous.Modes.AutoMode;

public class AutoModeList {

    private TreeMap<String, AutoMode> modeList = new TreeMap<String, AutoMode>();
    private ArrayList<String> orderedModeNameList = new ArrayList<String>(); //Helps keep track of the order the modes were added in, to ensure they end up ordered that same way in the web UI.

    public void add(AutoMode in){
        modeList.put(in.humanReadableName, in);
        orderedModeNameList.add(in.humanReadableName);
    }

    public AutoMode get(String name){
        return modeList.get(name);
    }

    public String[] getNameList(){
        return orderedModeNameList.toArray(new String[orderedModeNameList.size()]);
    }

    public AutoMode getDefault(){
        return modeList.get(orderedModeNameList.get(0)); //TBD - just the first thing added?
    }
    
}
