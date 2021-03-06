package com.corsair.entity.template.psr;

import java.io.Serializable;
import java.util.Set;

/**
 * Created by 洪祥 on 16/2/23.
 * 电力系统资源类型,如厂站\阵列\逆变器等
 */
public class PSRType implements Serializable {
    private String id;
    private String name;
    private String comments;
    private Set<DeviceTemplate> deviceTemplates;

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

    public String getComments() {
        return comments;
    }

    public void setComments(String comments) {
        this.comments = comments;
    }

    public Set<DeviceTemplate> getDeviceTemplates() {
        return deviceTemplates;
    }

    public void setDeviceTemplates(Set<DeviceTemplate> deviceTemplates) {
        this.deviceTemplates = deviceTemplates;
    }
}
