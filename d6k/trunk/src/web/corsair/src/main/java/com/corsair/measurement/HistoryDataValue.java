package com.corsair.measurement;

/**
 * Created by zjq on 2015/11/10.
 */
public class HistoryDataValue {
    String name;
    Integer sdate;
    Integer time;
    Double data;
    Boolean invalid;
    String desc;

    public HistoryDataValue(){

    }

    public HistoryDataValue(Integer time,String desc){
        setTime(time);
        setDesc(desc);
    }

    public HistoryDataValue(Integer time,Double value){
        setTime(time);
        setData(value);
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getSdate() {
        return sdate;
    }

    public void setSdate(Integer sdate) {
        this.sdate = sdate;
    }

    public Integer getTime() {
        return time;
    }

    public void setTime(Integer time) {
        this.time = time;
    }

    public Double getData() {
        return data;
    }

    public void setData(Double data) {
        this.data = data;
    }

    public Boolean getInvalid() {
        return invalid;
    }

    public void setInvalid(Boolean invalid) {
        this.invalid = invalid;
    }

    public String getDesc() {
        return desc;
    }

    public void setDesc(String desc) {
        this.desc = desc;
    }
}
