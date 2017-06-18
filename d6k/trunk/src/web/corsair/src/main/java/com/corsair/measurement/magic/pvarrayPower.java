package com.corsair.measurement.magic;

import com.corsair.device.BasePSR;
import com.corsair.device.Junction;
import com.corsair.measurement.*;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by libh on 2015/11/10.
 */
@Repository("pvarray-power")
public class pvarrayPower extends AbstractSingleMeasurement {
    @Autowired
    RealTimeClient realTimeClient;
    @Autowired
    ScadaPSRService scadaPSRService;

    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
        return null;
    }

    //适用于长园集团光伏项目 交流汇流箱,取所有汇流箱的功率总和
    @Override
    public List<MeasurementValue> getMeasurementValue(List<MeasurementInfo> measurements) {
        List<MeasurementValue> ret = new ArrayList<>();

        List<MeasurementInfo> pvarryPSRList = new ArrayList<>();
        Map idTypeMap = new HashMap();
        for (MeasurementInfo measurement : measurements) {
            List<BasePSR> junctionList = scadaPSRService.getJunction(measurement.getPsrID());
            for (BasePSR junction : junctionList){
                MeasurementInfo measurementI = new MeasurementInfo();
                measurementI.setPsrID(measurement.getPsrID());
                measurementI.setMeasurementID(scadaPSRService.getIformJunction(junction.getId()));
                measurementI.setMagic("");
                measurementI.setMeasurementType(2);
                measurementI.setTemplateIndex(2);
                pvarryPSRList.add(measurementI);

                MeasurementInfo measurementU = new MeasurementInfo();
                measurementU.setPsrID(measurement.getPsrID());
                measurementU.setMeasurementID(scadaPSRService.getUformJunction(junction.getId()));
                measurementU.setMagic("");
                measurementU.setMeasurementType(2);
                measurementU.setTemplateIndex(2);
                pvarryPSRList.add(measurementU);
            }
            Double currentValue = 0.0;
            int n = 1;
            Double tmpU = 0.0;
            Double tmpI = 0.0;
            List<MeasurementValue> tempValueList = realTimeClient.getMeasurementValue(pvarryPSRList);

            for (MeasurementValue measurementValue : tempValueList){
                if(n%2 != 0){
                    tmpI = measurementValue.getData();
                }
                else {
                    tmpU = measurementValue.getData();
                    currentValue += tmpU * tmpI;
                }
                n++;
            }
            currentValue /= 1000;
//            MeasurementValue mv = new MeasurementValue(Math.random()*10 + 50, null);
            MeasurementValue mv = new MeasurementValue(currentValue, null);
            mv.setKey(measurement.getMagic()+measurement.getPsrID());
            ret.add(mv);
        }
        return ret;
    }
}
