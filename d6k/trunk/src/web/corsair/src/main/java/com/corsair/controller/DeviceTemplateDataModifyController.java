package com.corsair.controller;

import com.corsair.entity.template.psr.AccumulateType;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.entity.SpecialChart;
import com.corsair.form.DeviceTemplateDataModifyForm;
import com.corsair.service.ConfigFileService;
import com.corsair.service.DeviceTemplateDataService;
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
public class DeviceTemplateDataModifyController {
    @Autowired
    DeviceTemplateDataService deviceTemplateDataService ;

    @Autowired
    DeviceTemplateService deviceTemplateService;

    @Autowired
    ConfigFileService configFileService;

    @RequestMapping(value ="/deviceTemplateDataModify", method = RequestMethod.GET)
    public ModelAndView deviceTemplateDataModifyGetControl(DeviceTemplateDataModifyForm deviceTemplateDataModifyForm,
                                              HttpServletRequest request,HttpServletResponse response){

        ModelAndView model = new ModelAndView("/deviceTemplateDataModify");

        if(CommonUtils.isNotBlank(deviceTemplateDataModifyForm.getId())){ //编辑数据

            String id = deviceTemplateDataModifyForm.getId();
            DeviceTemplateData deviceTemplateData = deviceTemplateDataService.getDeviceTemplateDataById(id);
            deviceTemplateDataModifyForm.setDeviceTemplate(deviceTemplateData.getDeviceTemplate().getDesc());
            String type = configFileService.getDeviceTemplateDataStringTypeByIntType(deviceTemplateData.getType());
            deviceTemplateDataModifyForm.setType(type);
            deviceTemplateDataModifyForm.setIndex(deviceTemplateData.getIndex().toString());
            deviceTemplateDataModifyForm.setName(deviceTemplateData.getName());
            deviceTemplateDataModifyForm.setDesc(deviceTemplateData.getDesc());
            //todo 张云为了调整表结构暂时注释
//            deviceTemplateDataModifyForm.setUnit(deviceTemplateData.getUnitType());
//            //以下两项在数据库中存在大量空值，为避免程序执行toString出错，需要加一层判断
//            if (deviceTemplateData.getDefaultValue() != null){
//                deviceTemplateDataModifyForm.setDefaultValue(deviceTemplateData.getDefaultValue().toString());
//            }
//
//            if(deviceTemplateData.getMaxValue() != null){
//                deviceTemplateDataModifyForm.setMaxValue(deviceTemplateData.getMaxValue().toString());
//            }
//
//            deviceTemplateDataModifyForm.setMagic(deviceTemplateData.getMagic());
//            deviceTemplateDataModifyForm.setChangePattern(convertChangePatternToString(deviceTemplateData.getChangePattern()));
            deviceTemplateDataModifyForm.setInvisibleInChart(convertInvisibleInChart(deviceTemplateData.getInvisibleInChart()));
            deviceTemplateDataModifyForm.setAccumulateType(convertAccumulateType(deviceTemplateData.getAccumulateType()));
            deviceTemplateDataModifyForm.setSpecialChart(convertSpecialChart(deviceTemplateData.getSpecialChart()));
        }

        model.addObject("deviceTemplates",deviceTemplateDataService.getDeviceTemplateDescList(null,null));
        model.addObject("types",configFileService.getDeviceTemplateDataStringTypes());
        model.addObject("changePatterns",deviceTemplateDataService.getDeviceTemplateDataChangePattern());
        model.addObject("invisibleInCharts",deviceTemplateDataService.getDeviceTemplateDataInvisibleInChart());
        model.addObject("accumulateTypes",deviceTemplateDataService.getAccumulateTypeDescList());
        model.addObject("specialCharts",deviceTemplateDataService.getSpecialChartDescList());

        return model;
    }

    @RequestMapping(value ="/deviceTemplateDataModify", method = RequestMethod.POST)
    public ModelAndView deviceTemplateDataModifyPostControl(DeviceTemplateDataModifyForm deviceTemplateDataModifyForm,
                                                       HttpServletRequest request,HttpServletResponse response){
        ModelAndView model = null;
        //todo 张云,为了调整表结构暂时注释
//        if (CommonUtils.isNotBlank(deviceTemplateDataModifyForm.getSubmit())){
//            String id = deviceTemplateDataModifyForm.getId();
//            if (CommonUtils.isNotBlank(id)){    //修改模板
//                DeviceTemplateData deviceTemplateData = deviceTemplateDataService.getDeviceTemplateDataById(id);
//                DeviceTemplate deviceTemplate = deviceTemplateService.getDeviceTemplateByDesc(deviceTemplateDataModifyForm.getDeviceTemplate());
//                deviceTemplateData.setDeviceTemplate(deviceTemplate);
//                Integer type = configFileService.getDeviceTemplateDataIntTypeByStringType(deviceTemplateDataModifyForm.getType());
//                deviceTemplateData.setType(type);
//                deviceTemplateData.setIndex(Integer.parseInt(deviceTemplateDataModifyForm.getIndex()));
//                deviceTemplateData.setName(deviceTemplateDataModifyForm.getName());
//                deviceTemplateData.setDesc(deviceTemplateDataModifyForm.getDesc());
//                deviceTemplateData.setUnitType(deviceTemplateDataModifyForm.getUnit());
//
//                if (deviceTemplateDataModifyForm.getDefaultValue() != null
//                        && !deviceTemplateDataModifyForm.getDefaultValue().equals("")){
//                    deviceTemplateData.setDefaultValue(Double.parseDouble(deviceTemplateDataModifyForm.getDefaultValue()));
//                }
//
//                if (deviceTemplateDataModifyForm.getMaxValue() != null
//                        && !deviceTemplateDataModifyForm.getMaxValue().equals("")){
//                    deviceTemplateData.setMaxValue(Double.parseDouble(deviceTemplateDataModifyForm.getMaxValue()));
//                }
//
//                deviceTemplateData.setMagic(deviceTemplateDataModifyForm.getMagic());
//                deviceTemplateData.setChangePattern(convertChangePatternToInteger(deviceTemplateDataModifyForm.getChangePattern()));
//                deviceTemplateData.setInvisibleInChart(convertInvisibleInChart(deviceTemplateDataModifyForm.getInvisibleInChart()));
//                AccumulateType accumulateType = deviceTemplateDataService.getAccumulateTypeByDesc(deviceTemplateDataModifyForm.getAccumulateType());
//                deviceTemplateData.setAccumulateType(accumulateType);
//                SpecialChart specialChart = deviceTemplateDataService.getSpecialChartByDesc(deviceTemplateDataModifyForm.getSpecialChart());
//                deviceTemplateData.setSpecialChart(specialChart);
//
//                deviceTemplateDataService.saveOrUpdateDeviceTemplateData(deviceTemplateData);
//                model = new ModelAndView("redirect:/deviceTemplateData?id=" + deviceTemplateData.getDeviceTemplate().getId());
//
//            }else { //新增模板
//
//                DeviceTemplateData deviceTemplateData = new DeviceTemplateData();
//                DeviceTemplate deviceTemplate = deviceTemplateService.getDeviceTemplateByDesc(deviceTemplateDataModifyForm.getDeviceTemplate());
//                deviceTemplateData.setDeviceTemplate(deviceTemplate);
//                Integer type = configFileService.getDeviceTemplateDataIntTypeByStringType(deviceTemplateDataModifyForm.getType());
//                deviceTemplateData.setType(type);
//                deviceTemplateData.setIndex(Integer.parseInt(deviceTemplateDataModifyForm.getIndex()));
//                deviceTemplateData.setName(deviceTemplateDataModifyForm.getName());
//                deviceTemplateData.setDesc(deviceTemplateDataModifyForm.getDesc());
//                deviceTemplateData.setUnitType(deviceTemplateDataModifyForm.getUnit());
//
//                if (deviceTemplateDataModifyForm.getDefaultValue() != null
//                        && !deviceTemplateDataModifyForm.getDefaultValue().equals("")){
//                    deviceTemplateData.setDefaultValue(Double.parseDouble(deviceTemplateDataModifyForm.getDefaultValue()));
//                }
//
//                if (deviceTemplateDataModifyForm.getMaxValue() != null
//                        && !deviceTemplateDataModifyForm.getMaxValue().equals("")){
//                    deviceTemplateData.setMaxValue(Double.parseDouble(deviceTemplateDataModifyForm.getMaxValue()));
//                }
//
//                deviceTemplateData.setMagic(deviceTemplateDataModifyForm.getMagic());
//                deviceTemplateData.setChangePattern(convertChangePatternToInteger(deviceTemplateDataModifyForm.getChangePattern()));
//                deviceTemplateData.setInvisibleInChart(convertInvisibleInChart(deviceTemplateDataModifyForm.getInvisibleInChart()));
//                AccumulateType accumulateType = deviceTemplateDataService.getAccumulateTypeByDesc(deviceTemplateDataModifyForm.getAccumulateType());
//                deviceTemplateData.setAccumulateType(accumulateType);
//                SpecialChart specialChart = deviceTemplateDataService.getSpecialChartByDesc(deviceTemplateDataModifyForm.getSpecialChart());
//                deviceTemplateData.setSpecialChart(specialChart);
//
//                deviceTemplateDataService.saveOrUpdateDeviceTemplateData(deviceTemplateData);
//                model = new ModelAndView("redirect:/deviceTemplateData?id=" + deviceTemplateData.getDeviceTemplate().getId());
//
//            }
//        }

        return model;
    }

    private String convertTypeToString(Integer type){

        String ret = "";

        if (type == 1){
            ret = "遥信";
        }else  if (type == 2){
            ret = "遥测";
        }else if (type == 3){
            ret = "电度";
        }else {
            ret = "特殊图表";
        }

        return ret;
    }

    private Integer convertTypeToInteger(String type){

        int ret = 0;

        if (type.equals("遥信")){
            ret = 1;
        }else  if (type.equals("遥测")){
            ret = 2;
        }else if (type.equals("电度")){
            ret = 3;
        }else {
            ret = 4;
        }

        return ret;
    }

    private String convertChangePatternToString(Integer changePattern){

        String ret = "";

        if(changePattern == null){
            ret = "实采值";
        }else if (changePattern == 0){
            ret = "不变化";
        }else  if (changePattern == 1){
            ret = "正态变化";
        }else if (changePattern == 2){
            ret = "线性变化";
        }

        return ret;
    }

    private Integer convertChangePatternToInteger(String changePattern){

        Integer ret = null;

        if(!changePattern.equals("实采值")){

            if (changePattern.equals("不变化")){
                ret = 0;
            }else  if (changePattern.equals("正态变化")){
                ret = 1;
            }else if (changePattern.equals("线性变化")){
                ret = 2;
            }

        }
        return ret;
    }

    private String convertInvisibleInChart(Boolean invisibleInChart){

        String ret = "";

        if (invisibleInChart == null){
            ret = "不设置";
        }else  if (invisibleInChart == true){
            ret = "可见";
        }else if (invisibleInChart == false){
            ret = "不可见";
        }

        return ret;
    }

    private Boolean convertInvisibleInChart(String invisibleInChart){

        Boolean ret = null;

        if (!invisibleInChart.equals("不设置")){

            if (invisibleInChart.equals("可见")){
                ret = true;
            }else if (invisibleInChart.equals("不可见")) {
                ret = false;
            }

        }
        return ret;
    }

    private String convertAccumulateType(AccumulateType accumulateType){

        String ret = "";

        if (accumulateType == null){
            ret = "无";
        }else {
            ret = accumulateType.getComments();
        }

        return ret;
    }

    private String convertSpecialChart(SpecialChart specialChart){

        String ret = "";

        if (specialChart == null){
            ret = "无";
        }else {
            ret = specialChart.getComments();
        }

        return ret;
    }
}
