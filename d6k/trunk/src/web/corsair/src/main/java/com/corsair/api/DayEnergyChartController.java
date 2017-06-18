package com.corsair.api;

import com.corsair.device.PhotovoltaicArray;
import com.corsair.service.ScadaPSRService;
import com.github.abel533.echarts.axis.Axis;
import com.github.abel533.echarts.axis.CategoryAxis;
import com.github.abel533.echarts.json.GsonOption;
import com.github.abel533.echarts.json.GsonUtil;
import com.github.abel533.echarts.series.Line;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by zjq on 2016/1/28.
 */
@RestController
public class DayEnergyChartController {

    private static GsonOption stationOption;
    @Autowired
    ScadaPSRService scadaPSRService;

    @RequestMapping(value = "dayEnergyChart", method = RequestMethod.POST, produces = "text/html;charset=UTF-8")
    public ResponseEntity<String> dayEnergyChart(String type) {

        if (type.equals("DayPowerCurve")) {

            if (stationOption == null) {

                stationOption = new GsonOption();

                List<PhotovoltaicArray> stationarry = scadaPSRService.getPVArray();

                List<Axis> categoryAxises = new ArrayList<Axis>();
                CategoryAxis categoryAxis = new CategoryAxis();
                categoryAxises.add(categoryAxis);

                Line energyline = new Line();
                Line hourline = new Line();
                stationOption.setxAxis(categoryAxises);
                stationOption.series(energyline, hourline);

                for (PhotovoltaicArray temp : stationarry) {
                    categoryAxis.data().add(temp.getName());
                    energyline.data().add(Math.random() * 50);
                    hourline.data().add(Math.random() * 10);
                }
            }
        }
        return new ResponseEntity<>(GsonUtil.format(stationOption), HttpStatus.OK);
    }
}
