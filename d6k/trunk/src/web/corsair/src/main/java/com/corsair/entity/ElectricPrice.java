package com.corsair.entity;

import java.util.HashSet;
import java.util.Set;

/**
 * Created by libh on 2015/12/18.
 */
public class ElectricPrice implements java.io.Serializable{
    private String id;
    private String name;
    private Double price;
    private String comments;
    private Set<ElectricTime> electricTimes = new HashSet<ElectricTime>();

    public String getId() {
        return id;
    }

    public void setId(String id) { this.id = id;}

    public String getName() {
        return name;
    }

    public void setName(String name) { this.name = name;}

    public Double getPrice(){ return price;}

    public void setPrice(Double price){ this.price = price;}

    public String getComments() {
        return comments;
    }

    public void setComments(String comments) { this.comments = comments;}

    public Set<ElectricTime> getElectricTimes() {
        return electricTimes;
    }

    public void setElectricTimes(Set<ElectricTime> electricTimes) {
        this.electricTimes = electricTimes;
    }
}
