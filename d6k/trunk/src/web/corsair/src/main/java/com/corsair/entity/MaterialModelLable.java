package com.corsair.entity;

import java.util.HashSet;
import java.util.Set;

/**
 * Created by libh on 2015/10/16.
 */
public class MaterialModelLable implements java.io.Serializable {
    private String materialModelLableID;
    private String name;
    private Set<MaterialModel>  materialModels = new HashSet<MaterialModel>();

    public String getMaterialModelLableID() {
        return materialModelLableID;
    }

    public void setMaterialModelLableID(String materialModelLableID) {this.materialModelLableID = materialModelLableID;}

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Set<MaterialModel> getMaterialModels() {return materialModels;}

    public void setMaterialModels(Set<MaterialModel> materialModels) {
        this.materialModels = materialModels;
    }
}
