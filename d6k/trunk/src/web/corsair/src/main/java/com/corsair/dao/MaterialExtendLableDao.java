package com.corsair.dao;

import com.corsair.entity.MaterialExtendLable;
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
@Repository("materialExtendLableDao")
public class MaterialExtendLableDao extends BaseDaoSupport{
    public List<MaterialExtendLable> getMaterialExtendLables() {
        Criteria criteria = super.currentSession().createCriteria(MaterialExtendLable.class);
        criteria.addOrder(Order.asc("materialExtendLableID"));
        return criteria.list();
    }

    /**
     * 根据描述查找物料extendLabel，精确查找
     * @param extendName
     * @return
     */
    public MaterialExtendLable getMaterialExtendLabel(String extendName){
        Criteria crit = super.currentSession().createCriteria(MaterialExtendLable.class);
        if(CommonUtils.isNotEmpty(extendName)){
            crit.add(Restrictions.eq("name", extendName));
        }
        List result = crit.list();
        return result.isEmpty() ? null : (MaterialExtendLable) result.get(0);
    }

    /**
     * 根据物料扩展描述查找
     * @param extendName
     * @return
     */
    public List<MaterialExtendLable> getMaterialExtendLabelList(String extendName){
        Criteria crit = super.currentSession().createCriteria(MaterialExtendLable.class);
        if(CommonUtils.isNotEmpty(extendName)){
            crit.add(Restrictions.ilike("name", extendName, MatchMode.ANYWHERE));
        }
        return crit.list();
    }
}
