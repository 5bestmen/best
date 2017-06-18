package com.corsair.dao;

import com.corsair.entity.InverterPowerPoint;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.Criteria;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by zjq on 2015/8/8.
 */
@Repository("inverterPowerPointDao")
public class InverterPowerPointDao extends BaseDaoSupport {
    public List<InverterPowerPoint> getInverterPowerPointData(){
        Criteria criteria = super.currentSession().createCriteria(InverterPowerPoint.class);
        return criteria.list();
    }
}
