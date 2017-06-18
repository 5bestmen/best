package com.corsair.measurement.magic;

import com.corsair.measurement.AbstractSingleMeasurement;
import com.corsair.measurement.MeasurementInfo;
import com.corsair.measurement.MeasurementValue;
import com.corsair.service.MagicValueService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.util.List;


/**
 * Created by zjq on 2015/12/25.
 */
@Repository("system-efficiency")
public class SystemEfficiency  extends AbstractSingleMeasurement {
    @Autowired
    MagicValueService magicValueService;
    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
        Double systemEfficiency = magicValueService.getSystemEfficiency();
        MeasurementValue measurementValue = null;
        if (systemEfficiency == null)
        {
            measurementValue = new MeasurementValue("N/A");
        }else{
            measurementValue = new MeasurementValue(systemEfficiency);
        }
        return  measurementValue;
    }

    @Override
    public List<MeasurementValue> getMeasurementValue(List<MeasurementInfo> measurements) {
        return  null;

    }
}
