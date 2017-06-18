package com.corsair.measurement.magic;

import com.corsair.measurement.AbstractSingleMeasurement;
import com.corsair.measurement.GeneralMeasurementValue;
import com.corsair.measurement.MeasurementInfo;
import com.corsair.measurement.MeasurementValue;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

/**
 * Created by 洪祥 on 16/3/3.
 * 当日发电的特殊计算量,取当前表码减去本日第一个表码,要求表码需要是实时库中的值
 */
@Repository
public class CurrentDayGenerated extends AbstractSingleMeasurement {
    private static final String CURRENT_GENERATED_NAME = "TotalForwardActivePower";

    @Autowired
    GeneralMeasurementValue generalMeasurementValue;

    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
        MeasurementValue currentGenerated = generalMeasurementValue.getMeasurementValue(measurement.getPsrID(), CURRENT_GENERATED_NAME);




        return null;
    }
}
