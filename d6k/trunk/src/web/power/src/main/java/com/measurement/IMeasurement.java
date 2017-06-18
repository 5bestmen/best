package com.measurement;

import com.measurement.MeasurementInfo;
import com.measurement.MeasurementValue;

import java.util.List;

/**
 * Created by zyj on 17/6/12.
 */
public interface IMeasurement {
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement);
    public List<MeasurementValue> getMeasurementValue(List<MeasurementInfo> measurements);
}
