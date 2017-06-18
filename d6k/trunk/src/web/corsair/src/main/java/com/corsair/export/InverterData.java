package com.corsair.export;

/**
 * Created by guo_zhiqi on 2016/1/5.
 */
public class InverterData {
    private String InverterNumAndNominalPower;//逆变器编号及额定功率
    private String MPPTInverterCircuit;//逆变器MPPT回路
    private String InverterPort;//逆变器端口号
    private String PVString;//光伏串
    private String PVStringNumber;//光伏串编号
    private String current;//电流I(A)
    private String voltage;//电压V(V)
    private String inverterInPower;//输入功率P(W)
    private String inverterOutPower;//输出功率P(W)
    private String DCSideInputPower;//直流侧输入电量(kwh)
    private String power;//当日发电量（kwh)
    private String inverterEfficiency;//逆变器效率(%)
    private String effectiveNumber;//子系统当天有效小时数（h）
    private String total;//累计发电量（kwh）
    //private String remark;//备注
    private String KWhSubsidyMeter;//度电补贴电表

    private String averageInclination;//倾角均值
    private String ImpAverage;//最佳工作电流Imp(A)均值
    private String currentAverage;//最佳工作电压Vmp(V)均值
    private String powerMax;//最大输出功率Pmax(W)均值
    private String eachStringVoltage;//每个光伏串直流电压（V）
    private String eachStringCurrent;//每个光伏串直流电流（A）
    private String inverterOutVoltage;//逆变器交流输出电压（V）
    private String inverterOutCurrent;//逆变器交流输出电流（A）
    private String inverterOutputPower; //逆变器输出电量（KW.H）
    private String eachStringConversionEfficiency; //每串组件实际转换效率（%）
    private String inverterConversionEfficiency; //逆变器实际转换效率（%）
    private String EquivalentPowerHours; //等效发电小时数（h）

    public InverterData() {
    }

    public InverterData(String inverterNumAndNominalPower, String MPPTInverterCircuit, String inverterPort, String PVString, String PVStringNumber, String current, String voltage, String inverterInPower, String inverterOutPower, String DCSideInputPower, String power, String inverterEfficiency, String effectiveNumber, String total) {
        InverterNumAndNominalPower = inverterNumAndNominalPower;
        this.MPPTInverterCircuit = MPPTInverterCircuit;
        InverterPort = inverterPort;
        this.PVString = PVString;
        this.PVStringNumber = PVStringNumber;
        this.current = current;
        this.voltage = voltage;
        this.inverterInPower = inverterInPower;
        this.inverterOutPower = inverterOutPower;
        this.DCSideInputPower = DCSideInputPower;
        this.power = power;
        this.inverterEfficiency = inverterEfficiency;
        this.effectiveNumber = effectiveNumber;
        this.total = total;
    }

    public InverterData(String inverterNumAndNominalPower, String MPPTInverterCircuit, String inverterPort, String PVString, String PVStringNumber, String averageInclination, String impAverage, String currentAverage, String powerMax, String eachStringVoltage, String eachStringCurrent, String inverterOutVoltage, String inverterOutCurrent, String inverterOutputPower, String eachStringConversionEfficiency, String inverterConversionEfficiency, String equivalentPowerHours) {
        InverterNumAndNominalPower = inverterNumAndNominalPower;
        this.MPPTInverterCircuit = MPPTInverterCircuit;
        InverterPort = inverterPort;
        this.PVString = PVString;
        this.PVStringNumber = PVStringNumber;
        this.averageInclination = averageInclination;
        ImpAverage = impAverage;
        this.currentAverage = currentAverage;
        this.powerMax = powerMax;
        this.eachStringVoltage = eachStringVoltage;
        this.eachStringCurrent = eachStringCurrent;
        this.inverterOutVoltage = inverterOutVoltage;
        this.inverterOutCurrent = inverterOutCurrent;
        this.inverterOutputPower = inverterOutputPower;
        this.eachStringConversionEfficiency = eachStringConversionEfficiency;
        this.inverterConversionEfficiency = inverterConversionEfficiency;
        EquivalentPowerHours = equivalentPowerHours;
    }

    public String getKWhSubsidyMeter() {
        return KWhSubsidyMeter;
    }

    public void setKWhSubsidyMeter(String KWhSubsidyMeter) {
        this.KWhSubsidyMeter = KWhSubsidyMeter;
    }

    public String getInverterNumAndNominalPower() {
        return InverterNumAndNominalPower;
    }

    public void setInverterNumAndNominalPower(String inverterNumAndNominalPower) {
        InverterNumAndNominalPower = inverterNumAndNominalPower;
    }

    public String getMPPTInverterCircuit() {
        return MPPTInverterCircuit;
    }

    public void setMPPTInverterCircuit(String MPPTInverterCircuit) {
        this.MPPTInverterCircuit = MPPTInverterCircuit;
    }

    public String getInverterPort() {
        return InverterPort;
    }

    public void setInverterPort(String inverterPort) {
        InverterPort = inverterPort;
    }

    public String getPVString() {
        return PVString;
    }

    public void setPVString(String PVString) {
        this.PVString = PVString;
    }

    public String getPVStringNumber() {
        return PVStringNumber;
    }

    public void setPVStringNumber(String PVStringNumber) {
        this.PVStringNumber = PVStringNumber;
    }

    public String getCurrent() {
        return current;
    }

    public void setCurrent(String current) {
        this.current = current;
    }

    public String getVoltage() {
        return voltage;
    }

    public void setVoltage(String voltage) {
        this.voltage = voltage;
    }

    public String getInverterInPower() {
        return inverterInPower;
    }

    public void setInverterInPower(String inverterInPower) {
        this.inverterInPower = inverterInPower;
    }

    public String getPower() {
        return power;
    }

    public void setPower(String power) {
        this.power = power;
    }

    public String getEffectiveNumber() {
        return effectiveNumber;
    }

    public void setEffectiveNumber(String effectiveNumber) {
        this.effectiveNumber = effectiveNumber;
    }

    public String getTotal() {
        return total;
    }

    public void setTotal(String total) {
        this.total = total;
    }

    public String getAverageInclination() {
        return averageInclination;
    }

    public void setAverageInclination(String averageInclination) {
        this.averageInclination = averageInclination;
    }

    public String getImpAverage() {
        return ImpAverage;
    }

    public void setImpAverage(String impAverage) {
        ImpAverage = impAverage;
    }

    public String getCurrentAverage() {
        return currentAverage;
    }

    public void setCurrentAverage(String currentAverage) {
        this.currentAverage = currentAverage;
    }

    public String getPowerMax() {
        return powerMax;
    }

    public void setPowerMax(String powerMax) {
        this.powerMax = powerMax;
    }

    public String getEachStringVoltage() {
        return eachStringVoltage;
    }

    public void setEachStringVoltage(String eachStringVoltage) {
        this.eachStringVoltage = eachStringVoltage;
    }

    public String getEachStringCurrent() {
        return eachStringCurrent;
    }

    public void setEachStringCurrent(String eachStringCurrent) {
        this.eachStringCurrent = eachStringCurrent;
    }

    public String getInverterOutVoltage() {
        return inverterOutVoltage;
    }

    public void setInverterOutVoltage(String inverterOutVoltage) {
        this.inverterOutVoltage = inverterOutVoltage;
    }

    public String getInverterOutCurrent() {
        return inverterOutCurrent;
    }

    public void setInverterOutCurrent(String inverterOutCurrent) {
        this.inverterOutCurrent = inverterOutCurrent;
    }

    public String getInverterOutputPower() {
        return inverterOutputPower;
    }

    public void setInverterOutputPower(String inverterOutputPower) {
        this.inverterOutputPower = inverterOutputPower;
    }

    public String getEachStringConversionEfficiency() {
        return eachStringConversionEfficiency;
    }

    public void setEachStringConversionEfficiency(String eachStringConversionEfficiency) {
        this.eachStringConversionEfficiency = eachStringConversionEfficiency;
    }

    public String getInverterConversionEfficiency() {
        return inverterConversionEfficiency;
    }

    public void setInverterConversionEfficiency(String inverterConversionEfficiency) {
        this.inverterConversionEfficiency = inverterConversionEfficiency;
    }

    public String getEquivalentPowerHours() {
        return EquivalentPowerHours;
    }

    public void setEquivalentPowerHours(String equivalentPowerHours) {
        EquivalentPowerHours = equivalentPowerHours;
    }

    public String getInverterOutPower() {
        return inverterOutPower;
    }

    public void setInverterOutPower(String inverterOutPower) {
        this.inverterOutPower = inverterOutPower;
    }

    public String getDCSideInputPower() {
        return DCSideInputPower;
    }

    public void setDCSideInputPower(String DCSideInputPower) {
        this.DCSideInputPower = DCSideInputPower;
    }

    public String getInverterEfficiency() {
        return inverterEfficiency;
    }

    public void setInverterEfficiency(String inverterEfficiency) {
        this.inverterEfficiency = inverterEfficiency;
    }
}
