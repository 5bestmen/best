package com.corsair.entity.template.display;

import java.util.HashSet;
import java.util.Set;

/**
 * Created by 洪祥 on 15/7/15.
 */
public class DisplayArea extends Object implements java.io.Serializable {
    private String id;
    private String name;
    private String comments;

    private Set<DisplayTemplate> displayTemplates = new HashSet<DisplayTemplate>();

    public boolean equals(Object object) {
        if (object instanceof DisplayArea) {
            return id.equals(((DisplayArea)object).getId());
        } else {
            return super.equals(object);
        }
    }

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

    public Set<DisplayTemplate> getDisplayTemplates() {
        return displayTemplates;
    }

    public void setDisplayTemplates(Set<DisplayTemplate> displayTemplates) {
        this.displayTemplates = displayTemplates;
    }
}
