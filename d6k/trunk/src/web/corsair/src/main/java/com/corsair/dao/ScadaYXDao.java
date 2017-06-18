package com.corsair.dao;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowCallbackHandler;
import org.springframework.stereotype.Repository;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 15/7/14.
 */
@Repository("scadaYXDao")
public class ScadaYXDao {
    @Autowired
    private JdbcTemplate jdbcTemplate;
    //用这种SQL取出的值，除非测点序号有重复，否则都只有一个值
    /*private final String normalYXSQL = "select 代码 from 遥信参数表 where 设备代码='%1$s' " +
            "and 遥信序号=((select min(遥信序号) from 遥信参数表 where 设备代码='%1$s') + %2$d)";*/

    // select DI_TAG from SCD_DI where DEVICE_ID='51' and FIRST_NUMBER=((select min(FIRST_NUMBER) from SCD_DI where DEVICE_ID='51') + 33)
    private final String normalYXSQL = "select DI_TAG from SCD_DI where DEVICE_ID='%1$s' " +
            "and FIRST_NUMBER=((select min(FIRST_NUMBER) from SCD_DI where DEVICE_ID='%1$s') + (%2$d-1))";

    private final String junctionYXSQL = "select 代码 from 遥信参数表 where （设备代码='%1$s' " +
            "or 设备代码 in (select 代码 from scd设备参数表 where 所属馈线='%1$s')） " +
            "and 遥信序号=(select min(遥信序号) + %2$d from 遥信参数表 where 设备代码='%1$s' " +
            "or 设备代码 in (select 代码 from scd设备参数表 where 所属馈线='%1$s'))";

    public String GetMeasureID(int YxIndex, String psrID, boolean isJunction){
        final List<String> measureID = new ArrayList<>();
        String sql;

        if (!isJunction){
            sql = String.format(normalYXSQL, psrID, YxIndex);
        }else{
            sql = String.format(junctionYXSQL, psrID, YxIndex);
        }

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                        measureID.add(resultSet.getString("DI_TAG").trim());
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        return measureID.isEmpty() ? "" : measureID.get(0);
    }
}
