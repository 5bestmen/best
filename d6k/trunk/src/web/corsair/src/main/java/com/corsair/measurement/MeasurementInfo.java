package com.corsair.measurement;

import com.google.common.base.Strings;

/**
 * Created by 洪祥 on 15/7/24.
 */
public class MeasurementInfo {
    private String psrID;
    private String measurementID;
    private Integer measurementType;
    private String magic;
    private Integer templateIndex;
    private Boolean calculate;
    private String name;

    public MeasurementInfo() {
    }

    public MeasurementInfo(String measurementID) {
        this.measurementID = measurementID;
    }

    public Boolean getCalculate() {
        return calculate;
    }

    public void setCalculate(Boolean calculate) {
        this.calculate = calculate;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPsrID() {
        return psrID;
    }

    public void setPsrID(String psrID) {
        this.psrID = psrID;
    }

    public String getMeasurementID() {
        return measurementID;
    }

    public void setMeasurementID(String measurementID) {
        this.measurementID = measurementID;
    }

    public Integer getMeasurementType() {
        return measurementType;
    }

    public void setMeasurementType(Integer measurementType) {
        this.measurementType = measurementType;
    }

    public String getMagic() {
        return magic;
    }

    public void setMagic(String magic) {
        this.magic = magic;
    }

    public Integer getTemplateIndex() {
        return templateIndex;
    }

    public void setTemplateIndex(Integer templateIndex) {
        this.templateIndex = templateIndex;
    }

    public Boolean magic() {
        return !Strings.isNullOrEmpty(magic);
    }

    public Boolean realTime() {
        return Strings.isNullOrEmpty(measurementID);
    }

}
