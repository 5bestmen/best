package com.corsair.service;

import com.corsair.dao.DeviceTemplateDao;
import com.corsair.dao.DisplayTemplateDao;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.entity.template.display.DisplayTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 2016/1/5.
 */
@Service("displayTemplateService")
public class DisplayTemplateService {

    @Autowired
    private DisplayTemplateDao displayTemplateDao;

    @Autowired
    private DeviceTemplateDao deviceTemplateDao;

    public List<DisplayTemplate> getDisplayTemplateList() {
        return displayTemplateDao.getDisplayTemplate();
    }

    public List<DisplayTemplate> getDisplayTemplate(String name, String desc) {
        return displayTemplateDao.getDisplayTemplate(name,desc);
    }

    //遍历设备模板方式获取所有显示模板
    public List<DisplayTemplate> getDisplayTemplate() {
        List<DeviceTemplate> deviceTemplateList = deviceTemplateDao.getDeviceTemplate();
        List<DisplayTemplate> ret = new ArrayList<>();
        for (DeviceTemplate deviceTemplate : deviceTemplateList){
            for (DisplayTemplate displayTemplate : deviceTemplate.getDisplayTemplates()){
                ret.add(displayTemplate);
            }
        }
        return ret;
    }

    //由于目前数据库中显示模板的ID是人工录入的，id有可能会存在空格，所以不能用findByID方法，只能用这个接口来获取显示模板
    public DisplayTemplate getDisplayTemplateById(String id) {
        List<DisplayTemplate> list = displayTemplateDao.getDisplayTemplate();
        for (DisplayTemplate template : list){
            //去掉ID的空格
            if (template.getId().trim().equals(id)){
                return template;
            }
        }
        return null;
//        return displayTemplateDao.getDisplayTemplateById(id);
    }

    public DisplayTemplate getDisplayTemplateByName(String name) {
        List<DisplayTemplate> displayTemplateList = displayTemplateDao.getDisplayTemplate();
        for (DisplayTemplate displayTemplate : displayTemplateList){
            if (displayTemplate.getName().equals(name)){
                return displayTemplate;
            }
        }
        return null;
    }

    public DisplayTemplate getDisplayTemplateByDesc(String desc) {
        List<DisplayTemplate> displayTemplateList = displayTemplateDao.getDisplayTemplate();
        for (DisplayTemplate displayTemplate : displayTemplateList){
            if (displayTemplate.getDesc().equals(desc)){
                return displayTemplate;
            }
        }
        return null;
    }

    //根据组合描述，获取指定设备模板数据
    public DeviceTemplateData getDeviceTemplateDataByDesc(String desc) {
        String[] strings = desc.split("-");
        //根据描述获取设备模板
        DeviceTemplate deviceTemplate = deviceTemplateDao.getUniqueDeviceTemplateByDesc(strings[0]);
        //根据设备模板获取设备模板数据
        for (DeviceTemplateData data : deviceTemplate.getDeviceTemplateDatas()){
            //返回所选的设备模板数据
            if (data.getDesc().equals(strings[1])){
                return data;
            }
        }
        return null;
    }

    public void saveOrUpdateDisplayTemplate(DisplayTemplate displayTemplate){
        displayTemplateDao.saveOrUpdate(displayTemplate);
    }

    public void deleteDeviceTemplate(String id){
        DisplayTemplate displayTemplate  = this.getDisplayTemplateById(id);
        displayTemplateDao.delete(displayTemplate);
    }

    public List<String> getNeedContainerFlag() {
        List<String> ret = new ArrayList<>();
        ret.add("否");
        ret.add("是");
        return ret;
    }

    public List<String> getDisplayTemplateDescList() {
        List<String> ret = new ArrayList<>();
        for (DisplayTemplate template : getDisplayTemplateList()){
            ret.add(template.getDesc());
        }
        return ret;
    }

    public boolean checkNameExist(String name){
        List<DisplayTemplate> list = displayTemplateDao.getDisplayTemplate();
        for (DisplayTemplate template : list){
            if (template.getName().equals(name)){
                return true;
            }
        }
        return false;
    }

    public boolean checkDescExist(String desc){
        List<DisplayTemplate> list = displayTemplateDao.getDisplayTemplate();
        for (DisplayTemplate template : list){
            if (template.getDesc().equals(desc)){
                return true;
            }
        }
        return false;
    }
}
