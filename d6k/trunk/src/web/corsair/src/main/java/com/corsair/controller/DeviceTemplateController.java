package com.corsair.controller;

import com.corsair.service.DeviceTemplateService;
import com.rbac.util.CommonUtils;
import com.corsair.form.DeviceTemplateForm;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.net.URLEncoder;
import java.util.List;

/**
 * Created by 洪祥 on 2015/12/12.
 */
@Controller
public class DeviceTemplateController {
    @Autowired
    DeviceTemplateService deviceTemplateService ;

    @RequestMapping(value ="/deviceTemplate")
    public ModelAndView deviceTemplateControl(DeviceTemplateForm deviceTemplateForm, HttpServletRequest request,HttpServletResponse response){
        ModelAndView model = new ModelAndView("/deviceTemplate");
        if(CommonUtils.isNotBlank(deviceTemplateForm.getDel())){
            deviceTemplateService.deleteDeviceTemplate(deviceTemplateForm.getDeleteIds());
        }
        List deviceTemplateList = deviceTemplateService.getDeviceTemplateList(deviceTemplateForm.getName(), deviceTemplateForm.getDesc());
        model.addObject("deviceTemplateList", deviceTemplateList);
        return model;
    }
}
