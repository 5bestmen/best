package com.corsair.entity;

import com.rbac.entity.SysAccount;

import java.util.Date;

/**
 * Created by zjq on 2015/10/30.
 */
public class OperateTicketProcessRecord  implements java.io.Serializable {
    String id;
    SysAccount operator;
    int operateType;//0´´½¨£¬1 ÐÞ¸Ä£¬2 ÉóºË
    Date operateTime;
    BaseTicket baseTicket;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public SysAccount getOperator() {
        return operator;
    }

    public void setOperator(SysAccount operator) {
        this.operator = operator;
    }

    public int getOperateType() {
        return operateType;
    }

    public void setOperateType(int operateType) {
        this.operateType = operateType;
    }

    public Date getOperateTime() {
        return operateTime;
    }

    public void setOperateTime(Date operateTime) {
        this.operateTime = operateTime;
    }

    public BaseTicket getBaseTicket() {
        return baseTicket;
    }

    public void setBaseTicket(BaseTicket baseTicket) {
        this.baseTicket = baseTicket;
    }
}
