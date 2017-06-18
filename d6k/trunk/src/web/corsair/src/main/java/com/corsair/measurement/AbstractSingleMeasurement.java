package com.corsair.measurement;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 16/1/16.
 */
public abstract class AbstractSingleMeasurement implements IMeasurement {
    @Override
    public List<MeasurementValue> getMeasurementValue(List<MeasurementInfo> measurements) {
        List<MeasurementValue> values = new ArrayList<>();

        for (MeasurementInfo info : measurements) {
            values.add(getMeasurementValue(info));
        }

        return values;
    }
}
