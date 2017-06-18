package com.corsair.entity.template.psr;

import java.util.Comparator;
import java.util.Objects;

/**
 * Created by 洪祥 on 16/2/23.
 * 设备规格:序号\键\值, 与DeviceTemplate构成一对多关系(简单起见允许冗余),其中序号用于在DeviceTemplate中排序
 */
public class PSRSpec implements java.io.Serializable {
    private String id;
    private Integer index; //序号
    private String key; //键
    private String value; //值

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public Integer getIndex() {
        return index;
    }

    public void setIndex(Integer index) {
        this.index = index;
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
