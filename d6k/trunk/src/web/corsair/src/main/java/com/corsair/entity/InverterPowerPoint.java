package com.corsair.entity;

/**
 * Created by 洪祥 on 15/8/6.
 */
public class InverterPowerPoint implements java.io.Serializable {
    private String ID;
    private Integer point;

    public String getID() {
        return ID;
    }

    public void setID(String ID) {
        this.ID = ID;
    }

    public Integer getPoint() {
        return point;
    }

    public void setPoint(Integer point) {
        this.point = point;
    }
}
