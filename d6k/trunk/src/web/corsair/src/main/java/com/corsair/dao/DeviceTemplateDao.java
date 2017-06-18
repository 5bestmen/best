package com.corsair.dao;

import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.display.DisplayTemplate;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by 洪祥 on 15/7/14.
 */
@Repository("deviceTemplateDao")
public class DeviceTemplateDao extends BaseDaoSupport {

    public List<DeviceTemplate> getDeviceTemplate() {
        Criteria criteria = super.currentSession().createCriteria(DeviceTemplate.class);

        return criteria.list();
    }

    /**
     * 根据设备模板名和设备模板描述查找设备模板
     * @param name
     * @param desc
     * @return
     */
    public List<DeviceTemplate> getDeviceTemplate(String name, String desc) {
        Criteria criteria = super.currentSession().createCriteria(DeviceTemplate.class);
        if(CommonUtils.isNotEmpty(name)){
            criteria.add(Restrictions.ilike("name", name, MatchMode.ANYWHERE));
        }
        if(CommonUtils.isNotEmpty(desc)){
            criteria.add(Restrictions.ilike("desc", desc, MatchMode.ANYWHERE));
        }
        return criteria.list();
    }

    /**
     * 根据模板ID查找设备模板
     * @param id
     * @return
     */
    public List<DeviceTemplate> getDeviceTemplateListById(String id){
        Criteria criteria = super.currentSession().createCriteria(DeviceTemplate.class);
        if(CommonUtils.isNotEmpty(id)){
            criteria.add(Restrictions.eq("id", id));
        }
        return criteria.list();
    }

    /**
     * 根据模板ID查找设备模板
     * @param id
     * @return
     */
    public DeviceTemplate findById(String id){
        return super.findById(DeviceTemplate.class, id);
    }

    public List<DeviceTemplate> getDeviceTemplateByName(String name) {
        Criteria criteria = super.currentSession().createCriteria(DisplayTemplate.class);
        if(CommonUtils.isNotEmpty(name)){
            criteria.add(Restrictions.eq("name", name));
        }
        return criteria.list();
    }

    public List<DeviceTemplate> getDeviceTemplateByDesc(String desc) {
        Criteria criteria = super.currentSession().createCriteria(DisplayTemplate.class);
        if(CommonUtils.isNotEmpty(desc)){
            criteria.add(Restrictions.eq("desc", desc));
        }
        return criteria.list();
    }

    public DeviceTemplate getUniqueDeviceTemplateByDesc(String desc) {
        for (DeviceTemplate deviceTemplate : getDeviceTemplate()){
            if (deviceTemplate.getDesc().equals(desc)){
                return deviceTemplate;
            }
        }
        return null;
    }
}
