package com.corsair.controller;

import com.corsair.service.HistoryDataService;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by ChengXi on 2015/9/29.
 */
@Controller
public class OverallRatioController {
    @Autowired
    HistoryDataService historyDataService;
    @Autowired
    ScadaPSRService scadaPsrService;
    @RequestMapping(value = "/overallRatio", method = RequestMethod.GET)
    public ModelAndView redirect(){
        ModelAndView modelAndView = new ModelAndView("overallRatio");
//        String psrID = "oi815";
//        DateFormat date = new SimpleDateFormat("yyyy-MM-dd");
//        String time = date.format(new Date());
//
//        DateFormat hours = new SimpleDateFormat("HH");
//        int beginMinite=0;
//        int endMinite=Integer.parseInt(hours.format(new Date()))*60;
//
//        //实际发电量
//        PackageTransformer packageTransformer = scadaPsrService.getPackageTransformer(psrID);
//        List<HistoryDataValue> actualListHistoryData = eneralDataService.getHistoryData(packageTransformer.getId(),"transboxPtFawardGen",time,beginMinite,endMinite, HistoryDataService.DataType.KWH_VALUE);
//
//        //理论发电量
//        WeatherInstrument weatherInstrument = scadaPsrService.getWeatherInstrument();
//        List<HistoryDataValue> emigrationListHistoryData = eneralDataService.getHistoryData(weatherInstrument.getId(),"weatherRadiation",time,beginMinite,endMinite, HistoryDataService.DataType.REALTIME_VALUE);

        List<String> list = new ArrayList<>();
        for (int i = 1; i <= 24; i++) {
            list.add(String.valueOf(Math.round(Math.random()* 5) + 12));
        }
        modelAndView.addObject("list",list);
        //System.out.println("00000000000000");
        return modelAndView;
    }
}
