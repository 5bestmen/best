package com.corsair.api;

import com.corsair.service.DeviceTemplateDataService;
import com.corsair.service.DeviceTemplateService;
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
@RequestMapping(value = "/deviceTemplate")
public class CheckDeviceTemplateController {
    @Autowired
    DeviceTemplateDataService deviceTemplateDataService;
    @Autowired
    DeviceTemplateService deviceTemplateService;

    @RequestMapping(value = "checkDeviceTemplateDataIndex", method = RequestMethod.POST)
    public ResponseEntity<String> checkDeviceTemplateDataIndex(String index, String type, String deviceTemplate, HttpServletRequest request, HttpServletResponse response){

        String ret = null;


            if(CommonUtils.isNotBlank(index)) {

//                response.setContentType("text/html;charset=UTF-8");
//                response.setBufferSize(1024);
//                response.setHeader("Pragma", "No-cache");
//                response.setHeader("Cache-Control", "no-cache");

                if (!isNumber(index)) {
                    ret = "indexNotNumber";
                } else {
                    Boolean isExist = deviceTemplateDataService.checkIndexExist(index, deviceTemplate, type);
                    if (isExist) {
                        ret = "isExist";
                    } else {
                        ret = "ok";
                    }
                }
            }
        return new ResponseEntity<>(ret,HttpStatus.OK);
    }


    @RequestMapping(value = "checkDeviceTemplateDataName", method = RequestMethod.POST)
    public ResponseEntity<String> checkDeviceTemplateDataName(String name, HttpServletRequest request, HttpServletResponse response){

        String ret = null;
         if(CommonUtils.isNotBlank(name)){
             Boolean isExist = deviceTemplateDataService.checkNameExist(name);

             if (isExist) {
                 ret = "1";
             } else {
                 ret = "0";
             }
         }
        return new ResponseEntity<>(ret, HttpStatus.OK);
    }

    @RequestMapping(value = "checkDeviceTemplateDataValue", method = RequestMethod.POST, produces = "text/html;charset=UTF-8")
    public ResponseEntity<String> checkDeviceTemplateDataValue(String defaultValue,String maxValue, HttpServletRequest request, HttpServletResponse response){


        String ret = null;
        if(CommonUtils.isNotBlank(defaultValue) && !CommonUtils.isNotBlank(maxValue)){ //默认值是否为数字

            if (!isNumber(defaultValue)){
                ret = "defaultValueNotNumber";
            }else {
                ret = "defaultValueIsNumber";
            }

        }else if(!CommonUtils.isNotBlank(defaultValue) && CommonUtils.isNotBlank(maxValue)){	//最大值是否为数字

            if (!isNumber(maxValue)){
                ret = "maxValueNotNumber";
            }else {
                ret = "maxValueIsNumber";
            }

        }else if (CommonUtils.isNotBlank(defaultValue) && CommonUtils.isNotBlank(maxValue)) { //最大值是否小于默认值

            // 默认值和最大值都为数字时，才判断
            if (isNumber(defaultValue) && isNumber(maxValue)) {
                if (Double.parseDouble(defaultValue) > Double.parseDouble(maxValue)) {
                    ret = "maxValueError";
                } else {
                    ret = "ok";
                }
            } else {
                //走到这一步说明，要么默认值不是数字，要么最大值不是数字，或者两者都不是数字
                if (!isNumber(defaultValue)) {
                    ret = "defaultValueNotNumber";
                } else {
                    ret = "maxValueNotNumber";
                }
            }
        }
        return new ResponseEntity<>(ret, HttpStatus.OK);
    }


    @RequestMapping(value = "checkDeviceTemplateDesc", method = RequestMethod.POST, produces = "text/html;charset=UTF-8")
    public ResponseEntity<String> checkDeviceTemplateDesc(String desc, HttpServletRequest request, HttpServletResponse response){


        String ret = null;

        if (CommonUtils.isNotBlank(desc)) {
            Boolean isExist = deviceTemplateService.checkDescExist(desc);
            if (isExist) {
                ret = "1";
            } else {
                ret = "0";
            }
        }

        return new ResponseEntity<>(ret, HttpStatus.OK);
    }

    @RequestMapping(value = "checkDeviceTemplateName", method = RequestMethod.POST, produces = "text/html;charset=UTF-8")
    public ResponseEntity<String> checkDeviceTemplateName(String name, HttpServletRequest request, HttpServletResponse response){

        String ret = null;

            if (CommonUtils.isNotBlank(name)) {
                Boolean isExist = deviceTemplateService.checkNameExist(name);

                if (isExist) {
                    ret = "1";
                } else {
                    ret = "0";
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
