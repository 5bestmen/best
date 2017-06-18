package com.corsair.chart;

public class HistoryData {
    private String psrID;
    private String stationName;//厂站名称
    private String deviceName;//设备名称
    private String dateFrom; //开始日期
    private String dateTo; //结束日期
    private String name;
    private String unit;

    public HistoryData() {
    }

    public String getPsrID() {
        return psrID;
    }

    public void setPsrID(String psrID) {
        this.psrID = psrID;
    }

    public String getDateFrom() {
        return dateFrom;
    }

    public void setDateFrom(String dateFrom) {
        this.dateFrom = dateFrom;
    }

    public String getDateTo() {
        return dateTo;
    }

    public void setDateTo(String dateTo) {
        this.dateTo = dateTo;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getUnit() {
        return unit;
    }

    public void setUnit(String unit) {
        this.unit = unit;
    }

    public String getStationName() {
        return stationName;
    }

    public void setStationName(String stationName) {
        this.stationName = stationName;
    }

    public String getDeviceName() {
        return deviceName;
    }

    public void setDeviceName(String deviceName) {
        this.deviceName = deviceName;
    }
}
