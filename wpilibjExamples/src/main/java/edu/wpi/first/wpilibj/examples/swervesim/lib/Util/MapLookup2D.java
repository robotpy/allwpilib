package frc.lib.Util;

/**
 * DESCRIPTION: <br>
 * A simple way to define an arbitrary function of one input variable with linear interpolation
 * between that input and the output. The user defines a series of known X/Y points in the function,
 * and then calls lookupVal with an arbitrary XVal. This class will return an appropriate Y value by
 * linearly interpolating in defined regions, or extending end-point Y values for extreme X values.
 */

import java.util.ArrayList;
import java.util.Collections;

public class MapLookup2D {
    SortedArrayList<Double> xAxis;
    ArrayList<Double> yAxis;

    public MapLookup2D() {
        xAxis = new SortedArrayList<Double>();
        yAxis = new ArrayList<Double>();
    }

    /**
     * Insert a new X/Y point into the Map
     * 
     * @param x_val
     * @param y_val
     */
    public void insertNewPoint(double x_val, double y_val) {
        int insertion_index = xAxis.insertSorted(x_val);
        yAxis.add(insertion_index, y_val);
    }

    /**
     * Extract the value of the mapped function at a specific point.
     * 
     * @param req_x_val Input point to request the value of the function at.
     * @return Value of the map function at req_x_val
     */
    public double lookupVal(double req_x_val) {
        if (xAxis.size() == 0) {
            // Case, no array defined yet. Just return zero.
            return 0;
        } else if (xAxis.size() == 1) {
            // Case, only one element in array. Just return the single y value
            return yAxis.get(0);
        } else {
            // Case, at least two points. Find what interval the
            // requested value falls in.

            // If the requested value falls off the end of
            // the defined map array, just return the
            // extreme values
            if (req_x_val <= xAxis.get(0)) {
                return yAxis.get(0);
            } else if (req_x_val >= xAxis.get(xAxis.size() - 1)) {
                return yAxis.get(yAxis.size() - 1);
            } else {
                int lower_index;
                // Iterate over the xAxis array to find the interval that req_x_val falls within
                // Note this only works because xAxis is sorted.
                for (lower_index = 0; lower_index < xAxis.size() - 1; lower_index++) {
                    if ((xAxis.get(lower_index + 1) >= req_x_val) & (xAxis.get(lower_index) <= req_x_val)) {
                        break; // interval found
                    }
                }
                // math math math math mathy math
                double intervalXDelta = xAxis.get(lower_index + 1) - xAxis.get(lower_index);
                double intervalYDelta = yAxis.get(lower_index + 1) - yAxis.get(lower_index);
                double reqFractionIntoInterval = (req_x_val - xAxis.get(lower_index)) / intervalXDelta;
                return (reqFractionIntoInterval * intervalYDelta) + yAxis.get(lower_index);
            }

        }
    }

    /**
     * Main function - used for desktop testing of functionality. No use on a robot,
     * sadly :(
     * 
     * @param args
     */
    public static void main(String args[]) {
        MapLookup2D test_map;

        System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

        System.out.println("Testcase 1");
        test_map = new MapLookup2D();
        System.out.println(test_map.lookupVal(1348.2)); // No entries

        System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

        System.out.println("Testcase 2");
        test_map = new MapLookup2D();
        test_map.insertNewPoint(1, 2); // One entry
        System.out.println(test_map.lookupVal(1));
        System.out.println(test_map.lookupVal(3));

        System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

        System.out.println("Testcase 3");
        test_map = new MapLookup2D();
        test_map.insertNewPoint(1, 2); // Two entry
        test_map.insertNewPoint(2, 1);
        System.out.println(test_map.lookupVal(-1));
        System.out.println(test_map.lookupVal(1));
        System.out.println(test_map.lookupVal(2));
        System.out.println(test_map.lookupVal(4));

        System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

        System.out.println("Testcase 4");
        test_map = new MapLookup2D();
        test_map.insertNewPoint(1, 2); // many entries
        test_map.insertNewPoint(3, 4);
        test_map.insertNewPoint(5, 8);
        test_map.insertNewPoint(10, 15);
        test_map.insertNewPoint(-2, 10); // out of order insert

        System.out.println(test_map.lookupVal(-100));
        System.out.println(test_map.lookupVal(-1));
        System.out.println(test_map.lookupVal(1));
        System.out.println(test_map.lookupVal(2));
        System.out.println(test_map.lookupVal(7.25));
        System.out.println(test_map.lookupVal(11));
        System.out.println(test_map.lookupVal(100));

        System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

    }

}

@SuppressWarnings("serial")
class SortedArrayList<T> extends ArrayList<T> {

    @SuppressWarnings("unchecked")
    public int insertSorted(T value) {
        int i;
        add(value);
        Comparable<T> cmp = (Comparable<T>) value;
        for (i = size() - 1; i > 0 && cmp.compareTo(get(i - 1)) < 0; i--) {
            Collections.swap(this, i, i - 1);
        }

        return i;
    }
}
