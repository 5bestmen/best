package com.corsair.form;

/**
 * Created by 洪祥 on 2015/9/16.
 */
public class DisplayTemplateModifyForm{

    private String id;
    private String name;
    private String desc;
    private String needContainer;
    private String deviceTemplate;
    private String displayArea;
    private String submit;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getSubmit() {
        return submit;
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

    public void setSubmit(String submit) {
        this.submit = submit;
    }

    public String getDisplayArea() {
        return displayArea;
    }

    public void setDisplayArea(String displayArea) {
        this.displayArea = displayArea;
    }

    public String getNeedContainer() {
        return needContainer;
    }

    public void setNeedContainer(String needContainer) {
        this.needContainer = needContainer;
    }

    public String getDeviceTemplate() {
        return deviceTemplate;
    }

    public void setDeviceTemplate(String deviceTemplate) {
        this.deviceTemplate = deviceTemplate;
    }

}
