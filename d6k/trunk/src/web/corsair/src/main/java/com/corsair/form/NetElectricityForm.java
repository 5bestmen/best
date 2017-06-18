package com.corsair.form;

/**
 * Created by libh on 2015/12/17.
 */
public class NetElectricityForm{
    private String submit;
    private String netDate;
    private String totalKwh;
    private String peakKwh;
    private String normalKwh;
    private String valleyKwh;
    private String reverseTotalKwh;
    private String totalKVar;
    private String multiple;
    private Boolean isExistDate;
    private String stationId;

    public String getSubmit() {
        return submit;
    }

    public void setSubmit(String submit) {
        this.submit = submit;
    }

    public String getNetDate() {
        return netDate;
    }

    public void setNetDate(String netDate) { this.netDate = netDate; }

    public String getTotalKwh(){ return  totalKwh;}

    public void setTotalKwh(String totalKwh){this.totalKwh = totalKwh;}

    public String getPeakKwh(){ return  peakKwh;}

    public void setPeakKwh(String peakKwh){this.peakKwh = peakKwh;}

    public String getNormalKwh(){ return  normalKwh;}

    public void setNormalKwh(String normalKwh){this.normalKwh = normalKwh;}

    public String getValleyKwh(){ return  valleyKwh;}

    public String getReverseTotalKwh(){ return  reverseTotalKwh;}

    public void setReverseTotalKwh(String reverseTotalKwh){this.reverseTotalKwh = reverseTotalKwh;}

    public void setValleyKwh(String valleyKwh){this.valleyKwh = valleyKwh;}

    public String getTotalKVar(){ return  totalKVar;}

    public void setTotalKVar(String totalKVar){this.totalKVar = totalKVar;}

    public String getMultiple(){ return  multiple;}

    public void setMultiple(String multiple){this.multiple = multiple;}

    public Boolean getIsExistDate(){ return isExistDate;}

    public void setIsExistDate(Boolean isExistDate){this.isExistDate = isExistDate;}

    public String getStationId() {
        return stationId;
    }

    public void setStationId(String stationId) {
        this.stationId = stationId;
    }
 }
