package com.corsair.util;

import java.util.List;

/**
 * Created by 洪祥 on 2016/2/25.
 */
public class StationMeasureUtil {

    private String stationID; //厂站ID
    private String stationName; //厂站名
    private List<MeasurementUtil> measurementUtils; //厂站下的所有量测

    public String getStationID() {
        return stationID;
    }

    public void setStationID(String stationID) {
        this.stationID = stationID;
    }

    public String getStationName() {
        return stationName;
    }

    public void setStationName(String stationName) {
        this.stationName = stationName;
    }

    public List<MeasurementUtil> getMeasurementUtils() {
        return measurementUtils;
    }

    public void setMeasurementUtils(List<MeasurementUtil> measurementUtils) {
        this.measurementUtils = measurementUtils;
    }
}
