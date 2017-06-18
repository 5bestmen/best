package com.corsair.dao;

import com.corsair.entity.ElectricPrice;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.Criteria;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by libh on 2015/12/22.
 */
@Repository("electricPriceDao")
public class ElectricPriceDao extends BaseDaoSupport{
    public List<ElectricPrice> getElectricPriceList() {
        Criteria criteria = super.currentSession().createCriteria(ElectricPrice.class);
        return criteria.list();
    }
}
