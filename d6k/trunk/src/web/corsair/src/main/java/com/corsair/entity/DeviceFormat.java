package com.corsair.entity;

import com.corsair.entity.template.psr.DeviceTemplate;

/**
 * Created by libh on 2015/12/1.
 * 设备规格,可扩展;
 */

public class DeviceFormat implements java.io.Serializable{
    private String id;
    private String name;
    private String comments;
    private DeviceTemplate deviceTemplate;

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

    public DeviceTemplate getDeviceTemplate() {
        return deviceTemplate;
    }

    public void setDeviceTemplate(DeviceTemplate deviceTemplate) {
        this.deviceTemplate = deviceTemplate;
    }
}
