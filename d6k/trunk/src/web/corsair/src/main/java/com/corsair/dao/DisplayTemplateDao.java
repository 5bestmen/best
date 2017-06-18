package com.corsair.dao;

import com.corsair.entity.template.display.DisplayTemplate;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by 洪祥 on 2016/1/5.
 */
@Repository("displayTemplateDao")
public class DisplayTemplateDao extends BaseDaoSupport {

    public List<DisplayTemplate> getDisplayTemplate() {
        Criteria criteria = super.currentSession().createCriteria(DisplayTemplate.class);

        return criteria.list();
    }


    public List<DisplayTemplate> getDisplayTemplate(String name, String desc) {
        Criteria criteria = super.currentSession().createCriteria(DisplayTemplate.class);
        if(CommonUtils.isNotEmpty(name)){
            criteria.add(Restrictions.ilike("name", name, MatchMode.ANYWHERE));
        }
        if(CommonUtils.isNotEmpty(desc)){
            criteria.add(Restrictions.ilike("desc", desc, MatchMode.ANYWHERE));
        }
        return criteria.list();
    }

    public DisplayTemplate getDisplayTemplateById(String id){
        return super.findById(DisplayTemplate.class, id);
    }
}
