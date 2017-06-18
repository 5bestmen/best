package com.corsair.controller;

import com.corsair.entity.template.display.DisplayTemplate;
import com.corsair.entity.template.display.DisplayTemplateData;
import com.corsair.form.DisplayTemplateDataModifyForm;
import com.corsair.service.*;
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
public class DisplayTemplateDataModifyController {
    @Autowired
    DisplayTemplateDataService displayTemplateDataService ;

    @Autowired
    DeviceTemplateDataService deviceTemplateDataService ;

    @Autowired
    DisplayTemplateService displayTemplateService;

    @RequestMapping(value ="/displayTemplateDataModify", method = RequestMethod.GET)
    public ModelAndView displayTemplateDataModifyGetControl(DisplayTemplateDataModifyForm displayTemplateDataModifyForm,
                                              HttpServletRequest request,HttpServletResponse response){

        ModelAndView model = new ModelAndView("/displayTemplateDataModify");

        if(CommonUtils.isNotBlank(displayTemplateDataModifyForm.getId())){ //编辑数据

            String id = displayTemplateDataModifyForm.getId();
            DisplayTemplateData displayTemplateData = displayTemplateDataService.getDisplayTemplateDataById(id);
            displayTemplateDataModifyForm.setDisplayTemplate(displayTemplateData.getDisplayTemplate().getDesc());
            String device = displayTemplateData.getDeviceTemplateData().getDeviceTemplate().getDesc();
            String deviceData = displayTemplateData.getDeviceTemplateData().getDesc();
            displayTemplateDataModifyForm.setDeviceTemplateData(device + "-" + deviceData);
            displayTemplateDataModifyForm.setIndex(displayTemplateData.getIndex().toString());
            displayTemplateDataModifyForm.setIcon(displayTemplateData.getIcon());
            displayTemplateDataModifyForm.setExtraString1(displayTemplateData.getExtraString1());
            displayTemplateDataModifyForm.setExtraString2(displayTemplateData.getExtraString2());
            displayTemplateDataModifyForm.setExtraString3(displayTemplateData.getExtraString3());
        }

        model.addObject("displayTemplates",displayTemplateService.getDisplayTemplateDescList());
        model.addObject("deviceTemplateDatas",deviceTemplateDataService.getDeviceTemplateDataDescList());

        return model;
    }

    @RequestMapping(value ="/displayTemplateDataModify", method = RequestMethod.POST)
    public ModelAndView displayTemplateDataModifyPostControl(DisplayTemplateDataModifyForm displayTemplateDataModifyForm,
                                                       HttpServletRequest request,HttpServletResponse response){
        ModelAndView model = null;
        //保存
        if (CommonUtils.isNotBlank(displayTemplateDataModifyForm.getSubmit())){
            String id = displayTemplateDataModifyForm.getId();
            if (CommonUtils.isNotBlank(id)){    //修改模板数据
                DisplayTemplateData displayTemplateData = displayTemplateDataService.getDisplayTemplateDataById(id);
                DisplayTemplate displayTemplate = displayTemplateService.getDisplayTemplateByDesc(displayTemplateDataModifyForm.getDisplayTemplate());
                displayTemplateData.setDisplayTemplate(displayTemplate);
                displayTemplateData.setIndex(Integer.parseInt(displayTemplateDataModifyForm.getIndex()));
                displayTemplateData.setDeviceTemplateData(displayTemplateService.getDeviceTemplateDataByDesc(displayTemplateDataModifyForm.getDeviceTemplateData()));
                displayTemplateData.setIcon(displayTemplateDataModifyForm.getIcon());
                displayTemplateData.setExtraString1(displayTemplateDataModifyForm.getExtraString1());
                displayTemplateData.setExtraString2(displayTemplateDataModifyForm.getExtraString2());
                displayTemplateData.setExtraString3(displayTemplateDataModifyForm.getExtraString3());

                displayTemplateDataService.saveOrUpdateDisplayTemplateData(displayTemplateData);
                model = new ModelAndView("redirect:/displayTemplateData?id=" + displayTemplateData.getDisplayTemplate().getId());

            }else { //新增模板

                DisplayTemplateData displayTemplateData = new DisplayTemplateData();

                DisplayTemplate displayTemplate = displayTemplateService.getDisplayTemplateByDesc(displayTemplateDataModifyForm.getDisplayTemplate());
                displayTemplateData.setDisplayTemplate(displayTemplate);
                displayTemplateData.setIndex(Integer.parseInt(displayTemplateDataModifyForm.getIndex()));
                displayTemplateData.setDeviceTemplateData(displayTemplateService.getDeviceTemplateDataByDesc(displayTemplateDataModifyForm.getDeviceTemplateData()));
                displayTemplateData.setIcon(displayTemplateDataModifyForm.getIcon());
                displayTemplateData.setExtraString1(displayTemplateDataModifyForm.getExtraString1());
                displayTemplateData.setExtraString2(displayTemplateDataModifyForm.getExtraString2());
                displayTemplateData.setExtraString3(displayTemplateDataModifyForm.getExtraString3());

                displayTemplateDataService.saveOrUpdateDisplayTemplateData(displayTemplateData);

                model = new ModelAndView("redirect:/displayTemplateData?id=" + displayTemplateData.getDisplayTemplate().getId());

            }
        }

        return model;
    }
}
