package com.corsair.entity;

import java.util.HashSet;
import java.util.Set;

/**
 * Created by libh on 2015/10/16.
 */
public class MaterialModel implements java.io.Serializable{
    private String materialModelID;
    private String codeID;
    private MaterialType materialType;
    private Set<MaterialModelLable> materialModelLables = new HashSet<MaterialModelLable>();
    private Set<Material> materials = new HashSet<Material>();
    private String modelLables;

    public String getMaterialModelID() {
        return materialModelID;
    }

    public void setMaterialModelID(String materialModelID) {
        this.materialModelID = materialModelID;
    }

    public String getCodeID() {
        return codeID;
    }

    public void setCodeID(String codeID) {
        this.codeID = codeID;
    }

    public MaterialType getMaterialType() {
        return materialType;
    }

    public void setMaterialType(MaterialType materialType) {
        this.materialType = materialType;
    }

    public Set<MaterialModelLable> getMaterialModelLables() {
        return materialModelLables;
    }

    public void setMaterialModelLables(Set<MaterialModelLable> materialModelLables) {
        this.materialModelLables = materialModelLables;
    }

    public Set<Material> getMaterials(){
        return materials;
    }

    public void  setMaterials(Set<Material> materials){
        this.materials = materials;
    }

    public String getModelLables() {
        return modelLables;
    }

    public void setModelLables(String modelLables) {
        this.modelLables = modelLables;
    }
}
