package com.corsair.entity;

/**
 * Created by 洪祥 on 15/7/1.
 */
public class ApplicationInformation implements java.io.Serializable {
    private String key;
    private String value;

    public ApplicationInformation() {

    }

    public String getKey() {
        return key;
    }

    public void setKey(String key) {
        this.key = key;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }
}
