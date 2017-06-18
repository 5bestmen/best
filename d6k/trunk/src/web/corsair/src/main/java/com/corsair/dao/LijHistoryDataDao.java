package com.corsair.dao;

import com.corsair.device.BasePSR;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.measurement.HistoryDataValue;
import com.corsair.measurement.MeasurementInfo;
import com.corsair.measurement.MeasurementValue;
import com.corsair.measurement.RealTimeClient;
import com.corsair.service.ScadaMeasurementService;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowCallbackHandler;
import org.springframework.stereotype.Repository;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

/**
 * Created by 洪祥 on 2016/3/10.
 */
@Repository("lijHistoryDataDao")
public class LijHistoryDataDao {

    @Autowired
    private JdbcTemplate jdbcTemplate;
    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    DeviceTemplateDao deviceTemplateDao;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;
    @Autowired
    RealTimeClient realTimeClient;

    @Value("#{configProperties['scada.only.station']}")
    private String onlyStation;

    //获取累积发电量图表的数据
    public List<List<String>> getTotalKwhData(String date) throws ParseException {

        List<List<String>> ret = new ArrayList<>();

        int year = Integer.parseInt(date.substring(0, 4));
        Double tmp = 0.0;
        for(int i = 2016 ; i <= year; i++){
            List<String> oneRec = new ArrayList<>();
            if (i < year){
                oneRec.add(i + "年");
                //获取第i年最后一天发电量
                Double tmpValue = getLastYearEndDayKwh(getYearDate(i));
                Double value = tmpValue - tmp;
                oneRec.add(value.toString());
                ret.add(oneRec);
                tmp = tmpValue;
            }else {
                oneRec.add(i + "年");
                Double value = getThisMonthEndDayKwh() - tmp;
                oneRec.add(value.toString());
                ret.add(oneRec);
            }
        }

        return ret;
    }

    //获取年发电量的图表数据
    public List<List<String>> getYearKwhData(String date) throws ParseException {
        List<List<String>> ret = new ArrayList<>();

        SimpleDateFormat dateParse = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");

        Date oldDate = dateParse.parse(date);
        Long oldDayMillTime = oldDate.getTime();
        String year = date.substring(0, 4);
        String yearMonth = date.substring(0, 4) + date.substring(5, 7);
        int days = (int) ((oldDayMillTime / 1000 / 3600 + 8) / 24);
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(oldDate);
        int daysBegin = days - (calendar.get(Calendar.DAY_OF_MONTH) - 1);
        int month = calendar.get(Calendar.MONTH) + 1; //月份数

        Double tmp = 0.0;
        for(int i = 1; i <= month; i++){
            List<String> oneRec = new ArrayList<>();
            if(i < month){
                oneRec.add(i + "月");
                //获取第i月最后一天发电量
                Double tmpValue = getLastMonthEndDayKwh(getMonthDate(i+1));
                Double value = tmpValue - tmp;
                oneRec.add(value.toString());
                ret.add(oneRec);
                tmp = tmpValue;
            }else {
                oneRec.add(i + "月");
                Double value = getThisMonthEndDayKwh() - tmp;
                oneRec.add(value.toString());
                ret.add(oneRec);
            }

        }

        return ret;
    }

    //获取月份日期
    private String getMonthDate(int month){
        String date = getStringTime();
        String oldMonth = date.substring(4,7);
        String newMonth = "/" + 0 + month;
        return date.replaceFirst(oldMonth,newMonth);
    }

    //获取年份日期
    private String getYearDate(int year){
        String date = getStringTime();
        String oldYearMonth = date.substring(0,7);
        String newYearMonth = year + "/" + "01";
        return date.replaceFirst(oldYearMonth,newYearMonth);
    }


    //获取月发电量的图表数据
    public List<List<String>> getMonthKwhData(String date) throws ParseException {

        List<List<String>> ret = new ArrayList<>();

        SimpleDateFormat dateParse = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        SimpleDateFormat dateFormatDay = new SimpleDateFormat("yyyy/MM/dd");
        SimpleDateFormat dateFormatMonth = new SimpleDateFormat("yyyy/MM");

        Date oldDate = dateParse.parse(date);
        Long oldDayMillTime = oldDate.getTime();
        String year = date.substring(0, 4);
        String yearMonth = date.substring(0, 4) + date.substring(5, 7);
        int days = (int) ((oldDayMillTime / 1000 / 3600 + 8) / 24);
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(oldDate);
        int daysBegin = days - (calendar.get(Calendar.DAY_OF_MONTH) - 1);
        int month = (calendar.get(Calendar.YEAR) - 1970) * 12 + calendar.get(Calendar.MONTH) + 1;

        String tableName  = "kwh"+yearMonth;

        List<List<HistoryDataValue>> totalValues = new ArrayList<>();
        int num = 0;

        //第一步，获取所有逆变器的本月所有天的电度遥测表码值
        for (String id : getInvertersTotalKwhMeanId()){
            List<HistoryDataValue> historyDataValues = getMonthHisData(tableName,id,days,52);
            num = historyDataValues.size();
            totalValues.add(historyDataValues);
        }

        //第二步，将逆变器的所有单日电度值累加
        Double[] datas = new Double[num];
        Integer[] sdates = new Integer[num];
        for (int i = 0; i < num; i++){
            datas[i] = 0.0;
            sdates[i] = 0;
        }
        for(int i = 0; i < num; i++){
            for(List<HistoryDataValue> historyDataValues : totalValues){
                datas[i] += historyDataValues.get(i).getData();
                sdates[i] = historyDataValues.get(i).getSdate();
            }
        }

        List<HistoryDataValue> totalKwh = new ArrayList<>();
        //第三步，填充逆变器日电度的累加值
        for(int i = 0; i < num ; i++){
            HistoryDataValue historyDataValue = new HistoryDataValue();
            historyDataValue.setSdate(sdates[i]);
            historyDataValue.setData(datas[i]);
            totalKwh.add(historyDataValue);
        }

        //第四步，返回图表数据
        int monthDay = days - daysBegin + 1; //本月天数
        for (int i = 1 ; i < monthDay; i++){
            List<String> oneRec = new ArrayList<>();
            Boolean isHave = false;
            for(HistoryDataValue value : totalKwh){
                if (daysBegin == value.getSdate()){
                    isHave = true;
                }
            }
            if (isHave){
                oneRec.add(i+"日");
                for(HistoryDataValue value : totalKwh){
                    if (daysBegin == value.getSdate()){
                        oneRec.add(value.getData().toString());
                    }
                }
                ret.add(oneRec);
            }else {
                oneRec.add(i+"日");
                oneRec.add("-");
                ret.add(oneRec);
            }
            daysBegin++;
        }

        return ret;
    }

    //获取截止到今天的发电量，直接读实时库啊，读啥历史表，累加逆变器好玩啊！！！
    public Double getThisMonthEndDayKwh() throws ParseException {
        MeasurementInfo chartMeasure = new MeasurementInfo();
        chartMeasure.setPsrID("00521c0727v074");
        chartMeasure.setTemplateIndex(0);
        chartMeasure.setMeasurementType(3);
        chartMeasure.setMeasurementID("00521c0727v074po2hc0206");
        MeasurementValue chartValue = realTimeClient.getMeasurementValue(chartMeasure);
        return chartMeasure == null ? null : chartValue.getData();
    }

    //获取上个月最后一天的发电量
    public Double getLastMonthEndDayKwh(String date) throws ParseException {
        SimpleDateFormat dateParse = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        Date oldDate = dateParse.parse(date);
        Long oldDayMillTime = oldDate.getTime();
        String yearMonth = "";
        //如果本月是一月份，那么上个月是上年度的12月份
        if(Integer.parseInt(date.substring(5, 7)) == 1){
            yearMonth = (Integer.parseInt(date.substring(0, 4)) - 1) + "12";
        }else{
            yearMonth = date.substring(0, 4) + "0" + (Integer.parseInt(date.substring(5, 7)) - 1);
        }

        //2015年12月份发电量为0，特殊处理
        if (yearMonth.equals("201512")){
            return 0.0;
        }
        int thisDay = (int) ((oldDayMillTime / 1000 / 3600 + 8) / 24);
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(oldDate);
        //上月最后一天
        int lastMonthEndDay = thisDay - (calendar.get(Calendar.DAY_OF_MONTH) - 1) -1;

        String tableName  = "kwh"+yearMonth;

        List<List<HistoryDataValue>> totalValues = new ArrayList<>();
        int num = 0;

        //第一步，获取所有逆变器上月最后一天的电度遥测表码值
        for (String id : getInvertersTotalKwhMeanId()){
            List<HistoryDataValue> historyDataValues = getLastMonthEndHisData(tableName, id, lastMonthEndDay, 52);
            num = historyDataValues.size();
            totalValues.add(historyDataValues);
        }

        //第二步，将逆变器的日电度值累加
        Double[] datas = new Double[num];
        Integer[] sdates = new Integer[num];
        for (int i = 0; i < num; i++){
            datas[i] = 0.0;
            sdates[i] = 0;
        }
        for(int i = 0; i < num; i++){
            for(List<HistoryDataValue> historyDataValues : totalValues){
                datas[i] += historyDataValues.get(i).getData();
                sdates[i] = historyDataValues.get(i).getSdate();
            }
        }
        return datas[0];
    }

    //获取上个月最后一天的电度
    private List<HistoryDataValue> getLastMonthEndHisData(String tableName,String measureId,int dateDaysEnd, int type){
        final List<HistoryDataValue> historyDatas = new ArrayList<>();

        String sql = "select sdate,data from " + tableName + " where name = '" + measureId + "' and flag=" + Integer.toString(type) +" and sdate = " + Integer.toString(dateDaysEnd) + " and time = 885 " + " order by sdate ASC ";

        if (dateDaysEnd == -1) {
            sql = "select sdate,data from " + tableName + " where name = '" + measureId + "'  and flag=" + Integer.toString(type) +" order by sdate ASC";
        }

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    HistoryDataValue historyDataValue = new HistoryDataValue();
                    historyDataValue.setSdate(resultSet.getInt("sdate"));
                    historyDataValue.setData(resultSet.getDouble("data"));
                    historyDatas.add(historyDataValue);
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return historyDatas;
    }

    //获取上一年最后一天的发电量
    public Double getLastYearEndDayKwh(String date) throws ParseException {
        SimpleDateFormat dateParse = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        Date oldDate = dateParse.parse(date);
        Long oldDayMillTime = oldDate.getTime();
        String yearMonth = "";

        yearMonth = (Integer.parseInt(date.substring(0, 4)) -1) + "0" + "12";

        int thisDay = (int) ((oldDayMillTime / 1000 / 3600 + 8) / 24);
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(oldDate);
        //上个月最后一天
        int lastMonthEndDay = thisDay - (calendar.get(Calendar.DAY_OF_MONTH) - 1) -1;

        String tableName  = "kwh"+yearMonth;

        List<List<HistoryDataValue>> totalValues = new ArrayList<>();
        int num = 0;

        //第一步，获取所有逆变器上月最后一天的电度遥测表码值
        for (String id : getInvertersTotalKwhMeanId()){
            List<HistoryDataValue> historyDataValues = getLastMonthEndHisData(tableName, id, lastMonthEndDay, 52);
            num = historyDataValues.size();
            totalValues.add(historyDataValues);
        }

        //第二步，将逆变器的日电度值累加
        Double[] datas = new Double[num];
        Integer[] sdates = new Integer[num];
        for (int i = 0; i < num; i++){
            datas[i] = 0.0;
            sdates[i] = 0;
        }
        for(int i = 0; i < num; i++){
            for(List<HistoryDataValue> historyDataValues : totalValues){
                datas[i] += historyDataValues.get(i).getData();
                sdates[i] = historyDataValues.get(i).getSdate();
            }
        }
        return datas[0];
    }


    //坑先填上再说
    private List<HistoryDataValue> getMonthHisData(String tableName,String measureId,int dateDaysEnd, int type){
        final List<HistoryDataValue> historyDatas = new ArrayList<>();

        String sql = "select sdate,data from " + tableName + " where name = '" + measureId + "' and flag=" + Integer.toString(type) +" and sdate <= " + Integer.toString(dateDaysEnd) + " and time = 1440 " + " order by sdate ASC ";

        if (dateDaysEnd == -1) {
            sql = "select sdate,data from " + tableName + " where name = '" + measureId + "'  and flag=" + Integer.toString(type) +" order by sdate ASC";
        }

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        HistoryDataValue historyDataValue = new HistoryDataValue();
                        historyDataValue.setSdate(resultSet.getInt("sdate"));
                        historyDataValue.setData(resultSet.getDouble("data"));
                        historyDatas.add(historyDataValue);
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

        return historyDatas;
    }

    //返回有所逆变器总发电量的量测ID
    private List<String> getInvertersTotalKwhMeanId(){
        List<String> ret = new ArrayList<>();
        List<String> inverterIds = getInverters();
        DeviceTemplateData deviceTemplateData = getDeviceTemplateData();
        for(String id : inverterIds){
            //id:设备代码 deviceTemplateData：设备模板数据 hyq
            String meanId =  scadaMeasurementService.getMeasurementID(id, deviceTemplateData);
            ret.add(meanId);
        }
        return ret;
    }

    //返回所有逆变器的ID
    private List<String> getInverters(){
        List<String> ret = new ArrayList<>();
        List<BasePSR> inverters = scadaPSRService.getInverter(onlyStation);
        for (BasePSR inverter : inverters){
            ret.add(inverter.getId());
        }
        return ret;
    }

    //找出逆变器总发电量量测模板数据,我也不想把数据写死,但是写死真的很爽，阿门
    private DeviceTemplateData getDeviceTemplateData(){
        DeviceTemplate deviceTemplate = deviceTemplateDao.findById("inverter");
        //（type=2,index=3的逆变器总发电量测点） hyq
        DeviceTemplateData deviceTemplateData = deviceTemplate.getDeviceTemplateData(3, 2);
        return deviceTemplateData;
    }

    private String getStringTime(){
        SimpleDateFormat df = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");//设置日期格式
        return df.format(new Date());
    }
}
