package com.corsair.entity;

import java.util.Date;

/**
 * Created by zjq on 2015/11/4.
 */
public class MaterialOutInfomation implements java.io.Serializable  {
    String id;
    MaterialStore materialStore;
    Date outTime;
    long price;
    int outNumber;
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

    public Date getOutTime() {
        return outTime;
    }

    public void setOutTime(Date outTime) {
        this.outTime = outTime;
    }

    public long getPrice() {
        return price;
    }

    public void setPrice(long price) {
        this.price = price;
    }

    public int getOutNumber() {
        return outNumber;
    }

    public void setOutNumber(int outNumber) {
        this.outNumber = outNumber;
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }
}
