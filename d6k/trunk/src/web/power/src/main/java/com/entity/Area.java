package com.entity;

/**
 * Created by zyj on 2017/5/19.
 */
public class Area {
    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getSort() {
        return sort;
    }

    public void setSort(Integer sort) {
        this.sort = sort;
    }

    private Integer id;
    private String name;
    private Integer sort;
}
