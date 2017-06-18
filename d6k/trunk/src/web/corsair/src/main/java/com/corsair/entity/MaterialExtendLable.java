package com.corsair.entity;

import java.util.HashSet;
import java.util.Set;

/**
 * Created by libh on 2015/10/16.
 */
public class MaterialExtendLable implements java.io.Serializable{
    private String materialExtendLableID;
    private String name;
    private Set<Material> materials = new HashSet<Material>();

    public String getMaterialExtendLableID() {
        return materialExtendLableID;
    }

    public void setMaterialExtendLableID(String materialExtendLableID) {
        this.materialExtendLableID = materialExtendLableID;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Set<Material> getMaterials() {
        return materials;
    }

    public void setMaterials(Set<Material> materials) {
        this.materials = materials;
    }
}
