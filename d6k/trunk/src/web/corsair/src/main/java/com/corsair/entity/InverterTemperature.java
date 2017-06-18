package com.corsair.entity;

/**
 * Created by 洪祥 on 15/8/6.
 */
public class InverterTemperature implements java.io.Serializable {
    private String ID;
    private String name;
    private Integer temperature;

    public String getID() {
        return ID;
    }

    public void setID(String ID) {
        this.ID = ID;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getTemperature() {
        return temperature;
    }

    public void setTemperature(Integer temperature) {
        this.temperature = temperature;
    }
}
