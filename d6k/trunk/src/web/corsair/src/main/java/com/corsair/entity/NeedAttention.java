package com.corsair.entity;

import java.io.Serializable;

/**
 * Created by Administrator on 2015/12/25.
 */
public class NeedAttention implements Serializable {
    private String id;
    private String needAttention;//工作地点保留带电部分或注意事项（由工作票签发人填写）

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getNeedAttention() {
        return needAttention;
    }

    public void setNeedAttention(String needAttention) {
        this.needAttention = needAttention;
    }

    public WorkTicket getWorkTicket() {
        return workTicket;
    }

    public void setWorkTicket(WorkTicket workTicket) {
        this.workTicket = workTicket;
    }

    private WorkTicket workTicket; //所属工作票
}
