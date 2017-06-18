package com.corsair.controller;

import com.corsair.device.BasePSR;
import com.corsair.device.Inverter;
import com.corsair.device.PackageTransformer;
import com.corsair.device.WeatherInstrument;
import com.corsair.form.PVArrayForm;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

/**
 * Created by libh on 2015/9/28.
 */
@Controller
public class PVArrayController {
    @Autowired
    ScadaPSRService scadaPSRService;

    @Value("#{configProperties['corsair.site']}")
    private String site;

    @RequestMapping(value = "/pvarray", method = RequestMethod.GET)
    public ModelAndView redirect(PVArrayForm pvArrayForm, HttpServletRequest request, HttpSession session) {

        if (Objects.equals(site, "lijing")) {
            return redirectLijing(pvArrayForm, request, session);
        } else {
            return redirectChangyuan(pvArrayForm, request, session);
        }
    }

    public ModelAndView redirectLijing(PVArrayForm pvArrayForm, HttpServletRequest request, HttpSession session){
        ModelAndView modelAndView = new ModelAndView("pvarray_lijing");
        String arrayID = pvArrayForm.getPVArrayID();

        if (arrayID != null) {
            List inverters = getInverters(arrayID);
            List ammeters = scadaPSRService.getAmmeter(pvArrayForm.getPVArrayID());

            modelAndView.addObject("inverters", inverters);
            modelAndView.addObject("ammeters", ammeters);
        }
        return modelAndView;
    }

    public ModelAndView redirectChangyuan(PVArrayForm pvArrayForm, HttpServletRequest request, HttpSession session){
        ModelAndView modelAndView = new ModelAndView("pvarrayg");

        String arrayID = pvArrayForm.getPVArrayID();
        if (arrayID != null) {
            PackageTransformer packageTransformer = scadaPSRService.getPackageTransformer(arrayID);
            List inverters = scadaPSRService.getInverter(arrayID);
            List Junctions = scadaPSRService.getJunction(arrayID);
            String pvArrayName = scadaPSRService.getPVArrayName(arrayID);
            WeatherInstrument weatherInstrument = scadaPSRService.getWeatherInstrument();

            modelAndView.addObject("packageTransformer", packageTransformer);
            modelAndView.addObject("inverters", inverters);
            modelAndView.addObject("Junctions", Junctions);
            modelAndView.addObject("PVArrayID", pvArrayForm.getPVArrayID());
            modelAndView.addObject("PVArrayName", pvArrayName);
            modelAndView.addObject("weatherInstrument", weatherInstrument);
        }
        return modelAndView;
    }

    private List<BasePSR> getInverters(String arrayID){
        List<BasePSR> ret = new ArrayList<>();

        List<BasePSR> roofs = scadaPSRService.getRoof(arrayID);

        for(BasePSR roof : roofs){
            List<BasePSR> inverters = scadaPSRService.getChildren(roof.getId(),27);
            for (BasePSR inverter : inverters){
                inverter.setName(roof.getName() + "-" + inverter.getName());
                ret.add(inverter);
            }
        }
        return ret;
    }
}
