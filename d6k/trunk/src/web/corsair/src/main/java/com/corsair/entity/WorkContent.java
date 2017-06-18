package com.corsair.entity;

import java.io.Serializable;

/**
 * Created by Administrator on 2015/12/22.
 */
public class WorkContent implements Serializable {
    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public void setTaskName(String taskName) {
        this.taskName = taskName;
    }

    public void setTaskContent(String taskContent) {
        this.taskContent = taskContent;
    }

    public void setWorkTicket(WorkTicket workTicket) {
        this.workTicket = workTicket;
    }

    public String getTaskName() {
        return taskName;
    }

    public String getTaskContent() {
        return taskContent;
    }

    public WorkTicket getWorkTicket() {
        return workTicket;
    }

    private String id;
    private String taskName;//工作地点及设备双重名称
    private String taskContent; //工作内容
    private WorkTicket workTicket; //所属工作票
}
