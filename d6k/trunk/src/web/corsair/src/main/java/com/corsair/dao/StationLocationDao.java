package com.corsair.dao;

import com.corsair.entity.StationLocation;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.criterion.Restrictions;
import org.hibernate.Criteria;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by chris on 2015/9/15.
 */
@Repository("stationlocationDao")
public class StationLocationDao extends BaseDaoSupport{
    public List<StationLocation> getStationList() {
        Criteria cr = super.currentSession().createCriteria(StationLocation.class);
        return cr.list();
    }

    public StationLocation getStationInfoByName(String stname) {
        Criteria cr = super.currentSession().createCriteria(StationLocation.class);
        cr.add(Restrictions.eq("station_name",stname));

        List list = cr.list();

        return list.isEmpty() ? null : (StationLocation)list.get(0);
    }

    public List<StationLocation> getStationByCity(String city){
        Criteria cr = super.currentSession().createCriteria(StationLocation.class);
        cr.add(Restrictions.eq("station_city", city));

        return cr.list();
    }
}
