package com.corsair.dao;

import com.corsair.entity.MaterialModelLable;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Order;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by libh on 2015/10/19.
 */
@Repository("materialModelLableDao")
public class MaterialModelLableDao extends BaseDaoSupport {
    public List<MaterialModelLable> getMaterialModelLables() {
        Criteria criteria = super.currentSession().createCriteria(MaterialModelLable.class);
        criteria.addOrder(Order.asc("materialModelLableID"));
        return criteria.list();
    }

    /**
     * 根据描述查找物料modelLabel，精确查找
     * @param modelName
     * @return
     */
    public MaterialModelLable getMaterialModelLabel(String modelName){
        Criteria crit = super.currentSession().createCriteria(MaterialModelLable.class);
        if(CommonUtils.isNotEmpty(modelName)){
            crit.add(Restrictions.eq("name", modelName));
        }
        List result = crit.list();
        return result.isEmpty() ? null : (MaterialModelLable) result.get(0);
    }

    /**
     * 根据物料model描述查找
     * @param modelName
     * @return
     */
    public List<MaterialModelLable> getMaterialModelLabelList(String modelName){
        Criteria crit = super.currentSession().createCriteria(MaterialModelLable.class);
        if(CommonUtils.isNotEmpty(modelName)){
            crit.add(Restrictions.ilike("name", modelName, MatchMode.ANYWHERE));
        }
        return crit.list();
    }
}
