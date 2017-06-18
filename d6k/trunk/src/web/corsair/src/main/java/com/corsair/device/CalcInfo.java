package com.corsair.device;

/**
 * Created by zyj on 2016/12/2.
 */
public class CalcInfo {
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getCode() {
        return code;
    }

    public void setCode(String code) {
        this.code = code;
    }

    public String getUnit() {
        return unit;
    }

    public void setUnit(String unit) {
        this.unit = unit;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }

    private String name; // 点名
    private String code; // 代码(eg:1-1-1-0100)
    private String unit; // 单位
    private String value; // 值

    public float getfValue() {
        return fValue;
    }

    public void setfValue(float fValue) {
        this.fValue = fValue;
    }

    private float fValue;
}
