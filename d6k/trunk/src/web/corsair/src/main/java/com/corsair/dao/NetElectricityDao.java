package com.corsair.dao;

import com.corsair.entity.NetElectricity;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.Criteria;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by libh on 2015/12/17.
 */

@Repository("netElectricityDao")
public class NetElectricityDao extends BaseDaoSupport {
    public NetElectricity getNetElectricityByDate(String date) {
        Criteria criteria = super.currentSession().createCriteria(NetElectricity.class);
        criteria.add(Restrictions.eq("netDate", date));

        List result = criteria.list();

        return result.isEmpty() ? null : (NetElectricity) result.get(0);
    }
    public NetElectricity getNetElectricityByDate(String date, String station) {
        Criteria criteria = super.currentSession().createCriteria(NetElectricity.class);
        criteria.add(Restrictions.eq("netDate", date));
        criteria.add(Restrictions.eq("station", station));
        List result = criteria.list();

        return result.isEmpty() ? null : (NetElectricity) result.get(0);
    }

    public List<NetElectricity> getNetElectricityByStation(String station) {
        Criteria criteria = super.currentSession().createCriteria(NetElectricity.class);

        criteria.add(Restrictions.eq("station", station));
//        criteria.addOrder(Order.asc("to_date(netdate)"));
        List result = criteria.list();

        return result.isEmpty() ? null :  result;
    }
}
