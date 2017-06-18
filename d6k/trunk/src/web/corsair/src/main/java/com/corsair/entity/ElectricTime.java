package com.corsair.entity;

/**
 * Created by libh on 2015/12/18.
 */
public class ElectricTime implements java.io.Serializable{
    private String id;
    private String startTime;
    private String endTime;
    private ElectricPrice electricPrice;

    public String getId() {
        return id;
    }

    public void setId(String id) { this.id = id;}

    public String getStartTime() { return startTime;}

    public void setStartTime(String startTime) { this.startTime = startTime;}

    public String getEndTime() { return endTime; }

    public void setEndTime(String endTime) { this.endTime = endTime;}

    public ElectricPrice getElectricPrice() {
        return electricPrice;
    }

    public void setElectricPrice(ElectricPrice electricPrice) {
        this.electricPrice = electricPrice;
    }

}
