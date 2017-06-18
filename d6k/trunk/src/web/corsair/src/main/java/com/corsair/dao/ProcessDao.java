package com.corsair.dao;

import com.corsair.entity.WorkTicket;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Restrictions;
import org.snaker.engine.entity.Task;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by 洪祥 on 2015/11/06.
 */
@Repository("processDao")
public class ProcessDao extends BaseDaoSupport{

    public List<Process> getProcessList(String name,String displayName){
        Criteria criteria = super.currentSession().createCriteria(Process.class);
        if(CommonUtils.isNotEmpty(name)){
            criteria.add(Restrictions.ilike("name", name, MatchMode.ANYWHERE));
        }
        if(CommonUtils.isNotEmpty(displayName)){
            criteria.add(Restrictions.ilike("displayName", displayName, MatchMode.ANYWHERE));
        }
        return criteria.list();
    }
}
