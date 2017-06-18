package com.corsair.dao;

import com.corsair.entity.Material;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by libh on 2015/10/19.
 */
@Repository("materialDao")
public class MaterialDao extends BaseDaoSupport {
    /**
     * 根据物资编码查找物资型号，精确匹配
     * @param fixedID
     * @return
     */
    public List<Material> getMaterialModelListByExactFixedId(String fixedID){
        Criteria crit = super.currentSession().createCriteria(Material.class);

        if(CommonUtils.isNotEmpty(fixedID)){
            crit.add(Restrictions.eq("fixedID", fixedID));
        }
        return crit.list();
    }
}
