package com.corsair.measurement;

import com.corsair.measurement.MeasurementInfo;
import com.corsair.measurement.MeasurementValue;

import java.util.List;

/**
 * Created by 洪祥 on 15/7/24.
 */
public interface IMeasurement {
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement);
    public List<MeasurementValue> getMeasurementValue(List<MeasurementInfo> measurements);
}
