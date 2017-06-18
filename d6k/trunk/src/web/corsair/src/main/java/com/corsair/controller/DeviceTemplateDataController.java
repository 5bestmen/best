package com.corsair.controller;

import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.form.DeviceTemplateDataForm;
import com.corsair.service.DeviceTemplateDataService;
import com.corsair.service.DeviceTemplateService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 2015/12/16.
 */
@Controller
public class DeviceTemplateDataController {
    @Autowired
    DeviceTemplateDataService deviceTemplateDataService ;

    @Autowired
    DeviceTemplateService deviceTemplateService;

    @RequestMapping(value ="/deviceTemplateData")
    public ModelAndView deviceTemplateDataControl(DeviceTemplateDataForm deviceTemplateDataForm,
                                                  HttpServletRequest request,HttpServletResponse response){

        ModelAndView model = new ModelAndView("/deviceTemplateData");

        if(CommonUtils.isNotBlank(deviceTemplateDataForm.getDel())){ //删除模板数据后重绘页面

            //首先通过即将被删除的模板数据，找到它所对应的设备模板
            DeviceTemplate deviceTemplate = deviceTemplateDataService.getDeviceTemplateDataById(deviceTemplateDataForm.getDeleteIds()).getDeviceTemplate();
            String deviceTemplateId = deviceTemplate.getId();
            ModelAndView modelAndView = new ModelAndView("redirect:/deviceTemplateData?id=" + deviceTemplateId);

            deviceTemplateDataService.deleteDeviceTemplateData(deviceTemplateDataForm.getDeleteIds());

            return modelAndView;
        }

        if (CommonUtils.isNotBlank(deviceTemplateDataForm.getId())){

            List deviceTemplateDataList = deviceTemplateDataService.getDeviceTemplateDatas(deviceTemplateDataForm.getId());
            String titleDesc = deviceTemplateService.getDeviceTemplateById(deviceTemplateDataForm.getId()).getDesc();

            //设备模板有数据
            if(deviceTemplateDataList.size() > 0){
                //按照类型和索引对模板数据进行升序排序
                deviceTemplateDataList = sort(deviceTemplateDataList);
            }

            model.addObject("deviceTemplateDataList", deviceTemplateDataList);
            model.addObject("titleDesc", titleDesc + "-");

        }else if(CommonUtils.isNotBlank(deviceTemplateDataForm.getDesc())){ //提供模板数据的模糊查询功能

            List<DeviceTemplateData> deviceTemplateDataList = deviceTemplateDataService.getDeviceTemplateDataListByDesc(deviceTemplateDataForm.getDesc());
            model.addObject("deviceTemplateDataList", deviceTemplateDataList);
            model.addObject("titleDesc", "");
        }

        return model;
    }

    //按照数据类型对设备模板数据进行升序排序
    private void sortByType(List<DeviceTemplateData> deviceTemplateDataList) {

        for (int i = 0; i < deviceTemplateDataList.size(); i++) {

            for (int j = i + 1; j < deviceTemplateDataList.size(); j++) {

                int frontType= deviceTemplateDataList.get(i).getType();
                int behindType = deviceTemplateDataList.get(j).getType();

                if (frontType > behindType) {
                    DeviceTemplateData temp = deviceTemplateDataList.get(i);
                    deviceTemplateDataList.set(i, deviceTemplateDataList.get(j));
                    deviceTemplateDataList.set(j, temp);
                }
            }
        }
    }

    //按照数据索引对设备模板数据进行升序排序
    private void sortByIndex(List<DeviceTemplateData> deviceTemplateDataList) {

        for (int i = 0; i < deviceTemplateDataList.size(); i++) {

            for (int j = i + 1; j < deviceTemplateDataList.size(); j++) {

                int frontIndex= deviceTemplateDataList.get(i).getIndex();
                int behindIndex = deviceTemplateDataList.get(j).getIndex();

                if (frontIndex > behindIndex) {
                    DeviceTemplateData temp = deviceTemplateDataList.get(i);
                    deviceTemplateDataList.set(i, deviceTemplateDataList.get(j));
                    deviceTemplateDataList.set(j, temp);
                }
            }
        }
    }

    //对设备模板数据进行排序
    private List<DeviceTemplateData> sort(List<DeviceTemplateData> deviceTemplateDataList) {

        List<DeviceTemplateData> ret = new ArrayList<>();
        List<DeviceTemplateData> tempList = new ArrayList<>();

        //第一轮排序，按照数据类型升序排序
        sortByType(deviceTemplateDataList);

        //第二轮排序，对同一类型的模板数据按照索引号进行升序排序
        for (int i = 0; i < deviceTemplateDataList.size(); i++) {
            //保证不越界
            if (i < deviceTemplateDataList.size() - 1){
                // 判断前一个模板数据的类型是否和后一个相同
                if (deviceTemplateDataList.get(i).getType() == deviceTemplateDataList.get(i+1).getType()){
                    //把同一类型的模板数据放到一个中间链表中去
                    tempList.add(deviceTemplateDataList.get(i));
                    continue;
                }else {
                    //对同一类型的模板数据进行索引升序排序，并拼接
                    tempList.add(deviceTemplateDataList.get(i));
                    sortByIndex(tempList);
                    ret.addAll(tempList);
                    tempList.clear();
                }
            }else{
                //处理最后一个类型的模板数据
                tempList.add(deviceTemplateDataList.get(i));
                sortByIndex(tempList);
                ret.addAll(tempList);
            }
        }

        return ret;
    }
}
