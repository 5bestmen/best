package com.corsair.entity.template.psr;

/**
 * Created by 洪祥 on 15/7/27.
 */
public class AccumulateType implements java.io.Serializable {
    String id;
    String name;
    String comments;

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
