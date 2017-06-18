package com.corsair.entity;

import java.io.Serializable;

/**
 * Created by Administrator on 2015/12/24.
 */
public class WorkTicketDelay implements Serializable {
    private String id ;
    private String name;

    public String getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getStartTime() {
        return startTime;
    }

    public void setId(String id) {
        this.id = id;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setStartTime(String startTime) {
        this.startTime = startTime;
    }

    public void setEndTime(String endTime) {
        this.endTime = endTime;
    }

    public void setWorkTicket(WorkTicket workTicket) {
        this.workTicket = workTicket;
    }

    public String getEndTime() {
        return endTime;

    }

    public WorkTicket getWorkTicket() {
        return workTicket;
    }

    private String startTime;
    private String endTime;

    public String getCreateTime() {
        return createTime;
    }

    public void setCreateTime(String createTime) {
        this.createTime = createTime;
    }

    private String createTime;//操作延期的时间
    private WorkTicket workTicket;
}
