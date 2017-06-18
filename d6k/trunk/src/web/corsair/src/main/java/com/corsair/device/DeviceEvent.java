package com.corsair.device;

/**
 * Created by 洪祥 on 15/7/13.
 */
public class DeviceEvent {

    public DeviceEvent(String deviceName, String type, String event, String eventTime, double value) {
        this.deviceName = deviceName;
        this.type = type;
        this.event = event;
        this.value = value;
        this.eventTime = eventTime; //事项时间格式 例:2015/01/01 10:10:10:000
    }

    public DeviceEvent(String deviceName, String type, String event) {
        this.deviceName = deviceName;
        this.type = type;
        this.event = event;
    }

    public DeviceEvent() {
    }

    public String getDeviceName() {
        return deviceName;
    }

    public void setDeviceName(String deviceName) {
        this.deviceName = deviceName;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getEvent() {
        return event;
    }

    public void setEvent(String event) {
        this.event = event;
    }

    public double getValue() {
        return value;
    }

    public void setValue(double value) {
        this.value = value;
    }

    public long getTime() {
        return time;
    }

    public void setTime(long time) {
        this.time = time;
    }

    public void setEventTime(String eventTime) {
        this.eventTime = eventTime;
    }

    public String getEventTime() {
        return eventTime;
    }

    private long time;
    private String deviceName;
    private String type;
    private String event;
    private double value;
    private String eventTime;

    //其它可能需要的数据
}
