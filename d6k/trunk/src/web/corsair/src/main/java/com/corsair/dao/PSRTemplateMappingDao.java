package com.corsair.dao;

import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.PSRTemplateMapping;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.Criteria;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by 洪祥 on 15/7/15.
 */
@Repository("pSRTemplateMappingDao")
public class PSRTemplateMappingDao extends BaseDaoSupport {
    public DeviceTemplate getDeviceTemplateByPSRID(String id) {
        Criteria criteria = super.currentSession().createCriteria(PSRTemplateMapping.class);
        criteria.add(Restrictions.eq("PSRID", id));
        List result = criteria.list();

        return result.isEmpty() ? null : ((PSRTemplateMapping)result.get(0)).getDeviceTemplate();
    }

    public PSRTemplateMapping getPSRTemplateMapping(String name) {
        Criteria criteria = super.currentSession().createCriteria(PSRTemplateMapping.class);
        criteria.add(Restrictions.eq("PSRID", name));
        List result = criteria.list();

        return result.isEmpty() ? null : (PSRTemplateMapping) result.get(0);
    }

    public PSRTemplateMapping getPSRTemplateMappingById(String id) {
        Criteria criteria = super.currentSession().createCriteria(PSRTemplateMapping.class);
        criteria.add(Restrictions.eq("id", id));
        List result = criteria.list();

        return result.isEmpty() ? null : (PSRTemplateMapping) result.get(0);
    }

    public DeviceTemplate getDeviceTemplateById(String id) {
        Criteria criteria = super.currentSession().createCriteria(PSRTemplateMapping.class);
        criteria.add(Restrictions.eq("id", id));
        List result = criteria.list();

        return result.isEmpty() ? null : ((PSRTemplateMapping)result.get(0)).getDeviceTemplate();
    }
}
