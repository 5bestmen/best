package com.corsair.entity;

import java.util.Date;

/**
 * Created by zjq on 2015/11/4.
 */
public class MaterialInInformation implements java.io.Serializable  {
    String id;
    MaterialStore materialStore;
    Date inTime;
    long price;
    int inNumber;
    String comment;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public MaterialStore getMaterialStore() {
        return materialStore;
    }

    public void setMaterialStore(MaterialStore materialStore) {
        this.materialStore = materialStore;
    }

    public Date getInTime() {
        return inTime;
    }

    public void setInTime(Date inTime) {
        this.inTime = inTime;
    }

    public long getPrice() {
        return price;
    }

    public void setPrice(long price) {
        this.price = price;
    }

    public int getInNumber() {
        return inNumber;
    }

    public void setInNumber(int inNumber) {
        this.inNumber = inNumber;
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }
}
