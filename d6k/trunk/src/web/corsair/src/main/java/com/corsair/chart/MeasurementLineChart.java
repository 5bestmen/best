package com.corsair.chart;

import com.corsair.dao.HistoryDataDao;
import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.dao.ScadaYCDao;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.measurement.HistoryDataValue;
import com.corsair.service.HistoryDataService;
import com.corsair.service.ScadaMeasurementService;
import com.corsair.service.ScadaPSRService;
import com.corsair.util.ChartParaUtil;
import com.github.abel533.echarts.Option;
import com.github.abel533.echarts.axis.CategoryAxis;
import com.github.abel533.echarts.axis.ValueAxis;
import com.github.abel533.echarts.code.Magic;
import com.github.abel533.echarts.code.Tool;
import com.github.abel533.echarts.code.Trigger;
import com.github.abel533.echarts.feature.MagicType;
import com.github.abel533.echarts.series.Line;
import com.github.abel533.echarts.style.AreaStyle;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

/**
 * Created by 洪祥 on 15/7/30.
 */
@Service("measurementLine")
public class MeasurementLineChart  implements IPSRChart {

    @Autowired
    ScadaYCDao scadaYCDao;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;
    @Autowired
    HistoryDataService historyDataService;
    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    HistoryDataDao historyDataDao;

    @Override
    public Option getChart(ChartParaUtil chartParaUtil, String session) {

        Integer index = Integer.parseInt(chartParaUtil.getIndex());
        String psrID = chartParaUtil.getPsrID();
        DeviceTemplate template = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrID);
        DeviceTemplateData templateData = template.getDeviceTemplateData(2, index);
        SimpleDateFormat dateParse = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        SimpleDateFormat dateFormat = new SimpleDateFormat("HH:mm:ss");

        CategoryAxis categoryAxis = new CategoryAxis();
        categoryAxis.axisLine().onZero(false);
        categoryAxis.axisLabel().formatter("{value}");
        categoryAxis.boundaryGap(false);
        categoryAxis.axisLabel().textStyle().color("white");
        Line line = new Line();
        line.smooth(true).name(templateData.getDesc()).itemStyle().normal().label().show(false);
        AreaStyle areaStyle = new AreaStyle();
        areaStyle.setType("default");
        areaStyle.setColor("#32d2c9");
        line.itemStyle().normal().setAreaStyle(areaStyle);
        List<String> currentValues = new ArrayList<>();

        String date = chartParaUtil.getDate();
        if ("day".equals(chartParaUtil.getDateType())){

            try {
                String dateOldBegintime = date.substring(0,10) + " 00:00:00";
                Date oldDate = dateParse.parse(dateOldBegintime);
                Date endDate = dateParse.parse(date);
                Calendar endTime = Calendar.getInstance();
                endTime.setTime(endDate);
                Long oldDayMillTime = oldDate.getTime() ;

                int beginMinute = 8 * 60;
                int endMinute = 0;
                if (endTime.get(Calendar.HOUR_OF_DAY) >= 18){
                    endMinute = 18 * 60;
                }else{
                    endMinute = endTime.get(Calendar.HOUR_OF_DAY) * 60 + endTime.get(Calendar.MINUTE);
                }
                List<HistoryDataValue> historyDataValues = null;
                Boolean calculated = templateData.getCalculate();
                if (calculated != null && calculated){
                    historyDataValues = historyDataService.getHistoryData(psrID,templateData.getName(), date, beginMinute, endMinute,HistoryDataService.DataType.REALTIME_VALUE);
                }else{
                    String measureId = scadaMeasurementService.getMeasurementID(psrID,templateData);
                    String yearMonth = date.substring(0,4) + date.substring(5,7);
                    int days = (int)((oldDayMillTime/1000/3600+8)/24);
                    historyDataValues = historyDataDao.getHistoryData("YC" + yearMonth, measureId, days, beginMinute, endMinute, 1);
                }

                String currentTimeString;

//增加没有数据时的处理
                if (historyDataValues.isEmpty()){

                    for (int currentTime = beginMinute; currentTime <= endMinute; currentTime += 5){
                        Long DayMillTime = oldDayMillTime + 60 * 1000 * currentTime;
                        oldDate.setTime(DayMillTime);
                        currentTimeString = dateFormat.format(oldDate);
                        categoryAxis.data(currentTimeString);
                        currentValues.add("-");
        //                line.smooth(true).name(templateData.getDesc()).data("-").itemStyle().normal().label().show(false);
                    }

                }else{
                    int actualIndex = 0;

                    int actualNum = historyDataValues.size();
                    for (int currentTime = beginMinute; currentTime <= endMinute; currentTime += 5){
                        String dataValue = "-";
                        if (actualIndex < actualNum){
                            HistoryDataValue historyDataValue = historyDataValues.get(actualIndex);
                            int actualTime = historyDataValue.getTime();

                            if (currentTime == actualTime){
                                dataValue = String.format("%.2f", historyDataValue.getData());
                                actualIndex++;
                            }
                        }

                        Long DayMillTime = oldDayMillTime + 60 * 1000 * currentTime;
                        oldDate.setTime(DayMillTime);
                        currentTimeString = dateFormat.format(oldDate);
                        categoryAxis.data(currentTimeString);
                        currentValues.add(dataValue);
               //         line.smooth(true).name(templateData.getDesc()).data(dataValue).itemStyle().normal().label().show(false);
                    }
                }

             }catch (ParseException e){
                e.printStackTrace();
                 return null;
             }
        }

        String dataDesc = templateData.getDesc() == null ? "" : templateData.getDesc();
        String dataUnit = templateData.getChartUnit();

        Option option = new Option();
        option.legend(dataDesc);
        option.legend().textStyle().color("white");
        option.toolbox().show(true).feature(new MagicType(Magic.line, Magic.bar), Tool.restore, Tool.saveAsImage);
        option.calculable(true);
        String triggerFormatter = "时间/" + dataDesc + " : <br/>{b}/{c}" + dataUnit;
        option.tooltip().trigger(Trigger.axis).formatter(triggerFormatter);
        ValueAxis valueAxis = new ValueAxis();
        String xFormatter = "{value}" + dataUnit;
        valueAxis.axisLabel().formatter(xFormatter);
        valueAxis.axisLabel().textStyle().color("white");
        option.yAxis(valueAxis);
        option.xAxis(categoryAxis);
        option.series(line);
        option.getSeries().get(0).setData(currentValues);
        return option;
    }

}
