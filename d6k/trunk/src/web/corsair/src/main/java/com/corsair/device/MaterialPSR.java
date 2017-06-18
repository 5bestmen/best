package com.corsair.device;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by libh on 2015/10/21.
 */
public class MaterialPSR {
    private String id;
    private String name;
    private String desc;
    private String type;
    private String area;
    private List<MaterialPSR> children = new ArrayList<MaterialPSR>();

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

    public String getDesc() {return desc;}

    public void setDesc(String desc) {this.desc = desc;}

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getArea() {
        return area;
    }

    public void setArea(String area) {
        this.area = area;
    }

    public List<MaterialPSR> getChildren() {
        return children;
    }

    public void setChildren(List<MaterialPSR> children) {
        this.children = children;
    }
}
