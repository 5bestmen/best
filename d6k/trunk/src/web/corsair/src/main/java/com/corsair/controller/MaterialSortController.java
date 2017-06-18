package com.corsair.controller;


import com.corsair.form.MaterialForm;
import com.corsair.service.MaterialService;

import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;


/**
 * Created by libh on 2015/10/20.
 */
@Controller
public class MaterialSortController {
    @Autowired
    MaterialService materialService;

    @RequestMapping(value = "/materialSort")
    public ModelAndView redirect(MaterialForm materialForm, HttpServletRequest request,HttpSession session){
        ModelAndView modelAndView = new ModelAndView("materialSort");
        if (CommonUtils.isNotBlank(materialForm.getDel())) {
            if (materialForm.getDel().equals("materialType")){
                materialService.deleteMaterialType(materialForm.getDeleteIds());
            }
            else if(materialForm.getDel().equals("materialModel")){
                materialService.deleteMaterialModel(materialForm.getDeleteIds());
            }
            else if (materialForm.getDel().equals("material")){
                materialService.deleteMaterial(materialForm.getDeleteIds());
            }
        }
        modelAndView.addObject("menuJson",materialService.getMaterialTree());
        return modelAndView;
    }
}
