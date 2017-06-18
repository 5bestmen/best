package com.corsair.dao;

import com.corsair.entity.MaterialInformation;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by zjq on 2015/11/5.
 */
@Repository("materialInformationDao")
public class MaterialInformationDao extends BaseDaoSupport {
    public List<MaterialInformation> getMaterialInformationByNoAndId(String seriesNo,String id){
        Criteria crit = super.currentSession().createCriteria(MaterialInformation.class);

        if(CommonUtils.isNotEmpty(seriesNo)){
            crit.add(Restrictions.eq("materialSeriesNo", seriesNo));
        }
        if(CommonUtils.isNotEmpty(id)){
            crit.add(Restrictions.eq("materialId", id));
        }

        return crit.list();
    }

}
