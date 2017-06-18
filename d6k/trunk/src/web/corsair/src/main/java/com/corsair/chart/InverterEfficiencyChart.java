package com.corsair.chart;

import com.corsair.dao.*;
import com.corsair.util.ChartParaUtil;
import com.github.abel533.echarts.Option;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Created by zjq on 2015/8/6.
 */
@Service("inverterEfficiency")
public class InverterEfficiencyChart implements IPSRChart {
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;
    @Autowired
    InverterEfficiencyDao inverterEfficiencyDao;
    @Autowired
    InverterEfficiencyDataDao inverterEfficiencyDataDao;
    @Autowired
    InverterTemperatureDao inverterTemperatureDao;
    @Autowired
    InverterPowerPointDao inverterPowerPointDao;

    @Override
    public Option getChart(ChartParaUtil chartParaUtil, String session) {
        //从数据库中取数据
//      DeviceTemplate deviceTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrID);
//        DeviceTemplateData templateData = deviceTemplate.getDeviceTemplateData(2, index);
//      List<InverterTemperature> inverterTemperatures = inverterTemperatureDao.getInverterTemperatureData();
//      InverterEfficiency inverterEfficiency =  inverterEfficiencyDao.getInverterEfficiency(deviceTemplate.getId());
//      String inverterEfficiencyID = inverterEfficiency.getID();
//
//      List<String> xValues = new ArrayList<>();
//      List<InverterPowerPoint> inverterPowerPoints = inverterPowerPointDao.getInverterPowerPointData();
//
//      for (InverterPowerPoint inverterPowerPoint : inverterPowerPoints){
//          xValues.add(Integer.toString(inverterPowerPoint.getPoint()));
//      }
//
//      List<InverterEfficiencyData> inverterEfficiencyDataNormal = inverterEfficiencyDataDao.getInverterEfficiencyData(inverterEfficiencyID, inverterTemperatures.get(0).getID());
//      List<String> valueNormal= new ArrayList<>();
//      for(InverterEfficiencyData inverterEfficiencyDatanormal : inverterEfficiencyDataNormal){
//          valueNormal.add(Double.toString(inverterEfficiencyDatanormal.getEfficiency()));
//      }
//
//      List<InverterEfficiencyData> inverterEfficiencyDataHigh = inverterEfficiencyDataDao.getInverterEfficiencyData(inverterEfficiencyID, inverterTemperatures.get(1).getID());
//      List<String> valueHigh= new ArrayList<>();
//      for(InverterEfficiencyData inverterEfficiencyDatahigh : inverterEfficiencyDataHigh){
//          valueHigh.add(Double.toString(inverterEfficiencyDatahigh.getEfficiency()));
//      }
//
//      Option option = new Option();
// //     option.title(templateData.getDesc());
//      option.legend(inverterTemperatures.get(0).getName(),inverterTemperatures.get(1).getName());
//
//      option.toolbox().show(true).feature(Tool.dataView, new MagicType(Magic.line, Magic.bar), Tool.restore, Tool.saveAsImage);
//
//      option.calculable(true);
//      option.tooltip().trigger(Trigger.axis);
//
//      ValueAxis valueAxis = new ValueAxis();
//      valueAxis.axisLabel().formatter("{value} %");
//      option.yAxis(valueAxis);
//
//      CategoryAxis categoryAxis = new CategoryAxis();
//      categoryAxis.axisLine().onZero(false);
//      categoryAxis.axisLabel().formatter("{value}");
//      categoryAxis.boundaryGap(false);
//      categoryAxis.data(xValues);
//      option.xAxis(categoryAxis);
//
//      Line lineTemHigh = new Line(inverterTemperatures.get(0).getName());
//      lineTemHigh.smooth(false).itemStyle().normal().lineStyle().shadowColor("rgba(0,0,0,0.4)");
//      lineTemHigh.data(valueHigh);
//      Line lineTemNormal = new Line(inverterTemperatures.get(1).getName());
//        lineTemNormal.smooth(true).itemStyle().normal().lineStyle().shadowColor("rgba(0,0,0,0.4)");
//      lineTemNormal.data(valueNormal);
//
//      option.series(lineTemNormal,lineTemHigh);
//      return option;
        return null;
    }
}
