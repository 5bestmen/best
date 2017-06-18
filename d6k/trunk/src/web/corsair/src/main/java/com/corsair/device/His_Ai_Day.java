package com.corsair.device;

/**
 * Created by zyj on 2017/2/17.
 */
public class His_Ai_Day {

    public double getValue() {
        return value;
    }

    public void setValue(double value) {
        this.value = value;
    }

    public String getTime() {
        return time;
    }

    public void setTime(String time) {
        this.time = time;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    private int id;
    private double value;
    private String time;
}
