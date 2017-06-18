package com.corsair.measurement.magic;

import com.corsair.device.BasePSR;
import com.corsair.device.Junction;
import com.corsair.measurement.*;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by libh on 2015/11/10.
 */
@Repository("pvarray-current")
public class pvarrayCurrent extends AbstractSingleMeasurement{
    @Autowired
    RealTimeClient realTimeClient;
    @Autowired
    ScadaPSRService scadaPSRService;

    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
        return null;
    }

//适用于长园集团光伏项目 交流汇流箱,取所有汇流箱的电流值总和
    @Override
    public List<MeasurementValue> getMeasurementValue(List<MeasurementInfo> measurements) {
        List<MeasurementValue> ret = new ArrayList<>();
        List<MeasurementInfo> pvarryPSRList = new ArrayList<>();
        for (MeasurementInfo measurement : measurements) {
            List<BasePSR> junctionList = scadaPSRService.getJunction(measurement.getPsrID());
            for (BasePSR junction : junctionList){
                MeasurementInfo measurementInfo = new MeasurementInfo();
                measurementInfo.setPsrID(measurement.getPsrID());
                measurementInfo.setMeasurementID(scadaPSRService.getIformJunction(junction.getId()));
                measurementInfo.setMagic("");
                measurementInfo.setMeasurementType(2);
                measurementInfo.setTemplateIndex(21);
                pvarryPSRList.add(measurementInfo);
            }
            Double currentValue = 0.0;
            List<MeasurementValue> tempValueList = realTimeClient.getMeasurementValue(pvarryPSRList);
            for (MeasurementValue measurementValue : tempValueList){

                currentValue = currentValue + measurementValue.getData();
            }
//            MeasurementValue mv = new MeasurementValue(Math.random()*10 + 50, null);
            MeasurementValue mv = new MeasurementValue(currentValue, null);
            mv.setKey(measurement.getMagic()+measurement.getPsrID());
            ret.add(mv);
        }
        return ret;
    }
}
