package com.corsair.entity.template.psr;

/**
 * Created by 洪祥 on 16/2/29.
 * 数据的单位,包含普通的单位和图表中的单位,普通单位可以包含HTML标签,图表单位不能(可以是中文),因为图表不能处理HTML标签
 */
public class UnitType implements java.io.Serializable {
    private String id;
    private String comments;
    private String dataUnit;
    private String chartUnit;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getComments() {
        return comments;
    }

    public void setComments(String comments) {
        this.comments = comments;
    }

    public String getDataUnit() {
        return dataUnit;
    }

    public void setDataUnit(String dataUnit) {
        this.dataUnit = dataUnit;
    }

    public String getChartUnit() {
        return chartUnit;
    }

    public void setChartUnit(String chartUnit) {
        this.chartUnit = chartUnit;
    }
}
