package com.corsair.dao;

import com.corsair.entity.template.display.DisplayTemplate;
import com.corsair.entity.template.display.DisplayTemplateData;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.Criteria;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

/**
 * Created by 洪祥 on 2016/1/7.
 */
@Repository("displayTemplateDataDao")
public class DisplayTemplateDataDao extends BaseDaoSupport {

    @Autowired
    DisplayTemplateDao displayTemplateDao;


    public List<DisplayTemplateData> getDeviceTemplateData() {
        Criteria criteria = super.currentSession().createCriteria(DisplayTemplateData.class);

        return criteria.list();
    }

    public List<DisplayTemplateData> getDisplayTemplateDataById(String id){
//        DisplayTemplate displayTemplate = displayTemplateDao.getDisplayTemplateById(id);
        DisplayTemplate displayTemplate = getDisplayTemplateById(id);
        List<DisplayTemplateData> ret = new ArrayList<>();
        if(displayTemplate != null){
            Set<DisplayTemplateData> set = displayTemplate.getDisplayTemplateDatas();
            for(DisplayTemplateData data : set){
                ret.add(data);
            }
        }
        return ret;
    }

    //由于目前数据库中显示模板的ID是人工录入的，id有可能会存在空格，所以不能用findByID方法，只能用这个接口来找显示模板
    private DisplayTemplate getDisplayTemplateById(String id){
        List<DisplayTemplate> list = displayTemplateDao.getDisplayTemplate();
        for (DisplayTemplate template : list){
            //去掉ID的空格
            if (template.getId().trim().equals(id)){
                return template;
            }
        }
        return null;
    }
}
