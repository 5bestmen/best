package com.corsair.entity;

import java.util.HashSet;
import java.util.Set;

/**
 * Created by libh on 2015/10/16.
 */
public class MaterialType implements java.io.Serializable {
    private String materialTypeID;
    private String name;
    private String comments;

    private Set<MaterialModel> materialModels = new HashSet<MaterialModel>();

    public String getMaterialTypeID() {
        return materialTypeID;
    }

    public void setMaterialTypeID(String materialTypeID) {
        this.materialTypeID = materialTypeID;
    }

    public String getName() {return name;}

    public void setName(String name) {this.name = name;}

   public String getComments() {return comments;}

    public void setComments(String comments) {this.comments = comments;}



    public Set<MaterialModel> getMaterialModels() {
        return materialModels;
    }

    public void setMaterialModels(Set<MaterialModel> materialModels) {
        this.materialModels = materialModels;
    }

}
