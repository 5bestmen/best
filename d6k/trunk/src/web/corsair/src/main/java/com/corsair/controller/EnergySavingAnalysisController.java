package com.corsair.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

/**
 * Created by zyj on 2017/3/17.
 */
@Controller
public class EnergySavingAnalysisController {
    @RequestMapping(value = "/energySavingAnalysis", method = RequestMethod.GET)
    public ModelAndView redirect(){
        ModelAndView modelAndView = new ModelAndView("energySavingAnalysis");

        return modelAndView;
    }
}
