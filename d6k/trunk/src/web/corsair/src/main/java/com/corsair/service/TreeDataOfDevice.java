package com.corsair.service.station;

/**
 * Created by zyj on 2017/2/7.
 */
public class TreeDataOfDevice {
    public String getStrDeviceID() {
        return strDeviceID;
    }

    public void setStrDeviceID(String strDeviceID) {
        this.strDeviceID = strDeviceID;
    }

    public String getStrStationID() {
        return strStationID;
    }

    public void setStrStationID(String strStationID) {
        this.strStationID = strStationID;
    }

    public String getStrDescribe() {
        return strDescribe;
    }

    public void setStrDescribe(String strDescribe) {
        this.strDescribe = strDescribe;
    }

    private String strDeviceID;
    private String strStationID;
    private String strDescribe;
}
