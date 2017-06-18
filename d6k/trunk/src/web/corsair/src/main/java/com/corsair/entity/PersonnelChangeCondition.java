package com.corsair.entity;

import java.io.Serializable;

/**
 * Created by guo_zhiqi on 2015/12/22.
 */
public class PersonnelChangeCondition implements Serializable{

    private String id ;
    private String name;//变更的工作负责人

    public String getWorkTicketIsurePerson() {
        return workTicketIsurePerson;
    }

    public void setWorkTicketIsurePerson(String workTicketIsurePerson) {
        this.workTicketIsurePerson = workTicketIsurePerson;
    }

    private String workTicketIsurePerson;//工作签发人
    private String startTime;
    private String endTime;

    public String getCreateTime() {
        return createTime;
    }

    public void setCreateTime(String createTime) {
        this.createTime = createTime;
    }

    private String createTime;//操作延期的时间
    public WorkTicket getWorkTicket() {
        return workTicket;
    }

    public void setWorkTicket(WorkTicket workTicket) {
        this.workTicket = workTicket;
    }

    private WorkTicket workTicket;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
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
}
