package com.corsair.entity.template.psr;

import com.corsair.entity.template.display.DisplayTemplate;

import java.util.HashSet;
import java.util.Set;

/**
 * Created by 洪祥 on 15/7/13.
 * 电力系统资源模板,可以包含厂站\阵列\具体设备等
 */
public class DeviceTemplate implements java.io.Serializable {

    private String id;
    private String name;
    private String desc;
    private String model;
    private Set<DeviceTemplateData> deviceTemplateDatas = new HashSet<>();
    private Set<DisplayTemplate> displayTemplates = new HashSet<>();

    private Set<PSRSpec> psrSpecs = new HashSet<>(); //设备规格
    private PSRType type; //设备类型
    private PSRFactory psrFactory; //设备厂家

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDesc() {
        return desc;
    }

    public void setDesc(String desc) {
        this.desc = desc;
    }

    public String getModel() {
        return model;
    }

    public void setModel(String model) {
        this.model = model;
    }

    public Set<DeviceTemplateData> getDeviceTemplateDatas() {
        return deviceTemplateDatas;
    }

    public void setDeviceTemplateDatas(Set<DeviceTemplateData> deviceTemplateDatas) {
        this.deviceTemplateDatas = deviceTemplateDatas;
    }

    public Set<DisplayTemplate> getDisplayTemplates() {
        return displayTemplates;
    }

    public void setDisplayTemplates(Set<DisplayTemplate> displayTemplates) {
        this.displayTemplates = displayTemplates;
    }

    public DeviceTemplateData getDeviceTemplateData(Integer type, Integer index) {
        for (DeviceTemplateData deviceTemplateData : deviceTemplateDatas) {
            if (deviceTemplateData.getType().equals(type) && deviceTemplateData.getIndex().equals(index)) {
                return deviceTemplateData;
            }
        }
        return null;
    }

    public DeviceTemplateData getDeviceTemplateDataByName(String name) {
        for (DeviceTemplateData deviceTemplateData : deviceTemplateDatas) {
            if (name.equals(deviceTemplateData.getName())) {
                return deviceTemplateData;
            }
        }
        return null;
    }

    public DeviceTemplateData getDeviceTemplateDataById(String id) {
        for (DeviceTemplateData deviceTemplateData : deviceTemplateDatas) {
            if (deviceTemplateData.getId().equals(id)) {
                return deviceTemplateData;
            }
        }
        return null;
    }

    public com.corsair.entity.template.psr.PSRType getType() {
        return type;
    }

    public void setType(com.corsair.entity.template.psr.PSRType type) {
        this.type = type;
    }

    public Set<PSRSpec> getPsrSpecs() {
        return psrSpecs;
    }

    public void setPsrSpecs(Set<PSRSpec> psrSpecs) {
        this.psrSpecs = psrSpecs;
    }

    public PSRFactory getPsrFactory() {
        return psrFactory;
    }

    public void setPsrFactory(PSRFactory psrFactory) {
        this.psrFactory = psrFactory;
    }

    public String getPSRType() {
        return type.getName();
    }

    public String getFactory() {
        return psrFactory == null ? null : psrFactory.getName();
    }
}
