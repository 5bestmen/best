package com.corsair.controller;

import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.form.DeviceTemplateModifyForm;
import com.corsair.service.DeviceTemplateService;
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
public class DeviceTemplateModifyController {
    @Autowired
    DeviceTemplateService deviceTemplateService ;

    @RequestMapping(value ="/deviceTemplateModify", method = RequestMethod.GET)
    public ModelAndView deviceTemplateModifyGetControl(DeviceTemplateModifyForm deviceTemplateModifyForm,
                                              HttpServletRequest request,HttpServletResponse response){

        ModelAndView model = new ModelAndView("/deviceTemplateModify");

        if(CommonUtils.isNotBlank(deviceTemplateModifyForm.getId())){

            String id = deviceTemplateModifyForm.getId();
            DeviceTemplate deviceTemplate = deviceTemplateService.getDeviceTemplateById(id);
            deviceTemplateModifyForm.setName(deviceTemplate.getName());
            deviceTemplateModifyForm.setDesc(deviceTemplate.getDesc());
            deviceTemplateModifyForm.setPsrType(deviceTemplate.getPSRType());
        }
        model.addObject("PSRTypes",deviceTemplateService.getDeviceTemplatePSRTypeList(null,null));
        return model;
    }

    @RequestMapping(value ="/deviceTemplateModify", method = RequestMethod.POST)
    public ModelAndView deviceTemplateModifyPostControl(DeviceTemplateModifyForm deviceTemplateModifyForm,
                                                       HttpServletRequest request,HttpServletResponse response){
        //todo 张云,为了调整表结构暂时注释
//        if (CommonUtils.isNotBlank(deviceTemplateModifyForm.getSubmit())){
//            String id = deviceTemplateModifyForm.getId();
//            if (CommonUtils.isNotBlank(id)){    //修改模板
//                DeviceTemplate deviceTemplate = deviceTemplateService.getDeviceTemplateById(id);
//                deviceTemplate.setName(deviceTemplateModifyForm.getName());
//                deviceTemplate.setDesc(deviceTemplateModifyForm.getDesc());
//                deviceTemplate.setPSRType(deviceTemplateModifyForm.getPsrType());
//                deviceTemplateService.saveOrUpdateDeviceTemplate(deviceTemplate);
//            }else { //新增模板
//                DeviceTemplate deviceTemplate = new DeviceTemplate();
//                deviceTemplate.setName(deviceTemplateModifyForm.getName());
//                deviceTemplate.setDesc(deviceTemplateModifyForm.getDesc());
//                deviceTemplate.setPSRType(deviceTemplateModifyForm.getPsrType());
//                deviceTemplateService.saveOrUpdateDeviceTemplate(deviceTemplate);
//            }
//        }

        return new ModelAndView("redirect:/deviceTemplate");
    }
}
