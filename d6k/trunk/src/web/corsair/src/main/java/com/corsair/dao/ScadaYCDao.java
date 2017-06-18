package com.corsair.dao;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowCallbackHandler;
import org.springframework.stereotype.Repository;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.*;

/**
 * Created by 洪祥 on 15/7/14.
 */
@Repository("scadaYCDao")
public class ScadaYCDao {
    @Autowired
    private JdbcTemplate jdbcTemplate;

    //用这种SQL取出的值，除非测点序号有重复，否则都只有一个值
    /*private final String normalYCSQL = "select 代码 from 遥测参数表 where 设备代码='%1$s' " +
            "and 遥测序号=((select min(遥测序号) from 遥测参数表 where 设备代码='%1$s') + %2$d)";*/
    private final String normalYCSQL = "select STATION_ID,DEVICE_ID,AI_ID from SCD_AI where DEVICE_ID='%1$s' " +
            "and YC_NUMBER=((select min(YC_NUMBER) from SCD_AI where DEVICE_ID='%1$s') + (%2$d-1))";

    private final String junctionYCSQL = "select 代码 from 遥测参数表 where （设备代码='%1$s' " +
            "or 设备代码 in (select 代码 from scd设备参数表 where 所属馈线='%1$s')） " +
            "and 遥测序号=(select min(遥测序号) + %2$d from 遥测参数表 where 设备代码='%1$s' " +
            "or 设备代码 in (select 代码 from scd设备参数表 where 所属馈线='%1$s'))";

    public String GetMeasureID(int YcIndex, String psrID, boolean isJunction){
        final List<String> measurements = new ArrayList<>();
        String sql;

        if (!isJunction){
            sql = String.format(normalYCSQL, psrID, YcIndex);
        }else{
            sql = String.format(junctionYCSQL, psrID, YcIndex);
        }

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    String strStationID = resultSet.getString("STATION_ID");
                    String strDeviceID = resultSet.getString("DEVICE_ID");
                    String strID = resultSet.getString("AI_ID");

                    String strMeasurements = "";
                    String deviceID = "53";
                    Boolean res = strDeviceID.equals(deviceID);

                    if (res) {
                        strMeasurements = strStationID + "-" + strDeviceID + "-" + "47" + "-" + "0200";
                    } else {
                        strMeasurements = strStationID + "-" + strDeviceID + "-" + strID + "-" + "0200";
                    }
                    measurements.add(strMeasurements);
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        return measurements.isEmpty() ? "" : measurements.get(0);
    }
}

