package com.corsair.device;

/**
 * Created by zyj on 2017/2/14.
 */
public class NameAndUnit {
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getUnit() {
        return unit;
    }

    public void setUnit(String unit) {
        this.unit = unit;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    int id;
    String name;    //测点名
    String unit;    //测点单位
}
