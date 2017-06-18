package com.corsair.dao;

import com.corsair.entity.template.psr.AccumulateType;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by 洪祥 on 15/12/16.
 */
@Repository("accumulateType")
public class AccumulateTypeDao extends BaseDaoSupport {

    public List<AccumulateType> getAccumulateType() {
        Criteria criteria = super.currentSession().createCriteria(AccumulateType.class);

        return criteria.list();
    }

    /**
     * 根据类型名和类型描述查找累积类型
     * @param name
     * @param comments
     * @return
     */
    public List<AccumulateType> getAccumulateType(String name, String comments) {
        Criteria criteria = super.currentSession().createCriteria(AccumulateType.class);
        if(CommonUtils.isNotEmpty(name)){
            criteria.add(Restrictions.ilike("name", name, MatchMode.ANYWHERE));
        }
        if(CommonUtils.isNotEmpty(comments)){
            criteria.add(Restrictions.ilike("comments", comments, MatchMode.ANYWHERE));
        }
        return criteria.list();
    }

    public List<AccumulateType> getAccumulateTypeByName(String name) {
        Criteria criteria = super.currentSession().createCriteria(AccumulateType.class);
        if(CommonUtils.isNotEmpty(name)){
            criteria.add(Restrictions.eq("name", name));
        }
        return criteria.list();
    }

    /**
     * 根据ID查找累积类型
     * @param id
     * @return
     */
    public AccumulateType getAccumulateTypeById(String id){
        return findById(id);
    }

    public AccumulateType findById(String id){
        return super.findById(AccumulateType.class, id);
    }
}
