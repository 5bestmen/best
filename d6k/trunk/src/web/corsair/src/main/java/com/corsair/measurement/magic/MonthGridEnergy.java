package com.corsair.measurement.magic;

import com.corsair.dao.HistoryDataDao;
import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.device.BasePSR;
import com.corsair.device.Inverter;
import com.corsair.device.PackageTransformer;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.measurement.*;
import com.corsair.service.ScadaMeasurementService;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

/**
 * Created by zjq on 2015/11/25.
 */
@Repository("month-grid-energy")
public class MonthGridEnergy extends AbstractSingleMeasurement {
    @Autowired
    RealTimeClient realTimeClient;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;
    @Autowired
    RealTimeClient client;
    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;
    @Autowired
    HistoryDataDao historyDataDao;

    @Value("#{configProperties['scada.only.station']}")
    private String onlyStation;

    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
        return null;
    }


    @Override
    public List<MeasurementValue> getMeasurementValue(List<MeasurementInfo> measurements) {

        List<MeasurementValue> ret = new ArrayList<>();

        for (MeasurementInfo measurementInfo : measurements) {
//            DeviceTemplate template = psrTemplateMappingDao.getDeviceTemplateByPSRID(measurementInfo.getPsrID());
            PackageTransformer packageTransformer = scadaPSRService.getPackageTransformer(onlyStation);
            DeviceTemplate transformerTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID(packageTransformer.getId());
            DeviceTemplateData templateDataForData = transformerTemplate.getDeviceTemplateData(3, 3);
            String measureId = scadaMeasurementService.getMeasurementID(packageTransformer.getId(),templateDataForData);

            Calendar calendar = Calendar.getInstance();
            int month = (calendar.get(Calendar.YEAR) - 1970) * 12 + calendar.get(Calendar.MONTH)+1;
            String year = Integer.toString(calendar.get(Calendar.YEAR));
            String yearMonth = year + Integer.toString(month);
            //    List<HistoryDataValue> historyDataValues = historyDataService.getChartHistroyDataByEqual("monthkwh" + year, measureId, month);
            int days = (int) ((calendar.getTime().getTime() / 1000 / 3600 + 8) / 24);
            List<HistoryDataValue> historyDataValues = historyDataDao.getHistoryData("daykwh" + yearMonth, measureId, days,79);

            List<MeasurementInfo> chartMeasures = new ArrayList<>();
            List<BasePSR> inverters = scadaPSRService.getInverter(onlyStation);
            for (BasePSR inverter : inverters){
                DeviceTemplate inverterTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID(inverter.getId());
                DeviceTemplateData inverterTemplateData = inverterTemplate.getDeviceTemplateData(3, 0);
                String inverterId = scadaMeasurementService.getMeasurementID(inverter.getId(),inverterTemplateData);
                MeasurementInfo chartMeasure = new MeasurementInfo();
                chartMeasure.setPsrID(inverter.getId());
                chartMeasure.setTemplateIndex(0);
                chartMeasure.setMagic(inverterTemplateData.getMagic());
                chartMeasure.setMeasurementType(3);
                chartMeasure.setMeasurementID(inverterId);
                chartMeasures.add(chartMeasure);
            }

            List<MeasurementValue> chartValues = client.getMeasurementValue(chartMeasures);
            Double power = 0d;

            for (MeasurementValue measurementValue : chartValues){
                power += measurementValue.getData();
            }

            if (historyDataValues.isEmpty() && chartValues.isEmpty()){
                MeasurementValue mv = new MeasurementValue(0d, "N/A");
                mv.setKey(measurementInfo.getMagic() + measurementInfo.getPsrID());
                ret.add(mv);

            }else{
                for (HistoryDataValue historyDataValue : historyDataValues){
                    power += historyDataValue.getData();
                }

                MeasurementValue mv = new MeasurementValue(98d, null);
                mv.setKey(measurementInfo.getMagic() + measurementInfo.getPsrID());
                ret.add(mv);
            }
        }
        return ret;
    }
}
