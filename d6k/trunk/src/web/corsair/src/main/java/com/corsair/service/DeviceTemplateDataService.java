package com.corsair.service;

import com.corsair.dao.*;
import com.corsair.entity.template.psr.AccumulateType;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.entity.SpecialChart;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

/**
 * Created by 洪祥 on 2015/9/15.
 */
@Service("deviceTemplateDataService")
public class DeviceTemplateDataService {
    @Autowired
    private DeviceTemplateDataDao deviceTemplateDataDao;

    @Autowired
    private DeviceTemplateDao deviceTemplateDao;

    @Autowired
    private AccumulateTypeDao accumulateTypeDao;

    @Autowired
    private SpecialChartDao specialChartDao;

    @Autowired
    private ConfigFileService configFileService;

    /**
     * 保存设备模板数据
     * @param deviceTemplateData
     * @return
     */
    public void saveOrUpdateDeviceTemplateData(DeviceTemplateData deviceTemplateData){
        deviceTemplateDataDao.saveOrUpdate(deviceTemplateData);
    }

    /**
     * 根据id查找模板数据实体
     * @param id
     * @return
     */
    public DeviceTemplateData getDeviceTemplateDataById(String id){
        return deviceTemplateDataDao.findById(DeviceTemplateData.class, id);
    }

    /**
     * 根据name查找模板数据实体
     * @param name
     * @return
     */
    public DeviceTemplateData getDeviceTemplateDataByName(String name){
        List<DeviceTemplateData> dataList = deviceTemplateDataDao.getDeviceTemplateData();
        for (DeviceTemplateData data : dataList){
            if (name.equals(data.getName())){
                return data;
            }
        }
        return null;
    }

    /**
     * 根据模板id查找属于该模板的模板数据
     * @param id
     * @return
     */
    public List<DeviceTemplateData> getDeviceTemplateDatas(String id){
        return deviceTemplateDataDao.getDeviceTemplateDataById(id);
    }

    /**
     * 根据描述查找模板数据列表
     * @param desc
     * @return
     */
    public List<DeviceTemplateData> getDeviceTemplateDataListByDesc(String desc){
        return deviceTemplateDataDao.getDeviceTemplateDataByDesc(desc);
    }

    /**
     * 获取所有模板描述
     * @param desc
     * @return
     */
    public List<String> getDeviceTemplateDescList(String name, String desc){
        List<DeviceTemplate> deviceTemplateList = deviceTemplateDao.getDeviceTemplate(name, desc);
        List<String> ret = new ArrayList<>();
        for (DeviceTemplate deviceTemplate : deviceTemplateList){
            ret.add(deviceTemplate.getDesc());
        }
        return ret;
    }

    //获取所有设备模板数据描述，格式为：设备模板描述+“-”+设备模板数据描述
    public List<String> getDeviceTemplateDataDescList(){
        List<DeviceTemplate> deviceTemplateList = deviceTemplateDao.getDeviceTemplate();
        List<String> ret = new ArrayList<>();
        for (DeviceTemplate deviceTemplate : deviceTemplateList){
            String deviceTemplateDesc = deviceTemplate.getDesc();
            for (DeviceTemplateData data : deviceTemplate.getDeviceTemplateDatas()){
                String dataDesc = data.getDesc();
                ret.add(deviceTemplateDesc + "-" + dataDesc);
            }
        }
        return ret;
    }

    /**
     * 获取所有模板数据类型描述
     * @param
     * @return
     */
    public List<String> getDeviceTemplateDataType(){
        List<String> ret = new ArrayList<>();
        ret.add("遥信");
        ret.add("遥测");
        ret.add("电度");
        ret.add("特殊图表");
        return ret;
    }

    /**
     * 获取所有数据变化模式
     * @param
     * @return
     */
    public List<String> getDeviceTemplateDataChangePattern(){
        List<String> ret = new ArrayList<>();
        ret.add("实采值");
        ret.add("不变化");
        ret.add("正太变化");
        ret.add("线性变化");
        return ret;
    }

    /**
     * 获取所有数据在图表中的可见性
     * @param
     * @return
     */
    public List<String> getDeviceTemplateDataInvisibleInChart(){
        List<String> ret = new ArrayList<>();
        ret.add("不设置");
        ret.add("可见");
        ret.add("不可见");
        return ret;
    }

    /**
     * 获取所有累积描述
     * @param
     * @return
     */
    public List<String> getAccumulateTypeDescList(){
        List<AccumulateType> accumulateTypes = accumulateTypeDao.getAccumulateType();
        List<String> ret = new ArrayList<>();
        ret.add("无");
        for (AccumulateType accumulateType : accumulateTypes){
            ret.add(accumulateType.getComments());
        }
        return ret;
    }

    /**
     * 根据描述获取积累类型
     * @param desc
     * @return
     */
    public AccumulateType getAccumulateTypeByDesc(String desc){
        List<AccumulateType> accumulateTypes = accumulateTypeDao.getAccumulateType();
        AccumulateType ret = null;
        if (!desc.equals("无")){
            for (AccumulateType accumulateType : accumulateTypes){
                if (accumulateType.getComments().equals(desc)){
                    ret = accumulateType;
                }
            }
        }
        return ret;
    }

    /**
     * 获取所有特殊图表描述
     * @param
     * @return
     */
    public List<String> getSpecialChartDescList(){
        List<SpecialChart> specialCharts = specialChartDao.getSpecialChart();
        List<String> ret = new ArrayList<>();
        ret.add("无");
        for (SpecialChart specialChart : specialCharts){
            ret.add(specialChart.getComments());
        }
        return ret;
    }

    /**
     * 根据描述获取特殊图表
     * @param desc
     * @return
     */
    public SpecialChart getSpecialChartByDesc(String desc){
        List<SpecialChart> specialCharts = specialChartDao.getSpecialChart();
        SpecialChart ret = null;
        if (!desc.equals("无")){
            for (SpecialChart specialChart : specialCharts){
                if (specialChart.getComments().equals(desc)){
                    ret = specialChart;
                }
            }
        }
        return ret;
    }

    /**
     * 删除模板
     * @param id
     */
    public void deleteDeviceTemplateData(String id){
        DeviceTemplateData deviceTemplateData  = this.getDeviceTemplateDataById(id);
        deviceTemplateDataDao.delete(deviceTemplateData);
    }

    /**
     * 检查数据索引是否已经存在
     * @param index,template,type
     * @return
     */
    public boolean checkIndexExist(String index,String template,String type){
        List<DeviceTemplate> deviceTemplates = deviceTemplateDao.getDeviceTemplate();
        for (DeviceTemplate deviceTemplate : deviceTemplates) {
            //首先找到指定模板
            if (deviceTemplate.getDesc().equals(template)) {
                //再获取模板下所有模板数据
                Set<DeviceTemplateData> deviceTemplateDataSet = deviceTemplate.getDeviceTemplateDatas();
                List<DeviceTemplateData> deviceTemplateDataList = new ArrayList<>();
                //接着获取所有指定数据类型的模板数据
                for (DeviceTemplateData deviceTemplateData : deviceTemplateDataSet){
                    Integer deviceTemplateDataType = configFileService.getDeviceTemplateDataIntTypeByStringType(type);
                    if (deviceTemplateData.getType() == deviceTemplateDataType){
                        deviceTemplateDataList.add(deviceTemplateData);
                    }
                }
                //最后在指定模板指定数据类型的模板数据中，检查数据索引是否存在
                for (DeviceTemplateData deviceTemplateData : deviceTemplateDataList){
                    if (deviceTemplateData.getIndex().toString().equals(index)){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    /**
     * 检查数据名是否已经存在
     * @param name
     * @return
     */
    public boolean checkNameExist(String name){
        List<DeviceTemplateData> deviceTemplateDataList = deviceTemplateDataDao.getDeviceTemplateData();
        for (DeviceTemplateData deviceTemplateData : deviceTemplateDataList) {
            if (deviceTemplateData.getName() != null){
                if (deviceTemplateData.getName().equals(name)){
                    return true;
                }
            }
        }
        return false;
    }
}
