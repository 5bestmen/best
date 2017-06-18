package com.corsair.measurement;

import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.service.ScadaMeasurementService;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Created by 洪祥 on 16/1/19.
 */
@Service
public class GeneralMeasurementValue {
    @Autowired
    RealTimeClient realTimeClient;
    @Autowired
    MagicValue magicValue;
    @Autowired
    CalculateValue calculateValue;
    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;


    public MeasurementValue getMeasurementValue(String psrID, String name) {
        DeviceTemplate template = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrID);

        if (template == null) {
            return invalidValue();
        }

        DeviceTemplateData data = template.getDeviceTemplateDataByName(name);

        if (data == null) {
            return invalidValue();
        }

        MeasurementInfo info = new MeasurementInfo();
        info.setPsrID(psrID);
        info.setMeasurementID(scadaMeasurementService.getMeasurementID(psrID, data));
        info.setCalculate(data.getCalculate());
        info.setMagic(data.getMagic());
        info.setMeasurementType(data.getType());
        info.setTemplateIndex(data.getIndex());
        info.setName(data.getName());

        return getMeasurementValue(info);
    }

    private MeasurementValue invalidValue() {
        return new MeasurementValue("N/A");
    }

    private MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
        IMeasurement iMeasurement;

        if (Boolean.TRUE.equals(measurement.magic())) {
            iMeasurement = magicValue;
        } else if (Boolean.TRUE.equals(measurement.getCalculate())) {
            iMeasurement = calculateValue;
        } else {
            iMeasurement = realTimeClient;
        }

        return iMeasurement.getMeasurementValue(measurement);
    }
}
