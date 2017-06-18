package com.corsair.entity;

/**
 * Created by chris on 2015/9/15.
 */
public class StationLocation implements java.io.Serializable {

    private String  station_id;
    private String  station_name;
    private String  station_city;
    private Float   longitude;
    private Float   latitude;
    private String  linkurl;

    public StationLocation() {

    }

    public String getStation_id() {
        return station_id;
    }

    public void setStation_id(String station_id) {
        this.station_id = station_id;
    }

    public String getStation_name() {
        return station_name;
    }

    public void setStation_name(String station_name) {
        this.station_name = station_name;
    }

    public String getStation_city() {
        return station_city;
    }

    public void setStation_city(String station_city) {
        this.station_city = station_city;
    }

    public Float getLongitude() {
        return longitude;
    }

    public void setLongitude(float longitude) {
        this.longitude = longitude;
    }

    public Float getLatitude() {
        return latitude;
    }

    public void setLatitude(float latitude) {
        this.latitude = latitude;
    }

    public String getLinkurl() {
        return linkurl;
    }

    public void setLinkurl(String linkurl) {
        this.linkurl = linkurl;
    }


}
