package com.corsair.entity;

import java.io.Serializable;

/**
 * Created by guo_zhiqi on 2015/12/23.
 */
public class WorkTicketRecord implements Serializable {
    private String id;
    private String startTime; //每日开工时间
    private String endTime; //每日收工时间
    private String responsiblePerson; //工作责任人
    private String permitPerson; //工作许可人
    private String recordDate;
    private WorkTicket workTicket; //所属工作票

    public String getRecordDate() {
        return recordDate;
    }

    public void setRecordDate(String recordDate) {
        this.recordDate = recordDate;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getPermitPerson() {
        return permitPerson;
    }

    public void setPermitPerson(String permitPerson) {
        this.permitPerson = permitPerson;
    }

    public String getStartTime() {
        return startTime;
    }

    public void setStartTime(String startTime) {
        this.startTime = startTime;
    }

    public String getEndTime() {
        return endTime;
    }

    public void setEndTime(String endTime) {
        this.endTime = endTime;
    }

    public String getResponsiblePerson() {
        return responsiblePerson;
    }

    public void setResponsiblePerson(String responsiblePerson) {
        this.responsiblePerson = responsiblePerson;
    }

    public WorkTicket getWorkTicket() {
        return workTicket;
    }

    public void setWorkTicket(WorkTicket workTicket) {
        this.workTicket = workTicket;
    }
}
