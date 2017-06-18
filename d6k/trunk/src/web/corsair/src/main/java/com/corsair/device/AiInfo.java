package com.corsair.device;

/**
 * Created by zyj on 2016/12/2.
 */
public class AiInfo {
    public String name; // 点名
    public String code; // 代码(eg:1-1-1-0100)
    public String unit; // 单位

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
}
