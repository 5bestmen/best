package com.corsair.service;

import com.corsair.dao.HistoryDataDao;
import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.dao.ScadaYCDao;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.measurement.HistoryDataValue;
import com.corsair.measurement.MeasurementInfo;
import com.corsair.measurement.MeasurementValue;
import com.corsair.measurement.RealTimeClient;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

/**
 * Created by zjq on 2015/11/12.
 */
@Service("measurementGenerateService")
public class MeasurementGenerateService{
    @Autowired
    ScadaYCDao scadaYCDao;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;
    @Autowired
    ScadaPSRService scadaPsrService;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;
    @Autowired
    HistoryDataDao historyDataDao;
    @Autowired
    RealTimeClient client;

    public List<String[]> getChart(String psrID, Integer index, String dateType, String date, String magic, String session) {

        DeviceTemplate template = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrID);
        DeviceTemplateData templateDataForData = template.getDeviceTemplateData(3, index);

        SimpleDateFormat dateParse = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        SimpleDateFormat dateParseMonthBeginDay = new SimpleDateFormat("yyyy/MM/01 HH:mm:ss");
        SimpleDateFormat dateFormatDay = new SimpleDateFormat("yyyy/MM/dd");
        SimpleDateFormat dateFormatMonth = new SimpleDateFormat("yyyy/MM");

        String measureId = scadaMeasurementService.getMeasurementID(psrID, templateDataForData);
        List<String[]> retList = new ArrayList<>();

        try {
            Date oldDate = dateParse.parse(date);
            Long oldDayMillTime = oldDate.getTime();
            String year = date.substring(0, 4);
            String yearMonth = date.substring(0, 4) + date.substring(5, 7);
            int days = (int) ((oldDayMillTime / 1000 / 3600 + 8) / 24);
            Calendar calendar = Calendar.getInstance();
            calendar.setTime(oldDate);
            int daysBegin = days - (calendar.get(Calendar.DAY_OF_MONTH) - 1);
            int month = (calendar.get(Calendar.YEAR) - 1970) * 12 + calendar.get(Calendar.MONTH) + 1;
            int monthBegin = month - calendar.get(Calendar.MONTH);
            int minutes = calendar.get(Calendar.HOUR_OF_DAY) * 60 + calendar.get(Calendar.MINUTE);


            if ("day".equals(dateType)) {
                List<HistoryDataValue> historyDataValues = historyDataDao.getHistoryData("KWH" + yearMonth, measureId, days, 0, minutes,52);
                String currentTimeString;
                Double[] hourPoint = new Double[24];

                int currentHour = calendar.get(Calendar.HOUR_OF_DAY);
                for (int hour = 0; hour <= currentHour; hour++) {
                    int i = 0;
                    for (i = 0; i < historyDataValues.size(); i++) {
                        int oldDayMinuteTime = historyDataValues.get(i).getTime();
                        Double hourPointVale = historyDataValues.get(i).getData();
                        int pointValue = 0;
                        if (hour == 0){
                            pointValue = 15;
                        }else{
                            pointValue = hour * 60;
                        }
                        if (oldDayMinuteTime == pointValue) {
                            hourPoint[hour] = hourPointVale;
                            break;
                        }
                    }
                }
                for (int valuePointpos = 0; valuePointpos < currentHour; valuePointpos++){
                        currentTimeString = Integer.toString(valuePointpos) + "-" + Integer.toString(valuePointpos+1) + "时";
                        String[] retData = new String[2];
                        retData[0] = currentTimeString;
                    if (hourPoint[valuePointpos] != null && hourPoint[valuePointpos+1] != null) {
                        Double value = hourPoint[valuePointpos + 1] - hourPoint[valuePointpos];
                        retData[1] = String.format("%.2f", (value > 0 ? value : 0));
                    }else{
                        retData[1] = "-";
                    }
                    retList.add(retData);
                }
            }
            if ("month".equals(dateType)) {
                List<HistoryDataValue> historyDataValues = historyDataDao.getHistoryData("DAYKWH" + yearMonth, measureId, days, 79);
                String currentTimeString;
                if (historyDataValues.isEmpty()){
                    for (int day = daysBegin; day <= days; day++){
                        oldDayMillTime = (long)60 * 1000 * day * 60 * 24;
                        oldDate.setTime(oldDayMillTime);
                        currentTimeString = dateFormatDay.format(oldDate);
                        String[] retData = new String[2];
                        retData[0] = currentTimeString;
                        retData[1] = "-";
                        retList.add(retData);
                    }
                }else {
                    int dayPos = 0;
                    int dayNum = historyDataValues.size();
                    for (int day = daysBegin; day <= days; day++){
                        String[] retData = new String[2];
                        oldDayMillTime = (long)60 * 1000 * day * 60 * 24;
                        oldDate.setTime(oldDayMillTime);
                        currentTimeString = dateFormatDay.format(oldDate);
                        retData[0] = currentTimeString;
                        retData[1] = "-";

                        if (dayPos < dayNum){
                            HistoryDataValue historyDataValue = historyDataValues.get(dayPos);
                            Double currentValue = historyDataValue.getData();
                            if (day == historyDataValue.getSdate()) {
                                retData[1] = String.format("%.2f", currentValue);
                                dayPos++;
                            }
                        }

                        retList.add(retData);
                    }
                  }

            } else if ("year".equals(dateType)) {

                List<HistoryDataValue> historyDataValues = historyDataDao.getHistoryData("MONTHKWH" + year, measureId, month, 79);
                String currentTimeString = null;

                if (historyDataValues.isEmpty()){
                    for (int monthIndex = monthBegin; monthIndex < month; monthIndex++){
                        int currentYear = (monthIndex - 1)/12 + 1970;
                        int actualMonth = (monthIndex - 1)%12;
                        calendar.set(Calendar.MONTH,actualMonth);
                        calendar.set(Calendar.YEAR,currentYear);
                        currentTimeString = dateFormatMonth.format(calendar.getTime());
                        String[] retData = new String[2];
                        retData[0] = currentTimeString;
                        retData[1] = "-";
                        retList.add(retData);
                    }
                }else{
                    int monthPos = 0;
                    int monthNum = historyDataValues.size();
                    for (int monthIndex = monthBegin; monthIndex <= month; monthIndex++){
                        int currentYear = (monthIndex - 1)/12 + 1970;
                        int actualMonth = (monthIndex - 1)%12;
                        calendar.set(Calendar.MONTH,actualMonth);
                        calendar.set(Calendar.YEAR,currentYear);
                        currentTimeString = dateFormatMonth.format(calendar.getTime());
                        String[] retData = new String[2];
                        retData[0] = currentTimeString;
                        retData[1] = "-";
                        if (monthPos < monthNum){
                            HistoryDataValue historyDataValue = historyDataValues.get(monthPos);
                            Double currentValue = historyDataValue.getData();
                            if (monthIndex == historyDataValue.getSdate()){
                                retData[1] = String.format("%.2f",currentValue);
                                monthPos++;
                            }
                        }

                        retList.add(retData);
                    }
                }


            } else if ("accumulate".equals(dateType)) {//累积量基于实时量

                MeasurementInfo chartMeasure = new MeasurementInfo();
                chartMeasure.setPsrID(psrID);
                chartMeasure.setTemplateIndex(index);
                chartMeasure.setMagic(templateDataForData.getMagic());
                chartMeasure.setMeasurementType(3);
                chartMeasure.setMeasurementID(measureId);
                MeasurementValue chartValue = client.getMeasurementValue(chartMeasure);
                String[] retData = new String[2];
                retData[0] = "累计";
                retData[1] = String.format("%.2f",chartValue.getData());
                retList.add(retData);
            }

        } catch (ParseException e) {
            e.printStackTrace();
        }
        return retList;
    }
}
