package com.corsair.device;

/**
 * Created by 洪祥 on 15/7/27.
 */
public class ChartDataType {
    private Integer type;
    private String name;
    private Integer index;
    private String magic;
    private String chartName;

    public ChartDataType(Integer type, String name, Integer index, String magic, String chartName) {
        this.type = type;
        this.name = name;
        this.index = index;
        this.magic = magic;
        this.chartName = chartName;
    }

    public String getChartName() {
        return chartName;
    }

    public void setChartName(String chartName) {
        this.chartName = chartName;
    }

    public Integer getType() {
        return type;
    }

    public void setType(Integer type) {
        this.type = type;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getIndex() {
        return index;
    }

    public void setIndex(Integer index) {
        this.index = index;
    }

    public String getMagic() {
        return magic;
    }

    public void setMagic(String magic) {
        this.magic = magic;
    }
}
