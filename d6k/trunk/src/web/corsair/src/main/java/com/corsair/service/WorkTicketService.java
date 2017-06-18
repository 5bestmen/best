package com.corsair.service;

import com.corsair.dao.SecurityMeasuresDao;
import com.corsair.dao.WorkTicketDao;
import com.corsair.dao.WorkTicketRecordDao;
import com.corsair.entity.SecurityMeasures;
import com.corsair.entity.WorkTicket;
import com.corsair.entity.WorkTicketRecord;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * Created by 洪祥 on 2015/10/23.
 */
@Service("workTicketService")
public class WorkTicketService {
    @Autowired
    private WorkTicketDao workTicketDao;
    @Autowired
    private SecurityMeasuresDao securityMeasuresDao;
    @Autowired
    private WorkTicketRecordDao workTicketRecordDao;

    public List<WorkTicket> getWorkTicketList(String workTicketType, String code) {
        return workTicketDao.getWorkTicketList(workTicketType, code);
    }

    public List<WorkTicketRecord> getWorkTicketRecordList(WorkTicket workTicket) {
        return workTicketRecordDao.getWorkTicketRecordList(workTicket);
    }

    public WorkTicket getWorkTicketById(String id) {
        return workTicketDao.findById(WorkTicket.class, id);
    }

    public WorkTicket getWorkTicketByProcessId(String processId) {
        return workTicketDao.getWorkTicketByProcessId(processId);
    }

    public WorkTicket getWorkTicketByTaskId(String taskId) {
        return workTicketDao.getWorkTicketByTaskId(taskId);
    }

    public void saveOrUpdateWorkTicket(WorkTicket workTicket) {
        workTicketDao.saveOrUpdate(workTicket);
    }

    public List<SecurityMeasures> getSecurityMeasureListByTypeAndId(String type,WorkTicket workTicket) {
        return securityMeasuresDao.getSecurityMeasureListByTypeAndId(type, workTicket);
    }

    public void deleteWorkTicket(String id) {
        WorkTicket workTicket = workTicketDao.findById(WorkTicket.class, id);
        workTicketDao.delete(workTicket);
    }
}
