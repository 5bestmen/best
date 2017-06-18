package com.corsair.controller;

import com.corsair.service.HistoryDataService;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

/**
 * Created by 洪祥 on 2016/2/22.
 */
@Controller
public class HistoricalDataQueryController {
    @Autowired
    HistoryDataService historyDataService;
    @Autowired
    ScadaPSRService scadaPsrService;

    @RequestMapping(value = "/historicalDataQuery", method = RequestMethod.GET)
    public ModelAndView redirect(){
        ModelAndView modelAndView = new ModelAndView("historicalDataQuery");

        return modelAndView;
    }
}
