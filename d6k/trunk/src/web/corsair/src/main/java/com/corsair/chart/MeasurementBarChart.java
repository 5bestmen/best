package com.corsair.chart;

import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.dao.ScadaYCDao;
import com.corsair.device.PackageTransformer;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.service.MeasurementGenerateService;
import com.corsair.service.ScadaPSRService;
import com.corsair.util.ChartParaUtil;
import com.github.abel533.echarts.Option;
import com.github.abel533.echarts.axis.CategoryAxis;
import com.github.abel533.echarts.axis.ValueAxis;
import com.github.abel533.echarts.code.Magic;
import com.github.abel533.echarts.code.Tool;
import com.github.abel533.echarts.code.Trigger;
import com.github.abel533.echarts.feature.MagicType;
import com.github.abel533.echarts.series.Bar;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 15/7/30.
 */
@Service("measurementBar")
public class MeasurementBarChart implements IPSRChart {

    @Autowired
    ScadaYCDao scadaYCDao;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;
    @Autowired
    MeasurementGenerateService measurementGenerateService;
    @Autowired
    ScadaPSRService scadaPsrService;

    @Override
    public Option getChart(ChartParaUtil chartParaUtil, String session) {

        String psrID  = chartParaUtil.getPsrID();
        Integer index = Integer.parseInt(chartParaUtil.getIndex());
        String  dateType = chartParaUtil.getDateType();
        String date = chartParaUtil.getDate();
        String magic = chartParaUtil.getMagic();
        DeviceTemplate template = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrID);
        DeviceTemplateData templateData = template.getDeviceTemplateData(3, index);
        Option option = new Option();
        String dataDesc = templateData.getDesc() == null ? "" : templateData.getDesc();
        String dataUnit = templateData.getChartUnit();
        option.legend(dataDesc);
        option.legend().textStyle().color("white");
        option.toolbox().show(true).feature(new MagicType(Magic.line, Magic.bar), Tool.restore, Tool.saveAsImage);
        option.calculable(true);
        String triggerFormatter = "日期/" + dataDesc + " : <br/>{b}/{c}" + dataUnit;
        option.tooltip().trigger(Trigger.axis).formatter(triggerFormatter);
        ValueAxis valueAxis = new ValueAxis();
        valueAxis.axisLabel().formatter("{value}" + dataUnit);
        valueAxis.axisLabel().textStyle().color("white");
        option.yAxis(valueAxis);

        CategoryAxis categoryAxis = new CategoryAxis();
        categoryAxis.axisLabel().formatter("{value}");
        categoryAxis.boundaryGap(true);
        Bar valueBar = new Bar();
        valueBar.name(templateData.getDesc()).itemStyle().normal().label().show(true);
        valueBar.itemStyle().normal().color("#32d2c9");

        List<String[]> xyValues = null;
        if ("oi815".equals(psrID)){//阵列主页面的棒图
            PackageTransformer packageTransformer = scadaPsrService.getPackageTransformer(psrID);
            xyValues = measurementGenerateService.getChart(packageTransformer.getId(), 3, dateType, date, magic, session);
        }else{//逆变器下面的棒图
            xyValues = measurementGenerateService.getChart(psrID, index, dateType, date, magic, session);
        }

        List<String> currentValues = new ArrayList<>();

        for (String[] xyValue:xyValues){
            categoryAxis.data(xyValue[0]);
            currentValues.add(xyValue[1]);
     //       valueBar.name(templateData.getDesc()).data(xyValue[1]).itemStyle().normal().label().show(true);
        }
        categoryAxis.axisLabel().textStyle().color("white");
        option.xAxis(categoryAxis);
        option.series(valueBar);
        option.getSeries().get(0).setData(currentValues);
        return option;
    }

}
