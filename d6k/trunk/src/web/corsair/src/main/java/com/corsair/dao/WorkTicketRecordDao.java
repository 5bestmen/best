package com.corsair.dao;

import com.corsair.entity.SecurityMeasures;
import com.corsair.entity.WorkTicket;
import com.corsair.entity.WorkTicketRecord;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by 洪祥 on 2015/10/26.
 */
@Repository("workTicketRecordDao")
public class WorkTicketRecordDao extends BaseDaoSupport {

    public List<WorkTicketRecord> getWorkTicketRecordList(WorkTicket workTicket){
        Criteria criteria = super.currentSession().createCriteria(WorkTicketRecord.class);
        if(CommonUtils.isNotEmpty(workTicket.getId())){
            criteria.add(Restrictions.ilike("workTicket.id", workTicket.getId(), MatchMode.ANYWHERE));
        }
        return criteria.list();
    }
}
