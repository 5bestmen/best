package com.corsair.service;

import com.corsair.dao.HistoryDataDao;
import com.corsair.dao.HistoryValueDao;
import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.measurement.HistoryDataValue;
import com.corsair.measurement.RealTimeClient;
import com.corsair.measurement.history.HistoryValue;
import org.apache.commons.lang3.tuple.ImmutablePair;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

/**
 * Created by zjq on 2015/12/18.
 */
@Service("historyDataService")
public class HistoryDataService {

    @Autowired
    RealTimeClient client;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;
    @Autowired
    HistoryCalculateValueService historyCalculateValueService ;
    @Autowired
    HistoryDataDao historyDataDao;
    @Autowired
    HistoryValueDao historyValueDao;

    public enum DataType{
        REALTIME_VALUE(1),//实时值
        DEAD_VALUE(4),//死数
        TERMINAL_ERROR(5),//终端故障
        KWH_VALUE(52),//电度表码
        DAY_ACCUMULATE(79);//日累计电量

        private int value;
        private DataType(int value) {
            this.value = value;
        }

        public int getValue(){
            return  value;
        }
    }

    /**
     * 请求某时段统计数据
     *
     * @param psrId     设备id.
     * @param dataName  数据名，对应deviceTemplateData有name字段.
     * @param timeBegin 开始时间,格式yyyy-MM-dd hh24:mm:ss.
     * @param timeEnd   结束时间,格式yyyy-MM-dd hh24:mm:ss.
     * @param step      步长
     * @return 返回某时段数据, 如果没有数据，则为空，如果出错，为null
     */
    public List<HistoryValue> getHistoryStepData(String psrId, String dataName, String timeBegin, String timeEnd, Integer step) {

        DeviceTemplate deviceTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrId);
        if (deviceTemplate == null) {
            return null;
        }

        DeviceTemplateData deviceTemplateData = deviceTemplate.getDeviceTemplateDataByName(dataName);
        if (deviceTemplateData == null) {
            return null;
        }

        ImmutablePair<String, Integer> measureIdPair = scadaMeasurementService.getMeasurementID(psrId, dataName);

        if (measureIdPair == null) {
            return null;
        }

        SimpleDateFormat dateParseMinite = new SimpleDateFormat("yyyy-MM-dd HH:mm");
        Calendar calendar = Calendar.getInstance();

        try {
            Date date = dateParseMinite.parse(timeBegin);
            calendar.setTime(date);
            if (calendar.get(Calendar.DAY_OF_MONTH) == 1 && calendar.get(Calendar.HOUR_OF_DAY) == 0 && calendar.get(Calendar.MINUTE) == 0) {//考虑跨月0点的值
                calendar.add(Calendar.MINUTE, -1);
                timeBegin = dateParseMinite.format(calendar.getTime());
            }
        } catch (ParseException e) {
            e.printStackTrace();
            return null;
        }

        List<HistoryValue> historyValues = historyValueDao.getStepHistoryData(measureIdPair.getLeft(), timeBegin, timeEnd, step, measureIdPair.getRight() == 3);
        return checkData(historyValues, measureIdPair.getLeft().trim(), measureIdPair.getRight(), timeBegin, timeEnd, step);
    }

    /**
     * 对某时段数据进行缺失处理，内部使用
     *
     * @param historyValues 需要处理的数据.
     * @param dateId        数据代码，对应YC/KWHyyyyMM中的name字段.
     * @param dateType      数据类型：2代表遥信或遥测，3代表电度.
     * @param beginTime     开始时间,格式yyyy-MM-dd hh24:mm:ss.
     * @param endTime       结束时间,格式yyyy-MM-dd hh24:mm:ss.
     * @return 返回某时段数据, 如果没有数据，则为空，如果出错，为null
     */
    private List<HistoryValue> checkData(List<HistoryValue> historyValues, String dateId, Integer dateType, String beginTime, String endTime, Integer step) {

        SimpleDateFormat dateParseMinite = new SimpleDateFormat("yyyy-MM-dd HH:mm");
        Calendar calendar = Calendar.getInstance();
        List<HistoryValue> retHistoryValues = new ArrayList<>();
        int modBase = step;

        try {
            Date dateBegin = dateParseMinite.parse(beginTime);
            Date dateEnd = dateParseMinite.parse(endTime);
            calendar.setTime(dateBegin);

            while (calendar.get(Calendar.MINUTE) % modBase != 0) {
                calendar.add(Calendar.MINUTE, 1);
            }

            int listPos = 0;
            int end = historyValues.size();

            while (!calendar.getTime().after(dateEnd)) {
                String currentTimeStr = dateParseMinite.format(calendar.getTime());

                if (listPos < end) {//数据列表中还有数据
                    HistoryValue historyValue = historyValues.get(listPos);//获取数据
                    String actualTimeStr = historyValue.getDate() + historyValue.getTime();//数据对应的实际时间

                    if (!currentTimeStr.equals(actualTimeStr)) {//实际时间与期望时间不一致，说明有缺失，返回队列中加入一条无效数据
                        retHistoryValues.add(new HistoryValue(dateId, currentTimeStr.substring(0, 10), currentTimeStr.substring(10), Double.NaN, false));
                    } else {//时间一致
                        retHistoryValues.add(historyValue);//将实际数据加入返回队列中
                        listPos++;
                    }

                } else {//数据列表中没有数据，但还没到截止时间，说明最后面数据有缺失，需要补足
                    retHistoryValues.add(new HistoryValue(dateId, currentTimeStr.substring(0, 10), currentTimeStr.substring(10), Double.NaN, false));
                }

                calendar.add(Calendar.MINUTE, modBase);
            }
        } catch (ParseException e) {
            e.printStackTrace();
            return null;
        }
        return retHistoryValues;
    }

//    /**
//     * 请求实时数据
//     * @param psrId 设备id.
//     * @param dataName 数据名，对应deviceTemplateData有name字段.
//     * @return 返回实时数据
//     */
//
//    public List<MeasurementValue> getRealTimeData(String psrId,String dataName){
//        DeviceTemplate deviceTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrId);
//        DeviceTemplateData deviceTemplateData = deviceTemplate.getDeviceTemplateDataByName(dataName);
//        String measureId = scadaMeasurementService.getMeasurementID(psrId,deviceTemplateData);
//
//        List<MeasurementInfo> chartMeasures = new ArrayList<>();
//        MeasurementInfo chartMeasure = new MeasurementInfo();
//        chartMeasure.setPsrID(psrId);
//        chartMeasure.setTemplateIndex(deviceTemplateData.getIndex());
//        chartMeasure.setMagic(deviceTemplateData.getMagic());
//        chartMeasure.setMeasurementType(deviceTemplateData.getType());
//        chartMeasure.setMeasurementID(measureId);
//        chartMeasures.add(chartMeasure);
//        return client.getMeasurementValue(chartMeasures);
//    }

    /**
     * 请求年统计数据
     * @param psrId 设备id.
     * @param dataName 数据名，对应deviceTemplateData有name字段.
     * @param year 年数，格式为yyyy
     * @param dataType 数据类型.具体如下
                  DataType.REALTIME_VALUE,//实时值
                  DataType.DEAD_VALUE,//死数
                  DataType.TERMINAL_ERROR,//终端故障
                  DataType.KWH_VALUE,//电度表码
                  DataType.DAY_ACCUMULATE;//日累计电量
     * @return 返回年统计数据
     */

    public List<HistoryDataValue> getHistoryYearData(String psrId,String dataName,String year,DataType dataType){

        DeviceTemplate deviceTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrId);
        DeviceTemplateData deviceTemplateData = deviceTemplate.getDeviceTemplateDataByName(dataName);
        String measureId = scadaMeasurementService.getMeasurementID(psrId,deviceTemplateData);
        String tableName = null;
        if (deviceTemplateData.getType() == 1){//遥信
               tableName = "YEARYX" + year;
        }else if (deviceTemplateData.getType() == 2){
            tableName = "YEARYC" + year;
        }else if (deviceTemplateData.getType() == 3){
            tableName = "YEARKWH" + year;
        }

        return  historyDataDao.getHistoryData(tableName, measureId, -1, dataType.getValue());

    }//请求年统计数据

    /**
     * 请求月统计数据
     * @param psrId 设备id.
     * @param dataName 数据名，对应deviceTemplateData有name字段.
     * @param date 当年截止年数月数，格式为yyyy/MM
     * @param dataType 数据类型.具体如下
                DataType.REALTIME_VALUE,//实时值
                DataType.DEAD_VALUE,//死数
                DataType.TERMINAL_ERROR,//终端故障
                DataType.KWH_VALUE,//电度表码
                DataType.DAY_ACCUMULATE;//日累计电量
     * @return 返回月统计数据
     */
    public List<HistoryDataValue> getHistoryMonthData(String psrId,String dataName,String date,DataType dataType){
        DeviceTemplate deviceTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrId);
        DeviceTemplateData deviceTemplateData = deviceTemplate.getDeviceTemplateDataByName(dataName);
        String measureId = scadaMeasurementService.getMeasurementID(psrId,deviceTemplateData);
        String tableName = null;
        SimpleDateFormat dateParse = new SimpleDateFormat("yyyy/MM");
        int month = 0;
        try {
            Date oldDate = dateParse.parse(date);
            Calendar calendar = Calendar.getInstance();
            calendar.setTime(oldDate);
            month = (calendar.get(Calendar.YEAR) - 1970) * 12 + calendar.get(Calendar.MONTH) + 1;
            String year = date.substring(0, 4);
            if (deviceTemplateData.getType() == 1){//遥信
                tableName = "MONTHYX" + year;
            }else if (deviceTemplateData.getType() == 2){
                tableName = "MONTHYC" + year;
            }else if (deviceTemplateData.getType() == 3){
                tableName = "MONTHKWH" + year;
            }

        }catch (ParseException e){
            e.printStackTrace();
            return  null;
        }

        return  historyDataDao.getHistoryData(tableName, measureId, month, dataType.getValue());

    }//请求月统计数据

    /**
     * 请求日统计数据
     * @param psrId 设备id.
     * @param dataName 数据名，对应deviceTemplateData有name字段.
     * @param date 当月截止日期，格式为yyyy/MM/dd
     * @param dataType 数据类型.具体如下
                DataType.REALTIME_VALUE,//实时值
                DataType.DEAD_VALUE,//死数
                DataType.TERMINAL_ERROR,//终端故障
                DataType.KWH_VALUE,//电度表码
                DataType.DAY_ACCUMULATE;//日累计电量
     * @return 返回日统计数据
     */
    public List<HistoryDataValue> getHistoryDayData(String psrId,String dataName,String date,DataType dataType){
        DeviceTemplate deviceTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrId);
        DeviceTemplateData deviceTemplateData = deviceTemplate.getDeviceTemplateDataByName(dataName);
        String measureId = scadaMeasurementService.getMeasurementID(psrId,deviceTemplateData);
        SimpleDateFormat dateParse = new SimpleDateFormat("yyyy/MM/dd");
        int days = 0;
        String yearMonth;
        if (date.length() > 6){
            yearMonth = date.substring(0,4) + date.substring(5,7);
            if (date.length() > 9){
                try{
                    Date dDate = dateParse.parse(date);
                    Long oldDayMillTime = dDate.getTime() ;
                    days = (int)((oldDayMillTime/1000/3600+8)/24);
                }catch (ParseException e){
                    return null;
                }
            }
        }
        else {
            return null;
        }

        String tableName = null;
        if (deviceTemplateData.getType() == 1){//遥信
            tableName = "DAYYX" + yearMonth;
        }else if (deviceTemplateData.getType() == 2){
            tableName = "DAYYC" + yearMonth;
        }else if (deviceTemplateData.getType() == 3){
            tableName = "DAYKWH" + yearMonth;
        }

        return  historyDataDao.getHistoryData(tableName, measureId, days, dataType.getValue());

    }

    /**
     * 请求某时段统计数据
     * @param psrId 设备id.
     * @param dataName 数据名，对应deviceTemplateData有name字段.
     * @param date 年月日  格式yyyy/MM/dd
     * @param beginMinite 开始分钟数.
     * @param endMinite 结束分钟数.
     * @param dataType 数据类型.具体如下
                  DataType.REALTIME_VALUE,//实时值
                  DataType.DEAD_VALUE,//死数
                  DataType.TERMINAL_ERROR,//终端故障
                  DataType.KWH_VALUE,//电度表码
                  DataType.DAY_ACCUMULATE;//日累计电量
     * @return 返回某时段统计数据
     */
    public List<HistoryDataValue> getHistoryData(String psrId,String dataName,String date,int beginMinite,int endMinite,DataType dataType){

        DeviceTemplate deviceTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrId);
        DeviceTemplateData deviceTemplateData = deviceTemplate.getDeviceTemplateDataByName(dataName);
        if (deviceTemplateData == null){
            return null;
        }
        Boolean isCalculated = deviceTemplateData.getCalculate();
        if (isCalculated != null && isCalculated){//需要计算的数据
            return historyCalculateValueService.getCalculateValue(psrId,dataName,date,beginMinite,endMinite,dataType);
        }
        String measureId = scadaMeasurementService.getMeasurementID(psrId,deviceTemplateData);
        SimpleDateFormat dateParse = new SimpleDateFormat("yyyy/MM/dd");
        int days = 0;
        String yearMonth;
        if (date.length() > 6){
            yearMonth = date.substring(0,4) + date.substring(5,7);
            if (date.length() > 9){
                try{
                    Date dDate = dateParse.parse(date);
                    Long oldDayMillTime = dDate.getTime() ;
                    days = (int)((oldDayMillTime/1000/3600+8)/24);

                }catch (ParseException e){
                    return null;
                }
            }
        }
        else{
            return null;
        }

        String tableName = null;
        if (deviceTemplateData.getType() == 1){//遥信
            tableName = "YX" + yearMonth;
        }else if (deviceTemplateData.getType() == 2){
            tableName = "YC" + yearMonth;
        }else if (deviceTemplateData.getType() == 3){
            tableName = "KWH" + yearMonth;
        }

        return  historyDataDao.getHistoryData(tableName, measureId, days, beginMinite, endMinite, dataType.getValue());

    }

//    public HistoryDataValue getHistoryMaxData(String psrId,String dataName,String date,int beginMinite,int endMinite,DataType dataType){
//        DeviceTemplate deviceTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrId);
//        DeviceTemplateData deviceTemplateData = deviceTemplate.getDeviceTemplateDataByName(dataName);
//        if (deviceTemplateData == null){
//            return null;
//        }
//
//        String measureId = scadaMeasurementService.getMeasurementID(psrId,deviceTemplateData);
//        SimpleDateFormat dateParse = new SimpleDateFormat("yyyy/MM/dd");
//        int days = 0;
//        String yearMonth = date.substring(0,4) + date.substring(5,7);
//        try{
//            Date dDate = dateParse.parse(date);
//            Long oldDayMillTime = dDate.getTime() ;
//            days = (int)((oldDayMillTime/1000/3600+8)/24);
//
//        }catch (ParseException e){
//            return null;
//        }
//
//        String tableName = null;
//        if (deviceTemplateData.getType() == 1){//遥信
//            tableName = "YX" + yearMonth;
//        }else if (deviceTemplateData.getType() == 2){
//            tableName = "YC" + yearMonth;
//        }else if (deviceTemplateData.getType() == 3){
//            tableName = "KWH" + yearMonth;
//        }
//        return historyDataService.getHistoryMaxData(tableName,measureId,days,beginMinite,endMinite,dataType.getValue());
//    }


}
