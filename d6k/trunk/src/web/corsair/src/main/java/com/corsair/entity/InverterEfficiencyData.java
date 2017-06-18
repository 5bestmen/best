package com.corsair.entity;

/**
 * Created by 洪祥 on 15/8/6.
 */
public class InverterEfficiencyData implements java.io.Serializable {
    public String getID() {
        return ID;
    }

    public void setID(String ID) {
        this.ID = ID;
    }

    private String ID;
    private InverterEfficiency inverterEfficiency;

    private InverterTemperature inverterTemperature;
    private InverterPowerPoint inverterPowerPoint;

    private double efficiency;

    public InverterEfficiency getInverterEfficiency() {
        return inverterEfficiency;
    }

    public void setInverterEfficiency(InverterEfficiency inverterEfficiency) {
        this.inverterEfficiency = inverterEfficiency;
    }

    public InverterTemperature getInverterTemperature() {
        return inverterTemperature;
    }

    public void setInverterTemperature(InverterTemperature inverterTemperature) {
        this.inverterTemperature = inverterTemperature;
    }

    public InverterPowerPoint getInverterPowerPoint() {
        return inverterPowerPoint;
    }

    public void setInverterPowerPoint(InverterPowerPoint inverterPowerPoint) {
        this.inverterPowerPoint = inverterPowerPoint;
    }

    public double getEfficiency() {
        return efficiency;
    }

    public void setEfficiency(double efficiency) {
        this.efficiency = efficiency;
    }
}
