package com.corsair.form;

/**
 * Created by 洪祥 on 2015/9/16.
 */
public class DeviceTemplateModifyForm{

    private String id;
    private String submit;
    private String name;
    private String desc;
    private String psrType;

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

    public String getPsrType() {
        return psrType;
    }

    public void setPsrType(String psrType) {
        this.psrType = psrType;
    }
}
