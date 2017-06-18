package com.corsair.device;

/**
 * Created by zyj on 2016/12/2.
 */
public class DiInfo {
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

    public String name; // 点名
    public String code; // 代码(eg:1-1-1-0100)
}
