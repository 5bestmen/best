package com.corsair.entity.template.display;

import java.util.HashSet;
import java.util.Set;

/**
 * Created by 洪祥 on 16/2/29.
 */
public class DisplayElementTemplate implements java.io.Serializable {
    private String id;
    private String comments;
    private String fileName;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getComments() {
        return comments;
    }

    public void setComments(String comments) {
        this.comments = comments;
    }

    public String getFileName() {
        return fileName;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }
}
