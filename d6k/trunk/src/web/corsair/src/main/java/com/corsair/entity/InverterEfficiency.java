package com.corsair.entity;

import com.corsair.entity.template.psr.DeviceTemplate;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 15/8/6.
 */
public class InverterEfficiency implements java.io.Serializable {
    private String ID;
    private DeviceTemplate deviceTemplate;

    private List<InverterEfficiencyData> inverterEfficiencyData = new ArrayList<>();

    public String getID() {
        return ID;
    }

    public void setID(String ID) {
        this.ID = ID;
    }

    public DeviceTemplate getDeviceTemplate() {
        return deviceTemplate;
    }

    public void setDeviceTemplate(DeviceTemplate deviceTemplate) {
        this.deviceTemplate = deviceTemplate;
    }

    public List<InverterEfficiencyData> getInverterEfficiencyData() {
        return inverterEfficiencyData;
    }

    public void setInverterEfficiencyData(List<InverterEfficiencyData> inverterEfficiencyData) {
        this.inverterEfficiencyData = inverterEfficiencyData;
    }
}
