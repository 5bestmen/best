package com.corsair.entity;

import java.util.HashSet;
import java.util.Set;

/**
 * Created by zjq on 2015/11/4.
 */
public class MaterialStore implements java.io.Serializable {
    String id;
    MaterialInformation materialInformation;
    MaterialWarehouse materialWarehouse;
    int stockNumber;
    int inventoryUpperlimit;
    int inventoryfloor;
    String comment;
    Set<MaterialOutInfomation> materialOutInfomations = new HashSet<>();
    Set<MaterialInInformation> materialInInformations = new HashSet<>();

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public MaterialInformation getMaterialInformation() {
        return materialInformation;
    }

    public void setMaterialInformation(MaterialInformation materialInformation) {
        this.materialInformation = materialInformation;
    }

    public MaterialWarehouse getMaterialWarehouse() {
        return materialWarehouse;
    }

    public void setMaterialWarehouse(MaterialWarehouse materialWarehouse) {
        this.materialWarehouse = materialWarehouse;
    }

    public int getStockNumber() {
        return stockNumber;
    }

    public void setStockNumber(int stockNumber) {
        this.stockNumber = stockNumber;
    }

    public int getInventoryUpperlimit() {
        return inventoryUpperlimit;
    }

    public void setInventoryUpperlimit(int inventoryUpperlimit) {
        this.inventoryUpperlimit = inventoryUpperlimit;
    }

    public int getInventoryfloor() {
        return inventoryfloor;
    }

    public void setInventoryfloor(int inventoryfloor) {
        this.inventoryfloor = inventoryfloor;
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }

    public Set<MaterialOutInfomation> getMaterialOutInfomations() {
        return materialOutInfomations;
    }

    public void setMaterialOutInfomations(Set<MaterialOutInfomation> materialOutInfomations) {
        this.materialOutInfomations = materialOutInfomations;
    }

    public Set<MaterialInInformation> getMaterialInInformations() {
        return materialInInformations;
    }

    public void setMaterialInInformations(Set<MaterialInInformation> materialInInformations) {
        this.materialInInformations = materialInInformations;
    }
}
