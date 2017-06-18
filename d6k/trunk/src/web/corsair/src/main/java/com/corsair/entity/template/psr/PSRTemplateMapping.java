package com.corsair.entity.template.psr;

import com.corsair.entity.ElectricStationInfo;

/**
 * Created by 洪祥 on 15/7/15.
 */
public class PSRTemplateMapping implements java.io.Serializable {
    private String id;
    private String PSRID;
    private String devEncode;
    private DeviceTemplate deviceTemplate;
    private ElectricStationInfo electricStationInfo;

    public String getId() { return id; }

    public void setId(String id) { this.id = id;}

    public String getPSRID() {
        return PSRID;
    }

    public void setPSRID(String PSRID) {
        this.PSRID = PSRID;
    }

    public String getDevEncode(){return devEncode;}

    public void setDevEncode(String devEncode) {
        this.devEncode = devEncode;
    }

    public DeviceTemplate getDeviceTemplate() { return deviceTemplate; }

    public void setDeviceTemplate(DeviceTemplate deviceTemplate) {
        this.deviceTemplate = deviceTemplate;
    }

    public ElectricStationInfo getElectricStationInfo() { return electricStationInfo; }

    public void setElectricStationInfo(ElectricStationInfo electricStationInfo) { this.electricStationInfo = electricStationInfo; }
}
