package com.corsair.controller;

import com.corsair.device.BasePSR;
import com.corsair.form.PackageTransformerForm;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.util.List;

/**
 * Created by libh on 2015/9/28.
 */
@Controller
public class PackageTransformerController {
    @Autowired
    ScadaPSRService scadaPSRService;

    @RequestMapping(value = "/packageTransformer", method = RequestMethod.GET)
    public ModelAndView redirect(PackageTransformerForm packageTransformerForm, HttpServletRequest request, HttpSession session) {
        ModelAndView modelAndView = new ModelAndView("transBox");

        String id = packageTransformerForm.getPackageTransformerID();
        if (id != null) {
            String name = scadaPSRService.getDeivceName(id);
            String arrayID = scadaPSRService.getPVArrayId(id);
            String arrayName = scadaPSRService.getPVArrayName(arrayID);
            List<BasePSR> scdDevices = scadaPSRService.getScdDevices(arrayID, 52);
            if (scdDevices.size() > 0){
                String measureDeviceID = scdDevices.get(0).getId();
                String measureDeviceName = scdDevices.get(0).getName();
                modelAndView.addObject("measureDeviceID", measureDeviceID);
                modelAndView.addObject("measureDeviceName", measureDeviceName);
            }
            modelAndView.addObject("id", id);
            modelAndView.addObject("name", name);
            modelAndView.addObject("arrayID", arrayID);
            modelAndView.addObject("arrayName", arrayName);
        }
        return modelAndView;
    }
}
