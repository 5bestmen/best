package com.corsair.form;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by zjq on 2015/10/20.
 */
public class OperateTicketTemplateEditForm{
    private String submit;
    private String id;
    private String operateTicketTemplateName;
    private String operateTask;
    private String comment ;
    private List<String> operateColumn = new ArrayList<>();

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

    public String getOperateTicketTemplateName() {
        return operateTicketTemplateName;
    }

    public void setOperateTicketTemplateName(String operateTicketTemplateName) {
        this.operateTicketTemplateName = operateTicketTemplateName;
    }

    public String getOperateTask() {
        return operateTask;
    }

    public void setOperateTask(String operateTask) {
        this.operateTask = operateTask;
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }

    public List<String> getOperateColumn() {
        return operateColumn;
    }

    public void setOperateColumn(List<String> operateColumn) {
        this.operateColumn = operateColumn;
    }
}
