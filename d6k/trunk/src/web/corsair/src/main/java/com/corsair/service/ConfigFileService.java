package com.corsair.service;

import com.corsair.dao.ConfigDao;
import com.corsair.entity.config.DeviceTemplateDataType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 2015/12/17.
 */
@Service
public class ConfigFileService {
    @Autowired
    ConfigDao configDao;

    public List<DeviceTemplateDataType> getDeviceTemplateDataTypes() {
        return configDao.getDeviceTemplateDataConfig().getDeviceTemplateDataTypes();
    }

    /**
     * 获取所有设备模板数据类型的字符串表达形式
     * @param
     * @return
     */
    public List<String> getDeviceTemplateDataStringTypes(){
        List<DeviceTemplateDataType> typeList = getDeviceTemplateDataTypes();
        List<String> stringList = new ArrayList<>();
        for (DeviceTemplateDataType type : typeList){
            stringList.add(type.getTypeString());
        }
        return stringList;
    }

    /**
     * 通过设备模板数据字符串类型获取整形类型
     * @param intType
     * @return
     */
    public String getDeviceTemplateDataStringTypeByIntType(Integer intType){
        List<DeviceTemplateDataType> typeList = getDeviceTemplateDataTypes();
        for(DeviceTemplateDataType type : typeList){
            if(Integer.parseInt(type.getTypeInt()) == intType){
                return type.getTypeString();
            }
        }

        return null;
    }

    /**
     * 通过设备模板数据整形类型获取字符串类型
     * @param stringType
     * @return
     */
    public Integer getDeviceTemplateDataIntTypeByStringType(String stringType){
        List<DeviceTemplateDataType> typeList = getDeviceTemplateDataTypes();
        for(DeviceTemplateDataType type : typeList){
            if(type.getTypeString().equals(stringType)){
                return Integer.parseInt(type.getTypeInt());
            }
        }

        return null;
    }

}
