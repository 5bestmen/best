package com.corsair.entity;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 2015/10/12.
 */
public class WorkTicket implements Serializable {

    private String id;

    public String getNewResponsiblePerson() {
        return newResponsiblePerson;
    }

    public void setNewResponsiblePerson(String newResponsiblePerson) {
        this.newResponsiblePerson = newResponsiblePerson;
    }

    private String newResponsiblePerson;//更改的负责人

    public Boolean getIsDelay() {
        return isDelay;
    }

    public void setIsDelay(Boolean isDelay) {
        this.isDelay = isDelay;
    }

    public Boolean getIsChangePerson() {
        return isChangePerson;
    }

    public void setIsChangePerson(Boolean isChangePerson) {
        this.isChangePerson = isChangePerson;
    }

    private Boolean isDelay;//是否延期
    private Boolean isChangePerson;//是否更换负责人
    private Integer workTicketType; //工作票类型
    //private String stationName; //厂站名
    private String code; //编号
    private String responsiblePerson; //责任人
    private String department; //部门和班组
    private String plannedWorkingStartingTime; //计划工作开始时间
    private String plannedWorkingEndingTime; //计划工作开始时间
    private List<WorkTicketRecord> workTicketRecords = new ArrayList<>();//每日工作记录
    private List<AttenTionMeasure> attenTionMeasure = new ArrayList<>();//安全措施
    private List<NeedAttention> needAttentions = new ArrayList<>();//注意事项
    private List<WorkContent> workContents = new ArrayList<>(); //工作任务
    private List<WorkPermit> workPermits = new ArrayList<>(); //工作许可
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

    public String getOtherRemark() {
        return otherRemark;
    }

    public void setOtherRemark(String otherRemark) {
        this.otherRemark = otherRemark;
    }

    private String fullTimePerson;//指定专职监护人
    private String fullRemark;//负责监护
    private String otherRemark;//其他事项

    private List<String> crew; //工作班人员
    private String task;  //工作任务
    //private String workPlace; //工作地点
    private List<SecurityMeasures> securityMeasureses = new ArrayList<>(); //安全措施
    private String summary; //工作终结
    private String summaryTime; //工作时间
    private String workTicketEnd;   //工作票终结
    private String remark; //备注
    private Integer state; //审核状态 1：审核中、3：审核通过、4：审核不通过
    private String workCondition;//工作条件
    private String workTicketSigner;//工作票签发人
    private String signTime;//签发日期
    private String extensionTime;//延期日期
    private String addSecurityMeasures;//补充安全措施
    private String PermittingWorkTime; //许可工作时间
    private String responsiblePersonTime; //工作责任人日期
    private String permitPersonTime; //工作许可人签名日期
    private String otherMatters;//其他事项
    private String receivedWorkTicketTime;//收到工作票时间
    private String RunningDuty;//运行值班人员签名
    private String WorkPermitPerson;//工作许可人
    //跟流程审核相关
    private String processId;   //流程ID
    private String taskId;  //任务ID

    public String getWorkTicketCreateTime() {
        return workTicketCreateTime;
    }

    public void setWorkTicketCreateTime(String workTicketCreateTime) {
        this.workTicketCreateTime = workTicketCreateTime;
    }

    private String workTicketCreateTime;//工作票创建时间

    public String getWorkPermitPerson() {
        return WorkPermitPerson;
    }

    public void setWorkPermitPerson(String workPermitPerson) {
        WorkPermitPerson = workPermitPerson;
    }

    public List<AttenTionMeasure> getAttenTionMeasure() {
        return attenTionMeasure;
    }

    public void setAttenTionMeasure(List<AttenTionMeasure> attenTionMeasure) {
        this.attenTionMeasure = attenTionMeasure;
    }

    public List<NeedAttention> getNeedAttentions() {
        return needAttentions;
    }

    public void setNeedAttentions(List<NeedAttention> needAttentions) {
        this.needAttentions = needAttentions;
    }

    public List<WorkTicketRecord> getWorkTicketRecords() {
        return workTicketRecords;
    }

    public void setWorkTicketRecords(List<WorkTicketRecord> workTicketRecords) {
        this.workTicketRecords = workTicketRecords;
    }

    public List<WorkTicketDelay> getWorkTicketDelays() {
        return workTicketDelays;
    }

    public void setWorkTicketDelays(List<WorkTicketDelay> workTicketDelays) {
        this.workTicketDelays = workTicketDelays;
    }

    private List<WorkTicketDelay> workTicketDelays = new ArrayList<>();//工作票延期

    public String getFillWorkTicketTime() {
        return fillWorkTicketTime;
    }

    public List<PersonnelChangeCondition> getPersonnelChanges() {
        return personnelChanges;
    }

    public void setPersonnelChanges(List<PersonnelChangeCondition> personnelChanges) {
        this.personnelChanges = personnelChanges;
    }

    private List<PersonnelChangeCondition> personnelChanges = new ArrayList<>();

    public void setFillWorkTicketTime(String fillWorkTicketTime) {
        this.fillWorkTicketTime = fillWorkTicketTime;
    }

    private String fillWorkTicketTime;//填写工作票日期

    public List<String> getCrew() {
        return crew;
    }

    public void setCrew(List<String> crew) {
        this.crew = crew;
    }


    public List<WorkContent> getWorkContents() {
        return workContents;
    }

    public void setWorkContents(List<WorkContent> workContents) {
        this.workContents = workContents;
    }


    public String getPlannedWorkingStartingTime() {
        return plannedWorkingStartingTime;
    }

    public void setPlannedWorkingStartingTime(String plannedWorkingStartingTime) {
        this.plannedWorkingStartingTime = plannedWorkingStartingTime;
    }

    public String getPlannedWorkingEndingTime() {
        return plannedWorkingEndingTime;
    }

    public void setPlannedWorkingEndingTime(String plannedWorkingEndingTime) {
        this.plannedWorkingEndingTime = plannedWorkingEndingTime;
    }

    public String getSummaryTime() {
        return summaryTime;
    }

    public void setSummaryTime(String summaryTime) {
        this.summaryTime = summaryTime;
    }

    public String getWorkCondition() {
        return workCondition;
    }

    public void setWorkCondition(String workCondition) {
        this.workCondition = workCondition;
    }

    public String getWorkTicketSigner() {
        return workTicketSigner;
    }

    public void setWorkTicketSigner(String workTicketSigner) {
        this.workTicketSigner = workTicketSigner;
    }

    public String getSignTime() {
        return signTime;
    }

    public void setSignTime(String signTime) {
        this.signTime = signTime;
    }

    public String getExtensionTime() {
        return extensionTime;
    }

    public void setExtensionTime(String extensionTime) {
        this.extensionTime = extensionTime;
    }

    public String getAddSecurityMeasures() {
        return addSecurityMeasures;
    }

    public void setAddSecurityMeasures(String addSecurityMeasures) {
        this.addSecurityMeasures = addSecurityMeasures;
    }

    public String getPermittingWorkTime() {
        return PermittingWorkTime;
    }

    public void setPermittingWorkTime(String permittingWorkTime) {
        PermittingWorkTime = permittingWorkTime;
    }

    public String getResponsiblePersonTime() {
        return responsiblePersonTime;
    }

    public void setResponsiblePersonTime(String responsiblePersonTime) {
        this.responsiblePersonTime = responsiblePersonTime;
    }

    public String getPermitPersonTime() {
        return permitPersonTime;
    }

    public void setPermitPersonTime(String permitPersonTime) {
        this.permitPersonTime = permitPersonTime;
    }

    public String getOtherMatters() {
        return otherMatters;
    }

    public void setOtherMatters(String otherMatters) {
        this.otherMatters = otherMatters;
    }

    public String getReceivedWorkTicketTime() {
        return receivedWorkTicketTime;
    }

    public void setReceivedWorkTicketTime(String receivedWorkTicketTime) {
        this.receivedWorkTicketTime = receivedWorkTicketTime;
    }

    public String getRunningDuty() {
        return RunningDuty;
    }

    public void setRunningDuty(String runningDuty) {
        RunningDuty = runningDuty;
    }


    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getProcessId() {
        return processId;
    }

    public void setProcessId(String processId) {
        this.processId = processId;
    }

    public String getTaskId() {
        return taskId;
    }

    public void setTaskId(String taskId) {
        this.taskId = taskId;
    }

//    public String getStationName() {
//        return stationName;
//    }
//
//    public void setStationName(String stationName) {
//        this.stationName = stationName;
//    }

    public Integer getWorkTicketType() {
        return workTicketType;
    }

    public void setWorkTicketType(Integer workTicketType) {
        this.workTicketType = workTicketType;
    }

    public String getCode() {
        return code;
    }

    public String getResponsiblePerson() {
        return responsiblePerson;
    }

    public void setResponsiblePerson(String responsiblePerson) {
        this.responsiblePerson = responsiblePerson;
    }

    public void setCode(String code) {
        this.code = code;

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

    public List<SecurityMeasures> getSecurityMeasureses() {
        return securityMeasureses;
    }

    public void setSecurityMeasureses(List<SecurityMeasures> securityMeasureses) {
        this.securityMeasureses = securityMeasureses;
    }

    public List<WorkPermit> getWorkPermits() {
        return workPermits;
    }

    public void setWorkPermits(List<WorkPermit> workPermits) {
        this.workPermits = workPermits;
    }

    public String getSummary() {
        return summary;
    }

    public void setSummary(String summary) {
        this.summary = summary;
    }

    public String getWorkTicketEnd() {
        return workTicketEnd;
    }

    public void setWorkTicketEnd(String workTicketEnd) {
        this.workTicketEnd = workTicketEnd;
    }

    public String getRemark() {
        return remark;
    }

    public void setRemark(String remark) {
        this.remark = remark;
    }

    public Integer getState() {
        return state;
    }

    public void setState(Integer state) {
        this.state = state;
    }
}
