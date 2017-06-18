package com.corsair.entity;

/**
 * Created by libh on 2015/12/17.
 */
public class NetElectricity implements java.io.Serializable{
    private String id;
    private String netDate;
    private Double totalKwh;
    private Double peakKwh;
    private Double normalKwh;
    private Double valleyKwh;
    private Double reverseTotalKwh;
    private Double totalKVar;
    private int multiple;
    private String station;

    public String getId() {
        return id;
    }

    public void setId(String id) { this.id = id;}

    public String getNetDate() {
        return netDate;
    }

    public void setNetDate(String netDate) {
        this.netDate = netDate;
    }

    public Double getTotalKwh(){ return  totalKwh;}

    public void setTotalKwh(Double totalKwh){this.totalKwh = totalKwh;}

    public Double getPeakKwh(){ return  peakKwh;}

    public void setPeakKwh(Double peakKwh){this.peakKwh = peakKwh;}

    public Double getNormalKwh(){ return  normalKwh;}

    public void setNormalKwh(Double normalKwh){this.normalKwh = normalKwh;}

    public Double getValleyKwh(){ return  valleyKwh;}

    public void setValleyKwh(Double valleyKwh){this.valleyKwh = valleyKwh;}

    public Double getReverseTotalKwh(){ return  reverseTotalKwh;}

    public void setReverseTotalKwh(Double reverseTotalKwh){this.reverseTotalKwh = reverseTotalKwh;}

    public Double getTotalKVar(){ return  totalKVar;}

    public void setTotalKVar(Double totalKVar){this.totalKVar = totalKVar;}

    public int getMultiple(){ return  multiple;}

    public void setMultiple(int multiple){this.multiple = multiple;}

    public String getStation() {
        return station;
    }

    public void setStation(String station) { this.station = station;}
}
