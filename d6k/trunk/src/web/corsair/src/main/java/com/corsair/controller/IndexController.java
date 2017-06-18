package com.corsair.controller;

import com.corsair.device.BasePSR;
import com.corsair.device.PVPowerStation;
import com.corsair.device.PackageTransformer;
import com.corsair.device.WeatherInstrument;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import java.util.Objects;

/**
 * Created by 洪祥 on 2015/9/25.
 */
@Controller
public class IndexController {
    @Autowired
    ScadaPSRService scadaPSRService;

    @Value("#{configProperties['corsair.site']}")
    private String site;

    @Value("#{configProperties['scada.only.station']}")
    private String onlyStation;

    @RequestMapping(value = "/index", method = RequestMethod.GET)
    public ModelAndView redirectIndex() {

        if (Objects.equals(site, "lijing")) {
            return redirectLijingIndex();
        } else {
            return redirectChangyuanIndex();
        }
    }

    private ModelAndView redirectLijingIndex() {
        ModelAndView modelAndView = new ModelAndView("index_lijing");
        modelAndView.addObject("station", onlyStation);
        return modelAndView;
    }

    private ModelAndView redirectChangyuanIndex() {
        ModelAndView modelAndView = new ModelAndView("index");
        PVPowerStation station = scadaPSRService.getPVStation();
        WeatherInstrument weatherInstrument = scadaPSRService.getWeatherInstrument();
        BasePSR photovoltaicArray = (BasePSR) scadaPSRService.getPVArray().get(0);
        PackageTransformer packageTransformer = scadaPSRService.getPackageTransformer(photovoltaicArray.getId());
        modelAndView.addObject("station", station);
        modelAndView.addObject("weatherInstrument", weatherInstrument);
        modelAndView.addObject("packageTransformer", packageTransformer);
        return modelAndView;
    }
}
