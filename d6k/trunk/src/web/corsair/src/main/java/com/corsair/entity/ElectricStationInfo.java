package com.corsair.entity;

import com.corsair.entity.template.psr.PSRTemplateMapping;

import java.util.HashSet;
import java.util.Set;

/**
 * Created by libh on 2015/12/23.
 */
public class ElectricStationInfo implements java.io.Serializable{
    private String id;
    private String name;
    private String phoneNo;
    private String comments;
    private String address;
    private String remark;
    private Set<PSRTemplateMapping> psrTemplateMappings = new HashSet<>();

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

    public String getPhoneNo() {
        return phoneNo;
    }

    public void setPhoneNo(String phoneNo) {
        this.phoneNo = phoneNo;
    }

    public String getComments() {
        return comments;
    }

    public void setComments(String comments) {
        this.comments = comments;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getRemark() {
        return remark;
    }

    public void setRemark(String remark) {
        this.remark = remark;
    }

    public Set<PSRTemplateMapping> getPsrTemplateMappings() {
        return psrTemplateMappings;
    }

    public void setPsrTemplateMappings(Set<PSRTemplateMapping> psrTemplateMappings) {
        this.psrTemplateMappings = psrTemplateMappings;
    }

}

