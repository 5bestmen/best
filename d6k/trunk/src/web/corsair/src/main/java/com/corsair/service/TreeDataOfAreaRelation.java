package com.corsair.service;

/**
 * Created by zyj on 2017/2/7.
 */
public class TreeDataOfAreaRelation {
    public String getStrLevel() {
        return strLevel;
    }

    public void setStrLevel(String strLevel) {
        this.strLevel = strLevel;
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

    private String strLevel;
    private String strStationID;
    private String strDescribe;

    public int getnIsCalc() {
        return nIsCalc;
    }

    public void setnIsCalc(int nIsCalc) {
        this.nIsCalc = nIsCalc;
    }

    public String getStrStationAndDeviceRelationID() {
        return strStationAndDeviceRelationID;
    }

    public void setStrStationAndDeviceRelationID(String strStationAndDeviceRelationID) {
        this.strStationAndDeviceRelationID = strStationAndDeviceRelationID;
    }

    private int nIsCalc;
    private String strStationAndDeviceRelationID;
}
