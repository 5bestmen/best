package com.corsair.service;

import com.corsair.dao.NetElectricityDao;
import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.entity.NetElectricity;
import com.corsair.measurement.GeneralMeasurementValue;
import com.corsair.measurement.HistoryDataValue;
import com.corsair.measurement.MeasurementValue;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

/**
 * Created by zjq on 2015/12/17.
 */

@Service("magicValueService")
public class MagicValueService {

    @Autowired
    GeneralMeasurementValue generalMeasurementValue;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;
    @Autowired
    HistoryDataService historyDataService;
    @Autowired
    NetElectricityDao netElectricityDao;

    private  static final int ValuePoint = 240;
    private  static final int Wp = 120;
    private  static final int I0 = 1000;
    private List<Double> powerDatas = new ArrayList();
    private List<Double> irradiationDatas = new ArrayList();


    private  Double dayBeginGenerated = null;
    private  Double monthGenerated = null;
    private  Double systemEfficiency = null;
    private  Double yesterdayGenerated = null;
    private  Double totalGenerated = null;

    public Double getYesterdayGenerated() {
        return yesterdayGenerated;
    }

    public void setYesterdayGenerated(Double yesterdayGenerated) {
        this.yesterdayGenerated = yesterdayGenerated;
    }

    public Double getSystemEfficiency() {
        return systemEfficiency;
    }

    public void setSystemEfficiency(Double systemEfficiency) {
        this.systemEfficiency = systemEfficiency;
    }

    public Double getDayBeginGenerated() {
        return dayBeginGenerated;
    }

    public void setDayBeginGenerated(Double dayBeginGenerated) {
        this.dayBeginGenerated = dayBeginGenerated;
    }

    public Double getMonthGenerated() {
        return monthGenerated;
    }

    public void setMonthGenerated(Double monthGenerated) {
        this.monthGenerated = monthGenerated;
    }

    public Double getTotalGenerated() {
        return totalGenerated;
    }

    public void setTotalGenerated(Double totalGenerated) {
        this.totalGenerated = totalGenerated;
    }

    public List<Double> getPowerDatas(){
        return powerDatas;
    }

//    @Scheduled(cron="0 10 01 * * ?" )//每天凌晨1:10触发
//    @Scheduled(fixedDelay = 15000)
    public void RefreshYesterdayEfficiencyData() {//刷新昨日系统效率
//        DeviceTemplate deviceTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID("0022oi815p8cmc");
//        DeviceTemplateData deviceTemplateData = deviceTemplate.getDeviceTemplateDataByName("transboxPtotal");
//        String measureId = scadaMeasurementService.getMeasurementID("0022oi815p8cmc", deviceTemplateData);
//        MeasurementInfo measurementInfoPower = new MeasurementInfo(measureId);
//
//        DeviceTemplate deviceTemplateWeather = psrTemplateMappingDao.getDeviceTemplateByPSRID("0029oi815ih9f1");
//        DeviceTemplateData deviceTemplateDataWeather = deviceTemplateWeather.getDeviceTemplateDataByName("totalIrradiation");
//        String measureIdWeather = scadaMeasurementService.getMeasurementID("0029oi815ih9f1", deviceTemplateDataWeather);
//        MeasurementInfo measurementInfoWeather = new MeasurementInfo(measureIdWeather);
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        Calendar todayNow = Calendar.getInstance();
        todayNow.set(Calendar.DAY_OF_MONTH, todayNow.get(Calendar.DAY_OF_MONTH) - 1);
        int beginMinutes = todayNow.get(Calendar.HOUR_OF_DAY) * 60 + todayNow.get(Calendar.MINUTE) - 60;
        int endMinutes = todayNow.get(Calendar.HOUR_OF_DAY) * 60 + todayNow.get(Calendar.MINUTE);

        String date = dateFormat.format(todayNow.getTime());
        MeasurementValue measurementValue = generalMeasurementValue.getMeasurementValue("0022oi815p8cmc","transboxPtotal");
        MeasurementValue measurementValueIrradiation = generalMeasurementValue.getMeasurementValue("0029oi815ih9f1","totalIrradiation");

        double averageValue = 0d;
        double actualPower = 0d;
        double theoryPower = 0d;
        if (powerDatas.size() < ValuePoint) {
            powerDatas.add(measurementValue.getData());
            irradiationDatas.add(measurementValueIrradiation.getData());
        } else {
            //获取一小时最大峰值功率
            Double maxPower = 0d;
            for (Double currentPower:powerDatas ) {
                if (currentPower > maxPower) {
                    maxPower = currentPower;
                }
            }
            powerDatas.clear();

            Double sumIrradiationValues = 0d;
            if (!irradiationDatas.isEmpty()){
                for (Double irradiation : irradiationDatas) {
                    sumIrradiationValues += irradiation;
                }
                averageValue = sumIrradiationValues / 60;
            }
            irradiationDatas.clear();

//

            //获取理论值,
   //         List<HistoryDataValue> historyDataIrradiationValues = historyDataService.getHistoryData("0029oi815ih9f1", "totalIrradiation", date, beginMinutes, endMinutes, HistoryDataService.DataType.REALTIME_VALUE);
//获取昨日最大峰值功率
//            List<HistoryDataValue> yesterdayPowers = historyDataService.getHistoryData("0022oi815p8cmc", "transboxPtotal", date, beginMinutes, endMinutes, HistoryDataService.DataType.REALTIME_VALUE);
//            Double maxPower = 0d;
//            for (HistoryDataValue historyDataValue : yesterdayPowers) {
//                Double currentPower = historyDataValue.getData();
//                if (currentPower > maxPower) {
//                    maxPower = currentPower;
//                }
//            }

//            Double sumIrradiationValues = 0d;
//            int dataNum = historyDataIrradiationValues.size();
//            for (int i = 0; i < dataNum; i++) {
//                sumIrradiationValues += historyDataIrradiationValues.get(i).getData();
//            }
//            if (dataNum != 0){
//                averageValue = sumIrradiationValues / dataNum;
//            }else{
//                averageValue = 0;
//            }

            theoryPower = averageValue * maxPower / I0 * 1 / 1000;
            List<HistoryDataValue> historyDataValues = historyDataService.getHistoryData("0022oi815p8cmc", "transboxPtFawardGen", date, beginMinutes, endMinutes, HistoryDataService.DataType.KWH_VALUE);
            actualPower = historyDataValues.get(historyDataValues.size() - 1).getData() - historyDataValues.get(0).getData();

            setSystemEfficiency(theoryPower == 0 ? 0 :actualPower / theoryPower * 100);
        }

    }

    //每天0点30分更新当天电表起始读数
        @Scheduled(cron="0 30 00 * * ?" )//每天凌晨00:30触发
    public void RefreshDayBeginGeneratedData(){

        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        Calendar todayNow = Calendar.getInstance();
        int beginMinutes = 0;
        int endMinutes = todayNow.get(Calendar.HOUR_OF_DAY) * 60 + todayNow.get(Calendar.MINUTE);

        String date = dateFormat.format(todayNow.getTime());
        List<HistoryDataValue>  historyDataValues = historyDataService.getHistoryData("0052oi815tbsbl","transboxMeaForwardPTotal",date,beginMinutes,endMinutes,HistoryDataService.DataType.KWH_VALUE);

        if (!historyDataValues.isEmpty()){
           // setTotalGenerated(historyDataValues.get(historyDataValues.size()-1).getData());
            setDayBeginGenerated(historyDataValues.get(0).getData());
        }
    }

//    @Scheduled(fixedDelay = 10000)
    public void RefreshMonthGeneratedData(){

        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/01 HH:mm:ss");
        SimpleDateFormat dateFormatToday = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        Calendar todayNow = Calendar.getInstance();
        int beginMinutes = 0;
        int endMinutes = todayNow.get(Calendar.HOUR_OF_DAY) * 60 + todayNow.get(Calendar.MINUTE);

        String dateBegin = dateFormat.format(todayNow.getTime());
        String dateToday = dateFormatToday.format(todayNow.getTime());
        List<HistoryDataValue>  historyDataValuesBegin = historyDataService.getHistoryData("0052oi815tbsbl","transboxMeaForwardPTotal",dateBegin,beginMinutes,endMinutes,HistoryDataService.DataType.KWH_VALUE);
        List<HistoryDataValue>  historyDataValuesEnd = historyDataService.getHistoryData("0052oi815tbsbl","transboxMeaForwardPTotal",dateToday,beginMinutes,endMinutes,HistoryDataService.DataType.KWH_VALUE);

        if (!historyDataValuesBegin.isEmpty() && !historyDataValuesEnd.isEmpty()){
            setMonthGenerated(historyDataValuesBegin.get(historyDataValuesEnd.size() - 1).getData() - historyDataValuesBegin.get(0).getData());
        }
    }



//   @Scheduled(fixedDelay = 10000)
    public void RefreshYesterDayGridGeneratedData(){//总上网电量
        List<NetElectricity> netElectricities = netElectricityDao.getNetElectricityByStation("oi815");
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd");

        try{
            if (!netElectricities.isEmpty()){
                Date date = dateFormat.parse(netElectricities.get(0).getNetDate());
                NetElectricity retNetElectricity = netElectricities.get(0);
                for (NetElectricity netElectricity : netElectricities){
                    Date currentDate = dateFormat.parse(netElectricity.getNetDate());
                    if (currentDate.after(date)){
                        date = currentDate;
                        retNetElectricity = netElectricity;
                    }
                }
                setYesterdayGenerated(retNetElectricity.getReverseTotalKwh() * retNetElectricity.getMultiple());
            }

        }catch (ParseException e){
            e.printStackTrace();
        }
    }

//    @Scheduled(fixedDelay = 10000)
    public void test(){
        historyDataService.getHistoryDayData("0027oi815kl9ce","dc1DayGen","2016/01/22 13:43:32", HistoryDataService.DataType.DAY_ACCUMULATE);
        historyDataService.getHistoryMonthData("0027oi815kl9ce", "dc1DayGen", "2016/01/22 13:43:32", HistoryDataService.DataType.DAY_ACCUMULATE);
        historyDataService.getHistoryYearData("0027oi815kl9ce", "dc1DayGen", "2016/01/22", HistoryDataService.DataType.DAY_ACCUMULATE);

    }

}
