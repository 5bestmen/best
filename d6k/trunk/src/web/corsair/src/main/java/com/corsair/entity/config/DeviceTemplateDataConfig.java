package com.corsair.entity.config;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 2015/12/17.
 */
@XmlRootElement
public class DeviceTemplateDataConfig {

    private List<DeviceTemplateDataType> deviceTemplateDataTypes = new ArrayList<DeviceTemplateDataType>();

    @XmlElement
    public List<DeviceTemplateDataType> getDeviceTemplateDataTypes() {
        return deviceTemplateDataTypes;
    }

    public void setDeviceTemplateDataTypes(List<DeviceTemplateDataType> deviceTemplateDataTypes) {
        this.deviceTemplateDataTypes = deviceTemplateDataTypes;
    }

}
