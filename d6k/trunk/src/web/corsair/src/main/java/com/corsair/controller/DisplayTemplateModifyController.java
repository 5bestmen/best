package com.corsair.controller;

import com.corsair.entity.template.display.DisplayTemplate;
import com.corsair.form.DisplayTemplateModifyForm;
import com.corsair.service.DeviceTemplateService;
import com.corsair.service.DisplayAreaService;
import com.corsair.service.DisplayTemplateService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Created by 洪祥 on 2015/12/15.
 */
@Controller
public class DisplayTemplateModifyController {

    @Autowired
    DisplayTemplateService displayTemplateService ;

    @Autowired
    DisplayAreaService displayAreaService ;

    @Autowired
    DeviceTemplateService deviceTemplateService ;

    @RequestMapping(value ="/displayTemplateModify", method = RequestMethod.GET)
    public ModelAndView displayTemplateModifyGetControl(DisplayTemplateModifyForm displayTemplateModifyForm,
                                              HttpServletRequest request,HttpServletResponse response){

        ModelAndView model = new ModelAndView("/displayTemplateModify");

        if(CommonUtils.isNotBlank(displayTemplateModifyForm.getId())){

            String id = displayTemplateModifyForm.getId();
            DisplayTemplate displayTemplate = displayTemplateService.getDisplayTemplateById(id);
            displayTemplateModifyForm.setName(displayTemplate.getName());
            displayTemplateModifyForm.setDesc(displayTemplate.getDesc());
            displayTemplateModifyForm.setNeedContainer(convertNeedContainer(displayTemplate.getNeedContainer()));
            displayTemplateModifyForm.setDeviceTemplate(displayTemplate.getDeviceTemplate().getDesc());
            displayTemplateModifyForm.setDisplayArea(displayTemplate.getDisplayArea().getComments());
        }
        model.addObject("needFlags",displayTemplateService.getNeedContainerFlag());
        model.addObject("deviceTemplates",deviceTemplateService.getDeviceTemplateDescList());
        model.addObject("displayAreas",displayAreaService.getDisplayAreaDescList());
        return model;
    }

    @RequestMapping(value ="/displayTemplateModify", method = RequestMethod.POST)
    public ModelAndView deviceTemplateModifyPostControl(DisplayTemplateModifyForm displayTemplateModifyForm,
                                                       HttpServletRequest request,HttpServletResponse response){
        //保存
        if (CommonUtils.isNotBlank(displayTemplateModifyForm.getSubmit())){
            String id = displayTemplateModifyForm.getId();
            if (CommonUtils.isNotBlank(id)){    //修改模板
                DisplayTemplate displayTemplate = displayTemplateService.getDisplayTemplateById(id);
                displayTemplate.setName(displayTemplateModifyForm.getName());
                displayTemplate.setDesc(displayTemplateModifyForm.getDesc());
                displayTemplate.setNeedContainer(displayTemplateModifyForm.getNeedContainer().equals("否") ? false : true);
                displayTemplate.setDeviceTemplate(deviceTemplateService.getDeviceTemplateByDesc(displayTemplateModifyForm.getDeviceTemplate()));
                displayTemplate.setDisplayArea(displayAreaService.getDisplayAreaByComments(displayTemplateModifyForm.getDisplayArea()));
                displayTemplateService.saveOrUpdateDisplayTemplate(displayTemplate);
            }else { //新增模板
                DisplayTemplate displayTemplate = new DisplayTemplate();
                displayTemplate.setName(displayTemplateModifyForm.getName());
                displayTemplate.setDesc(displayTemplateModifyForm.getDesc());
                displayTemplate.setNeedContainer(displayTemplateModifyForm.getNeedContainer().equals("否") ? false : true);
                displayTemplate.setDeviceTemplate(deviceTemplateService.getDeviceTemplateByDesc(displayTemplateModifyForm.getDeviceTemplate()));
                displayTemplate.setDisplayArea(displayAreaService.getDisplayAreaByComments(displayTemplateModifyForm.getDisplayArea()));
                displayTemplateService.saveOrUpdateDisplayTemplate(displayTemplate);
            }
        }

        return new ModelAndView("redirect:/displayTemplate");
    }

    private String convertNeedContainer(Boolean isNeed){
        String ret = "否";
        if (isNeed == true){
            ret = "是";
        }
        return ret;
    }
}
