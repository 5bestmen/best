package com.corsair.measurement;

/**
 * Created by 洪祥 on 15/7/24.
 */
public class MeasurementValue {
    String key;
    Double data;
    String text;

    Boolean dead;
    Boolean invalid;
    Boolean upperLimit;
    Boolean upperUpperLimit;
    Boolean lowerLimit;
    Boolean lowerLowerLimit;
    Boolean suspicious;


    public MeasurementValue(String key,Double data, String text, Boolean dead, Boolean invalid, Boolean upperLimit, Boolean upperUpperLimit, Boolean lowerLimit, Boolean lowerLowerLimit, Boolean suspicious) {
        this.key = key;
        this.data = data;
        this.text = text;
        this.dead = dead;
        this.invalid = invalid;
        this.upperLimit = upperLimit;
        this.upperUpperLimit = upperUpperLimit;
        this.lowerLimit = lowerLimit;
        this.lowerLowerLimit = lowerLowerLimit;
        this.suspicious = suspicious;
    }

    public MeasurementValue(Double data, String text) {
        this.data = data;
        this.text = text;
    }

    public MeasurementValue(String text) {
        this.text = text;
    }

    public MeasurementValue(Double data) {
        this.data = data;
    }

    public MeasurementValue() {

    }

    public String getKey() {
        return key;
    }

    public void setKey(String key) {
        this.key = key;
    }

    public Double getData() {
        return data;
    }

    public void setData(Double data) {
        this.data = data;
    }

    public String getText() {
        return text;
    }

    public void setText(String text) {
        this.text = text;
    }

    public Boolean getDead() {
        return dead;
    }

    public void setDead(Boolean dead) {
        this.dead = dead;
    }

    public Boolean getInvalid() {
        return invalid;
    }

    public void setInvalid(Boolean invalid) {
        this.invalid = invalid;
    }

    public Boolean getUpperLimit() {
        return upperLimit;
    }

    public void setUpperLimit(Boolean upperLimit) {
        this.upperLimit = upperLimit;
    }

    public Boolean getUpperUpperLimit() {
        return upperUpperLimit;
    }

    public void setUpperUpperLimit(Boolean upperUpperLimit) {
        this.upperUpperLimit = upperUpperLimit;
    }

    public Boolean getLowerLimit() {
        return lowerLimit;
    }

    public void setLowerLimit(Boolean lowerLimit) {
        this.lowerLimit = lowerLimit;
    }

    public Boolean getLowerLowerLimit() {
        return lowerLowerLimit;
    }

    public void setLowerLowerLimit(Boolean lowerLowerLimit) {
        this.lowerLowerLimit = lowerLowerLimit;
    }

    public Boolean getSuspicious() {
        return suspicious;
    }

    public void setSuspicious(Boolean suspicious) {
        this.suspicious = suspicious;
    }
}
