package com.corsair.dao;

import com.corsair.entity.OperateTicket;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.Criteria;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by zjq on 2015/10/12.
 */
@Repository("operateTicketDao")
public class OperateTicketDao extends BaseDaoSupport {

    public OperateTicket getOperatorTicketByName(String name) {
        Criteria criteria = super.currentSession().createCriteria(OperateTicket.class);
        criteria.add(Restrictions.eq("name", name));
        List result = criteria.list();

        return result.isEmpty() ? null : (OperateTicket) result.get(0);
    }

    public List<OperateTicket> getOperateTicketList() {
        Criteria criteria = super.currentSession().createCriteria(OperateTicket.class);
        List result = criteria.list();
        return result.isEmpty() ? null : result;
    }

}
