package com.corsair.service;

import com.corsair.dao.*;
import com.corsair.device.*;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.PSRTypeConstant;
import com.corsair.measurement.MeasurementInfo;
import com.rbac.common.PagerOfAlarm;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Vector;

/**
 * Created by 洪祥 on 15/7/22.
 */
@Service("scadaPSRService")
public class ScadaPSRService {
    @Autowired
    ScdDeviceDao scdDeviceDao;
    @Autowired
    DeviceInfoDao deviceInfoDao;
    @Autowired
    StationDao stationDao;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;

    //获取光伏电站
    public PVPowerStation getPVStation() {
        return stationDao.GetPVStation();
    }

    public WeatherInstrument getWeatherInstrument() {
        return scdDeviceDao.GetWeatherInstrument();
    }

    /**
     * 获取厂站
     * @return 厂站列表
     */
    public List getStations() {
        return scdDeviceDao.getStations();
    }

    /**
     * 获取日报表历史数据
     * @return 日报表数据List
     */
    public List<HisDayValueData> getHisDayValueData(String strStationID, String strDeviceID, String strDate) {
        return scdDeviceDao.getDayReportHistoryData(strStationID, strDeviceID, strDate);
    }

    public List<List<String>> getHisDayValueDataOf5Minute(String strStationID, String strDeviceID, String strDate) {
        return scdDeviceDao.getDataOf5Minute(strStationID, strDeviceID, strDate);
    }

    /**
     * 获取月报表历史数据
     * @return 月报表数据Vector
     */
    public Vector<HisMonthValueData> getHisMonthValueData(String strStationID, String strDeviceID, String strDate) {

        return scdDeviceDao.getMonthReportHistoryData(strStationID, strDeviceID, strDate);
    }

    /**
     * 获取告警记录
     * @return 告警记录数据List
     */
    public PagerOfAlarm searchAlarmRecordDay(String strStationID, String strDeviceID, String strDate, String strAlarmType, String strCurrentPage, String strLevel, String strChooseName) {
        return scdDeviceDao.getAlarmPagerOfDay(strStationID, strDeviceID, strDate, strAlarmType, strCurrentPage, strLevel, strChooseName);
    }

    /**
     * 获取告警记录
     * @return 告警记录数据List
     */
    public List<String> getDeviceDesc(String strStationID, String strDeviceID) {
        return scdDeviceDao.getDeviceDesc(strStationID, strDeviceID);
    }

    /**
     * 获取告警记录
     * @return 告警记录数据List
     */
    public PagerOfAlarm getAlarmRecordInterval(String strStationID, String strDeviceID, String strBeginDate, String strEndDate, String strAlarmType, String strCurrentPage, String strLevel, String strChooseName) {
        return scdDeviceDao.getAlarmPagerOfInterval(strStationID, strDeviceID, strBeginDate, strEndDate, strAlarmType, strCurrentPage, strLevel, strChooseName);
    }

    public List<His_AI_Day_Display> getHistoryDataOfDay(String strStationID, String strDeviceID, String strDate, String strAlarmType) {
        return scdDeviceDao.getHistoryDataOfDay(strStationID, strDeviceID, strDate, strAlarmType);
    }

//    /**
//     * 获取报表导出历史数据
//     * @return 报表
//     */
//    public Integer getreportExportData() {
//        return scdDeviceDao.getreportExportData();
//
//    }

    /**
     * 获取设备信息
     * @return 设备信息列表
     */
    public DeviceInfo getDeviceInfo(String strStationID, String strDeviceID) {
        return deviceInfoDao.getDeviceInfo(strStationID, strDeviceID);
    }

    public List<AiInfo> getAiInfo(String strStationID, String strDeviceID) {
        return deviceInfoDao.getAiInfo(strStationID, strDeviceID);
    }

    public List<DiInfo> getDiInfo(String strStationID, String strDeviceID) {
        return deviceInfoDao.getDiInfo(strStationID, strDeviceID);
    }

    public List<CalcInfo> getCalcInfo(String strStationID, String strDeviceID) {
        return deviceInfoDao.getCalcInfo(strStationID, strDeviceID);
    }

    public Total_YMD_Info getTotalCalcInfo(String strName, String strType, String strDate) {
        return deviceInfoDao.getTotalCalcInfo(strName, strType, strDate);
    }

    public Total_YMD_Info getDeviceCalcInfo(String strDate, String strStationID, String strDeviceID) {
        return deviceInfoDao.getDeviceCalcInfo(strDate, strStationID, strDeviceID);
    }

    public DeviceRunningInfo getDeviceRunningInfo(String strName, String strType) {
        return deviceInfoDao.getDeviceRunningInfo(strName, strType);
    }

    /**
     * 获取子站的所有设备,不顾及层次关系
     * @param subStationID 子站ID
     * @return 子设备列表
     */
    public List getDeviceOfStation(String subStationID) {
        return scdDeviceDao.getDeviceOfStation(subStationID);
    }

    /**
     * 获取设备的遥测信息
     * @param DeviceID 设备ID
     * @return 设备遥测列表
     */
    public List getAIsofDevice(String StationID,String DeviceID) { return scdDeviceDao.getAIsofDevice(StationID,DeviceID); }


    /**
     * 获取设备的电度信息
     * @param DeviceID 设备ID
     * @return 设备电度量列表
     */
    public List getKWHsofDevice(String StationID,String DeviceID) { return scdDeviceDao.getKWHsofDevice(StationID,DeviceID); }

    /**
     * 获取设备的计算量信息
     * @param DeviceID 设备ID
     * @return 设备计算量列表
     */
    public List getCALCsofDevice(String StationID,String DeviceID) { return scdDeviceDao.getCALCsofDevice(StationID,DeviceID); }


    /**
     * 根据设备ID获取设备模板
     * @param id 设备ID
     * @return 设备模板,没有返回空
     */
    public DeviceTemplate getDeviceTemplateByPSRID(String id) {
        return psrTemplateMappingDao.getDeviceTemplateByPSRID(id);
    }

    /**
     * 获取光伏阵列或分布式厂站
     * @return 阵列或分布式厂站列表
     */
    public List getPVArray() {
        return scdDeviceDao.getArray();
    }


    //获取一个光伏阵列下的所有设备，以箱变、逆变器、汇流箱排列
//    public List<BasePSR> getPVArrayDevice(String arrayID) {
//        return scdDeviceDao.GetPVArrayDevices(arrayID);
//    }

    //获取一个光伏阵列的箱变
    public PackageTransformer getPackageTransformer(String arrayID) {
        return scdDeviceDao.GetPackageTransformer(arrayID);
    }

    /**
     * 获取分布式厂站下的电表
     * @param arrayID 阵列ID
     * @return 电表名称
     */
    public List getAmmeter(String arrayID) {
        return scdDeviceDao.getPSRFromArray(arrayID, PSRTypeConstant.METER);
    }

    /**
     * 获取一个阵列或分布式厂站的所有逆变器
     * @param arrayID 阵列或分布式厂站的ID
     * @return 列表
     */
    public List getInverter(String arrayID) {
        return scdDeviceDao.getPSRFromArray(arrayID, PSRTypeConstant.INVERTER);
    }

    /**
     * 获取一个阵列或分布式厂站的所有屋顶
     * @param arrayID 阵列或分布式厂站的ID
     * @return 列表
     */
    public List getRoof(String arrayID) {
        return scdDeviceDao.getPSRFromArray(arrayID, PSRTypeConstant.ROOF);
    }

    /**
     * 获取一个设备下挂的子设备
     * @param psrID 设备ID
     * @param subType 子设备类型
     * @return 列表
     */
    public List getChildren(String psrID, Integer subType) {
        return scdDeviceDao.getChildren(psrID, subType);
    }

    //获取一个光伏阵列的汇流箱
    public List getJunction(String arrayID) {
        return scdDeviceDao.getPSRFromArray(arrayID, PSRTypeConstant.JUNCTION);
    }

    //获取具体具体设备的所属阵列
    public String getPVArrayId(String id){
        return scdDeviceDao.GetPVArray(id);
    }

    //获取一个光伏阵列的汇流箱矩阵
    public List<MeasurementInfo> getMatrixs(String arrayID) { return scdDeviceDao.getMatrixs(arrayID);
    }

    public String getPVArrayName(String arrayID) {
        return stationDao.getPVArrayName(arrayID);
    }

    public String getDeivceName(String id) {
        return scdDeviceDao.getDeviceName(id);
    }

    //获取一个逆变器的汇流箱
    public List<Junction> getChildJunction(String id) { return scdDeviceDao.GetChildJunctions(id);
    }
    //获取一个逆变器相关联的电表
    public List<PackageTransformer> getChildTransformers(String id) { return scdDeviceDao.GetChildTransformers(id);
    }

    //获取厂站的电度代码和设备代码,用于报表显示200类型的有功电度;
    public String getEnergyId(String id){return  scdDeviceDao.getEnergyId(id);}

    //获取指定汇流箱的总电流代码
    public String getIformJunction(String id){return scdDeviceDao.getIfromJunction(id);}

    //获取指定汇流箱的电压代码
    public String getUformJunction(String id){return scdDeviceDao.getUfromJunction(id);}

    //获取一个逆变器的汇流箱
    public List<BasePSR> getScdDevices(String id, int type) { return scdDeviceDao.getScdDevices(id, type);
    }
}
