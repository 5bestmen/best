package com.corsair.device;

/**
 * Created by libh on 2015/12/21.
 */
public class ElectricPSR {
    private String name;
    private Double curValue;
    private Double lastValue;
    private Double electricValue;
    private int multiple;
    private Double electricPrice;
    private Double fee;


    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Double getCurValue() {
        return curValue;
    }

    public void setCurValue(Double curValue) {
        this.curValue = curValue;
    }

    public Double getLastValue() {
        return lastValue;
    }

    public void setLastValue(Double lastValue) {
        this.lastValue = lastValue;
    }

    public Double getElectricValue() { return electricValue; }

    public void setElectricValue(Double electricValue) { this.electricValue = electricValue; }

    public int getMultiple() { return multiple; }

    public void setMultiple(int multiple) { this.multiple = multiple; }

    public Double getElectricPrice() {
        return electricPrice;
    }

    public void setElectricPrice(Double electricPrice) {
        this.electricPrice = electricPrice;
    }

    public Double getFee() {
        return fee;
    }

    public void setFee(Double fee) {
        this.fee = fee;
    }
}

