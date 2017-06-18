package com.corsair.dao;

import com.corsair.entity.SecurityMeasures;
import com.corsair.entity.WorkTicket;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by 洪祥 on 2015/10/26.
 */
@Repository("securityMeasuresDao")
public class SecurityMeasuresDao extends BaseDaoSupport {

    public List<SecurityMeasures> getSecurityMeasuresList(String measure) {
        Criteria criteria = super.currentSession().createCriteria(SecurityMeasures.class);
        if (CommonUtils.isNotEmpty(measure)) {
            criteria.add(Restrictions.ilike("measure", measure, MatchMode.ANYWHERE));
        }
        return criteria.list();
    }

    //根据流程ID查找工作票实例
    public List<SecurityMeasures> getSecurityMeasureListByTypeAndId(String securityMeasuresType, WorkTicket workTicket) {

       /* List list = super.currentSession().createCriteria(WorkTicket.class).add(
                Restrictions.eq("securityMeasuresType", securityMeasuresType)).list();*/
        Criteria criteria = super.currentSession().createCriteria(SecurityMeasures.class);
        criteria.add(Restrictions.eq("securityMeasuresType", securityMeasuresType));
        criteria.add(Restrictions.eq("workTicket.id", workTicket.getId()));
        return criteria.list();
    }
}
