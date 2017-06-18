package com.corsair.form;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by zjq on 2015/10/22.
 */
public class OperateTicketEditForm{

    private String isCheckAuthority;
    private String checkFailed;
    private String checkSuccess;

    private String submit;
    private String id;
    private String operateTicketName;
    private String operateTicketTemplateId;
    private String sendName;
    private String recvName;
    private String sendTime;
    private String beginTime;
    private String endTime;
    private String guardianName;
    private String singleOperator;
    private String repairOperator;
    private String comment ;
    private String operateTask;
    private List<String> operateColumn = new ArrayList<>();

    public String getCheckFailed() {
        return checkFailed;
    }

    public void setCheckFailed(String checkFailed) {
        this.checkFailed = checkFailed;
    }

    public String getCheckSuccess() {
        return checkSuccess;
    }

    public void setCheckSuccess(String checkSuccess) {
        this.checkSuccess = checkSuccess;
    }

    public String getIsCheckAuthority() {
        return isCheckAuthority;
    }

    public void setIsCheckAuthority(String isCheckAuthority) {
        this.isCheckAuthority = isCheckAuthority;
    }

    public String getOperateTicketTemplateId() {
        return operateTicketTemplateId;
    }

    public void setOperateTicketTemplateId(String operateTicketTemplateId) {
        this.operateTicketTemplateId = operateTicketTemplateId;
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

    public String getOperateTicketName() {
        return operateTicketName;
    }

    public void setOperateTicketName(String operateTicketName) {
        this.operateTicketName = operateTicketName;
    }

    public String getSendName() {
        return sendName;
    }

    public void setSendName(String sendName) {
        this.sendName = sendName;
    }

    public String getRecvName() {
        return recvName;
    }

    public void setRecvName(String recvName) {
        this.recvName = recvName;
    }

    public String getSendTime() {
        return sendTime;
    }

    public void setSendTime(String sendTime) {
        this.sendTime = sendTime;
    }

    public String getBeginTime() {
        return beginTime;
    }

    public void setBeginTime(String beginTime) {
        this.beginTime = beginTime;
    }

    public String getEndTime() {
        return endTime;
    }

    public void setEndTime(String endTime) {
        this.endTime = endTime;
    }

    public String getGuardianName() {
        return guardianName;
    }

    public void setGuardianName(String guardianName) {
        this.guardianName = guardianName;
    }

    public String getSingleOperator() {
        return singleOperator;
    }

    public void setSingleOperator(String singleOperator) {
        this.singleOperator = singleOperator;
    }

    public String getRepairOperator() {
        return repairOperator;
    }

    public void setRepairOperator(String repairOperator) {
        this.repairOperator = repairOperator;
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }

    public String getOperateTask() {
        return operateTask;
    }

    public void setOperateTask(String operateTask) {
        this.operateTask = operateTask;
    }

    public List<String> getOperateColumn() {
        return operateColumn;
    }

    public void setOperateColumn(List<String> operateColumn) {
        this.operateColumn = operateColumn;
    }
}
