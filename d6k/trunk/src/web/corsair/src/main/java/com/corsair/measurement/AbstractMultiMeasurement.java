package com.corsair.measurement;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 16/1/16.
 */
public abstract class AbstractMultiMeasurement implements IMeasurement {
    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
        List<MeasurementInfo> measurementInfoList = new ArrayList<>();
        measurementInfoList.add(measurement);

        return getMeasurementValue(measurementInfoList).get(0);
    }
}
