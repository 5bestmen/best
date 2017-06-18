package com.corsair.measurement.magic;

import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.device.BasePSR;
import com.corsair.device.Inverter;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.measurement.*;
import com.corsair.service.ScadaMeasurementService;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by zjq on 2015/11/25.
 */
@Repository("month-realtime-power")
public class MonthRealtimePower extends AbstractSingleMeasurement {
    @Autowired
    RealTimeClient realTimeClient;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;
    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;
    @Autowired
    RealTimeClient client;

    @Value("#{configProperties['scada.only.station']}")
    private String onlyStation;

    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
        return null;
    }

    @Override
    public List<MeasurementValue> getMeasurementValue(List<MeasurementInfo> measurements) {

        List<MeasurementValue> ret = new ArrayList<>();
        List<MeasurementInfo> chartMeasures = new ArrayList<>();
        for (MeasurementInfo measurementInfo : measurements){
            List<BasePSR> inverters = scadaPSRService.getInverter(onlyStation);
            for (BasePSR inverter : inverters){
                DeviceTemplate inverterTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID(inverter.getId());
                DeviceTemplateData inverterTemplateData = inverterTemplate.getDeviceTemplateData(3, 0);
                String measureId = scadaMeasurementService.getMeasurementID(inverter.getId(),inverterTemplateData);
                MeasurementInfo chartMeasure = new MeasurementInfo();
                chartMeasure.setPsrID(inverter.getId());
                chartMeasure.setTemplateIndex(0);
                chartMeasure.setMagic(inverterTemplateData.getMagic());
                chartMeasure.setMeasurementType(3);
                chartMeasure.setMeasurementID(measureId);
                chartMeasures.add(chartMeasure);
            }

            List<MeasurementValue> chartValues = client.getMeasurementValue(chartMeasures);
            Double power = 0d;

            for (MeasurementValue measurementValue : chartValues){
                power += measurementValue.getData();
            }

            MeasurementValue mv = new MeasurementValue(power,null);
            mv.setKey(measurementInfo.getMagic() + measurementInfo.getPsrID());
            ret.add(mv);
        }

        return ret;
    }

}
