package com.corsair.measurement.history;


/**
 * Created by 洪祥 on 16/3/3.
 * 历史值类,包含代码\时间\日期\是否有效值标志
 */
public class HistoryValue {
    private String name;
    private String date;
    private String time;
    private Double value;
    private Boolean valid;

    public HistoryValue(String name, String date, String time, Double value, Boolean valid) {
        this.name = name;
        this.date = date;
        this.time = time;
        this.value = value;
        this.valid = valid;
    }

    public HistoryValue(String name, String date, String time, Double value) {
        this.value = value;
        this.name = name;
        this.date = date;
        this.time = time;
        this.valid = Boolean.TRUE;
    }

    public HistoryValue(String date, String time, Double value) {
        this.date = date;
        this.time = time;
        this.value = value;
        this.valid = Boolean.TRUE;
    }

    public HistoryValue(Double value) {
        this.value = value;
        this.valid = Boolean.TRUE;
    }

    public Boolean isValid() {
        return value != null && !value.isInfinite() && !value.isNaN() && java.util.Objects.equals(valid, Boolean.TRUE) ;
    }


    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public String getTime() {
        return time;
    }

    public void setTime(String time) {
        this.time = time;
    }

    public Double getValue() {
        return value;
    }

    public void setValue(Double value) {
        this.value = value;
    }

    public Boolean getValid() {
        return valid;
    }

    public void setValid(Boolean valid) {
        this.valid = valid;
    }
}
