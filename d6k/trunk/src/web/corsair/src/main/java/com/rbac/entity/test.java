package com.rbac.entity;

/**
 * Created by zyj on 2016/11/21.
 */

public class test implements java.io.Serializable {
    public Long getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public void setName(String name) {
        this.name = name;
    }

    private Long id;
    private String name;
}
