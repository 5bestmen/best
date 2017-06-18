package com.corsair.form;

/**
 * Created by 洪祥 on 2015/9/16.
 */
public class DisplayAreaModifyForm{

    private String id;
    private String submit;
    private String name;
    private String comments;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getSubmit() {
        return submit;
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

    public void setSubmit(String submit) {
        this.submit = submit;
    }
}
