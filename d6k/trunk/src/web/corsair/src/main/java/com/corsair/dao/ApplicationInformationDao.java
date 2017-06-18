package com.corsair.dao;

import com.corsair.entity.ApplicationInformation;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.Criteria;
import org.hibernate.criterion.Restrictions;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by 洪祥 on 15/7/1.
 */

@Repository("applicationInformationDao")
public class ApplicationInformationDao extends BaseDaoSupport {
    @Autowired
    JdbcTemplate jdbcTemplate;
    public ApplicationInformationDao() {
    }
    public String getAppName() {
        return getValue("APP_NAME");
    }
    public String getMenuWay(){
        return  getValue("MENU_WAY");
    }
    public String getVersion() {
        return getValue("APP_VERSION");
    }

    public String getCenterPoint_long() {return getValue("CENTER_POINT_LONGITUDE");}

    public String getCenterPoint_lat() {return getValue("CENTER_POINT_LATITUDE");}

    public String getMapZoom() { return getValue("ZOOM"); }

    public String getZoomFlag() {return getValue("ZOOMABLE"); }

    public String getDragFlag() {return getValue("DRAGABLE"); }

    public String getStationName() {return getValue("STATION_NAME");}
    public String getStationCapacity() {return getValue("STATION_CAPACITY");}
    public String getPowerGridCapacity() {return getValue("POWER_GRID_CAPACITY");}
    public String getRunningTime() {return getValue("RUNNING_TIME");}

    private String getValue(String key) {
        Criteria criteria = super.currentSession().createCriteria(ApplicationInformation.class);
        criteria.add(Restrictions.eq("key", key));

        List list = criteria.list();

        if (list.isEmpty()) {
            return null;
        } else {
            ApplicationInformation information = (ApplicationInformation) list.get(0);
            return information.getValue();
        }

    }
}
