package com.corsair.entity.template.display;

import com.corsair.entity.template.psr.DeviceTemplateData;

/**
 * Created by 洪祥 on 15/7/13.
 */
public class DisplayTemplateData implements java.io.Serializable {
    private String id;
    private Integer index;
    private DisplayTemplate displayTemplate;
    private DeviceTemplateData deviceTemplateData;
    private String icon;
    private String extraString1;
    private String extraString2;
    private String extraString3;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public Integer getIndex() {
        return index;
    }

    public void setIndex(Integer index) {
        this.index = index;
    }

    public DisplayTemplate getDisplayTemplate() {
        return displayTemplate;
    }

    public void setDisplayTemplate(DisplayTemplate displayTemplate) {
        this.displayTemplate = displayTemplate;
    }

    public DeviceTemplateData getDeviceTemplateData() {
        return deviceTemplateData;
    }

    public void setDeviceTemplateData(DeviceTemplateData deviceTemplateData) {
        this.deviceTemplateData = deviceTemplateData;
    }

    public String getIcon() {
        return icon;
    }

    public void setIcon(String icon) {
        this.icon = icon;
    }

    public String getExtraString1() {
        return extraString1;
    }

    public void setExtraString1(String extraString1) {
        this.extraString1 = extraString1;
    }

    public String getExtraString2() {
        return extraString2;
    }

    public void setExtraString2(String extraString2) {
        this.extraString2 = extraString2;
    }

    public String getExtraString3() {
        return extraString3;
    }

    public void setExtraString3(String extraString3) {
        this.extraString3 = extraString3;
    }
}
