package com.corsair.service;

import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.dao.ScadaKWHDao;
import com.corsair.dao.ScadaYCDao;
import com.corsair.dao.ScadaYXDao;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import org.apache.commons.lang3.tuple.ImmutablePair;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.Objects;

/**
 * Created by 洪祥 on 15/7/14.
 * 获取量测代码服务
 */
@Service("scadaMeasurementService")
public class ScadaMeasurementService {
    @Autowired
    ScadaYXDao scadaYXDao;
    @Autowired
    ScadaYCDao scadaYCDao;
    @Autowired
    ScadaKWHDao scadaKWHDao;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;

    private static final String JUNCTION_TYPE = "junction";

    /**
     * 获取量测代码
     * @param PSRID 对象ID
     * @param deviceTemplateData 设备模板数据
     * @return 量测代码,找不到为null
     */
    public String getMeasurementID(String PSRID, DeviceTemplateData deviceTemplateData) {

        int measureType = deviceTemplateData.getType();
        int index = deviceTemplateData.getIndex();
        String psrType = deviceTemplateData.getDeviceTemplate().getPSRType();
        String retMeasureID = null;

        if (deviceTemplateData.getMagic() != null || (deviceTemplateData.getCalculate() != null && deviceTemplateData.getCalculate().equals(Boolean.TRUE))) {
            return "";
        }

        switch (measureType){
            case 1:
                retMeasureID = scadaYXDao.GetMeasureID(index,PSRID,JUNCTION_TYPE.equals(psrType));
                break;
            case 2:
                retMeasureID = scadaYCDao.GetMeasureID(index,PSRID,JUNCTION_TYPE.equals(psrType));
                break;
            case 3:
                retMeasureID = scadaKWHDao.GetMeasureID(index,PSRID,JUNCTION_TYPE.equals(psrType));
                break;
            default:
                break;
        }

        return retMeasureID;
    }

    /**
     * 获取量测代码
     * @param psrID 对象ID
     * @param measureType 量测类型,1遥信\2遥测\3电度
     * @param index 在模板中的索引
     * @return 量测代码,找不到为null
     */
    public String getMeasurementID(String psrID, Integer measureType, Integer index) {
        String retMeasureID = null;

        switch (measureType) {
            case 1:
                retMeasureID = scadaYXDao.GetMeasureID(index, psrID, false);
                break;
            case 2:
                retMeasureID = scadaYCDao.GetMeasureID(index, psrID, false);
                break;
            case 3:
                retMeasureID = scadaKWHDao.GetMeasureID(index, psrID, false);
                break;
            default:
                break;
        }

        return retMeasureID;
    }

    /**
     * 根据对象ID和数据名称获取量测代码和类型
     * @param psrID 对象ID
     * @param name 量测名称
     * @return 量测代码好类型的对,找不到返回NULL
     */
    public ImmutablePair<String, Integer> getMeasurementID(String psrID, String name) {
        DeviceTemplate template = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrID);

        if (template == null) {
            return null;
        }

        DeviceTemplateData data = template.getDeviceTemplateDataByName(name);

        if (data == null) {
            return null;
        }

        String retMeasureID = null;

        switch (data.getType()) {
            case 1:
                retMeasureID = scadaYXDao.GetMeasureID(data.getIndex(), psrID, false);
                break;
            case 2:
                retMeasureID = scadaYCDao.GetMeasureID(data.getIndex(), psrID, false);
                break;
            case 3:
                retMeasureID = scadaKWHDao.GetMeasureID(data.getIndex(), psrID, false);
                break;
            default:
                break;
        }

        if (retMeasureID == null) {
            return null;
        }

        return new ImmutablePair<>(retMeasureID, data.getType());
    }
}
