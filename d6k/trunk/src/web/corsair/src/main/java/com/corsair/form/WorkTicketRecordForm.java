package com.corsair.form;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 2015/12/23.
 */
public class WorkTicketRecordForm{

    private String id;
    private String startTime; //每日开工时间
    private String endTime; //每日收工时间
    private String responsiblePerson; //工作责任人
    private String permitPerson; //工作许可人
    private String submit; //页面提交
    private List<String> records = new ArrayList<>();

    public List<String> getRecords() {
        return records;
    }

    public void setRecords(List<String> records) {
        this.records = records;
    }

    public String getSubmit() {
        return submit;
    }

    public void setSubmit(String submit) {
        this.submit = submit;
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
}
