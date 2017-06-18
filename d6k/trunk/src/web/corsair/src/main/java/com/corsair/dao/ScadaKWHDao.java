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
@Repository("scadaKWHDao")
public class ScadaKWHDao {
    @Autowired
    private JdbcTemplate jdbcTemplate;

    //用这种SQL取出的值，除非测点序号有重复，否则都只有一个值
    private final String normalKWHSQL = "select 代码 from 电度参数表 where 设备代码='%1$s' " +
            "and 电度序号=((select min(电度序号) from 电度参数表 where 设备代码='%1$s') + %2$d)";
    /*private final String normalKWHSQL = "select DD_TAG from SCD_DD where 设备代码='%1$s' " +
            "and DD_NUMBER=((select min(DD_NUMBER) from SCD_DD where DEVICE_ID='%1$s') + %2$d)";*/

    private final String junctionKWHSQL = "select 代码 from 电度参数表 where （设备代码='%1$s' " +
            "or 设备代码 in (select 代码 from scd设备参数表 where 所属馈线='%1$s')） " +
            "and 电度序号=(select min(电度序号) + %2$d from 电度参数表 where 设备代码='%1$s' " +
            "or 设备代码 in (select 代码 from scd设备参数表 where 所属馈线='%1$s'))";

    public String GetMeasureID(int KWHIndex, String psrID, boolean isJunction){
        final List<String> measureID = new ArrayList<>();
        String sql;

        if (!isJunction){
            sql = String.format(normalKWHSQL, psrID, KWHIndex);
        }else{
            sql = String.format(junctionKWHSQL, psrID, KWHIndex);
        }

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    measureID.add(resultSet.getString("代码").trim());
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        return measureID.isEmpty() ? "" : measureID.get(0);
    }
}
