package com.corsair.dao;

import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Restrictions;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowCallbackHandler;
import org.springframework.stereotype.Repository;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

/**
 * Created by 洪祥 on 15/9/16.
 */
@Repository("deviceTemplateDataDao")
public class DeviceTemplateDataDao extends BaseDaoSupport {

    @Autowired
    private JdbcTemplate jdbcTemplate;
    @Autowired DeviceTemplateDao deviceTemplateDao;

    public List<DeviceTemplateData> getDeviceTemplateData() {
        Criteria criteria = super.currentSession().createCriteria(DeviceTemplateData.class);

        return criteria.list();
    }

    /**
     * 根据设备模板名和设备模板描述查找设备模板
     * @param desc
     * @return
     */
    public List<DeviceTemplateData> getDeviceTemplateDataByDesc(String desc) {
        Criteria criteria = super.currentSession().createCriteria(DeviceTemplateData.class);
        if(CommonUtils.isNotEmpty(desc)){
            criteria.add(Restrictions.ilike("desc", desc, MatchMode.ANYWHERE));
        }
        return criteria.list();
    }

    /**
     * 根据设备模板ID查找归属于它的设备模板数据列表
     * @param id
     * @return
     */
    public List<DeviceTemplateData> getDeviceTemplateDataById(String id){
        DeviceTemplate deviceTemplate = deviceTemplateDao.findById(id);
        List<DeviceTemplateData> ret = new ArrayList<>();
        Set<DeviceTemplateData> set = deviceTemplate.getDeviceTemplateDatas();
        for(DeviceTemplateData i : set){
            ret.add(i);
        }
        return ret;
    }

    /**
     * jdbc方式获取设备模板数据列表
     * @param id
     * @return
     */
//    public List<DeviceTemplateData> getDeviceTemplateDatas(String id){
//        final List<DeviceTemplateData> deviceTemplateDatas = new ArrayList<DeviceTemplateData>();
//        String sql = "select * from devicetemplatedata where devicetemplate = '" + id +"'";
//
//        try {
//            jdbcTemplate.query(sql, new RowCallbackHandler() {
//                public void processRow(ResultSet resultSet) throws SQLException {
//                    DeviceTemplateData deviceTemplateData = new DeviceTemplateData();
//                    deviceTemplateData.setId(resultSet.getString("id").trim());
//                    deviceTemplateData.setType(Integer.parseInt(resultSet.getString("type").trim()));
//                    deviceTemplateData.setIndex(Integer.parseInt(resultSet.getString("indexnumber").trim()));
//                    deviceTemplateData.setDesc(resultSet.getString("comments"));
//                    if(resultSet.getString("unit") != null){
//                        deviceTemplateData.setUnitType(resultSet.getString("unit"));
//                    }
//                    if(resultSet.getString("defaultvalue") != null){
//                        deviceTemplateData.setDefaultValue(Double.parseDouble(resultSet.getString("defaultvalue")));
//                    }
//                    if(resultSet.getString("maxvalue") != null){
//                        deviceTemplateData.setMaxValue(Double.parseDouble(resultSet.getString("maxvalue")));
//                    }
//                    deviceTemplateDatas.add(deviceTemplateData);
//                }
//            });
//
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
//
//        return deviceTemplateDatas;
//    }
}
