package com.corsair.entity;

import java.util.HashSet;
import java.util.Set;

/**
 * Created by libh on 2015/10/16.
 */
public class Material implements java.io.Serializable{
    private String materialID;
    private String fixedID;
    private MaterialModel materialModel;
    private Set<MaterialExtendLable> materialExtendLables = new HashSet<MaterialExtendLable>();
    private String extendLabels;

    public String getMaterialID() {
        return materialID;
    }

    public void setMaterialID(String materialID) {
        this.materialID = materialID;
    }

    public String getFixedID() {
        return fixedID;
    }

    public void setFixedID(String fixedID) {
        this.fixedID = fixedID;
    }

    public MaterialModel getMaterialModel() {
        return materialModel;
    }

    public void setMaterialModel(MaterialModel materialModel) {
        this.materialModel = materialModel;
    }

    public Set<MaterialExtendLable> getMaterialExtendLables() {
        return materialExtendLables;
    }

    public void setMaterialExtendLables(Set<MaterialExtendLable> materialExtendLables) {
        this.materialExtendLables = materialExtendLables;
    }

    public String getExtendLabels() {
        return extendLabels;
    }

    public void setExtendLabels(String extendLabels) {
        this.extendLabels = extendLabels;
    }
}
