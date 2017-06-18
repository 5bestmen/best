package com.corsair.measurement;

import java.util.List;

/**
 * Created by zyj on 2017/3/3.
 */
public class YMD_StatisticsInfo {

    public List<HistoryDataValue> getListYear() {
        return listYear;
    }

    public void setListYear(List<HistoryDataValue> listYear) {
        this.listYear = listYear;
    }

    public List<HistoryDataValue> getListMonth() {
        return listMonth;
    }

    public void setListMonth(List<HistoryDataValue> listMonth) {
        this.listMonth = listMonth;
    }

    public List<HistoryDataValue> getListDay() {
        return listDay;
    }

    public void setListDay(List<HistoryDataValue> listDay) {
        this.listDay = listDay;
    }

    private List<HistoryDataValue> listYear;
    private List<HistoryDataValue> listMonth;
    private List<HistoryDataValue> listDay;
}
