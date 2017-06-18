package com.corsair.entity;

/**
 * Created by 洪祥 on 15/7/27.
 */
public class OperateTicket extends BaseTicket implements java.io.Serializable {

    public OperateTicket(){

    }
    public OperateTicket(BaseTicket baseTicket){
        super(baseTicket);
    }
    String id;
    String seriesNo;
    String name;
    int state;
    String sendName;
    String recvName;
    String sendTime;
    String beginTime;
    String endTime;
    String guardianName;
    String singleOperator;
    String repairOperator;
    String comments;


    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getSeriesNo() {
        return seriesNo;
    }

    public void setSeriesNo(String seriesNo) {
        this.seriesNo = seriesNo;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getComments() {
        return comments;
    }

    public void setComments(String comments) {
        this.comments = comments;
    }

    public int getState() {
        return state;
    }

    public void setState(int state) {
        this.state = state;
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

}
