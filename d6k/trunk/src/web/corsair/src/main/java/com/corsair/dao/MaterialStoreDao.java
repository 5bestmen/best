package com.corsair.dao;

import com.corsair.entity.MaterialStore;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by zjq on 2015/11/6.
 */
@Repository("materialStoreDao")
public class MaterialStoreDao extends BaseDaoSupport {
    public List<MaterialStore> getMaterialStoresByInfoId(String id){
        Criteria crit = super.currentSession().createCriteria(MaterialStore.class);

        if(CommonUtils.isNotEmpty(id)){
            crit.add(Restrictions.eq("materialInformation", id));
        }

        return crit.list();
    }
}
