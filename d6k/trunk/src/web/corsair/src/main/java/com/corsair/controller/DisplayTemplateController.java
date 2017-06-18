package com.corsair.controller;

import com.corsair.form.DisplayTemplateForm;
import com.corsair.service.DisplayTemplateService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.List;

/**
 * Created by 洪祥 on 2015/12/12.
 */
@Controller
public class DisplayTemplateController {
    @Autowired
    DisplayTemplateService displayTemplateService ;

    @RequestMapping(value ="/displayTemplate")
    public ModelAndView displayTemplateControl(DisplayTemplateForm displayTemplateForm, HttpServletRequest request,HttpServletResponse response){
        ModelAndView model = new ModelAndView("/displayTemplate");
        if(CommonUtils.isNotBlank(displayTemplateForm.getDel())){
            displayTemplateService.deleteDeviceTemplate(displayTemplateForm.getDeleteIds());
        }else if (CommonUtils.isNotBlank(displayTemplateForm.getName()) ||CommonUtils.isNotBlank(displayTemplateForm.getDesc())){
            //页面查询
            List displayTemplateList = displayTemplateService.getDisplayTemplate(displayTemplateForm.getName(),displayTemplateForm.getDesc());
            model.addObject("displayTemplateList", displayTemplateList);
        }else {
            //按照设备模板遍历显示模板，页面排序需要
            List displayTemplateList = displayTemplateService.getDisplayTemplate();
            model.addObject("displayTemplateList", displayTemplateList);
        }

        return model;
    }
}
