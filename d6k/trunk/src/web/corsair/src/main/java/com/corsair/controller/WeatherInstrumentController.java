package com.corsair.controller;

import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;


@Controller
public class WeatherInstrumentController {
    @Autowired
    ScadaPSRService scadaPSRService;

    @RequestMapping(value = "/weatherInstrument", method = RequestMethod.GET)
    public ModelAndView redirect(String weatherInstrumentID, HttpServletRequest request, HttpSession session) {
        ModelAndView modelAndView = new ModelAndView("weatherInstrument");

        if (weatherInstrumentID != null) {
            String name = scadaPSRService.getDeivceName(weatherInstrumentID);
            String arrayID = scadaPSRService.getPVArrayId(weatherInstrumentID);
            String arrayName = scadaPSRService.getPVArrayName(arrayID);
//            List<BasePSR> scdDevices = scadaPSRService.getScdDevices(arrayID, 29);
//            if (scdDevices.size() > 0){
//                String measureDeviceID = scdDevices.get(0).getId();
//                String measureDeviceName = scdDevices.get(0).getName();
//                modelAndView.addObject("measureDeviceID", measureDeviceID);
//                modelAndView.addObject("measureDeviceName", measureDeviceName);
//            }
            modelAndView.addObject("id", weatherInstrumentID);
            modelAndView.addObject("name", name);
            modelAndView.addObject("arrayID", arrayID);
            modelAndView.addObject("arrayName", arrayName);
        }
        return modelAndView;
    }
}
