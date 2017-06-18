package com.corsair.entity;

import java.io.Serializable;

/**
 * Created by Administrator on 2015/12/25.
 */
public class AttenTionMeasure implements Serializable {
    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getAttenTionMeasure() {
        return attenTionMeasure;
    }

    public void setAttenTionMeasure(String attenTionMeasure) {
        this.attenTionMeasure = attenTionMeasure;
    }

    public WorkTicket getWorkTicket() {
        return workTicket;
    }

    public void setWorkTicket(WorkTicket workTicket) {
        this.workTicket = workTicket;
    }

    private String id;
    private String attenTionMeasure;//补充工作地点保留带电部分和安全措施（由工作许可人填写）
    private WorkTicket workTicket; //所属工作票
}
