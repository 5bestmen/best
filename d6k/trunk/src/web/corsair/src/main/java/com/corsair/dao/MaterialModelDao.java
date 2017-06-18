package com.corsair.dao;

import com.corsair.entity.MaterialModel;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by libh on 2015/10/19.
 */
@Repository("materialModelDao")
public class MaterialModelDao extends BaseDaoSupport {
    /**
     * 根据物资编码查找物资型号，精确匹配
     * @param codeID
     * @return
     */
    public List<MaterialModel> getMaterialModelListByExactCodeId(String codeID){
        Criteria crit = super.currentSession().createCriteria(MaterialModel.class);

        if(CommonUtils.isNotEmpty(codeID)){
            crit.add(Restrictions.eq("codeID", codeID));
        }
        return crit.list();
    }
}
