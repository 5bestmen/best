package com.corsair.measurement.magic;

import com.corsair.measurement.AbstractSingleMeasurement;
import com.corsair.measurement.MeasurementInfo;
import com.corsair.measurement.MeasurementValue;
import com.corsair.measurement.RealTimeClient;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 15/7/24.
 */
@Repository("communicate-status")
public class CommunicationStatus extends AbstractSingleMeasurement {
    @Autowired
    RealTimeClient realTimeClient;


    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
        return null;
    }

    @Override
    public List<MeasurementValue> getMeasurementValue(List<MeasurementInfo> measurements) {
        List<MeasurementValue> ret = new ArrayList<>();

        for (MeasurementInfo measurement : measurements) {
            MeasurementValue mv = new MeasurementValue(null, "通信正常");
            mv.setKey(measurement.getMagic()+measurement.getPsrID());
            ret.add(mv);
        }

        return ret;
    }
}
