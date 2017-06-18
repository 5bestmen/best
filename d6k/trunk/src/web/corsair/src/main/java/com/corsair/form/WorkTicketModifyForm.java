package com.corsair.form;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 2015/10/23.
 */
public class WorkTicketModifyForm {

    private String id;
    private String workTicketType;  //工作票类别
    private String stationName; //厂站名
    private String code; //编码
    private String responsiblePerson; //责任人
    private String workCondition; //工作条件
    private String signTime; //签发日期
    private String permitPersonTime;//工作许可人签名日期

    public String getChangeEndTime() {
        return changeEndTime;
    }

    public void setChangeEndTime(String changeEndTime) {
        this.changeEndTime = changeEndTime;
    }

    public String getChangeStartTime() {
        return changeStartTime;
    }

    public void setChangeStartTime(String changeStartTime) {
        this.changeStartTime = changeStartTime;
    }

    private String changeStartTime;
    private String changeEndTime;

    public String getResponseResetPerson() {
        return responseResetPerson;
    }

    public void setResponseResetPerson(String responseResetPerson) {
        this.responseResetPerson = responseResetPerson;
    }

    private String responseResetPerson;//重置工作负责人

    public String getOtherRemark() {
        return otherRemark;
    }

    public void setOtherRemark(String otherRemark) {
        this.otherRemark = otherRemark;
    }

    public String getFullTimePerson() {
        return fullTimePerson;
    }

    public void setFullTimePerson(String fullTimePerson) {
        this.fullTimePerson = fullTimePerson;
    }

    public String getFullRemark() {
        return fullRemark;
    }

    public void setFullRemark(String fullRemark) {
        this.fullRemark = fullRemark;
    }

    private String fullTimePerson;//指定专职监护人
    private String fullRemark;//负责监护
    private String otherRemark;//其他事项

    public String getWorkTicketCreateTime() {
        return workTicketCreateTime;
    }

    public void setWorkTicketCreateTime(String workTicketCreateTime) {
        this.workTicketCreateTime = workTicketCreateTime;
    }

    private String workTicketCreateTime;//工作票创建时间

    public List<String> getSecurityMeasuresType() {
        return securityMeasuresType;
    }

    public void setSecurityMeasuresType(List<String> securityMeasuresType) {
        this.securityMeasuresType = securityMeasuresType;
    }

    private List<String> securityMeasuresType;//安全措施类型

    public String getTicketType() {
        return ticketType;
    }

    public void setTicketType(String ticketType) {
        this.ticketType = ticketType;
    }

    private String ticketType;//工作票类型

    public String getFillWorkTicketTime() {
        return fillWorkTicketTime;
    }

    public void setFillWorkTicketTime(String fillWorkTicketTime) {
        this.fillWorkTicketTime = fillWorkTicketTime;
    }

    private String fillWorkTicketTime;//填写工作票日期

    public String getSelectResponsiblePerson() {
        return selectResponsiblePerson;
    }

    public void setSelectResponsiblePerson(String selectResponsiblePerson) {
        this.selectResponsiblePerson = selectResponsiblePerson;
    }

    private String selectResponsiblePerson;//是否变更负责人

    public String getSelectdelayTime() {
        return selectdelayTime;
    }

    public void setSelectdelayTime(String selectdelayTime) {
        this.selectdelayTime = selectdelayTime;
    }

    private String selectdelayTime;//是否延期

    public String getProcessWorkPerson() {
        return processWorkPerson;
    }

    public void setProcessWorkPerson(String processWorkPerson) {
        this.processWorkPerson = processWorkPerson;
    }

    private String processWorkPerson;//签发工作责任人

    public String getCrew() {
        return crew;
    }

    public void setCrew(String crew) {
        this.crew = crew;
    }

    private String crew; //工作班人员
    private String department; //部门和班组

    public void setPlannedWorkingStartTime(String plannedWorkingStartTime) {
        this.plannedWorkingStartTime = plannedWorkingStartTime;
    }

    public String getPermitPersonTime() {
        return permitPersonTime;
    }

    public void setPermitPersonTime(String permitPersonTime) {
        this.permitPersonTime = permitPersonTime;
    }

    public String getSignTime() {
        return signTime;
    }

    public void setSignTime(String signTime) {
        this.signTime = signTime;
    }

    public String getPlannedWorkingStartTime() {
        return plannedWorkingStartTime;
    }

    private String plannedWorkingStartTime; //计划工作开始时间

    public String getPlannedWorkingEndTime() {
        return plannedWorkingEndTime;
    }

    public void setPlannedWorkingEndTime(String plannedWorkingEndTime) {
        this.plannedWorkingEndTime = plannedWorkingEndTime;
    }

    private String plannedWorkingEndTime; //计划工作结束时间
    private String task;    //工作任务
    private String workPlace; //工作地点
    private String remark; //备注
    private String submit; //页面提交

    public List<String> getWorkNameAndPlace() {
        return workNameAndPlace;
    }

    public List<String> getWorkContent() {
        return workContent;
    }

    public void setWorkNameAndPlace(List<String> workNameAndPlace) {
        this.workNameAndPlace = workNameAndPlace;
    }

    public void setWorkContent(List<String> workContent) {
        this.workContent = workContent;
    }

    private List<String> workNameAndPlace = new ArrayList<>(); //工作地点及设备双重名称

    public List<String> getAttention() {
        return attention;
    }

    public void setAttention(List<String> attention) {
        this.attention = attention;
    }

    private List<String> attention= new ArrayList<>();//工作地点保留带电部分或注意事项（由工作票签发人填写）
    private List<String> workContent = new ArrayList<>(); //工作内容

    private List<String> securityMeasure= new ArrayList<>(); //安全措施

    public List<String> getSecurityMeasure() {
        return securityMeasure;
    }

    public void setSecurityMeasure(List<String> securityMeasure) {
        this.securityMeasure = securityMeasure;
    }

    private List<String> workPermit = new ArrayList<>(); //安全措施列表
    private String permitPerson; //工作许可人
    private String responsiblePermitPerson; //工作许可责任人

    public List<String> getAttentionMeasureses() {
        return attentionMeasureses;
    }

    public void setAttentionMeasureses(List<String> attentionMeasureses) {
        this.attentionMeasureses = attentionMeasureses;
    }

    private List<String> attentionMeasureses = new ArrayList<>();// 补充工作地点保留带电部分和安全措施（由工作许可人填写）

    //用于记录工作票流程相关ID
    private String processId;
    private String orderId;
    private String taskId;

    //工作票签发流程的签发和放弃选项
    private String issued;
    private String quit;

    //工作票许可流程的通过和不通过选项,以及
    private String agree;
    private String disagree;

    public String getAlter() {
        return alter;
    }

    public String getExtension() {
        return extension;
    }

    public void setAlter(String alter) {
        this.alter = alter;
    }

    public void setExtension(String extension) {
        this.extension = extension;
    }

    private String extension;//延期
    private String alter;//变更负责人

    //办理完结手续
    private String workEndTime; //工作终结时间
    private String workEndPerson; //办理验收、终结手续的负责人
    private String workTicketEndPerson; //值班负责人

    //选择流程审核人
    private String processIssuedPerson; //流程签发人
    private String processPermitPerson; //流程许可人
    private String processEndPerson; //流程终结人



    public String getProcessEndPerson() {
        return processEndPerson;
    }

    public void setProcessEndPerson(String processEndPerson) {
        this.processEndPerson = processEndPerson;
    }

    public String getProcessIssuedPerson() {
        return processIssuedPerson;
    }

    public void setProcessIssuedPerson(String processIssuedPerson) {
        this.processIssuedPerson = processIssuedPerson;
    }

    public String getProcessPermitPerson() {
        return processPermitPerson;
    }

    public void setProcessPermitPerson(String processPermitPerson) {
        this.processPermitPerson = processPermitPerson;
    }

    public String getWorkEndTime() {
        return workEndTime;
    }

    public void setWorkEndTime(String workEndTime) {
        this.workEndTime = workEndTime;
    }

    public String getWorkEndPerson() {
        return workEndPerson;
    }

    public void setWorkEndPerson(String workEndPerson) {
        this.workEndPerson = workEndPerson;
    }

    public String getWorkTicketEndPerson() {
        return workTicketEndPerson;
    }

    public void setWorkTicketEndPerson(String workTicketEndPerson) {
        this.workTicketEndPerson = workTicketEndPerson;
    }

    public String getDisagree() {
        return disagree;
    }

    public void setDisagree(String disagree) {
        this.disagree = disagree;
    }

    public String getAgree() {
        return agree;
    }

    public void setAgree(String agree) {
        this.agree = agree;
    }

    public String getQuit() {
        return quit;
    }

    public void setQuit(String quit) {
        this.quit = quit;
    }

    public String getIssued() {
        return issued;
    }

    public void setIssued(String issued) {
        this.issued = issued;
    }

    public String getProcessId() {
        return processId;
    }

    public void setProcessId(String processId) {
        this.processId = processId;
    }

    public String getOrderId() {
        return orderId;
    }

    public void setOrderId(String orderId) {
        this.orderId = orderId;
    }

    public String getTaskId() {
        return taskId;
    }

    public void setTaskId(String taskId) {
        this.taskId = taskId;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getSubmit() {
        return submit;
    }


    public void setSubmit(String submit) {
        this.submit = submit;
    }


    public String getWorkTicketType() {
        return workTicketType;
    }

    public void setWorkTicketType(String workTicketType) {
        this.workTicketType = workTicketType;
    }

    public String getStationName() {
        return stationName;
    }

    public void setStationName(String stationName) {
        this.stationName = stationName;
    }

    public String getCode() {
        return code;
    }

    public void setCode(String code) {
        this.code = code;
    }

    public String getResponsiblePerson() {
        return responsiblePerson;
    }

    public void setResponsiblePerson(String responsiblePerson) {
        this.responsiblePerson = responsiblePerson;
    }

    public String getDepartment() {
        return department;
    }

    public void setDepartment(String department) {
        this.department = department;
    }

    public String getTask() {
        return task;
    }

    public void setTask(String task) {
        this.task = task;
    }

    public String getWorkPlace() {
        return workPlace;
    }

    public void setWorkPlace(String workPlace) {
        this.workPlace = workPlace;
    }

    public String getRemark() {
        return remark;
    }

    public void setRemark(String remark) {
        this.remark = remark;
    }

    public List<String> getWorkPermit() {
        return workPermit;
    }

    public void setWorkPermit(List<String> workPermit) {
        this.workPermit = workPermit;
    }


    public String getPermitPerson() {
        return permitPerson;
    }

    public void setPermitPerson(String permitPerson) {
        this.permitPerson = permitPerson;
    }

    public String getResponsiblePermitPerson() {
        return responsiblePermitPerson;
    }

    public void setResponsiblePermitPerson(String responsiblePermitPerson) {
        this.responsiblePermitPerson = responsiblePermitPerson;
    }

    public String getWorkCondition() {
        return workCondition;
    }

    public void setWorkCondition(String workCondition) {
        this.workCondition = workCondition;
    }
}
