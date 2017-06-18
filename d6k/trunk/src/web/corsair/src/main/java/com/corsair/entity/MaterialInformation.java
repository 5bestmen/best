package com.corsair.entity;

import java.util.HashSet;
import java.util.Set;

/**
 * Created by zjq on 2015/11/4.
 */
public class MaterialInformation implements java.io.Serializable {
    String id;
    String materialSeriesNo;
    String materialId;
    String materialName;
    String materialType;
    String materialDesc;
    String materialExtendDesc;
    MaterialSupply materialSupply;
    MaterialManufacturer materialManufacturer;
    Set<MaterialStore> materialStores = new HashSet<>();
    String comment;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getMaterialSeriesNo() {
        return materialSeriesNo;
    }

    public void setMaterialSeriesNo(String materialSeriesNo) {
        this.materialSeriesNo = materialSeriesNo;
    }

    public String getMaterialName() {
        return materialName;
    }

    public void setMaterialName(String materialName) {
        this.materialName = materialName;
    }

    public String getMaterialExtendDesc() {
        return materialExtendDesc;
    }

    public void setMaterialExtendDesc(String materialExtendDesc) {
        this.materialExtendDesc = materialExtendDesc;
    }

    public String getMaterialType() {
        return materialType;
    }

    public void setMaterialType(String materialType) {
        this.materialType = materialType;
    }

    public String getMaterialId() {
        return materialId;
    }

    public void setMaterialId(String materialId) {
        this.materialId = materialId;
    }

    public MaterialSupply getMaterialSupply() {
        return materialSupply;
    }

    public void setMaterialSupply(MaterialSupply materialSupply) {
        this.materialSupply = materialSupply;
    }

    public MaterialManufacturer getMaterialManufacturer() {
        return materialManufacturer;
    }

    public void setMaterialManufacturer(MaterialManufacturer materialManufacturer) {
        this.materialManufacturer = materialManufacturer;
    }

    public String getMaterialDesc() {
        return materialDesc;
    }

    public void setMaterialDesc(String materialDesc) {
        this.materialDesc = materialDesc;
    }

    public Set<MaterialStore> getMaterialStores() {
        return materialStores;
    }

    public void setMaterialStores(Set<MaterialStore> materialStores) {
        this.materialStores = materialStores;
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }
}
