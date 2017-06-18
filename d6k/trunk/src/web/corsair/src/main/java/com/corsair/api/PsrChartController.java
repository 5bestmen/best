package com.corsair.api;

import com.corsair.chart.IPSRChart;
import com.corsair.chart.MeasurementBarChart;
import com.corsair.chart.MeasurementLineChart;
import com.corsair.device.ChartDataType;
import com.corsair.service.PSRDataTypeService;
import com.corsair.util.ChartParaUtil;
import com.github.abel533.echarts.Option;
import com.github.abel533.echarts.axis.CategoryAxis;
import com.github.abel533.echarts.axis.ValueAxis;
import com.github.abel533.echarts.code.Magic;
import com.github.abel533.echarts.code.Tool;
import com.github.abel533.echarts.code.Trigger;
import com.github.abel533.echarts.feature.MagicType;
import com.github.abel533.echarts.json.GsonUtil;
import com.github.abel533.echarts.series.Line;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import javax.annotation.Resource;
import javax.servlet.http.HttpSession;
import java.util.List;
import java.util.Map;

/**
 * Created by zjq on 2016/1/25.
 */
@RestController
@RequestMapping(value = "/charts")
public class PsrChartController {
    @Autowired
    MeasurementLineChart measurementLineChart;
    @Autowired
    MeasurementBarChart measurementBarChart;
    @Autowired
    PSRDataTypeService psrDataTypeService;

    @Resource(name = "specialChartMap")
    Map<String, IPSRChart> specialChartMap;

    @RequestMapping(value = "data", method = RequestMethod.POST)
    public ResponseEntity<String> getChartDataType(ChartParaUtil chartParaUtil,HttpSession session) {

        Integer dataType = Integer.parseInt(chartParaUtil.getDataType());
        Integer index = Integer.parseInt(chartParaUtil.getIndex());


        IPSRChart psrChart;
        Option option;

        if (dataType.equals(2)) {
            //普通量测的日数据
            psrChart = measurementLineChart;
            option = psrChart.getChart(chartParaUtil, session.toString());
        } else if (dataType.equals(3)) {
            //普通量测的日数据
            psrChart = measurementBarChart;
            option = psrChart.getChart(chartParaUtil, session.toString());
        } else if (dataType.equals(4)) {
            //特殊图表数据,会根据chartName找到不同的bean并由不同的bean处理
            psrChart = (IPSRChart) specialChartMap.get(chartParaUtil.getChartName());
            option = psrChart.getChart(chartParaUtil, session.toString());
        } else {

            //todo 以下只是一个生成ECharts结构的例子，以后会被删除
            option = new Option();
            option.legend("高度(km)与气温(°C)变化关系");

            option.toolbox().show(true).feature(Tool.mark, Tool.dataView, new MagicType(Magic.line, Magic.bar), Tool.restore, Tool.saveAsImage);

            option.calculable(true);
            option.tooltip().trigger(Trigger.axis).formatter("Temperature : <br/>{b}km : {c}°C");

            ValueAxis valueAxis = new ValueAxis();
            valueAxis.axisLabel().formatter("{value} °C");
            option.xAxis(valueAxis);

            CategoryAxis categoryAxis = new CategoryAxis();
            categoryAxis.axisLine().onZero(false);
            categoryAxis.axisLabel().formatter("{value} km");
            categoryAxis.boundaryGap(false);
            categoryAxis.data(0, 10, 20, 30, 40, 50, 60, 70, 80);
            option.yAxis(categoryAxis);

            Line line = new Line();
            line.smooth(true).name("高度(km)与气温(°C)变化关系").data(15, -50, -56.5, -46.5, -22.1, -2.5, -27.7, -55.7, -76.5).itemStyle().normal().lineStyle().shadowColor("rgba(0,0,0,0.4)");
            option.series(line);
        }
         return new ResponseEntity<>(GsonUtil.format(option), HttpStatus.OK);
    }


    @RequestMapping(value = "type", method = RequestMethod.POST)
    public ResponseEntity<List<ChartDataType>> getChartDataType(String psrID, String dateType) {

        return new ResponseEntity<>(psrDataTypeService.getChartDataType(psrID, dateType), HttpStatus.OK);
    }

}
