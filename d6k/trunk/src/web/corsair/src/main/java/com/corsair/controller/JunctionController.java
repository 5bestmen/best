package com.corsair.controller;

import com.corsair.form.JunctionForm;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

/**
 * Created by libh on 2015/9/28.
 */
@Controller
public class JunctionController {
    @Autowired
    ScadaPSRService scadaPSRService;

    @RequestMapping(value = "/junction", method = RequestMethod.GET)
    public ModelAndView redirect(JunctionForm junctionForm, HttpServletRequest request, HttpSession session) {
        ModelAndView modelAndView = new ModelAndView("junctionBox");

        String id = junctionForm.getJunctionID();
        if (id != null) {
            String name = scadaPSRService.getDeivceName(id);
            String arrayID = scadaPSRService.getPVArrayId(id);
            String arrayName = scadaPSRService.getPVArrayName(arrayID);

            modelAndView.addObject("id", id);
            modelAndView.addObject("name", name);
            modelAndView.addObject("arrayID", arrayID);
            modelAndView.addObject("arrayName", arrayName);
        }
        return modelAndView;
    }
}
