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
 * Created by ChengXi on 2015/8/3.
 */
@Repository("realtime-power")
public class RealtimePower extends  AbstractSingleMeasurement {
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

    @Value("#{configProperties['scada.only.station']}")
    private String onlyStation;


    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
   //     List<MeasurementValue> ret = new ArrayList<>();
//        List<MeasurementInfo> chartMeasures = new ArrayList<>();
//        List<MeasurementValue> chartValues = new ArrayList<>();
//        List<Inverter> inverters = scadaPSRService.getInverter("oi815");
//        for (Inverter inverter : inverters) {
//            DeviceTemplate inverterTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID(inverter.getId());
//            DeviceTemplateData inverterTemplateData = inverterTemplate.getDeviceTemplateDataByName("inverterOutPower");
//            String measureId = scadaMeasurementService.getMeasurementID(inverter.getId(), inverterTemplateData);
//            MeasurementInfo chartMeasure = new MeasurementInfo();
//            chartMeasure.setPsrID(inverter.getId());
//            chartMeasure.setTemplateIndex(inverterTemplateData.getIndex());
//            chartMeasure.setMagic(inverterTemplateData.getMagic());
//            chartMeasure.setMeasurementType(inverterTemplateData.getType());
//            chartMeasure.setMeasurementID(measureId);
//            chartMeasure.setName("inverterOutPower");
////            chartMeasures.add(chartMeasure);
//            Boolean calculated = inverterTemplateData.getCalculate();
//            if (calculated != null && calculated){
//                MeasurementValue chartValue = calculateValueService.getCalculateValue(chartMeasure);
//                chartValues.add(chartValue);
//            }else{
//                MeasurementValue chartValue = realTimeClient.getMeasurementValue(chartMeasure);
//                chartValues.add(chartValue);
//            }
//        }
//
//        Double power = 0d;
//
//        for (MeasurementValue measurementValue : chartValues) {
//            power += measurementValue.getData();
//        }
//
//        DeviceTemplate inverterTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID("0022oi815p8cmc");
//            DeviceTemplateData inverterTemplateData = inverterTemplate.getDeviceTemplateDataByName("transboxPtotal");
//            String measureId = scadaMeasurementService.getMeasurementID("0022oi815p8cmc", inverterTemplateData);
//            MeasurementInfo chartMeasure = new MeasurementInfo();
//            chartMeasure.setPsrID("0022oi815p8cmc");
//            chartMeasure.setTemplateIndex(inverterTemplateData.getIndex());
//            chartMeasure.setMagic(inverterTemplateData.getMagic());
//            chartMeasure.setMeasurementType(inverterTemplateData.getType());
//            chartMeasure.setMeasurementID(measureId);
//            chartMeasure.setName("transboxPtotal");
//////            chartMeasures.add(chartMeasure);
//            MeasurementValue chartValue = null;
//            Boolean calculated = inverterTemplateData.getCalculate();
//            if (calculated != null && calculated){
//                chartValue = calculateValueService.getCalculateValue(chartMeasure);
//            }else{
//                chartValue = realTimeClient.getMeasurementValue(chartMeasure);
//            }
        MeasurementValue chartValue = generalMeasurementValue.getMeasurementValue("0022oi815p8cmc","transboxPtotal");

//        MeasurementValue mv = new MeasurementValue(power/1000, null);
        chartValue.setData(chartValue.getData()/1000);
        chartValue.setKey(measurement.getMagic() + measurement.getPsrID());
        return  chartValue;
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

            List<MeasurementValue> chartValues = realTimeClient.getMeasurementValue(chartMeasures);
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
