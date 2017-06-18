package com.corsair.controller;


import com.corsair.entity.MaterialType;
import com.corsair.form.MaterialTypeModifyForm;
import com.corsair.service.MaterialService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

/**
 * Created by libh on 2015/10/26.
 */
@Controller
public class MaterialTypeModifyController {
    @Autowired
    MaterialService materialService;

    @RequestMapping(value = "/materialTypeModify", method = RequestMethod.GET)
    public ModelAndView ActionModify(MaterialTypeModifyForm materialTypeModifyForm, HttpServletRequest request, HttpSession session){
        ModelAndView modelAndView ;
        if(CommonUtils.isNotBlank(materialTypeModifyForm.getId())){
            String id = materialTypeModifyForm.getId();
            MaterialType materialType = materialService.getMaterialTypeById(id);
            materialTypeModifyForm.setMaterialTypeName(materialType.getName());
            materialTypeModifyForm.setMaterialTypeDesc(materialType.getComments());
            modelAndView = new ModelAndView("materialTypeModify");
        }
        else {
            modelAndView = new ModelAndView("materialTypeModify");
        }
        return modelAndView;
    }

    @RequestMapping(value= "/materialTypeModify", method = RequestMethod.POST)
    public ModelAndView ActionModifyAction(MaterialTypeModifyForm materialTypeModifyForm,HttpServletRequest request,HttpSession session){
        ModelAndView model;
        if(CommonUtils.isNotBlank(materialTypeModifyForm.getSubmit())){
            MaterialType materialType = new MaterialType();
            if(CommonUtils.isNotBlank(materialTypeModifyForm.getId())){
                materialType = materialService.getMaterialTypeById(materialTypeModifyForm.getId());
            }
            materialType.setName(materialTypeModifyForm.getMaterialTypeName());
            materialType.setComments(materialTypeModifyForm.getMaterialTypeDesc());
            materialService.saveOrUpdateMaterialType(materialType);
            model = new ModelAndView("redirect:/materialSort");
        }
        else {
            model = new ModelAndView("materialTypeModify");
        }
        return model;
    }
}
