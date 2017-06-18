package com.corsair.dao;

import com.corsair.entity.template.psr.DeviceTemplateData;
import com.rbac.common.BaseDaoSupport;
import org.springframework.stereotype.Repository;

import java.util.UUID;

/**
 * Created by 洪祥 on 15/7/15.
 */
@Repository("scadaMeasurementDao")
public class ScadaMeasurementDao extends BaseDaoSupport {
    //todo 暂时就先生成个随机字符串用，要和3000的数据库调试
    public String getMeasurementID(String PSRID, DeviceTemplateData deviceTemplateData) {
        return UUID.randomUUID().toString();
    }
}
