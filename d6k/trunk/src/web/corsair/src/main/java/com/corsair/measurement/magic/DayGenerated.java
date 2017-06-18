package com.corsair.measurement.magic;

import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.measurement.*;
import com.corsair.service.MagicValueService;
import com.corsair.service.ScadaMeasurementService;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

/**
 * Created by ChengXi on 2015/8/3.
 */
@Repository("day-generated")
public class DayGenerated extends AbstractSingleMeasurement{

    @Autowired
    MagicValueService magicValueService;
    @Autowired
    RealTimeClient realTimeClient;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;
    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;

    @Autowired
    GeneralMeasurementValue generalMeasurementValue;


    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
//
//        Double totalGenerated = magicValueService.getTotalGenerated();
//        MeasurementValue measurementValue = null;
//        if (totalGenerated == null)
//        {
//            measurementValue = new MeasurementValue("N/A");
//        }else{
//            measurementValue = new MeasurementValue(totalGenerated);
//        }
//        return  measurementValue;

//        DeviceTemplate inverterTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID("0052oi815tbsbl");
//        DeviceTemplateData inverterTemplateData = inverterTemplate.getDeviceTemplateDataByName("transboxMeaForwardPTotal");
//        String measureId = scadaMeasurementService.getMeasurementID("0052oi815tbsbl", inverterTemplateData);
//        MeasurementInfo chartMeasure = new MeasurementInfo();
//        chartMeasure.setPsrID("0052oi815tbsbl");
//        chartMeasure.setTemplateIndex(inverterTemplateData.getIndex());
//        chartMeasure.setMagic(inverterTemplateData.getMagic());
//        chartMeasure.setMeasurementType(inverterTemplateData.getType());
//        chartMeasure.setMeasurementID(measureId);
//        chartMeasure.setName("transboxMeaForwardPTotal");
//////            chartMeasures.add(chartMeasure);
//        MeasurementValue chartValue = null;
//        Boolean calculated = inverterTemplateData.getCalculate();
//        if (calculated != null && calculated){
//            chartValue = calculateValue.getMeasurementValue(chartMeasure);
//        }else{
//            chartValue = realTimeClient.getMeasurementValue(chartMeasure);
//        }

        MeasurementValue chartValue = generalMeasurementValue.getMeasurementValue("0052oi815tbsbl", "transboxMeaForwardPTotal");
//        MeasurementValue mv = new MeasurementValue(power/1000, null);
        if (magicValueService.getDayBeginGenerated() == null){
            magicValueService.RefreshDayBeginGeneratedData();
        }
        chartValue.setData(chartValue.getData() - magicValueService.getDayBeginGenerated());
        chartValue.setKey(measurement.getMagic() + measurement.getPsrID());
        return  chartValue;
    }



}
