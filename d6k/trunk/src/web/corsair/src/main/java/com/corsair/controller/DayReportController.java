package com.corsair.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

/**
 * Created by ChengXi on 2015/9/29.
 */
@Controller
public class DayReportController {
    @RequestMapping(value = "/dayReport", method = RequestMethod.GET)
    public ModelAndView redirect(){
        ModelAndView modelAndView = new ModelAndView("dayReport");
        return modelAndView;
    }
}
