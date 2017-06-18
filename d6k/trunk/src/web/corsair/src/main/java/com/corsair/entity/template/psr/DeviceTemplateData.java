package com.corsair.entity.template.psr;

import com.corsair.entity.template.display.DisplayTemplateData;
import com.corsair.entity.SpecialChart;
import com.google.common.base.Strings;

import java.util.LinkedList;
import java.util.List;

/**
 * Created by 洪祥 on 15/7/13.
 * 设备模板的具体数据
 */
public class DeviceTemplateData implements java.io.Serializable {

    private String id;
    private Integer type;  //类型，遥测、遥信、电度、特殊图表
    private Integer index;
    private Boolean calculate; //是否是计算量
    private String expression; //计算量表达式
    private String name;
    private String desc;

    private Boolean invisibleInChart; //图表列表中不可见
    private DeviceTemplate deviceTemplate;
    private AccumulateType accumulateType;
    private SpecialChart specialChart;
    private List<DisplayTemplateData> displayTemplateDatas = new LinkedList<DisplayTemplateData>();

    private MagicType magicType;
    private UnitType unitType;

    public MagicType getMagicType() {
        return magicType;
    }

    public void setMagicType(MagicType magicType) {
        this.magicType = magicType;
    }

    public UnitType getUnitType() {
        return unitType;
    }

    public void setUnitType(UnitType unitType) {
        this.unitType = unitType;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public Integer getType() {
        return type;
    }

    public void setType(Integer type) {
        this.type = type;
    }

    public Integer getIndex() {
        return index;
    }

    public void setIndex(Integer index) {
        this.index = index;
    }

    public String getDesc() {
        return desc;
    }

    public void setDesc(String desc) {
        this.desc = desc;
    }

    public DeviceTemplate getDeviceTemplate() {
        return deviceTemplate;
    }

    public void setDeviceTemplate(DeviceTemplate deviceTemplate) {
        this.deviceTemplate = deviceTemplate;
    }

    public List<DisplayTemplateData> getDisplayTemplateDatas() {
        return displayTemplateDatas;
    }

    public void setDisplayTemplateDatas(List<DisplayTemplateData> displayTemplateDatas) {
        this.displayTemplateDatas = displayTemplateDatas;
    }

    public AccumulateType getAccumulateType() {
        return accumulateType;
    }

    public void setAccumulateType(AccumulateType accumulateType) {
        this.accumulateType = accumulateType;
    }

    public SpecialChart getSpecialChart() {
        return specialChart;
    }

    public void setSpecialChart(SpecialChart specialChart) {
        this.specialChart = specialChart;
    }

    public Boolean getInvisibleInChart() {
        return invisibleInChart;
    }

    public void setInvisibleInChart(Boolean invisibleInChart) {
        this.invisibleInChart = invisibleInChart;
    }

    public Boolean getCalculate() {
        return calculate;
    }

    public void setCalculate(Boolean calculate) {
        this.calculate = calculate;
    }

    public String getExpression() {
        return expression;
    }

    public void setExpression(String expression) {
        this.expression = expression;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getMagic() {
        return magicType == null ? null : magicType.getName();
    }

    public String getUnit() {
        return unitType == null ? "" : unitType.getDataUnit();
    }

    public String getChartUnit() {
        String ret;

        if (unitType == null) {
            ret = "";
        } else if (!Strings.isNullOrEmpty(unitType.getChartUnit())) {
            ret = unitType.getChartUnit();
        } else {
            ret = unitType.getDataUnit();
        }

        return ret;
    }
}
