package com.corsair.dao;

import com.corsair.entity.OperateTicketTemplate;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.Criteria;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by zjq on 2015/10/19.
 */
@Repository("operateTicketTemplateDao")
public class OperateTicketTemplateDao extends BaseDaoSupport {

    public List<OperateTicketTemplate> getOperateTicketTemplateList() {
        Criteria criteria = super.currentSession().createCriteria(OperateTicketTemplate.class);
        List result = criteria.list();
        return result.isEmpty() ? null : result;
    }

}
