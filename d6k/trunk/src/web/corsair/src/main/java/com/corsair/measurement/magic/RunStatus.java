package com.corsair.measurement.magic;

/**
 * Created by ChengXi on 2015/9/9.
 */

import com.corsair.measurement.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.List;


@Repository("run-status")
public class RunStatus extends AbstractSingleMeasurement {
    @Autowired
    RealTimeClient realTimeClient;

    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
        MeasurementValue measurementValue = realTimeClient.getMeasurementValue(measurement);
        if (measurementValue.getData() == 0){
            measurementValue.setText("等待");
        }
        else if (measurementValue.getData() == 1){
            measurementValue.setText("正常");
        }
        else {
            measurementValue.setText("故障");
        }

        return measurementValue;
    }

    @Override
    public List<MeasurementValue> getMeasurementValue(List<MeasurementInfo> measurements) {
        List<MeasurementValue> ret = new ArrayList<>();
        for (MeasurementInfo measurement : measurements) {
            MeasurementValue mv = new MeasurementValue(null, "运行正常");
            mv.setKey(measurement.getMagic()+measurement.getPsrID());
            ret.add(mv);
        }

        return ret;
    }
}
