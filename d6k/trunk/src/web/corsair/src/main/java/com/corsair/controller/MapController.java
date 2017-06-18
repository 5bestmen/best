package com.corsair.controller;

import com.corsair.entity.StationLocation;
import com.corsair.service.ScadaPSRService;
import com.corsair.service.StationLocationService;
import net.sf.json.JSONArray;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import java.util.List;

/**
 * Created by libh on 2015/9/29.
 */
@Controller
public class MapController {
    @Autowired
    ScadaPSRService scadaPSRService;

    @Autowired
    StationLocationService stationLocationService;

    @RequestMapping(value = "/map", method = RequestMethod.GET)
    public ModelAndView redirect(){
        ModelAndView modelAndView = new ModelAndView("map");

        modelAndView.addObject("total_capacity", 90);   //总装机容量
        modelAndView.addObject("station_num", scadaPSRService.getPVArray().size());       //光伏电站的个数
        modelAndView.addObject("pvarry_num", 1500);    //光伏组件个数
        modelAndView.addObject("Inverter_num", 50);    //逆变器个数


        modelAndView.addObject("total_P", 122);    //总有功功率
        modelAndView.addObject("day_grid_power", 34.17);    //日上网电量
        modelAndView.addObject("total_grid_power", 155.23);    //总上网电量

        JSONArray jsonarry = new JSONArray();
        List<StationLocation> stationlist = stationLocationService.getAllStationInfo();
        for (StationLocation sttemp:stationlist){
            jsonarry.add(sttemp);
        }

        modelAndView.addObject("locationinfo", jsonarry);

        return modelAndView;
    }
}
