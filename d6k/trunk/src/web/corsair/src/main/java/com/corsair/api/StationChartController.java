package com.corsair.api;

import com.corsair.dao.HistoryDataDao;
import com.corsair.dao.LijHistoryDataDao;
import com.corsair.measurement.*;
import com.corsair.service.PowerDataService;
import com.corsair.service.ScadaPSRService;
import com.github.abel533.echarts.Option;
import com.github.abel533.echarts.axis.Axis;
import com.github.abel533.echarts.axis.CategoryAxis;
import com.github.abel533.echarts.json.GsonOption;
import com.github.abel533.echarts.json.GsonUtil;
import com.github.abel533.echarts.series.Bar;
import com.github.abel533.echarts.style.AreaStyle;
import javafx.scene.shape.Line;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import javax.servlet.http.HttpServletRequest;
import java.text.DecimalFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

/**
 * Created by 洪祥 on 16/1/5.
 */
@RestController
@RequestMapping(value = "/charts/stations")
public class StationChartController {
    @Autowired
    GeneralMeasurementValue generalMeasurementValue;
    @Autowired
    RealTimeClient realTimeClient;
    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    PowerDataService powerDataService;
    @Autowired
    HistoryDataDao historyDataDao;
    @Autowired
    LijHistoryDataDao lijHistoryDataDao;

    final int YEAR = 3;
    final int MONTH = 2;
    final int DAY = 1;


    @Value("#{configProperties['scada.only.station']}")
    private String onlyStation;

    final String STATIONDAYKWHID = "00521c0727v074vvd800206";

    @RequestMapping(value = "realTime", method = RequestMethod.POST)
    public ResponseEntity<String> realTimeController(HttpServletRequest request) {
        String strStationID = request.getParameter("name");
        String strDeviceID = request.getParameter("deviceId");
        String strDataType = request.getParameter("dataType");

        Option option = getRealTime();

        return new ResponseEntity<String>(GsonUtil.format(option), HttpStatus.OK);
    }

    @RequestMapping(value = "stationDayKwh", method = RequestMethod.POST)
    public ResponseEntity<String> stationPowerCurveController(HttpServletRequest request) {

        Option option = getStationDayKwhChart();

        return new ResponseEntity<String>(GsonUtil.format(option), HttpStatus.OK);
    }

    @RequestMapping(value = "stationMonthKwh", method = RequestMethod.POST)
    public ResponseEntity<String> stationMonthPowerCurveController() {

        Option option = getStationMonthKwhChart();

        return new ResponseEntity<String>(GsonUtil.format(option), HttpStatus.OK);
    }

    @RequestMapping(value = "stationYearKwh", method = RequestMethod.POST)
    public ResponseEntity<String> stationInverterGeneratedChart() {
        GsonOption option = getStationYearKwhChart(onlyStation);

        if (option == null) {
            return new ResponseEntity<String>(HttpStatus.NOT_FOUND);
        }

        return new ResponseEntity<String>(GsonUtil.format(option), HttpStatus.OK);
    }

    @RequestMapping(value = "stationTotalKwh", method = RequestMethod.POST)
    public ResponseEntity<String> stationTotalPowerCurveController() {

        Option option = getStationTotalKwhChart();

        return new ResponseEntity<String>(GsonUtil.format(option), HttpStatus.OK);//直接转换option存在属性null的问题，前端无法识别
    }

    @RequestMapping(value = "saveElecOfDay", method = RequestMethod.POST)
    public ResponseEntity<String> saveElecOfDayController(HttpServletRequest request) {
        String strLevel = request.getParameter("level");
        String strName = request.getParameter("name");
        String strDate = request.getParameter("date");

        Option option = getSaveElecOfDayChart(strLevel, strName, strDate);

        return new ResponseEntity<String>(GsonUtil.format(option), HttpStatus.OK);
    }

    @RequestMapping(value = "saveElecOfMonth", method = RequestMethod.POST)
    public ResponseEntity<String> saveElecOfMonthController(HttpServletRequest request) {

        String strLevel = request.getParameter("level");
        String strName = request.getParameter("name");
        String strDate = request.getParameter("date");

        Option option = getSaveElecOfMonthChart(strLevel, strName, strDate);

        return new ResponseEntity<String>(GsonUtil.format(option), HttpStatus.OK);
    }

    @RequestMapping(value = "saveElecOfYear", method = RequestMethod.POST)
    public ResponseEntity<String> saveElecOfYearController(HttpServletRequest request) {

        String strLevel = request.getParameter("level");
        String strName = request.getParameter("name");
        String strDate = request.getParameter("date");

        Option option = getSaveElecOfYearChart(strLevel, strName, strDate);

        return new ResponseEntity<String>(GsonUtil.format(option), HttpStatus.OK);
    }

    @RequestMapping(value = "realCurve", method = RequestMethod.POST)
    public ResponseEntity<String> getRealCurveController(HttpServletRequest request) {

        String strStationID = request.getParameter("s_id");
        String strDeviceID = request.getParameter("d_id");
        String strParaID = request.getParameter("p_id");

        Option option = getRealCurveChart(strStationID, strDeviceID, strParaID);

        return new ResponseEntity<String>(GsonUtil.format(option), HttpStatus.OK);
    }

    private GsonOption getRealTime() {
        Long dayMillTime = new Date().getTime();
        String dateTime = getStringTime();
        String year = dateTime.substring(0, 4);
        String yearMonth = dateTime.substring(0, 4) + dateTime.substring(5, 7);
        int days = (int) ((dayMillTime / 1000 / 3600 + 8) / 24);
        Calendar calendar = Calendar.getInstance();
        try {
            calendar.setTime(new SimpleDateFormat("yyyy/MM/dd HH:mm:ss").parse(dateTime));
        } catch (ParseException e) {
            e.printStackTrace();
        }
        int daysBegin = days - (calendar.get(Calendar.DAY_OF_MONTH) - 1);
        int month = (calendar.get(Calendar.YEAR) - 1970) * 12 + calendar.get(Calendar.MONTH) + 1;
        int monthBegin = month - calendar.get(Calendar.MONTH);
        int minutes = calendar.get(Calendar.HOUR_OF_DAY) * 60 + calendar.get(Calendar.MINUTE);

        List<HistoryDataValue> historyDataValues = historyDataDao.getHistoryData("HIS_AI" + year,
                STATIONDAYKWHID,
                days, 0, minutes,52);

        GsonOption dayOption = new GsonOption();
        List<Axis> categoryAxises = new ArrayList<>();
        CategoryAxis powerAxis = new CategoryAxis();
        powerAxis.setShow(true);
        categoryAxises.add(powerAxis);

        Bar bar = new Bar();
        bar.setName("日发电量");

        AreaStyle areaStyle = new AreaStyle();
        areaStyle.setType("default");
        areaStyle.setColor("#32d2c9");
        bar.itemStyle().normal().setAreaStyle(areaStyle);

        dayOption.setxAxis(categoryAxises);
        dayOption.series(bar);

        //初始化图表，避免过零点图表显示区为空
        for (int i = 6 ; i <= 20; i++) {
            powerAxis.data().add(i + "时");
            bar.data().add(0);
        }

        List<String> datas = new ArrayList<>();
        if (historyDataValues.size() == 0){
            return dayOption;
        }else {
            for(int i = 0; i < historyDataValues.size(); i++ ){
                datas.add(historyDataValues.get(i).getData().toString());
            }
        }

        //只显示6点到20点的发电量
        List<String> xValue = new ArrayList<>();
        List<String> yValue = new ArrayList<>();
        //Double tmp = Double.parseDouble(datas.get(0));
        Double tmp = 0.0;
        for (int i = 0; i < datas.size();i++){
            if (i <= 20){
                xValue.add((i + 6)+ "时");
                //Double value = Double.parseDouble(datas.get(i)) - tmp;
                Double value = Double.parseDouble(datas.get(i)) - tmp;
                String valueStr = String.format("%.1f", (value > 0 ? value : 0));
                yValue.add(valueStr);
                tmp = Double.parseDouble(datas.get(i));
            }
            //设置两头都为0，避免获取不到数据的时候纵坐标不出现
//            dayOption.getxAxis().get(0).setData(xValue);
            dayOption.getSeries().get(0).setData(yValue);
        }

        return dayOption;
    }

    /**
     * 两个时间之间的天数
     *
     * @param enddate
     * @param begindate
     * @return
     */
    public long getDays(String enddate, String begindate) {
        if (enddate == null || enddate.equals(""))
            return 0;
        if (begindate == null || enddate.equals(""))
            return 0;
        // 转换为标准时间
        SimpleDateFormat myFormatter = new SimpleDateFormat("yyyy-MM-dd");
        java.util.Date date = null;
        java.util.Date mydate = null;
        try {
            date = myFormatter.parse(enddate);
            mydate = myFormatter.parse(begindate);
        } catch (Exception e) {
        }
        long day = (date.getTime() - mydate.getTime()) / (24 * 60 * 60 * 1000);
        return day;
    }

    public long getDays() {

        // 转换为标准时间
        SimpleDateFormat myFormatter = new SimpleDateFormat("yyyy-MM-dd");
        Date date = new Date();
        java.util.Date mydate = null;
        try {
            mydate = myFormatter.parse("1970-1-1");
        } catch (Exception e) {
        }
        long day = (date.getTime() - mydate.getTime()) / (24 * 60 * 60 * 1000);

        return day;
    }

    // add by zyj 2017/2/24
    private GsonOption getSaveElecOfDayChart(String strLevel, String strName, String strDate) {

        // 计算1970-1-1到strDate的天数
        long days = getDays(strDate, "1970-1-1");

        String dateTime = getStringTime();
        String year = strDate.substring(0, 4);

        Calendar calendar = Calendar.getInstance();
        try {
            calendar.setTime(new SimpleDateFormat("yyyy/MM/dd HH:mm:ss").parse(dateTime));
        } catch (ParseException e) {
            e.printStackTrace();
        }

        List<HistoryDataValue> historyDataValues = historyDataDao.getSaveElecInfo("HIS_CALC_DS" + year, days, strLevel, strName, DAY);

        GsonOption dayOption = new GsonOption();
        List<Axis> categoryAxises = new ArrayList<>();
        CategoryAxis powerAxis = new CategoryAxis();
        powerAxis.setShow(true);
        categoryAxises.add(powerAxis);

        Bar bar = new Bar();
        bar.setName("日节电量");

        AreaStyle areaStyle = new AreaStyle();
        areaStyle.setType("default");
        areaStyle.setColor("#32d2c9");
        bar.itemStyle().normal().setAreaStyle(areaStyle);

        dayOption.setxAxis(categoryAxises);
        dayOption.series(bar);

        //初始化图表，避免过零点图表显示区为空
        for (int i = 0 ; i < historyDataValues.size(); i++) {
            powerAxis.data().add(historyDataValues.get(i).getName());
            bar.data().add(0);
        }

        List<String> datas = new ArrayList<>();
        if (historyDataValues.size() == 0){
            return dayOption;
        }else {
            for(int i = 0; i < historyDataValues.size(); i++ ){
                datas.add(historyDataValues.get(i).getData().toString());
            }
        }

        //只显示6点到20点的发电量
        List<String> yValue = new ArrayList<>();

        for (int i = 0; i < datas.size();i++){

            Double value = Double.parseDouble(datas.get(i));
            String valueStr = String.format("%.2f", value);
            yValue.add(valueStr);

            //设置两头都为0，避免获取不到数据的时候纵坐标不出现
            dayOption.getSeries().get(0).setData(yValue);
        }

        return dayOption;
    }

    private GsonOption getSaveElecOfMonthChart(String strLevel, String strName, String strDate) {

        // 计算1970-1-1到strDate的天数
        long days = getDays(strDate, "1970-1-1");

        String dateTime = getStringTime();
        String year = strDate.substring(0, 4);

        Calendar calendar = Calendar.getInstance();
        try {
            calendar.setTime(new SimpleDateFormat("yyyy/MM/dd HH:mm:ss").parse(dateTime));
        } catch (ParseException e) {
            e.printStackTrace();
        }

        List<HistoryDataValue> historyDataValues = historyDataDao.getSaveElecInfo("HIS_CALC_DS" + year, days, strLevel, strName, MONTH);

        GsonOption dayOption = new GsonOption();
        List<Axis> categoryAxises = new ArrayList<>();
        CategoryAxis powerAxis = new CategoryAxis();
        powerAxis.setShow(true);
        categoryAxises.add(powerAxis);

        Bar bar = new Bar();
        bar.setName("月节电量");

        AreaStyle areaStyle = new AreaStyle();
        areaStyle.setType("default");
        areaStyle.setColor("#32d2c9");
        bar.itemStyle().normal().setAreaStyle(areaStyle);

        dayOption.setxAxis(categoryAxises);
        dayOption.series(bar);

        //初始化图表，避免过零点图表显示区为空
        for (int i = 0 ; i < historyDataValues.size(); i++) {
            powerAxis.data().add(historyDataValues.get(i).getName());
            bar.data().add(0);
        }

        List<String> datas = new ArrayList<>();
        if (historyDataValues.size() == 0){
            return dayOption;
        }else {
            for(int i = 0; i < historyDataValues.size(); i++ ){
                datas.add(historyDataValues.get(i).getData().toString());
            }
        }

        //只显示6点到20点的发电量
        List<String> yValue = new ArrayList<>();

        for (int i = 0; i < datas.size();i++){

            Double value = Double.parseDouble(datas.get(i));
            String valueStr = String.format("%.2f", value);
            yValue.add(valueStr);

            //设置两头都为0，避免获取不到数据的时候纵坐标不出现
            dayOption.getSeries().get(0).setData(yValue);
        }

        return dayOption;
    }

    private GsonOption getSaveElecOfYearChart(String strLevel, String strName, String strDate) {

        // 计算1970-1-1到strDate的天数
        long days = getDays(strDate, "1970-1-1");

        String dateTime = getStringTime();
        String year = strDate.substring(0, 4);

        Calendar calendar = Calendar.getInstance();
        try {
            calendar.setTime(new SimpleDateFormat("yyyy/MM/dd HH:mm:ss").parse(dateTime));
        } catch (ParseException e) {
            e.printStackTrace();
        }

        List<HistoryDataValue> historyDataValues = historyDataDao.getSaveElecInfo("HIS_CALC_DS" + year, days, strLevel, strName, YEAR);

        GsonOption dayOption = new GsonOption();
        List<Axis> categoryAxises = new ArrayList<>();
        CategoryAxis powerAxis = new CategoryAxis();
        powerAxis.setShow(true);
        categoryAxises.add(powerAxis);

        Bar bar = new Bar();
        bar.setName("年节电量");

        AreaStyle areaStyle = new AreaStyle();
        areaStyle.setType("default");
        areaStyle.setColor("#32d2c9");
        bar.itemStyle().normal().setAreaStyle(areaStyle);

        dayOption.setxAxis(categoryAxises);
        dayOption.series(bar);

        //初始化图表，避免过零点图表显示区为空
        for (int i = 0 ; i < historyDataValues.size(); i++) {
            powerAxis.data().add(historyDataValues.get(i).getName());
            bar.data().add(0);
        }

        List<String> datas = new ArrayList<>();
        if (historyDataValues.size() == 0){
            return dayOption;
        }else {
            for(int i = 0; i < historyDataValues.size(); i++ ){
                datas.add(historyDataValues.get(i).getData().toString());
            }
        }

        //只显示6点到20点的发电量
        List<String> yValue = new ArrayList<>();

        for (int i = 0; i < datas.size();i++){

            Double value = Double.parseDouble(datas.get(i));
            String valueStr = String.format("%.2f", value);
            yValue.add(valueStr);

            //设置两头都为0，避免获取不到数据的时候纵坐标不出现
            dayOption.getSeries().get(0).setData(yValue);
        }

        return dayOption;
    }

    // strStationID, strDeviceID, strParaID
    private GsonOption getRealCurveChart(String strStationID, String strDeviceID, String strParaID) {

        Calendar c = Calendar.getInstance();
        long year = c.get(Calendar.YEAR);

        // 获取1970-1-1到今天的天数
        long days = getDays();

        String strTableName = "HIS_AI" + year;

        int nStationID = Integer.parseInt(strStationID);
        int nDeviceID = Integer.parseInt(strDeviceID);
        int nParaID = Integer.parseInt(strParaID);

        /*String dateTime = getStringTime();

        Calendar calendar = Calendar.getInstance();
        try {
            calendar.setTime(new SimpleDateFormat("yyyy/MM/dd HH:mm:ss").parse(dateTime));
        } catch (ParseException e) {
            e.printStackTrace();
        }*/

        List<List<HistoryDataValue>> historyDataValues = historyDataDao.getRealCurveInfo(strTableName, days, nStationID, nDeviceID, nParaID);

        GsonOption dayOption = new GsonOption();
        List<Axis> categoryAxises = new ArrayList<>();
        CategoryAxis powerAxis = new CategoryAxis();
        powerAxis.setShow(true);
        categoryAxises.add(powerAxis);

        //Bar bar = new Bar();
        //bar.setName("实时曲线");
        com.github.abel533.echarts.series.Line line = new com.github.abel533.echarts.series.Line();
        line.smooth(true).name("实时曲线").itemStyle().normal().label().show(false);

        AreaStyle areaStyle = new AreaStyle();
        areaStyle.setType("default");
        areaStyle.setColor("#32d2c9");
        line.itemStyle().normal().setAreaStyle(areaStyle);

        //List<String> currentValues = new ArrayList<>();

        dayOption.setxAxis(categoryAxises);
        dayOption.series(line);

        //初始化图表，避免过零点图表显示区为空
        for (int i = 0 ; i < historyDataValues.size(); i++) {
            //powerAxis.data().add(historyDataValues.get(i).getName());
            powerAxis.data().add(i);
            line.data().add(0);
            List<HistoryDataValue> listSignalHourData = historyDataValues.get(i);
            for (int j = 1; j < listSignalHourData.size(); j++) {
                powerAxis.data().add("");

                line.data().add(0);
            }
        }

        List<String> datas = new ArrayList<>();
        if (historyDataValues.size() == 0){
            return dayOption;
        }else {
            for(int i = 0; i < historyDataValues.size(); i++ ) {
                List<HistoryDataValue> listSignalHourData = historyDataValues.get(i);
                for (int j = 0; j < listSignalHourData.size(); j++) {
                    datas.add(listSignalHourData.get(j).getData().toString());
                }
            }
        }

        //只显示6点到20点的发电量
        List<String> yValue = new ArrayList<>();
        for (int i = 0; i < datas.size();i++) {

            Double value = Double.parseDouble(datas.get(i));
            String valueStr = String.format("%.2f", value);
            yValue.add(valueStr);

            //dayOption.getSeries().get(0).setData(yValue);

            if ((i+1)%12 == 0) {
                //设置两头都为0，避免获取不到数据的时候纵坐标不出现
                dayOption.getSeries().get(0).setData(yValue);
            }
        }

        return dayOption;
    }

    private GsonOption getStationDayKwhChart() {
        Long dayMillTime = new Date().getTime();
        String dateTime = getStringTime();
        String year = dateTime.substring(0, 4);
        String yearMonth = dateTime.substring(0, 4) + dateTime.substring(5, 7);
        int days = (int) ((dayMillTime / 1000 / 3600 + 8) / 24);
        Calendar calendar = Calendar.getInstance();
        try {
            calendar.setTime(new SimpleDateFormat("yyyy/MM/dd HH:mm:ss").parse(dateTime));
        } catch (ParseException e) {
            e.printStackTrace();
        }
        int daysBegin = days - (calendar.get(Calendar.DAY_OF_MONTH) - 1);
        int month = (calendar.get(Calendar.YEAR) - 1970) * 12 + calendar.get(Calendar.MONTH) + 1;
        int monthBegin = month - calendar.get(Calendar.MONTH);
        int minutes = calendar.get(Calendar.HOUR_OF_DAY) * 60 + calendar.get(Calendar.MINUTE);
        //List<HistoryDataValue> historyDataValues = historyDataDao.getHistoryData("KWH" + yearMonth, STATIONDAYKWHID,
        //        days, 0, minutes,52);
        List<HistoryDataValue> historyDataValues = historyDataDao.getHistoryData("HIS_CALC" + year,
                STATIONDAYKWHID,
                days, 0, minutes,52);

        GsonOption dayOption = new GsonOption();
        List<Axis> categoryAxises = new ArrayList<>();
        CategoryAxis powerAxis = new CategoryAxis();
        powerAxis.setShow(true);
        categoryAxises.add(powerAxis);

        Bar bar = new Bar();
        bar.setName("日发电量");

        AreaStyle areaStyle = new AreaStyle();
        areaStyle.setType("default");
        areaStyle.setColor("#32d2c9");
        bar.itemStyle().normal().setAreaStyle(areaStyle);

        dayOption.setxAxis(categoryAxises);
        dayOption.series(bar);

        //初始化图表，避免过零点图表显示区为空
        for (int i = 6 ; i <= 20; i++) {
            powerAxis.data().add(i + "时");
            bar.data().add(0);
        }

        List<String> datas = new ArrayList<>();
        if (historyDataValues.size() == 0){
            return dayOption;
        }else {
            for(int i = 0; i < historyDataValues.size(); i++ ){
                datas.add(historyDataValues.get(i).getData().toString());
            }
        }

        //只显示6点到20点的发电量
        List<String> xValue = new ArrayList<>();
        List<String> yValue = new ArrayList<>();
        //Double tmp = Double.parseDouble(datas.get(0));
        Double tmp = 0.0;
        for (int i = 0; i < datas.size();i++){
            if (i <= 20){
                xValue.add((i + 6)+ "时");
                //Double value = Double.parseDouble(datas.get(i)) - tmp;
                Double value = Double.parseDouble(datas.get(i)) - tmp;
                String valueStr = String.format("%.1f", (value > 0 ? value : 0));
                yValue.add(valueStr);
                tmp = Double.parseDouble(datas.get(i));
            }
            //设置两头都为0，避免获取不到数据的时候纵坐标不出现
//            dayOption.getxAxis().get(0).setData(xValue);
            dayOption.getSeries().get(0).setData(yValue);
        }

        return dayOption;
    }

    private GsonOption getStationMonthKwhChart() {

        GsonOption dayOption = new GsonOption();
        List<Axis> categoryAxises = new ArrayList<>();
        CategoryAxis powerAxis = new CategoryAxis();
        powerAxis.setShow(true);
        categoryAxises.add(powerAxis);

        Bar bar = new Bar();
        bar.setName("月发电量");

        AreaStyle areaStyle = new AreaStyle();
        areaStyle.setType("default");
        areaStyle.setColor("#32d2c9");
        bar.itemStyle().normal().setAreaStyle(areaStyle);

        dayOption.setxAxis(categoryAxises);
        dayOption.series(bar);

        //初始化图表，避免月初时显示区为空
        for (int i = 1 ; i <= 30; i++) {
            powerAxis.data().add(i + "日");
            bar.data().add(0);
        }

        List<String> xValue = new ArrayList<>();
        List<String> yValue = new ArrayList<>();
        try {
            List<List<String>> datas = lijHistoryDataDao.getMonthKwhData(getStringTime());
            //基准值，用来相减
            Double tmp = lijHistoryDataDao.getLastMonthEndDayKwh(getStringTime());
            for (int i = 0 ; i < datas.size(); i++) {
                yValue.add(datas.get(i).get(0));
                if (datas.get(i).get(1).equals("-")){
                    xValue.add(datas.get(i).get(1));
                }else {
                    Double value = Double.parseDouble(datas.get(i).get(1));
                    xValue.add((value - tmp) + "");
                    tmp = value;
                }
            }
            dayOption.getSeries().get(0).setData(xValue);
            dayOption.getxAxis().get(0).setData(yValue);
        } catch (ParseException e) {
            e.printStackTrace();
        }

        return dayOption;
    }

    private GsonOption getStationYearKwhChart(String id) {
        GsonOption dayOption = new GsonOption();
        List<Axis> categoryAxises = new ArrayList<>();
        CategoryAxis powerAxis = new CategoryAxis();
        powerAxis.setShow(true);
        categoryAxises.add(powerAxis);

        Bar bar = new Bar();
        bar.setName("年发电量");

        AreaStyle areaStyle = new AreaStyle();
        areaStyle.setType("default");
        areaStyle.setColor("#32d2c9");
        bar.itemStyle().normal().setAreaStyle(areaStyle);

        dayOption.setxAxis(categoryAxises);
        dayOption.series(bar);

        //初始化图表，避免元旦当天显示区为空，不吉利
        for (int i = 0 ; i < 12; i++) {
            powerAxis.data().add((i+ 1) + "月");
            bar.data().add("-");
        }


        List<String> xValue = new ArrayList<>();
        List<String> yValue = new ArrayList<>();
        try {
            List<List<String>> values = lijHistoryDataDao.getYearKwhData(getStringTime());
            for (int i = 0 ; i < values.size(); i++) {
                xValue.add(values.get(i).get(0));
                yValue.add(values.get(i).get(1));
            }
            dayOption.getxAxis().get(0).setData(xValue);
            dayOption.getSeries().get(0).setData(yValue);
        } catch (ParseException e) {
            e.printStackTrace();
        }

        return dayOption;
    }


    private GsonOption getStationTotalKwhChart() {

        GsonOption dayOption = new GsonOption();
        List<Axis> categoryAxises = new ArrayList<>();
        CategoryAxis powerAxis = new CategoryAxis();
        powerAxis.setShow(true);
        categoryAxises.add(powerAxis);

        Bar bar = new Bar();
        bar.setName("累积发电量");

        AreaStyle areaStyle = new AreaStyle();
        areaStyle.setType("default");
        areaStyle.setColor("#32d2c9");
        bar.itemStyle().normal().setAreaStyle(areaStyle);

        dayOption.setxAxis(categoryAxises);
        dayOption.series(bar);

        //初始化图表，避免获取不到数据的时候显示区为空，不吉利
        for (int i = 2015 ; i < Integer.parseInt(getStringTime().substring(0,4)); i++) {
            powerAxis.data().add((i+ 1) + "年");
            bar.data().add("-");
        }

        List<String> xValue = new ArrayList<>();
        List<String> yValue = new ArrayList<>();
        try {
            List<List<String>> values = lijHistoryDataDao.getTotalKwhData(getStringTime());
            for (int i = 0 ; i < values.size(); i++) {
                xValue.add(values.get(i).get(0));
                yValue.add(values.get(i).get(1));
            }
            dayOption.getxAxis().get(0).setData(xValue);
            dayOption.getSeries().get(0).setData(yValue);
        } catch (ParseException e) {
            e.printStackTrace();
        }

        return dayOption;
    }

    private String getStringTime(){
        SimpleDateFormat df = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");//设置日期格式
        return df.format(new Date());
    }
}
