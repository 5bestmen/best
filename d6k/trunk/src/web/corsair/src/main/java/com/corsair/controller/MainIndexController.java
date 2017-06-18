package com.corsair.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

/**
 * Created by guo_zhiqi on 2016/1/29.
 */
@Controller
public class MainIndexController {
    @RequestMapping(value = "/mainIndex", method = RequestMethod.GET)
    public ModelAndView redirect(){
        return new ModelAndView("mainIndex");
    }
}
