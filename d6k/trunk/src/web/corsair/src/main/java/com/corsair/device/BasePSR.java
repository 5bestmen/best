package com.corsair.device;

/**
 * Created by 洪祥 on 15/7/22.
 */
public class BasePSR {
    private String id;
    private String name;
    private String unit;    //测点单位
    private String area;    //集控中心新增区域序号字段


    public BasePSR(String id, String name, String unit) {
        this.id = id;
        this.name = name;
        this.unit = unit;
    }

    public BasePSR(String id, String name) {
        this.id = id;
        this.name = name;
    }

    public BasePSR(String id, String name,String unit, String area) {
        this.id = id;
        this.name = name;
        this.unit = unit;
        this.area = area;
    }

    public BasePSR() {
    }

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

    public String getArea() {
        return area;
    }

    public void setArea(String area) {
        this.area = area;
    }

    public String getUnit() {
        return unit;
    }

    public void setUnit(String unit) {
        this.unit = unit;
    }
}
