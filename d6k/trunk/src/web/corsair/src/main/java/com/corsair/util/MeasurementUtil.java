package com.corsair.util;

/**
 * Created by 洪祥 on 2016/2/25.
 */
public class MeasurementUtil {
    
    private String psrID; //量测psrID
    private String dataType;    //量测数据类型
    private String name; //量测名称
    private String index; //量测索引
    private String comments; //量测描述

    public String getPsrID() {
        return psrID;
    }

    public void setPsrID(String psrID) {
        this.psrID = psrID;
    }

    public String getDataType() {
        return dataType;
    }

    public void setDataType(String dataType) {
        this.dataType = dataType;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getIndex() {
        return index;
    }

    public void setIndex(String index) {
        this.index = index;
    }

    public String getComments() {
        return comments;
    }

    public void setComments(String comments) {
        this.comments = comments;
    }
}
