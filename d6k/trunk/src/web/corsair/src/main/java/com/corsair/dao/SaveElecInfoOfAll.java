package com.corsair.dao;

/**
 * Created by zyj on 2017/3/1.
 */
// 所有厂站的日节电量信息
public class SaveElecInfoOfAll {
    public int getnStationId() {
        return nStationId;
    }

    public void setnStationId(int nStationId) {
        this.nStationId = nStationId;
    }

    public String getStrStationDescribe() {
        return strStationDescribe;
    }

    public void setStrStationDescribe(String strStationDescribe) {
        this.strStationDescribe = strStationDescribe;
    }

    public int getnDeviceId() {
        return nDeviceId;
    }

    public void setnDeviceId(int nDeviceId) {
        this.nDeviceId = nDeviceId;
    }

    public String getStrDeviceDescribe() {
        return strDeviceDescribe;
    }

    public void setStrDeviceDescribe(String strDeviceDescribe) {
        this.strDeviceDescribe = strDeviceDescribe;
    }

    public int getnParaId() {
        return nParaId;
    }

    public void setnParaId(int nParaId) {
        this.nParaId = nParaId;
    }

    public String getStrParaDescribe() {
        return strParaDescribe;
    }

    public void setStrParaDescribe(String strParaDescribe) {
        this.strParaDescribe = strParaDescribe;
    }

    private int nStationId;
    private String strStationDescribe;
    private int nDeviceId;
    private String strDeviceDescribe;
    private int nParaId;
    private String strParaDescribe;
}
