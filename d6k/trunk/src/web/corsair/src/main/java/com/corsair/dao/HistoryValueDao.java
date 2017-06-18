package com.corsair.dao;

import com.corsair.measurement.history.HistoryValue;
import com.rbac.common.BaseDaoSupport;
import oracle.jdbc.OracleTypes;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;
import org.springframework.jdbc.core.CallableStatementCallback;
import org.springframework.jdbc.core.CallableStatementCreator;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * 获取历史数据的接口
 */
@Repository
public class HistoryValueDao extends BaseDaoSupport {
    @Autowired
    JdbcTemplate jdbcTemplate;

    private static final String KWH_TABLE = "KWH";
    private static final String YC_TABLE = "YC";

    private class HistoryDataStatementCallback implements CallableStatementCallback {
        public HistoryDataStatementCallback(int objectCount) {
            ObjectCount = objectCount;
        }

        private int ObjectCount;

        public Object doInCallableStatement(CallableStatement cs) throws SQLException, DataAccessException {
            List results = new ArrayList();
            cs.execute();
            ResultSet rs = (ResultSet) cs.getObject(ObjectCount);

            if (rs != null) {
                while (rs.next()) {
                    String date = rs.getString(1);
                    String time = rs.getString(2);
                    String data = rs.getString(3);

                    results.add(new HistoryValue(date, time, data != null ? Double.parseDouble(data) : Double.NaN));
                }
                rs.close();
            }

            return results;
        }
    }

    /**
     * 通用获取历史数据接口
     *
     * @param id        对象ID
     * @param startTime 开始时间, 格式2016-01-01 10:10
     * @param endTime   结束时间, 格式2016-01-01 10:10
     * @param fromKWH   true时从电度数据表中获取,否则从
     * @return 历史数据列表
     */
    public List getCommonHistoryData(final String id, final String startTime, final String endTime, final boolean fromKWH) {
        List resultList = (List) jdbcTemplate.execute(
                new CallableStatementCreator() {
                    public CallableStatement createCallableStatement(Connection con) throws SQLException {
                        String storedProc = "{call my_history_data_pack.getHistoryData(?,?,?,?,?)}";// 调用的sql
                        CallableStatement cs = con.prepareCall(storedProc);
                        cs.setString(1, id);
                        cs.setString(2, startTime);
                        cs.setString(3, endTime);
                        cs.setString(4, fromKWH ? KWH_TABLE : YC_TABLE);
                        cs.registerOutParameter(5, OracleTypes.CURSOR);
                        return cs;
                    }
                }, new HistoryDataStatementCallback(5));

        return resultList;
    }


    /**
     * 获取小时为单位的历史数据
     *
     * @param id      对象ID
     * @param time    时间, 格式2016-01-01 10
     * @param fromKWH true时从电度数据表中获取,否则从
     * @return 历史数据列表
     */
    public List getHourData(final String id, final String time, final boolean fromKWH) {
        List resultList = (List) jdbcTemplate.execute(
                new CallableStatementCreator() {
                    public CallableStatement createCallableStatement(Connection con) throws SQLException {
                        String storedProc = "{call my_hour_data_pack.gethourdata(?,?,?,?)}";// 调用的sql
                        CallableStatement cs = con.prepareCall(storedProc);
                        cs.setString(1, id);
                        cs.setString(2, time);
                        cs.setString(3, fromKWH ? KWH_TABLE : YC_TABLE);
                        cs.registerOutParameter(4, OracleTypes.CURSOR);
                        return cs;
                    }
                }, new HistoryDataStatementCallback(4));

        return resultList;
    }

    /**
     * 获取每日的小时数据
     *
     * @param id      对象ID
     * @param time    时间, 格式2016-01-01
     * @param fromKWH true时从电度数据表中获取,否则从
     * @return 历史数据列表
     */
    public List getHourDataOfDay(final String id, final String time, final boolean fromKWH) {
        List resultList = (List) jdbcTemplate.execute(
                new CallableStatementCreator() {
                    public CallableStatement createCallableStatement(Connection con) throws SQLException {
                        String storedProc = "{call my_hour_data_ofdate_pack.gethourdataofday(?,?,?,?)}";// 调用的sql
                        CallableStatement cs = con.prepareCall(storedProc);
                        cs.setString(1, id);
                        cs.setString(2, time);
                        cs.setString(3, fromKWH ? KWH_TABLE : YC_TABLE);
                        cs.registerOutParameter(4, OracleTypes.CURSOR);
                        return cs;
                    }
                }, new HistoryDataStatementCallback(4));

        return resultList;
    }

    /**
     * 获取一天的数据
     *
     * @param id      对象ID
     * @param time    时间, 格式2016-01-01
     * @param fromKWH true时从电度数据表中获取,否则从
     * @return 历史数据列表
     */
    public List getDayData(final String id, final String time, final boolean fromKWH) {
        List resultList = (List) jdbcTemplate.execute(
                new CallableStatementCreator() {
                    public CallableStatement createCallableStatement(Connection con) throws SQLException {
                        String storedProc = "{call my_hour_data_ofdate_pack.getdaydata(?,?,?,?)}";// 调用的sql
                        CallableStatement cs = con.prepareCall(storedProc);
                        cs.setString(1, id);
                        cs.setString(2, time);
                        cs.setString(3, fromKWH ? KWH_TABLE : YC_TABLE);
                        cs.registerOutParameter(4, OracleTypes.CURSOR);
                        return cs;
                    }
                }, new HistoryDataStatementCallback(4));

        return resultList;
    }


    /**
     * 获取一天首尾的数据
     *
     * @param id      对象ID
     * @param time    时间, 格式2016-01-01
     * @param fromKWH true时从电度数据表中获取,否则从
     * @return 历史数据当天首尾数据
     */
    public List getDayScopeData(final String id, final String time, final boolean fromKWH) {
        List resultList = (List) jdbcTemplate.execute(
                new CallableStatementCreator() {
                    public CallableStatement createCallableStatement(Connection con) throws SQLException {
                        String storedProc = "{call my_hour_data_ofdate_pack.getnewdaydata(?,?,?,?)}";// 调用的sql
                        CallableStatement cs = con.prepareCall(storedProc);
                        cs.setString(1, id);
                        cs.setString(2, time);
                        cs.setString(3, fromKWH ? KWH_TABLE : YC_TABLE);
                        cs.registerOutParameter(4, OracleTypes.CURSOR);
                        return cs;
                    }
                }, new HistoryDataStatementCallback(4));

        return resultList;
    }

    /**
     * 获取一个月首尾的数据
     *
     * @param id      对象ID
     * @param time    时间, 格式2016-01
     * @param fromKWH true时从电度数据表中获取,否则从
     * @return 历史数据当月首尾数据
     */
    public List getMonthScopeData(final String id, final String time, final boolean fromKWH) {
        List resultList = (List) jdbcTemplate.execute(
                new CallableStatementCreator() {
                    public CallableStatement createCallableStatement(Connection con) throws SQLException {
                        String storedProc = "{call my_month_data_pack.getnewmonthdata(?,?,?,?)}";// 调用的sql
                        CallableStatement cs = con.prepareCall(storedProc);
                        cs.setString(1, id);
                        cs.setString(2, time);
                        cs.setString(3, fromKWH ? KWH_TABLE : YC_TABLE);
                        cs.registerOutParameter(4, OracleTypes.CURSOR);
                        return cs;
                    }
                }, new HistoryDataStatementCallback(4));

        return resultList;
    }

    /**
     * 根据步长获取历史数据
     * @param id 对象
     * @param startTime 查询开始时间,格式2016-01-01 00:00
     * @param endTime 查询结束时间,格式2016-01-01 00:00
     * @param step 步长 分钟
     * @param fromKWH 是否从电度历史表中取值
     * @return 历史数据列表
     */
    public List getStepHistoryData(final String id, final String startTime, final String endTime, final Integer step, final boolean fromKWH) {
        List resultList = (List) jdbcTemplate.execute(
                new CallableStatementCreator() {
                    public CallableStatement createCallableStatement(Connection con) throws SQLException {
                        String storedProc = "{call my_history_data_pack.getNewHistoryData(?,?,?,?,?,?)}";// 调用的sql
                        CallableStatement cs = con.prepareCall(storedProc);
                        cs.setString(1, id);
                        cs.setString(2, startTime);
                        cs.setString(3, endTime);
                        cs.setInt(4, step);
                        cs.setString(5, fromKWH ? KWH_TABLE : YC_TABLE);
                        cs.registerOutParameter(6, OracleTypes.CURSOR);
                        return cs;
                    }
                }, new HistoryDataStatementCallback(6));

        return resultList;
    }
}
