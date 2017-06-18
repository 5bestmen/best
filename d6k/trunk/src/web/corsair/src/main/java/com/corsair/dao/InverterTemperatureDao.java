package com.corsair.dao;

import com.corsair.entity.InverterTemperature;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.Criteria;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by zjq on 2015/8/8.
 */
@Repository("inverterTemperatureDao")
public class InverterTemperatureDao extends BaseDaoSupport {
    public List<InverterTemperature> getInverterTemperatureData(){
        Criteria criteria = super.currentSession().createCriteria(InverterTemperature.class);

        return criteria.list();
    }
}
