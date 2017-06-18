package com.corsair.util;

import java.util.List;

/**
 * Created by 洪祥 on 2016/2/25.
 */
public class HisDataChartUtil {

    private String dataType; //数据类型：遥测 or 电度
    private String dateFrom; //开始日期
    private String dateTo; //结束日期
    private List<StationMeasureUtil> stationMeasureUtils; //分厂站记录量测列表

    public String getDataType() {
        return dataType;
    }

    public void setDataType(String dataType) {
        this.dataType = dataType;
    }

    public String getDateFrom() {
        return dateFrom;
    }

    public void setDateFrom(String dateFrom) {
        this.dateFrom = dateFrom;
    }

    public String getDateTo() {
        return dateTo;
    }

    public void setDateTo(String dateTo) {
        this.dateTo = dateTo;
    }

    public List<StationMeasureUtil> getStationMeasureUtils() {
        return stationMeasureUtils;
    }

    public void setStationMeasureUtils(List<StationMeasureUtil> stationMeasureUtils) {
        this.stationMeasureUtils = stationMeasureUtils;
    }
}
