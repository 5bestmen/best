package com.corsair.measurement.magic;

import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.measurement.*;
import com.corsair.service.MagicValueService;
import com.corsair.service.ScadaMeasurementService;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by zjq on 2015/11/25.
 */
@Repository("total-generated")
public class TotalGenerated extends  AbstractSingleMeasurement {
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
//            chartValue = calculateValueService.getCalculateValue(chartMeasure);
//        }else{
//            chartValue = realTimeClient.getMeasurementValue(chartMeasure);
//        }

        MeasurementValue chartValue = generalMeasurementValue.getMeasurementValue("0052oi815tbsbl","transboxMeaForwardPTotal");
//        MeasurementValue mv = new MeasurementValue(power/1000, null);
        chartValue.setData((chartValue.getData() - 10.58 * 60)/1000);// 单位为Mwh
        chartValue.setKey(measurement.getMagic() + measurement.getPsrID());
        return  chartValue;
    }


    @Override
    public List<MeasurementValue> getMeasurementValue(List<MeasurementInfo> measurements) {
        return  null;
    }

}
