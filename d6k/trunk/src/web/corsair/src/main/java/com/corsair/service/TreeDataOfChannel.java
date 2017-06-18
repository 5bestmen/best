package com.corsair.service;

/**
 * Created by zyj on 2017/2/6.
 */
public class TreeDataOfChannel {
    public String getStrChannelID() {
        return strChannelID;
    }

    public void setStrChannelID(String strChannelID) {
        this.strChannelID = strChannelID;
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

    private String strChannelID;
    private String strStationID;
    private String strDescribe;
}
