package com.corsair.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

/**
 * Created by zyj on 2017/2/14.
 */
@Controller
public class AlarmRecordController {
    @RequestMapping(value = "/alarmRecord", method = RequestMethod.GET)
    public ModelAndView redirect(){
        ModelAndView modelAndView = new ModelAndView("alarmRecord");

        return modelAndView;
    }
}
