package com.corsair.api;

import com.corsair.dao.HistoryDataDao;
import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.dao.ScadaYCDao;
import com.corsair.service.HistoryDataService;
import com.corsair.service.ScadaMeasurementService;
import com.corsair.service.ScadaPSRService;
import com.corsair.util.HisDataChartUtil;
import com.corsair.util.MeasurementUtil;
import com.corsair.util.StationMeasureUtil;
import com.github.abel533.echarts.axis.Axis;
import com.github.abel533.echarts.axis.CategoryAxis;
import com.github.abel533.echarts.json.GsonOption;
import com.github.abel533.echarts.json.GsonUtil;
import com.github.abel533.echarts.series.Bar;
import com.github.abel533.echarts.series.Line;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import javax.servlet.http.HttpSession;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 2016/2/25.
 */
@RestController
@RequestMapping(value = "/his/stations")
public class HisDataChartController {
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

    @RequestMapping(value = "data", method = RequestMethod.POST)
    public ResponseEntity<String> getChart(HisDataChartUtil hisDataChartUtil,HttpSession session) {
        GsonOption option = generateChartOption(hisDataChartUtil);
        if (option == null) {
            return new ResponseEntity<String>(HttpStatus.NOT_FOUND);
        }
        return new ResponseEntity<String>(GsonUtil.format(option), HttpStatus.OK);
    }


    private GsonOption generateChartOption(HisDataChartUtil hisDataChartUtil) {
        GsonOption option = new GsonOption();
        List<Axis> categoryAxises = new ArrayList<>();
        CategoryAxis categoryAxis = new CategoryAxis();
        categoryAxis.setShow(true);
        categoryAxises.add(categoryAxis);
        //类型为遥测
        if (hisDataChartUtil.getDataType().equals("1")){
            //两层循环，建立所有遥测曲线
            for(StationMeasureUtil stationMeasureUtil : hisDataChartUtil.getStationMeasureUtils()){
                for (MeasurementUtil measurementUtil : stationMeasureUtil.getMeasurementUtils()){
                    Line line = new Line();
                    line.setName(measurementUtil.getComments());
                }
            }
        }

        return option;
    }

}
