package com.corsair.entity;

import java.io.Serializable;
import java.util.List;

/**
 * Created by 洪祥 on 2015/10/13.
 */
public class WorkPermit implements Serializable {

    private String id;
    private List<String> permits; //许可项
    private String permitPerson; //工作许可人
    private String responsiblePerson; //工作责任人
    private WorkTicket workTicket; //所属工作票

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public List<String> getPermits() {
        return permits;
    }

    public void setPermits(List<String> permits) {
        this.permits = permits;
    }

    public String getPermitPerson() {
        return permitPerson;
    }

    public void setPermitPerson(String permitPerson) {
        this.permitPerson = permitPerson;
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
