package com.corsair.device;

/**
 * Created by zyj on 2017/2/25.
 */
public class StationDeviceParaID {
    public int getStationId() {
        return stationId;
    }

    public void setStationId(int stationId) {
        this.stationId = stationId;
    }

    public int getDeviceId() {
        return deviceId;
    }

    public void setDeviceId(int deviceId) {
        this.deviceId = deviceId;
    }

    public String getStrName() {
        return strName;
    }

    public void setStrName(String strName) {
        this.strName = strName;
    }

    private int stationId;
    private int deviceId;

    private String strName;
}
