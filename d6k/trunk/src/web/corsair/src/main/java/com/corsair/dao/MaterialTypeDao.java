package com.corsair.dao;

import com.corsair.entity.MaterialType;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.Order;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by libh on 2015/10/19.
 */
@Repository("materialTypeDao")
public class MaterialTypeDao extends BaseDaoSupport {
    /**
     * 获取所有物资类别的列表
     * @param
     * @return
     */
    public List<MaterialType> getMaterialTypes() {
        Criteria criteria = super.currentSession().createCriteria(MaterialType.class);
        criteria.addOrder(Order.asc("materialTypeID"));
        return criteria.list();
    }

    /**
     * 根据类别id查找物资类别，精确匹配
     * @param name
     * @return
     */
    public List<MaterialType> getMaterialTypeListByExactName(String name){
        Criteria crit = super.currentSession().createCriteria(MaterialType.class);

        if(CommonUtils.isNotEmpty(name)){
            crit.add(Restrictions.eq("name", name));
        }
        return crit.list();
    }
}
