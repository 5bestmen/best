package com.corsair.entity.config;

import javax.xml.bind.annotation.XmlElement;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 2015/12/17.
 */
public class DeviceTemplateDataType {

    private String typeInt;
    private String typeString;
    private List<DeviceTemplateDataType> templateDataTypes = new ArrayList<DeviceTemplateDataType>();

    @XmlElement
    public String getTypeInt() {
        return typeInt;
    }

    public void setTypeInt(String typeInt) {
        this.typeInt = typeInt;
    }

    @XmlElement
    public String getTypeString() {
        return typeString;
    }

    public void setTypeString(String typeString) {
        this.typeString = typeString;
    }

    @XmlElement
    public List<DeviceTemplateDataType> getTemplateDataTypes() {
        return templateDataTypes;
    }

    public void setTemplateDataTypes(List<DeviceTemplateDataType> templateDataTypes) {
        this.templateDataTypes = templateDataTypes;
    }
}
