package com.corsair.dao;

import com.corsair.device.PVPowerStation;
import com.corsair.device.PhotovoltaicArray;
import com.corsair.entity.template.psr.PSRTypeConstant;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowCallbackHandler;
import org.springframework.stereotype.Repository;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by zjq on 2015/7/22.
 */
@Repository("stationDao")
public class StationDao {
    @Autowired
    private JdbcTemplate jdbcTemplate;

    public PVPowerStation GetPVStation(){
        final List<PVPowerStation> pvPowerStations = new ArrayList<PVPowerStation>();
        String sql = "select 代码,描述 from 站所线系参数表 where 描述='系统站'";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    PVPowerStation pvPowerStation = new PVPowerStation();
                    pvPowerStation.setId(resultSet.getString("代码").trim());
                    pvPowerStation.setName(resultSet.getString("描述").trim());
                    pvPowerStations.add(pvPowerStation);
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        return pvPowerStations.isEmpty() ? null : pvPowerStations.get(0);
    }

    public List<PhotovoltaicArray> getPhotovoltaics(){
        final List<PhotovoltaicArray> photovoltaics = new ArrayList<PhotovoltaicArray>();
        String sql = String.format("select 代码,描述 from 站所线系参数表 where 类型 = %1$d", PSRTypeConstant.ARRAY) ;

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    PhotovoltaicArray photovoltaic = new PhotovoltaicArray();
                    photovoltaic.setId(resultSet.getString("代码").trim());
                    photovoltaic.setName(resultSet.getString("描述").trim());
                    photovoltaics.add(photovoltaic);
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        return photovoltaics;
    }

    public String getPVArrayName(String arrayID) {
        final List<String> names = new ArrayList<String>();
        String sql = "select 描述 from 站所线系参数表 where 代码='" + arrayID + "'";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    names.add(resultSet.getString("描述").trim());
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return names.isEmpty() ? "":names.get(0);
    }
}
