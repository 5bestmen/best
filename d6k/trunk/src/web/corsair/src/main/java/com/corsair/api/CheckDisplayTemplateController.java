package com.corsair.api;

import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.service.DisplayTemplateDataService;
import com.corsair.service.DisplayTemplateService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.regex.Pattern;

/**
 * Created by zjq on 2016/1/27.
 */
@RestController
@RequestMapping(value = "/displayTemplate")
public class CheckDisplayTemplateController {
    @Autowired
    DisplayTemplateDataService displayTemplateDataService;
    @Autowired
    DisplayTemplateService displayTemplateService;

    @RequestMapping(value = "checkDisplayTemplateDataIndex", method = RequestMethod.POST, produces = "text/html;charset=UTF-8")
    public ResponseEntity<String> checkDisplayTemplateDataIndex(String index,String displayTemplate, HttpServletRequest request, HttpServletResponse response) {

        String ret = null;

         if (CommonUtils.isNotBlank(index)) {

             if (!isNumber(index)) {
                 ret = "indexNotNumber";
             } else {
                 Boolean isExist = displayTemplateDataService.checkIndexExist(displayTemplate, index);

                 if (isExist) {
                     ret = "isExist";
                 } else {
                     ret = "ok";
                 }
             }

         }

        return new ResponseEntity<>(ret, HttpStatus.OK);
    }

    @RequestMapping(value = "checkDisplayTemplateDesc", method = RequestMethod.POST, produces = "text/html;charset=UTF-8")
    public ResponseEntity<String> checkDisplayTemplateDesc(String desc,HttpServletResponse response) {

        String ret = null;

        if (CommonUtils.isNotBlank(desc)) {
            Boolean isExist = displayTemplateService.checkDescExist(desc);

            if (isExist) {
                ret = "1";
            } else {
                ret = "0";
            }

        }

        return new ResponseEntity<>(ret, HttpStatus.OK);
    }

    @RequestMapping(value = "checkDisplayTemplateName", method = RequestMethod.POST, produces = "text/html;charset=UTF-8")
    public ResponseEntity<String> checkDisplayTemplateName(String name, HttpServletRequest request, HttpServletResponse response){
        String ret = null;

        if (CommonUtils.isNotBlank(name)) {
            Boolean isExist = displayTemplateService.checkNameExist(name);

            if (isExist) {
                ret = "1";
            } else {
                ret = "0";
            }
        }

        return new ResponseEntity<>(ret, HttpStatus.OK);
    }

    @RequestMapping(value = "confirmDeviceTemplate", method = RequestMethod.POST, produces = "text/html;charset=UTF-8")
    public ResponseEntity<String> confirmDeviceTemplate(String displayTemplate, HttpServletRequest request, HttpServletResponse response){

        String ret = null;

        if(CommonUtils.isNotBlank(displayTemplate)){
            DeviceTemplate deviceTemplate = displayTemplateService.getDisplayTemplateByDesc(displayTemplate).getDeviceTemplate();

            if (deviceTemplate != null){
                ret = deviceTemplate.getDesc();
            }else {
                ret = "error";
            }
        }

        return new ResponseEntity<>(ret, HttpStatus.OK);
    }


    //正则表达式判断字符串是否为数字
    private boolean isNumber(String str){
        Pattern pattern = Pattern.compile("[0-9]*");
        return pattern.matcher(str).matches();
    }
}
