package com.corsair.device;

/**
 * Created by zyj on 2017/2/22.
 */
public class DeviceRunningInfo {

    public int getnDeviceNumOfTotal() {
        return nDeviceNumOfTotal;
    }

    public void setnDeviceNumOfTotal(int nDeviceNumOfTotal) {
        this.nDeviceNumOfTotal = nDeviceNumOfTotal;
    }

    public String getStrDeviceNumOfRunning() {
        return strDeviceNumOfRunning;
    }

    public void setStrDeviceNumOfRunning(String strDeviceNumOfRunning) {
        this.strDeviceNumOfRunning = strDeviceNumOfRunning;
    }

    private int nDeviceNumOfTotal;

    private String strDeviceNumOfRunning;

    public String getStrDeviceNumOfStoping() {
        return strDeviceNumOfStoping;
    }

    public void setStrDeviceNumOfStoping(String strDeviceNumOfStoping) {
        this.strDeviceNumOfStoping = strDeviceNumOfStoping;
    }

    private String strDeviceNumOfStoping;

    public String getStrPercent() {
        return strPercent;
    }

    public void setStrPercent(String strPercent) {
        this.strPercent = strPercent;
    }

    private String strPercent;

}
