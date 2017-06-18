package com.corsair.entity;

/**
 * Created by 洪祥 on 15/7/30.
 */
public class SpecialChart implements java.io.Serializable {
    private String id;
    private String name;
    private String comments;

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

    public String getComments() {
        return comments;
    }

    public void setComments(String comments) {
        this.comments = comments;
    }
}
