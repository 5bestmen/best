package com.corsair.service;

import com.corsair.dao.StationLocationDao;
import com.corsair.entity.StationLocation;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * Created by chris on 2015/9/15.
 */
@Service("stationLocationService")
public class StationLocationService {
    @Autowired
    private StationLocationDao stationlocationdao;

    public List<StationLocation> getAllStationInfo() {
        List<StationLocation> stl = stationlocationdao.getStationList();
        return stl.isEmpty() ? null : stl;
    }

    public StationLocation getStationInfoByName(String stname) {
        StationLocation st = stationlocationdao.getStationInfoByName(stname);
        return st;
    }

    public List<StationLocation> getStationInfoByCity(String city) {
        List<StationLocation> stl = stationlocationdao.getStationByCity(city);
        return stl.isEmpty() ? null : stl;
    }
}
