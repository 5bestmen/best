package com.corsair.dao;

import com.corsair.entity.ElectricTime;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.Criteria;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by libh on 2015/12/22.
 */
@Repository("electricTimeDao")
public class ElectricTimeDao extends BaseDaoSupport {
    public List<ElectricTime> getElectricTimeList() {
        Criteria criteria = super.currentSession().createCriteria(ElectricTime.class);
        return criteria.list();
    }
}
