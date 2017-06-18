package com.corsair.dao;

import com.corsair.entity.InverterEfficiency;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.Criteria;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by zjq on 2015/8/7.
 */
@Repository("inverterEfficiencyDao")
public class InverterEfficiencyDao extends BaseDaoSupport {

    public InverterEfficiency getInverterEfficiency(String devicetemplateID){
        Criteria criteria = super.currentSession().createCriteria(InverterEfficiency.class);
        criteria.add(Restrictions.eq("deviceTemplate.id",devicetemplateID));
        List result = criteria.list();
        return  (InverterEfficiency)result.get(0);
    }
}
