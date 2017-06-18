package com.corsair.entity;

import java.util.HashSet;
import java.util.Set;

/**
 * Created by zjq on 2015/11/4.
 */
public class MaterialManufacturer implements java.io.Serializable {
    String id;
    String name;
    String address;
    String contacts;
    String comment;

    Set<MaterialInformation> materialInformations = new HashSet<>();

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getContacts() {
        return contacts;
    }

    public void setContacts(String contacts) {
        this.contacts = contacts;
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }

    public Set<MaterialInformation> getMaterialInformations() {
        return materialInformations;
    }

    public void setMaterialInformations(Set<MaterialInformation> materialInformations) {
        this.materialInformations = materialInformations;
    }
}
