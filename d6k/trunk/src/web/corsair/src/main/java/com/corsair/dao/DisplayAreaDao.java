package com.corsair.dao;

import com.corsair.entity.template.display.DisplayArea;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by 洪祥 on 2016/1/7.
 */
@Repository("displayAreaDao")
public class DisplayAreaDao extends BaseDaoSupport{

    public List<DisplayArea> getDisplayArea() {
        Criteria criteria = super.currentSession().createCriteria(DisplayArea.class);

        return criteria.list();
    }

    //用来页面查询
    public List<DisplayArea> getDisplayArea(String name, String comments) {
        Criteria criteria = super.currentSession().createCriteria(DisplayArea.class);
        if(CommonUtils.isNotEmpty(name)){
            criteria.add(Restrictions.ilike("name", name, MatchMode.ANYWHERE));
        }
        if(CommonUtils.isNotEmpty(comments)){
            criteria.add(Restrictions.ilike("comments", comments, MatchMode.ANYWHERE));
        }
        return criteria.list();
    }

    public DisplayArea getDisplayAreaByName(String name) {
        Criteria criteria = super.currentSession().createCriteria(DisplayArea.class);
        criteria.add(Restrictions.eq("name", name));
        List result = criteria.list();

        return result.isEmpty() ? null : (DisplayArea) result.get(0);
    }

    public DisplayArea getDisplayAreaById(String id){
        return super.findById(DisplayArea.class, id);
    }
}
