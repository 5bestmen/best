package com.corsair.chart;

import com.corsair.util.ChartParaUtil;
import com.github.abel533.echarts.Option;

/**
 * Created by 洪祥 on 15/7/30.
 * 图表通用接口，传入的内容有对象ID、日期和对象的测点ID,返回Echarts所需的Option结构
 */
public interface IPSRChart {
    public Option getChart(ChartParaUtil chartParaUtil, String session);
}
