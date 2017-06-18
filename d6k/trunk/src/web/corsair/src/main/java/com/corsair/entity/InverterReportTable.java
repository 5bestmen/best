package com.corsair.entity;

import java.util.Date;

/**
 * Created by zjq on 2015/8/24.
 */
public class InverterReportTable implements java.io.Serializable {
    String id;
    String psrID;
    Date date;
    Double electricity;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public Double getElectricity() {
        return electricity;
    }

    public void setElectricity(Double electricity) {
        this.electricity = electricity;
    }

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public String getPsrID() {
        return psrID;
    }

    public void setPsrID(String psrID) {
        this.psrID = psrID;
    }
}
