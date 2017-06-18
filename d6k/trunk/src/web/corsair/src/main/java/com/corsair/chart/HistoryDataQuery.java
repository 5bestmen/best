package com.corsair.chart;

import java.util.ArrayList;
import java.util.List;

public class HistoryDataQuery {
    private String dateType;//年月日
    private List<HistoryData> templates = new ArrayList<>();//测点名称
    private String dateFrom; //开始日期
    private String dateTo; //结束日期

    public HistoryDataQuery() {
    }

    public String getDateType() {
        return dateType;
    }

    public void setDateType(String dateType) {
        this.dateType = dateType;
    }

    public List<HistoryData> getTemplates() {
        return templates;
    }

    public void setTemplates(List<HistoryData> templates) {
        this.templates = templates;
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

}
