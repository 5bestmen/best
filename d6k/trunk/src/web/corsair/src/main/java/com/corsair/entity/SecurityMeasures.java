package com.corsair.entity;

import java.io.Serializable;

/**
 * Created by 洪祥 on 2015/10/13.
 */
public class SecurityMeasures implements Serializable {

    private String id;
    private String securityMeasuresType;//安全措施类型
    private String measure; //措施
    private String switchesAndKnife; //开关和刀闸
    private String groundLineName;//应装接地线，应装接地刀闸名称
    private String groundLineNumber;//接地线编号
    private String otherMeasures; //措施
    // private boolean finishedSwitchesAndKnife; //已经执行
    // private boolean finishedGroundLine; //已经执行
    // private boolean finishedOtherMeasures; //已经执行
    private Boolean finished; //已经执行
    private Integer count; //已经执行
    // private int countUnfinishedGroundLine;//记录未完成的接地线编号
    //private int countUnfinishedSwitchesAndKnife;//记录未完成的接地刀闸（小车）
    private WorkTicket workTicket; //所属工作票

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getMeasure() {
        return measure;
    }

    public void setMeasure(String measure) {
        this.measure = measure;
    }

    public WorkTicket getWorkTicket() {
        return workTicket;
    }

    public void setWorkTicket(WorkTicket workTicket) {
        this.workTicket = workTicket;
    }

    public String getSwitchesAndKnife() {
        return switchesAndKnife;
    }

    public void setSwitchesAndKnife(String switchesAndKnife) {
        this.switchesAndKnife = switchesAndKnife;
    }

    public String getGroundLineNumber() {
        return groundLineNumber;
    }

    public void setGroundLineNumber(String groundLineNumber) {
        this.groundLineNumber = groundLineNumber;
    }

    public String getOtherMeasures() {
        return otherMeasures;
    }

    public void setOtherMeasures(String otherMeasures) {
        this.otherMeasures = otherMeasures;
    }

    public String getGroundLineName() {
        return groundLineName;
    }

    public void setGroundLineName(String groundLineName) {
        this.groundLineName = groundLineName;
    }

    public Boolean getFinished() {
        return finished;
    }

    public void setFinished(Boolean finished) {
        this.finished = finished;
    }

    public Integer getCount() {
        return count;
    }

    public void setCount(Integer count) {
        this.count = count;
    }

    public String getSecurityMeasuresType() {
        return securityMeasuresType;
    }

    public void setSecurityMeasuresType(String securityMeasuresType) {
        this.securityMeasuresType = securityMeasuresType;
    }
}
