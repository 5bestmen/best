package com.corsair.service;

import com.corsair.dao.DeviceTemplateDao;
import com.corsair.entity.template.psr.DeviceTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 2015/9/15.
 */
@Service("deviceTemplateService")
public class DeviceTemplateService {
    @Autowired
    private DeviceTemplateDao deviceTemplateDao;

    /**
     * 保存设备模板
     * @param deviceTemplate
     * @return
     */
    public void saveOrUpdateDeviceTemplate(DeviceTemplate deviceTemplate){
        deviceTemplateDao.saveOrUpdate(deviceTemplate);
    }

    /**
     * 根据id查找模板实体
     * @param id
     * @return
     */
    public DeviceTemplate getDeviceTemplateById(String id){
        return deviceTemplateDao.findById(DeviceTemplate.class, id);
    }

    public DeviceTemplate getDeviceTemplateByDesc(String desc){
        DeviceTemplate deviceTemplate = null;
        List<DeviceTemplate> deviceTemplateList = deviceTemplateDao.getDeviceTemplate();
        for (DeviceTemplate template : deviceTemplateList){
            if (template.getDesc().equals(desc)){
                deviceTemplate = template;
            }
        }
        return deviceTemplate;
    }

    /**
     * 根据模板名和模板描述查找模板列表
     * @param name
     * @param comments
     * @return
     */
    public List<DeviceTemplate> getDeviceTemplateList(String name, String comments){
        return deviceTemplateDao.getDeviceTemplate(name, comments);
    }

    public List<DeviceTemplate> getDeviceTemplateList(){
        return deviceTemplateDao.getDeviceTemplate();
    }

    /**
     * 根据模板名和模板描述查找模板的PSRType列表
     * @param name
     * @param comments
     * @return
     */
    public List<String> getDeviceTemplatePSRTypeList(String name, String comments){
        List<String> type = new ArrayList<>();
        for(DeviceTemplate deviceTemplate : deviceTemplateDao.getDeviceTemplate(name, comments)){
            type.add(deviceTemplate.getPSRType());
        }
        return type;
    }

    public List<String> getDeviceTemplateDescList(){
        List<String> ret = new ArrayList<>();
        for(DeviceTemplate deviceTemplate : getDeviceTemplateList()){
            ret.add(deviceTemplate.getDesc());
        }
        return ret;
    }

    /**
     * 删除模板
     * @param id
     */
    public void deleteDeviceTemplate(String id){
        DeviceTemplate deviceTemplate  = this.getDeviceTemplateById(id);
        deviceTemplateDao.delete(deviceTemplate);
    }

    /**
     * 检查模板名是否已经存在
     * @param name
     * @return
     */
    public boolean checkNameExist(String name){
        List<DeviceTemplate> list = deviceTemplateDao.getDeviceTemplate();
        for (DeviceTemplate template : list){
            if (template.getName().equals(name)){
                return true;
            }
        }
        return false;
    }

    /**
     * 检查模板描述是否已经存在
     * @param desc
     * @return
     */
    public boolean checkDescExist(String desc){
        List<DeviceTemplate> list = deviceTemplateDao.getDeviceTemplate();
        for (DeviceTemplate template : list){
            if (template.getDesc().equals(desc)){
                return true;
            }
        }
        return false;
    }
}
