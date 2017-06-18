package com.corsair.service;

import com.corsair.dao.OperateTicketDao;
import com.corsair.dao.OperateTicketTemplateDao;
import com.corsair.dao.OperateTicketProcessRecordDao;
import com.corsair.entity.OperateTicket;
import com.corsair.entity.OperateTicketProcessRecord;
import com.corsair.entity.OperateTicketTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * Created by 洪祥 on 15/7/13.
 */
@Service("operateTicketService")
public class OperateTicketService {
    @Autowired
    OperateTicketDao operateTicketDao;
    @Autowired
    OperateTicketTemplateDao operateTicketTemplateDao;
    @Autowired
    OperateTicketProcessRecordDao operateTicketProcessRecordDao;


    public void saveOrUpdateTicketTemplate(OperateTicketTemplate operateTicketTemplate){
        operateTicketTemplateDao.saveOrUpdate(operateTicketTemplate);
    }

    public void deleteOperateTicketTemplate(String operateTicketTemplateId){
        OperateTicketTemplate operateTicketTemplate = operateTicketTemplateDao.findById(OperateTicketTemplate.class, operateTicketTemplateId);
        operateTicketTemplateDao.delete(operateTicketTemplate);
    }

    public void deleteOperateTicket(String operateTicketId){
        OperateTicket operateTicket = operateTicketDao.findById(OperateTicket.class, operateTicketId);
        operateTicketDao.delete(operateTicket);
    }

    public void saveOrUpdateTicket(OperateTicket operateTicket){
        operateTicketDao.saveOrUpdate(operateTicket);
    }

    public void saveOrUpdateTicketProcessRecord(OperateTicketProcessRecord operateTicketProcessRecord){
        operateTicketProcessRecordDao.saveOrUpdate(operateTicketProcessRecord);
    }

    public List<OperateTicketTemplate> getCommonTicketTemplateList(){
        return operateTicketTemplateDao.getOperateTicketTemplateList();
    }

    public List<OperateTicket> getOperateTicketList(){
        return operateTicketDao.getOperateTicketList();
    }

    public OperateTicketTemplate getOperateTicketTemplateById(String id){
        return operateTicketTemplateDao.findById(OperateTicketTemplate.class, id);
    }

    public OperateTicket getOperateTicketById(String id){
        return operateTicketDao.findById(OperateTicket.class, id);
    }

}
