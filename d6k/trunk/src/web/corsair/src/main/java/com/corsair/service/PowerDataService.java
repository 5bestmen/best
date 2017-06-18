package com.corsair.service;

import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.measurement.MeasurementInfo;
import com.corsair.measurement.MeasurementValue;
import com.corsair.measurement.RealTimeClient;
import com.corsair.measurement.magic.ChartDataValue;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

/**
 * Created by zjq on 2015/11/30.
 */
@Service("powerDataService")
public class PowerDataService {

    @Autowired
    RealTimeClient client;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;

    private  static final int POWERDATANUM = 120;
    private List<ChartDataValue> powerDatas = new ArrayList();


    public List<ChartDataValue> getPowerDatas(){
        return powerDatas;
    }

    @Scheduled(fixedDelay = 2000)
    public void RefreshPowerData(){
        //todo 张云,写死的代码必须要改
        DeviceTemplate deviceTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID("0022oi815p8cmc");

        if (deviceTemplate == null) {
            return;
        }

        DeviceTemplateData deviceTemplateData = deviceTemplate.getDeviceTemplateDataByName("transboxPtotal");

        String measureId = scadaMeasurementService.getMeasurementID("0022oi815p8cmc",deviceTemplateData);
        MeasurementInfo measurementInfo = new MeasurementInfo(measureId);
        SimpleDateFormat dateFormat = new SimpleDateFormat("HH:mm:ss");
        Calendar todayNow = Calendar.getInstance();
        String date = dateFormat.format(todayNow.getTime());
        MeasurementValue measurementValue = client.getMeasurementValue(measurementInfo);
        ChartDataValue chartDataValue = new ChartDataValue();
        chartDataValue.setData(measurementValue.getData());
        chartDataValue.setDate(date);
        chartDataValue.setUnit(deviceTemplateData.getUnit());
        if (powerDatas.size() < POWERDATANUM){
            powerDatas.add(chartDataValue);
        }else{
            powerDatas.remove(0);
            powerDatas.add(chartDataValue);
        }
    }
}
