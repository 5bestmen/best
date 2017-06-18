package com.corsair.entity;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * Created by 洪祥 on 15/7/27.
 */
public class BaseTicket implements java.io.Serializable,Cloneable{

    public BaseTicket() {

    }

    public BaseTicket(BaseTicket baseTicket){
        try{
            BaseTicket basicTicket = (BaseTicket)baseTicket.clone();
            this.setId(basicTicket.getId());
            this.setOperateTask(basicTicket.getOperateTask());
            this.setOperateTicketTemplateData(basicTicket.getOperateTicketTemplateData());
        }catch (CloneNotSupportedException e){
            e.printStackTrace();
        }
    }
    String id;
    String operateTask;

    public String getOperateTask() {
        return operateTask;
    }

    public void setOperateTask(String operateTask) {
        this.operateTask = operateTask;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    private List<OperateTicketTemplateData> operateTicketTemplateData = new ArrayList<>();
    private Set<OperateTicketProcessRecord> operateTicketProcessRecords = new HashSet<>();

    public Set<OperateTicketProcessRecord> getOperateTicketProcessRecords() {
        return operateTicketProcessRecords;
    }

    public void setOperateTicketProcessRecords(Set<OperateTicketProcessRecord> operateTicketProcessRecords) {
        this.operateTicketProcessRecords = operateTicketProcessRecords;
    }

    public List<OperateTicketTemplateData> getOperateTicketTemplateData() {
        return operateTicketTemplateData;
    }

    public void setOperateTicketTemplateData(List<OperateTicketTemplateData> operateTicketTemplateData) {
        this.operateTicketTemplateData = operateTicketTemplateData;
    }
}
