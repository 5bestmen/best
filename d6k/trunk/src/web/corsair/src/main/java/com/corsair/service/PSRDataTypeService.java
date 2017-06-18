package com.corsair.service;

import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.device.ChartDataType;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 15/7/27.
 */
@Service("psrDataTypeService")
public class PSRDataTypeService{
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;

    public List<ChartDataType> getChartDataType(String psrID, String dateType) {
        List<ChartDataType> chartDataTypes = new ArrayList<>();
        DeviceTemplate template = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrID);

        if (template == null) {
            return chartDataTypes;
        }

        //日数据，包含特殊图表、日累计和类型2的遥测量
        if (dateType.equals("day")) {
            for (DeviceTemplateData data : template.getDeviceTemplateDatas()) {
                if (data.getType().equals(4) && data.getSpecialChart() != null && !Boolean.TRUE.equals(data.getInvisibleInChart())) {
                    chartDataTypes.add(new ChartDataType(4, data.getDesc(), data.getIndex(), data.getMagic(), data.getSpecialChart().getName()));
                }

            }
            for (DeviceTemplateData data : template.getDeviceTemplateDatas()) {
                if (data.getType().equals(2) && !Boolean.TRUE.equals(data.getInvisibleInChart())) {
                    chartDataTypes.add(new ChartDataType(2, data.getDesc(), data.getIndex(), data.getMagic(), null));
                }
            }

            for (DeviceTemplateData data : template.getDeviceTemplateDatas()) {
                if (data.getType().equals(3) && (data.getAccumulateType() != null && dateType.equals(data.getAccumulateType().getName()))) {
                    chartDataTypes.add(new ChartDataType(3, data.getDesc(), data.getIndex(), data.getMagic(), null));
                }
            }

        } else {
            //其它就是月、年、累计这些数据了
            for (DeviceTemplateData data : template.getDeviceTemplateDatas()) {
                if (data.getType().equals(3) && data.getAccumulateType() != null && dateType.equals(data.getAccumulateType().getName())) {
                    chartDataTypes.add(new ChartDataType(3, data.getDesc(), data.getIndex(), data.getMagic(), null));
                }
            }
        }

        return chartDataTypes;
    }

}
