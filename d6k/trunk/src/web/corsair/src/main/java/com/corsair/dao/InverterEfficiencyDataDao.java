package com.corsair.dao;

import com.corsair.entity.InverterEfficiencyData;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.Criteria;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by zjq on 2015/8/7.
 */
@Repository("inverterEfficiencyDataDao")
public class InverterEfficiencyDataDao extends BaseDaoSupport {

    public List<InverterEfficiencyData> getInverterEfficiencyData(String inverterEfficiencyID,String inverterTemperatureID){
        Criteria criteria = super.currentSession().createCriteria(InverterEfficiencyData.class);
        criteria.add(Restrictions.eq("inverterEfficiency.id",inverterEfficiencyID));
        criteria.add(Restrictions.eq("inverterTemperature.id",inverterTemperatureID));
        List result = criteria.list();
        return  result;
    }
}
