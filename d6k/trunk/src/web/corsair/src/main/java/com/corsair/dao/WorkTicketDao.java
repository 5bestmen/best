package com.corsair.dao;

import com.corsair.entity.SecurityMeasures;
import com.corsair.entity.WorkTicket;
import com.corsair.entity.WorkTicketRecord;
import com.rbac.common.BaseDaoSupport;
import com.rbac.entity.SysAccount;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by 洪祥 on 2015/10/23.
 */
@Repository("workTicketDao")
public class WorkTicketDao extends BaseDaoSupport{

    public List<WorkTicket> getWorkTicketList(String workTicketType, String code){
        Criteria criteria = super.currentSession().createCriteria(WorkTicket.class);
        if(CommonUtils.isNotEmpty(workTicketType)){
            criteria.add(Restrictions.ilike("workTicketType", workTicketType, MatchMode.ANYWHERE));
        }
        if(CommonUtils.isNotEmpty(code)){
            criteria.add(Restrictions.ilike("code", code, MatchMode.ANYWHERE));
        }
        return criteria.list();
    }

    //根据流程ID查找工作票实例
    public WorkTicket getWorkTicketByProcessId(String processId ){

        List list = super.currentSession().createCriteria(WorkTicket.class).add(
                Restrictions.eq("processId", processId)).list();

        return list.size() > 0 ? (WorkTicket) list.get(0) : null;
    }

    //根据任务ID查找工作票实例
    public WorkTicket getWorkTicketByTaskId(String taskId ){

        List list = super.currentSession().createCriteria(WorkTicket.class).add(
                Restrictions.eq("taskId", taskId)).list();

        return list.size() > 0 ? (WorkTicket) list.get(0) : null;
    }

}
