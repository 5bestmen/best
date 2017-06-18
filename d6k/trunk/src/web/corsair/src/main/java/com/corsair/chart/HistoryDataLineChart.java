package com.corsair.chart;

import com.corsair.dao.HistoryDataDao;
import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.dao.ScadaYCDao;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.measurement.HistoryDataValue;
import com.corsair.measurement.history.HistoryValue;
import com.corsair.service.HistoryDataService;
import com.corsair.service.ScadaMeasurementService;
import com.corsair.service.ScadaPSRService;
import com.github.abel533.echarts.Option;
import com.github.abel533.echarts.axis.CategoryAxis;
import com.github.abel533.echarts.axis.ValueAxis;
import com.github.abel533.echarts.code.Magic;
import com.github.abel533.echarts.code.Symbol;
import com.github.abel533.echarts.code.Tool;
import com.github.abel533.echarts.code.Trigger;
import com.github.abel533.echarts.feature.MagicType;
import com.github.abel533.echarts.series.Line;
import com.github.abel533.echarts.style.AreaStyle;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;

@Service("historyDataLine")
public class HistoryDataLineChart {

    @Autowired
    ScadaYCDao scadaYCDao;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;
    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    HistoryDataDao historyDataDao;
    @Autowired
    HistoryDataService historyDataService;
    @Value("#{configProperties['corsair.yc.step']}")
    private Integer ycStep;


    public Option getChart(List<HistoryData> historyData, String session) {
        /*Option option = new Option();
        CategoryAxis categoryAxis = new CategoryAxis();//x轴
        categoryAxis.axisLine().onZero(false);
        categoryAxis.axisLabel().formatter("{value}");
        categoryAxis.boundaryGap(false);
        categoryAxis.axisLabel().textStyle().color("white");

        AreaStyle areaStyle = new AreaStyle();
        String[] legends = new String[historyData.size()];
        // todo:单位后续添加
        if (getDataByYAxis(historyData, option, areaStyle, legends)) return null;

//        String dateFrom = historyData.get(0).getDateFrom();
//        String dateTo = historyData.get(0).getDateTo();
//        Integer step = getStepLevel(dateFrom, dateTo);

        String PsrID = historyData.get(0).getPsrID();
        String StationID = historyData.get(0).getStationId();
        String DeviceID = historyData.get(0).getDeviceId();
        String dayReportTime = historyData.get(0).getQueryDate();

        //获取日报表历史数据
        List<HistoryDataValue> historyDataValues = historyDataService.getDayReportHistoryData(PsrID, StationID,
                DeviceID, dayReportTime);
        if (historyDataValues == null || historyDataValues.isEmpty()) {
            return null;
        }
        for (HistoryDataValue historyValue : historyDataValues) {
            categoryAxis.data(historyValue.getTime());
        }

//        //TODO 目前X轴为模拟，后期需要根据实际情况替换
//        for (int j = 0; j < 30; j++) {
//            categoryAxis.data(historyData.get(0).getDateFrom());
//        }

        option.xAxis(categoryAxis);
        option.tooltip().trigger(Trigger.axis);
        option.legend().data(legends);
        option.legend().textStyle().color("white");
        option.toolbox().show(true).feature(new MagicType(Magic.line, Magic.bar), Tool.restore, Tool.saveAsImage);
        option.calculable(true);
        return option;*/

        return  null;
    }

    private boolean getDataByYAxis(List<HistoryData> historyData, Option option, AreaStyle areaStyle, String[] legends) {
        //根据不同的y轴（即单位）得到对应的内容
        Map<String, Integer> map = new HashMap<String, Integer>();
        for (int i = 0; i < historyData.size(); i++) {
            if (null != map.get(historyData.get(i).getUnit())) {
                map.put(historyData.get(i).getUnit(), map.get(historyData.get(i-1).getUnit()) + 1); //value+1
            } else {
                map.put(historyData.get(i).getUnit(), 1);
            }
        }

        int count = 0;
        List<Double> minData = new ArrayList();

        for (Map.Entry<String, Integer> entry : map.entrySet()) {
            ValueAxis valueAxis = new ValueAxis();//y轴
            String xFormatter = "{value}" + entry.getKey();
            if (entry.getKey().equals("unit")) {
                xFormatter = "{value}";
            }
            valueAxis.axisLabel().formatter(xFormatter);
            valueAxis.axisLabel().textStyle().color("white");


            count++;
            for (int i = 0; i < historyData.size(); i++) {
                if (historyData.get(i).getUnit().equals(entry.getKey())) {
                    String psrID = historyData.get(i).getPsrID();
                    String name = historyData.get(i).getName();
                    String stationName = historyData.get(i).getStationName();
                    String deviceName = historyData.get(i).getDeviceName();
                    String startTime = historyData.get(i).getDateFrom();
                    String endTime = historyData.get(i).getDateTo();
                    DeviceTemplateData templateData = getTemplate(psrID, name);
                    if (templateData != null) {
                        Line line = new Line();
                        String desc = templateData.getDesc() == null ? "" : templateData.getDesc();
                        legends[i] = stationName + " " + deviceName + " " + desc;
                        line.smooth(true).symbol(Symbol.none).name(stationName + " " + deviceName + " " + templateData.getDesc()).itemStyle().normal().label().show(false);
                        line.itemStyle().normal().setAreaStyle(areaStyle);
                        if (count == 2) {
                            line.yAxisIndex(1);//两个y轴时
                        }
//                        Integer step = getStepLevel(startTime, endTime);
//                        List<HistoryValue> historyDataValues = historyDataService.getHistoryStepData(psrID, name, startTime + ":00", endTime + ":00", step);
//                        if (historyDataValues == null || historyDataValues.isEmpty()) {
//                            return true;
//                        }
//                        for (HistoryValue historyValue : historyDataValues) {
//                            if (historyValue.isValid()) {
//                                line.data().add(String.format("%.2f", historyValue.getValue()));
//                            } else {
//                                line.data().add("-");
//                            }
//                        }
//                        minData.add(historyDataValues.get(0).getValue());
                        //TODO 目前Y轴的数据为随机数，后期需要根替换成历史数据
                        for (int j = 0; j < 30; j++) {
                            line.data().add(String.format("%.2f", (Double)(Math.random()*100)));
                        }
//                        minData.add((Math.random()*100));
                        option.series(line);
                    }
                }
            }
            option.yAxis(valueAxis);
        }
        return false;
    }

    private DeviceTemplateData getTemplate(String psrID, String name) {
        DeviceTemplate template = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrID);
        if (template == null) {
            return null;
        }
        DeviceTemplateData templateData = template.getDeviceTemplateDataByName(name);
        if (templateData == null) {
            return null;
        }
        return templateData;
    }

    private Integer getStepLevel(String startTime, String endTime) {
        List<Integer> array = new ArrayList();
        getSuitableStep(startTime,endTime,array);
        return array.get(0)*ycStep;
    }

    private  void getSuitableStep(String timeBegin,String timeEnd,List array)  {
        DateFormat df = new SimpleDateFormat("yyyy-MM-dd hh");
        Date d1 = null;
        Date d2 = null;
        try {
            d1 = df.parse(timeEnd);
            d2 = df.parse(timeBegin);
        } catch (ParseException e) {
            e.printStackTrace();
        }
        int diff = (int) (d1.getTime() - d2.getTime());
        int hour=(diff/(60*60*1000));
        getData(hour,array);
    }

    private  void getData(int hour,List array){
        int newSum = hour*(60/ycStep);//步长为5分钟一次的基准，即1小时12个数据
        int i=1;
        compare(hour, i, newSum,array);
    }

    private  void compare(int hour, Integer i, int newSum,List array) {
        int count;
        if(newSum>204){//展示的数据不能超过204
            count= newSum/204;
            int step = ++i;
            newSum= hour*(60/(ycStep*step));
            compare(hour, i, newSum,array);
        }else {
            array.add(i);
        }
    }

    //去除数组中重复的记录
    public  List singleElement(List al){
        List newAl = new ArrayList();
        Iterator it = al.iterator();
        while (it.hasNext()){
            Object obj = it.next();//next()最好调用一次就hasNext()判断一次否则容易发生异常
            if (!newAl.contains(obj))
                newAl.add(obj);
        }
        return newAl;
    }

}

