package com.corsair.dao;

import com.corsair.device.*;
import com.corsair.entity.template.psr.PSRTypeConstant;
import com.corsair.measurement.HistoryDataValue;
import com.corsair.measurement.MeasurementInfo;
import com.corsair.service.TreeDataOfAreaRelation;
import com.rbac.common.PagerOfAlarm;
import org.apache.poi.hssf.usermodel.*;
import org.apache.poi.hssf.util.HSSFColor;
import org.apache.poi.ss.formula.functions.T;
import org.apache.poi.ss.util.CellRangeAddress;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowCallbackHandler;
import org.springframework.stereotype.Repository;

import java.io.FileOutputStream;
import java.math.BigDecimal;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Vector;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.text.ParseException;
import java.util.*;

/**
 * Created by zjq on 2015/7/22.
 */
@Repository("scdDeviceDao")
public class ScdDeviceDao {

    private String tableExist = null;

    @Autowired
    private JdbcTemplate jdbcTemplate;

    private String m_strDeviceDesc;

    // 最多只能查询1万条记录
    private int nRownum = 10000;

    /**
     * 获取阵列或分布式厂站,取的是站所线系参数表中的厂站
     * @return 列表
     */
    public List getStations() {
        final List stations = new ArrayList();
        //String sql = String.format("select 代码,描述 from 站所线系参数表 where 类型=%1$s", PSRTypeConstant.STATION);
        String sql = String.format("select STATION_ID,DESCRIBE from SCD_STATION", PSRTypeConstant
                .STATION);

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    stations.add(new BasePSR(resultSet.getString("STATION_ID").trim(), resultSet.getString("DESCRIBE").trim()));
                    //stations.add(new BasePSR(resultSet.getString("代码").trim(), resultSet.getString("描述").trim()));
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return stations;
    }

    /**
     * 获取子站的所有设备,不顾及层次关系
     * @param subStationID 子站ID
     * @return 子设备列表
     */
    public List getDeviceOfStation(String subStationID) {
        final List<BasePSR> basePSRs = new ArrayList<>();
        //String sql = "select * from scd设备参数表 where 站所线系代码 =  '" + subStationID + "' order by 类型,描述";
        String sql = "select * from SCD_DEVICE where STATION_ID =  '" + subStationID + "' order by DEVICE_ID";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    /*BasePSR basePSR = new BasePSR(resultSet.getString("代码").trim(), resultSet.getString("描述").trim()
                            , resultSet.getString("类型").trim());*/
                    //DEVICE_TAG暂未启用
                    BasePSR basePSR = new BasePSR(resultSet.getString("DEVICE_ID").trim(), resultSet.getString
                            ("DESCRIBE").trim(), resultSet.getString("DEVICE_TAG").trim(),resultSet.getString("STATION_ID").trim());
                    basePSRs.add(basePSR);
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        return basePSRs;
    }

    /**
     * 根据ID获取设备的信息
     * @param id id
     * @return 设备信息
     */
    public BasePSR getPSRFromId(String id) {
        final List basePSRs = new ArrayList();
        //String sql = "select 代码,描述 from SCD设备参数表 where 代码='" + id + "'";
        String sql = "select DEVICE_ID,DESCRIBE from SCD_DEVICE where DEVICE_ID='" + id + "'";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    //basePSRs.add(new BasePSR(resultSet.getString("代码").trim(), resultSet.getString("描述").trim()));
                    basePSRs.add(new BasePSR(resultSet.getString("DEVICE_ID").trim(), resultSet.getString("DESCRIBE").trim()));
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return basePSRs.isEmpty() ? null: (BasePSR)basePSRs.get(0);
    }

    /**
     * 请求日报表历史数据
     *
     * @param stationId  厂站ID
     * @param deviceId   设备ID
     * @param dayReportTime   日报表查询日期
     * @return 返回日报表数据, 如果没有数据，则为空，如果出错，为null
     */
    public List<HisDayValueData> getDayReportHistoryData(String stationId, String deviceId, final String dayReportTime){

        long sdate = getDays(dayReportTime, "1970-01-01");//1970-1-1至今的天数
        String year = dayReportTime.substring(0, 4);
        String tableName = null;
        int stationID = Integer.parseInt(stationId);
        int deviceID = Integer.parseInt(deviceId);
        int paraID = -1;

        tableName = "HIS_AI" + year;

        final List<HisDayValueData> historyDatas = new ArrayList<>();
        String sqlTableName = "select * from user_tables where table_name = '" + tableName + "'";
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        final Vector<NameAndUnit> vecNameAndUnit = new Vector<NameAndUnit>();

        if(tableExist != null && !tableExist.isEmpty()){

            String sqlPointNum = "select AI_ID,DESCRIBE,AI_TAG from SCD_AI where STATION_ID='" + stationId + "' and DEVICE_ID='" + deviceId + "' and CALCULATE_FLAG=1" + " order by AI_ID";

            try {
                jdbcTemplate.query(sqlPointNum, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            NameAndUnit nameAndUnit = new NameAndUnit();
                            int nID = resultSet.getInt("AI_ID");
                            String strDescribe = resultSet.getString("DESCRIBE");
                            String strUnit = resultSet.getString("AI_TAG");

                            nameAndUnit.setId(nID);
                            nameAndUnit.setName(strDescribe);
                            nameAndUnit.setUnit(strUnit);

                            vecNameAndUnit.add(nameAndUnit);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

            for(int i=0; i < vecNameAndUnit.size(); i++) {
                final NameAndUnit nameAndUnit = vecNameAndUnit.get(i);


                String sql = null;
                sql = "select * from " + tableName + " where STATION_ID=" + stationID + " and DEVICE_ID=" + deviceID + " " +
                        " and AI_ID=" + nameAndUnit.getId() + " and sdate=" + sdate;

                try {
                    jdbcTemplate.query(sql, new RowCallbackHandler() {
                        public void processRow(ResultSet resultSet) throws SQLException {
                            HisDayValueData historyDataValue = new HisDayValueData();

                            //取出6：00-20:00历史库存放的第一个数据
                            for (int i = 0; i <= 23; i++) {
                                try {
                                    String colname = "VALUE_" + String.valueOf(i);
                                    String strValue = resultSet.getString(colname).trim();
                                    String value = strValue.substring(0, strValue.indexOf(","));

                                    //数据保留到小数点后两位,四舍五入
                                    BigDecimal bgvalue = new BigDecimal(value);
                                    value = bgvalue.setScale(2, BigDecimal.ROUND_HALF_UP).toString();

                                    //数据为空则继续
                                    if (value.isEmpty()) {
                                        continue;
                                    }

                                    historyDataValue.setName(nameAndUnit.getName());
                                    historyDataValue.setUnit(nameAndUnit.getUnit());
                                    switch (i) {
                                        case 0:
                                            historyDataValue.setValue0(value);
                                            break;
                                        case 1:
                                            historyDataValue.setValue1(value);
                                            break;
                                        case 2:
                                            historyDataValue.setValue2(value);
                                            break;
                                        case 3:
                                            historyDataValue.setValue3(value);
                                            break;
                                        case 4:
                                            historyDataValue.setValue4(value);
                                            break;
                                        case 5:
                                            historyDataValue.setValue5(value);
                                            break;
                                        case 6:
                                            historyDataValue.setValue6(value);
                                            break;
                                        case 7:
                                            historyDataValue.setValue7(value);
                                            break;
                                        case 8:
                                            historyDataValue.setValue8(value);
                                            break;
                                        case 9:
                                            historyDataValue.setValue9(value);
                                            break;
                                        case 10:
                                            historyDataValue.setValue10(value);
                                            break;
                                        case 11:
                                            historyDataValue.setValue11(value);
                                            break;
                                        case 12:
                                            historyDataValue.setValue12(value);
                                            break;
                                        case 13:
                                            historyDataValue.setValue13(value);
                                            break;
                                        case 14:
                                            historyDataValue.setValue14(value);
                                            break;
                                        case 15:
                                            historyDataValue.setValue15(value);
                                            break;
                                        case 16:
                                            historyDataValue.setValue16(value);
                                            break;
                                        case 17:
                                            historyDataValue.setValue17(value);
                                            break;
                                        case 18:
                                            historyDataValue.setValue18(value);
                                            break;
                                        case 19:
                                            historyDataValue.setValue19(value);
                                            break;
                                        case 20:
                                            historyDataValue.setValue20(value);
                                            break;
                                        case 21:
                                            historyDataValue.setValue21(value);
                                            break;
                                        case 22:
                                            historyDataValue.setValue22(value);
                                            break;
                                        case 23:
                                            historyDataValue.setValue23(value);
                                            break;
                                        default:
                                            break;
                                    }

                                } catch (Exception e) {
                                    switch (i) {
                                        case 0:
                                            historyDataValue.setValue0("----");
                                            break;
                                        case 1:
                                            historyDataValue.setValue1("----");
                                            break;
                                        case 2:
                                            historyDataValue.setValue2("----");
                                            break;
                                        case 3:
                                            historyDataValue.setValue3("----");
                                            break;
                                        case 4:
                                            historyDataValue.setValue4("----");
                                            break;
                                        case 5:
                                            historyDataValue.setValue5("----");
                                            break;
                                        case 6:
                                            historyDataValue.setValue6("----");
                                            break;
                                        case 7:
                                            historyDataValue.setValue7("----");
                                            break;
                                        case 8:
                                            historyDataValue.setValue8("----");
                                            break;
                                        case 9:
                                            historyDataValue.setValue9("----");
                                            break;
                                        case 10:
                                            historyDataValue.setValue10("----");
                                            break;
                                        case 11:
                                            historyDataValue.setValue11("----");
                                            break;
                                        case 12:
                                            historyDataValue.setValue12("----");
                                            break;
                                        case 13:
                                            historyDataValue.setValue13("----");
                                            break;
                                        case 14:
                                            historyDataValue.setValue14("----");
                                            break;
                                        case 15:
                                            historyDataValue.setValue15("----");
                                            break;
                                        case 16:
                                            historyDataValue.setValue16("----");
                                            break;
                                        case 17:
                                            historyDataValue.setValue17("----");
                                            break;
                                        case 18:
                                            historyDataValue.setValue18("----");
                                            break;
                                        case 19:
                                            historyDataValue.setValue19("----");
                                            break;
                                        case 20:
                                            historyDataValue.setValue20("----");
                                            break;
                                        case 21:
                                            historyDataValue.setValue21("----");
                                            break;
                                        case 22:
                                            historyDataValue.setValue22("----");
                                            break;
                                        case 23:
                                            historyDataValue.setValue23("----");
                                            break;
                                        default:
                                            break;
                                    }
                                }

                            }

                            historyDatas.add(historyDataValue);
                        }
                    });
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        return historyDatas;

    }

    // 补零
    boolean zeroOfWrite(List<String> historyDatas, int cnt) {
        if (cnt <=0 || cnt > 12) {
            return false;
        }

        for (int i = 0; i < cnt; i++) {
            historyDatas.add("0");
        }

        return true;
    }


    public List<List<String>> getDataOf5Minute(String stationId, String deviceId, final String dayReportTime){

        long sdate = getDays(dayReportTime, "1970-01-01");//1970-1-1至今的天数
        String year = dayReportTime.substring(0, 4);
        String tableName = null;
        int stationID = Integer.parseInt(stationId);
        int deviceID = Integer.parseInt(deviceId);
        int paraID = -1;

        tableName = "HIS_AI" + year;

        final List<List<String>> historyDatasAll = new ArrayList<>();

        //final List<HisDayValueData> historyDatas = new ArrayList<>();
        String sqlTableName = "select * from user_tables where table_name = '" + tableName + "'";
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        final Vector<NameAndUnit> vecNameAndUnit = new Vector<NameAndUnit>();

        if(tableExist != null && !tableExist.isEmpty()){

            String sqlPointNum = "select AI_ID,DESCRIBE,AI_TAG from SCD_AI where STATION_ID='" + stationId + "' and DEVICE_ID='" + deviceId + "' and CALCULATE_FLAG=1" + " order by AI_ID";

            try {
                jdbcTemplate.query(sqlPointNum, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            NameAndUnit nameAndUnit = new NameAndUnit();
                            int nID = resultSet.getInt("AI_ID");
                            String strDescribe = resultSet.getString("DESCRIBE");
                            String strUnit = resultSet.getString("AI_TAG");

                            nameAndUnit.setId(nID);
                            nameAndUnit.setName(strDescribe);
                            nameAndUnit.setUnit(strUnit);

                            vecNameAndUnit.add(nameAndUnit);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

            for(int i=0; i < vecNameAndUnit.size(); i++) {

                final List<String> historyDatas = new ArrayList<>();

                final NameAndUnit nameAndUnit = vecNameAndUnit.get(i);

                String sql = null;
                sql = "select * from " + tableName + " where STATION_ID=" + stationID + " and DEVICE_ID=" + deviceID + " " +
                        " and AI_ID=" + nameAndUnit.getId() + " and sdate=" + sdate;

                try {
                    jdbcTemplate.query(sql, new RowCallbackHandler() {
                        public void processRow(ResultSet resultSet) throws SQLException {

                            //取出6：00-20:00历史库存放的第一个数据
                            for (int k = 0; k <= 23; k++) {
                                try {
                                    String colname = "VALUE_" + String.valueOf(k);
                                    String strValue = resultSet.getString(colname).trim();
                                    //String value = strValue.substring(0, strValue.indexOf(","));

                                    if (strValue.isEmpty()) {
                                        zeroOfWrite(historyDatas, 12);
                                        continue;
                                    }

                                    String [] dataArray = strValue.split(";");

                                    int len = dataArray.length;
                                    if (len > 12) {
                                        len = 12;
                                    }

                                    //数据保留到小数点后两位,四舍五入
                                    for (int j = 0; j < len; j++) {
                                        String value = dataArray[j].substring(0, dataArray[j].indexOf(","));
                                        BigDecimal bgvalue2 = new BigDecimal(value);
                                        String value2 = bgvalue2.setScale(2, BigDecimal.ROUND_HALF_UP).toString();
                                        historyDatas.add(value2);
                                    }

                                    zeroOfWrite(historyDatas, 12 - len);

                                } catch (Exception e) {
                                    zeroOfWrite(historyDatas, 12);
                                }
                            }
                        }
                    });
                } catch (Exception e) {
                    e.printStackTrace();
                }

                historyDatasAll.add(historyDatas);
            }
        }
        return historyDatasAll;

    }

    /**
     * 请求月报表历史数据
     *
     * @param stationId  厂站ID
     * @param deviceId   设备ID
     * @param monthReportTime   月报表查询日期
     * @return 返回月报表数据, 如果没有数据，则为空，如果出错，为null
     */
    public Vector<HisMonthValueData> getMonthReportHistoryData(String stationId, String deviceId, final String monthReportTime){

        String year = monthReportTime.substring(0, 4);
        String month = monthReportTime.substring(5);

        final int days = getMonthDay(monthReportTime);
        final long sdateBegin = getDays(getLastMonthDay(monthReportTime), "1970-1-1") + 1; //本月第一天的sdate
        final long sdateEnd = sdateBegin + days - 1;//本月最后一天的sdate


        String tableName = null;
        int stationID = Integer.parseInt(stationId);
        int deviceID = Integer.parseInt(deviceId);
        int paraID = -1;

        tableName = "HIS_AI_DS" + year;

        final  Vector<HisMonthValueData> historyMonthDatas = new Vector<HisMonthValueData>();

        final Vector<NameAndUnit> vecNameAndUnit = new Vector<NameAndUnit>();

        String sqlTableName = "select * from user_tables where table_name = '" + tableName + "'";
        tableExist = null;
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        if(tableExist != null && !tableExist.isEmpty()){
            String sqlPointNum = "select AI_ID,DESCRIBE,AI_TAG from SCD_AI where STATION_ID='" + stationId + "' and DEVICE_ID='" + deviceId + "' and CALCULATE_FLAG=1" + " order by AI_ID";

            try {
                jdbcTemplate.query(sqlPointNum, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            NameAndUnit nameAndUnit = new NameAndUnit();
                            int nID = resultSet.getInt("AI_ID");
                            String strDescribe = resultSet.getString("DESCRIBE");
                            String strUnit = resultSet.getString("AI_TAG");

                            nameAndUnit.setId(nID);
                            nameAndUnit.setName(strDescribe);
                            nameAndUnit.setUnit(strUnit);

                            vecNameAndUnit.add(nameAndUnit);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

            for(int i=0; i < vecNameAndUnit.size(); i++) {
                NameAndUnit nameAndUnit = vecNameAndUnit.get(i);

                String sql = null;
                sql = "select * from " + tableName + " where STATION_ID=" + stationID + " and DEVICE_ID=" +deviceID + " " +
                        " and AI_ID=" + nameAndUnit.getId() + " and flag=150" + " and sdate>=" + sdateBegin + " and sdate<=" + sdateEnd + " order by sdate";

                final HisMonthValueData hisMonthVal = new HisMonthValueData();
                hisMonthVal.setName(nameAndUnit.getName());  //设置测点名称
                hisMonthVal.setUnit(nameAndUnit.getUnit());  //设置测点单位

                try {
                    jdbcTemplate.query(sql, new RowCallbackHandler() {
                        public void processRow(ResultSet resultSet) throws SQLException {
                            //取出历史库存放的每一天的数
                            try {
                                HisMonthValueData historyMonthData = new HisMonthValueData();
                                long lSdate = resultSet.getLong("sdate");
                                String strData = resultSet.getString("data");
                                //数据保留到小数点后两位，四舍五入
                                BigDecimal bgdata = new BigDecimal(strData);
                                strData = bgdata.setScale(2,BigDecimal.ROUND_HALF_UP).toString();
                                int index = new Long(lSdate - sdateBegin).intValue() + 1;// Integer.parseInt(lSdate - sdateBegin);
                                switch (index) {
                                    case 1:
                                        hisMonthVal.setValue1(strData);
                                        break;
                                    case 2:
                                        hisMonthVal.setValue2(strData);
                                        break;
                                    case 3:
                                        hisMonthVal.setValue3(strData);
                                        break;
                                    case 4:
                                        hisMonthVal.setValue4(strData);
                                        break;
                                    case 5:
                                        hisMonthVal.setValue5(strData);
                                        break;
                                    case 6:
                                        hisMonthVal.setValue6(strData);
                                        break;
                                    case 7:
                                        hisMonthVal.setValue7(strData);
                                        break;
                                    case 8:
                                        hisMonthVal.setValue8(strData);
                                        break;
                                    case 9:
                                        hisMonthVal.setValue9(strData);
                                        break;
                                    case 10:
                                        hisMonthVal.setValue10(strData);
                                        break;
                                    case 11:
                                        hisMonthVal.setValue11(strData);
                                        break;
                                    case 12:
                                        hisMonthVal.setValue12(strData);
                                        break;
                                    case 13:
                                        hisMonthVal.setValue13(strData);
                                        break;
                                    case 14:
                                        hisMonthVal.setValue14(strData);
                                        break;
                                    case 15:
                                        hisMonthVal.setValue15(strData);
                                        break;
                                    case 16:
                                        hisMonthVal.setValue16(strData);
                                        break;
                                    case 17:
                                        hisMonthVal.setValue17(strData);
                                        break;
                                    case 18:
                                        hisMonthVal.setValue18(strData);
                                        break;
                                    case 19:
                                        hisMonthVal.setValue19(strData);
                                        break;
                                    case 20:
                                        hisMonthVal.setValue20(strData);
                                        break;
                                    case 21:
                                        hisMonthVal.setValue21(strData);
                                        break;
                                    case 22:
                                        hisMonthVal.setValue22(strData);
                                        break;
                                    case 23:
                                        hisMonthVal.setValue23(strData);
                                        break;
                                    case 24:
                                        hisMonthVal.setValue24(strData);
                                        break;
                                    case 25:
                                        hisMonthVal.setValue25(strData);
                                        break;
                                    case 26:
                                        hisMonthVal.setValue26(strData);
                                        break;
                                    case 27:
                                        hisMonthVal.setValue27(strData);
                                        break;
                                    case 28:
                                        hisMonthVal.setValue28(strData);
                                        break;
                                    case 29:
                                        hisMonthVal.setValue29(strData);
                                        break;
                                    case 30:
                                        hisMonthVal.setValue30(strData);
                                        break;
                                    case 31:
                                        hisMonthVal.setValue31(strData);
                                        break;
                                    default:
                                        break;
                                }
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                    });
                } catch (Exception e) {
                    e.printStackTrace();
                }

                historyMonthDatas.add(hisMonthVal);
            }
        }

        return historyMonthDatas;
    }

    public PagerOfAlarm getAlarmPagerOfDay(String strStationID, String strDeviceID, final String strDate, String strAlarmType, String strCurrentPage, String strLevel, String strChooseName) {

        if (strCurrentPage == null || "".equals(strCurrentPage.trim())) {
            return null;
        }

        List<AlarmRecordData> listAlarm = getAlarmRecordDay(strStationID, strDeviceID, strDate, strAlarmType, strLevel, strChooseName);

        int pageSize = Integer.parseInt(strCurrentPage);

        PagerOfAlarm pagerOfAlarm = new PagerOfAlarm(pageSize, 15, listAlarm);
        if (pagerOfAlarm.getDataList() == null) {
            List<AlarmRecordData> alarmRecordDatas = new ArrayList<>();
            pagerOfAlarm.setDataList(alarmRecordDatas);
        }

        return pagerOfAlarm;
    }

    public List<AlarmRecordData> getAlarmRecordDay(String stationId, String deviceId, final String dayReportTime, String strAlarmType, String strLevel, String strChooseName) {

        String sqlHistory = "select * from user_tables where table_name = 'HISTORY_MATTERS_INFO'";
        jdbcTemplate.query(sqlHistory, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        if(tableExist == null || tableExist.isEmpty()) {
            return null;
        }

        String sqlOperation = "select * from user_tables where table_name = 'OPERATION_MATTERS_INFO'";
        jdbcTemplate.query(sqlOperation, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        if(tableExist == null || tableExist.isEmpty()) {
            return null;
        }

        List<AlarmRecordData> alarmRecordDatas = new ArrayList<>();

        int nLevel = Integer.parseInt(strLevel);
        switch (nLevel) {
            case 1:
                alarmRecordDatas = getAlarmRecordDayOfProvince(dayReportTime, strAlarmType);
                break;
            case 2:
                alarmRecordDatas = getAlarmRecordDayOfCity(dayReportTime, strAlarmType, strLevel, strChooseName);
                break;
            case 3:
                alarmRecordDatas = getAlarmRecordDayOfStation(stationId, dayReportTime, strAlarmType);
                break;
            case 4:
                alarmRecordDatas = getAlarmRecordDayOfDevice(stationId, deviceId, dayReportTime, strAlarmType);
                break;
            default:
                return null;
        }

        return alarmRecordDatas;
    }

    public List<String> getDeviceDesc(String strStationID, String strDeviceID) {

        final List<String> deviceDesc = new ArrayList<>();

        int stationID = Integer.parseInt(strStationID);
        int deviceID = Integer.parseInt(strDeviceID);

        String strExistSql = "select * from user_tables where table_name = 'SCD_DEVICE'";
        tableExist = null;
        jdbcTemplate.query(strExistSql, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        if(tableExist == null || tableExist.isEmpty()) {
            return null;
        }

        String strSelectSql = String.format("select device_info  from scd_device where station_id=%d and device_id=%d", stationID, deviceID);

        try {
            jdbcTemplate.query(strSelectSql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    // 获取设备下所有的点
                    try {

                        m_strDeviceDesc = resultSet.getString("device_info");
                        deviceDesc.add(m_strDeviceDesc);

                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return deviceDesc;

    }

    // 获取整个省的告警信息
    public List<AlarmRecordData> getAlarmRecordDayOfProvince(final String dayReportTime, String strAlarmType) {

        String year = dayReportTime.substring(0, 4);
        String month = dayReportTime.substring(5, 7);
        String day = dayReportTime.substring(8, 10);

        String strDate = year + month + day;

        boolean bFlag = false;
        String sql = null;
        String sqlHistoryAll = null;
        String sqlOperationAll = null;

        if (strAlarmType.compareTo("allAlarm") == 0) {
            bFlag = true;

            sqlHistoryAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + " order by YEAR_MONTH_DAY";
            sqlOperationAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + " order by YEAR_MONTH_DAY";

        } else if (strAlarmType.compareTo("switchAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + " and TYPE=100 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("measureAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + " and TYPE>=0 and TYPE<=6 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("operationAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + " and TYPE=200 order by YEAR_MONTH_DAY";
        } else {
            return null;
        }

        final List<AlarmRecordData> alarmRecordDatas = new ArrayList<>();

        if (!bFlag) {
            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else {
            try {
                jdbcTemplate.query(sqlHistoryAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

            try {
                jdbcTemplate.query(sqlOperationAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return alarmRecordDatas;
    }

    // 获取指定市的告警信息
    public List<AlarmRecordData> getAlarmRecordDayOfCity(final String dayReportTime, String strAlarmType, String strLevel, String strChooseName) {


        List<TreeDataOfAreaRelation> listArea = getAreaRelation();

        String strSqlWhereContion = "";

        for (int i = 0; i < listArea.size(); ++i) {
            if (listArea.get(i).getStrLevel().compareTo(strLevel) == 0 && listArea.get(i).getStrDescribe().compareTo(strChooseName) == 0) {

                String strStationID = listArea.get(i).getStrStationID();

                String [] strStationIDArray = strStationID.split(",");
                if (strStationIDArray.length == 0) {
                    return null;
                }

                boolean flag = false;
                for (int j = 0; j < strStationIDArray.length; ++j) {
                    if (j != strStationIDArray.length) {
                        if (!flag) {
                            strSqlWhereContion += " and STATION_ID=";
                            flag = true;
                        } else {
                            strSqlWhereContion += " or STATION_ID=";
                        }
                    }
                    strSqlWhereContion += strStationIDArray[j].trim();
                }

                break;
            }
        }

        if (strSqlWhereContion.isEmpty() || "".compareTo(strSqlWhereContion) == 0) {
            return null;
        }

        String year = dayReportTime.substring(0, 4);
        String month = dayReportTime.substring(5, 7);
        String day = dayReportTime.substring(8, 10);

        String strDate = year + month + day;

        boolean bFlag = false;
        String sql = null;
        String sqlHistoryAll = null;
        String sqlOperationAll = null;

        if (strAlarmType.compareTo("allAlarm") == 0) {
            bFlag = true;
            sqlHistoryAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strSqlWhereContion + " order by YEAR_MONTH_DAY";
            sqlOperationAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strSqlWhereContion + " order by YEAR_MONTH_DAY";

        } else if (strAlarmType.compareTo("switchAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strSqlWhereContion + " and TYPE=100 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("measureAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strSqlWhereContion + " and TYPE>=0 and TYPE<=6 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("operationAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strSqlWhereContion + " and TYPE=200 order by YEAR_MONTH_DAY";
        } else {
            return null;
        }

        final List<AlarmRecordData> alarmRecordDatas = new ArrayList<>();

        if (!bFlag) {
            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else {
            try {
                jdbcTemplate.query(sqlHistoryAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

            try {
                jdbcTemplate.query(sqlOperationAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return alarmRecordDatas;
    }

    // 获取指定县的告警信息
    public List<AlarmRecordData> getAlarmRecordDayOfStation(String stationId, final String dayReportTime, String strAlarmType) {

        String year = dayReportTime.substring(0, 4);
        String month = dayReportTime.substring(5, 7);
        String day = dayReportTime.substring(8, 10);

        String strDate = year + month + day;

        String strCondtion = " and STATION_ID=" + stationId;

        boolean bFlag = false;
        String sql = null;
        String sqlHistoryAll = null;
        String sqlOperationAll = null;

        if (strAlarmType.compareTo("allAlarm") == 0) {
            bFlag = true;
            sqlHistoryAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strCondtion + " order by YEAR_MONTH_DAY";
            sqlOperationAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strCondtion + " order by YEAR_MONTH_DAY";

        } else if (strAlarmType.compareTo("switchAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strCondtion + " and TYPE=100 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("measureAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strCondtion + " and TYPE>=0 and TYPE<=6 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("operationAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strCondtion + " and TYPE=200 order by YEAR_MONTH_DAY";
        } else {
            return null;
        }

        final List<AlarmRecordData> alarmRecordDatas = new ArrayList<>();

        if (!bFlag) {
            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else {
            try {
                jdbcTemplate.query(sqlHistoryAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

            try {
                jdbcTemplate.query(sqlOperationAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return alarmRecordDatas;
    }

    public List<AlarmRecordData> getAlarmRecordDayOfDevice(String stationId, String deviceId, final String dayReportTime, String strAlarmType) {

        String year = dayReportTime.substring(0, 4);
        String month = dayReportTime.substring(5, 7);
        String day = dayReportTime.substring(8, 10);

        String strDate = year + month + day;

        boolean bFlag = false;
        String sql = null;
        String sqlHistoryAll = null;
        String sqlOperationAll = null;

        String strContion = " and STATION_ID=" + stationId + " and EQUIPMENT_ID=" + deviceId;

        if (strAlarmType.compareTo("allAlarm") == 0) {
            bFlag = true;

            sqlHistoryAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strContion + " order by YEAR_MONTH_DAY";
            sqlOperationAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strContion +" order by YEAR_MONTH_DAY";

        } else if (strAlarmType.compareTo("switchAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strContion+ " and TYPE=100 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("measureAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strContion+ " and TYPE>=0 and TYPE<=6 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("operationAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY=" + strDate + strContion+ " and TYPE=200 order by YEAR_MONTH_DAY";
        } else {
            return null;
        }

        final List<AlarmRecordData> alarmRecordDatas = new ArrayList<>();

        if (!bFlag) {
            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else {
            try {
                jdbcTemplate.query(sqlHistoryAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

            try {
                jdbcTemplate.query(sqlOperationAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return alarmRecordDatas;
    }

    List<TreeDataOfAreaRelation> getAreaRelation() {
        // 从区域关系表中，根据name和type查找设备ID
        final List<TreeDataOfAreaRelation> listArea = new ArrayList<TreeDataOfAreaRelation>();
        String sqlArea = String.format("select * from AREA_RELATION");
        try {
            jdbcTemplate.query(sqlArea, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    TreeDataOfAreaRelation area = new TreeDataOfAreaRelation();

                    area.setStrDescribe(resultSet.getString("DESCRIBE").trim());
                    area.setStrLevel(resultSet.getString("AREA_LEVEL").trim());
                    area.setStrStationID(resultSet.getString("STATION_ID").trim());
                    area.setnIsCalc(resultSet.getInt("IS_CALC"));
                    area.setStrStationAndDeviceRelationID(resultSet.getString("STATION_AND_DEVICE_ID").trim());

                    listArea.add(area);
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        if (listArea.size() == 0) {
            return null;
        }

        return listArea;
    }

    public PagerOfAlarm getAlarmPagerOfInterval(String strStationID, String strDeviceID, final String strBeginDate, final String strEndDate, String strAlarmType, String strCurrentPage, String strLevel, String strChooseName) {

        if (strCurrentPage == null || "".equals(strCurrentPage.trim())) {
            return null;
        }

        List<AlarmRecordData> listAlarm = getAlarmRecordInterval(strStationID, strDeviceID, strBeginDate, strEndDate, strAlarmType, strLevel, strChooseName);

        int pageSize = Integer.parseInt(strCurrentPage);

        PagerOfAlarm pagerOfAlarm = new PagerOfAlarm(pageSize, 15, listAlarm);
        if (pagerOfAlarm.getDataList() == null) {
            List<AlarmRecordData> alarmRecordDatas = new ArrayList<>();
            pagerOfAlarm.setDataList(alarmRecordDatas);
        }

        return pagerOfAlarm;
    }

    public List<AlarmRecordData> getAlarmRecordInterval(String stationId, String deviceId, final String strBeginDate, final String strEndDate, String strAlarmType, String strLevel, String strChooseName) {

        String sqlHistory = "select * from user_tables where table_name = 'HISTORY_MATTERS_INFO'";
        jdbcTemplate.query(sqlHistory, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        if(tableExist == null || tableExist.isEmpty()) {
            return null;
        }

        String sqlOperation = "select * from user_tables where table_name = 'OPERATION_MATTERS_INFO'";
        jdbcTemplate.query(sqlOperation, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        if(tableExist == null || tableExist.isEmpty()) {
            return null;
        }

        String yearB = strBeginDate.substring(0, 4);
        String monthB = strBeginDate.substring(5, 7);
        String dayB = strBeginDate.substring(8, 10);

        String yearE = strEndDate.substring(0, 4);
        String monthE = strEndDate.substring(5, 7);
        String dayE = strEndDate.substring(8, 10);

        String strDateB = yearB + monthB + dayB;
        String strDateE = yearE + monthE + dayE;

        boolean bFlag = false;
        String sql = null;
        String sqlHistoryAll = null;
        String sqlOperationAll = null;

        List<AlarmRecordData> alarmRecordDatas = new ArrayList<>();

        int nLevel = Integer.parseInt(strLevel);
        switch (nLevel) {
            case 1:
                alarmRecordDatas = getAlarmRecordIntervalOfProvince(strBeginDate, strEndDate, strAlarmType);
                break;
            case 2:
                alarmRecordDatas = getAlarmRecordIntervalOfCity(strBeginDate, strEndDate, strAlarmType, strLevel, strChooseName);
                break;
            case 3:
                alarmRecordDatas = getAlarmRecordIntervalOfStation(stationId, strBeginDate, strEndDate, strAlarmType);
                break;
            case 4:
                alarmRecordDatas = getAlarmRecordIntervalOfDevice(stationId, deviceId, strBeginDate, strEndDate, strAlarmType);
                break;
            default:
                return null;
        }

        return alarmRecordDatas;
    }

    public List<AlarmRecordData> getAlarmRecordIntervalOfProvince(final String strBeginDate, final String strEndDate, String strAlarmType) {

        String yearB = strBeginDate.substring(0, 4);
        String monthB = strBeginDate.substring(5, 7);
        String dayB = strBeginDate.substring(8, 10);

        String yearE = strEndDate.substring(0, 4);
        String monthE = strEndDate.substring(5, 7);
        String dayE = strEndDate.substring(8, 10);

        String strDateB = yearB + monthB + dayB;
        String strDateE = yearE + monthE + dayE;

        boolean bFlag = false;
        String sql = null;
        String sqlHistoryAll = null;
        String sqlOperationAll = null;

        if (strAlarmType.compareTo("allAlarm") == 0) {
            bFlag = true;
            sqlHistoryAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + " order by YEAR_MONTH_DAY";
            sqlOperationAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + " order by YEAR_MONTH_DAY";

        } else if (strAlarmType.compareTo("switchAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + " and TYPE=100 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("measureAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + " and TYPE>=0 and TYPE<=6 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("operationAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + " and TYPE=200 order by YEAR_MONTH_DAY";
        } else {
            return null;
        }

        final List<AlarmRecordData> alarmRecordDatas = new ArrayList<>();

        if (!bFlag) {
            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else {
            try {
                jdbcTemplate.query(sqlHistoryAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

            try {
                jdbcTemplate.query(sqlOperationAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return alarmRecordDatas;
    }

    public List<AlarmRecordData> getAlarmRecordIntervalOfCity(final String strBeginDate, final String strEndDate, String strAlarmType, String strLevel, String strChooseName) {

        List<TreeDataOfAreaRelation> listArea = getAreaRelation();

        String strSqlWhereContion = "";

        for (int i = 0; i < listArea.size(); ++i) {
            if (listArea.get(i).getStrLevel().compareTo(strLevel) == 0 && listArea.get(i).getStrDescribe().compareTo(strChooseName) == 0) {

                String strStationID = listArea.get(i).getStrStationID();

                String [] strStationIDArray = strStationID.split(",");
                if (strStationIDArray.length == 0) {
                    return null;
                }

                boolean flag = false;
                for (int j = 0; j < strStationIDArray.length; ++j) {
                    strSqlWhereContion += " ";
                    if (j != strStationIDArray.length) {
                        if (!flag) {
                            strSqlWhereContion += " and STATION_ID=";
                            flag = true;
                        } else {
                            strSqlWhereContion += " or STATION_ID=";
                        }
                    }
                    strSqlWhereContion += strStationIDArray[j].trim();
                }

                break;
            }
        }

        if (strSqlWhereContion.isEmpty() || "".compareTo(strSqlWhereContion) == 0) {
            return null;
        }

        String yearB = strBeginDate.substring(0, 4);
        String monthB = strBeginDate.substring(5, 7);
        String dayB = strBeginDate.substring(8, 10);

        String yearE = strEndDate.substring(0, 4);
        String monthE = strEndDate.substring(5, 7);
        String dayE = strEndDate.substring(8, 10);

        String strDateB = yearB + monthB + dayB;
        String strDateE = yearE + monthE + dayE;

        boolean bFlag = false;
        String sql = null;
        String sqlHistoryAll = null;
        String sqlOperationAll = null;

        if (strAlarmType.compareTo("allAlarm") == 0) {
            bFlag = true;
            sqlHistoryAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strSqlWhereContion + " order by YEAR_MONTH_DAY";
            sqlOperationAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strSqlWhereContion + " order by YEAR_MONTH_DAY";

        } else if (strAlarmType.compareTo("switchAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strSqlWhereContion + " and TYPE=100 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("measureAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strSqlWhereContion + " and TYPE>=0 and TYPE<=6 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("operationAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strSqlWhereContion + " and TYPE=200 order by YEAR_MONTH_DAY";
        } else {
            return null;
        }

        final List<AlarmRecordData> alarmRecordDatas = new ArrayList<>();

        if (!bFlag) {
            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else {
            try {
                jdbcTemplate.query(sqlHistoryAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

            try {
                jdbcTemplate.query(sqlOperationAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return alarmRecordDatas;
    }

    public List<AlarmRecordData> getAlarmRecordIntervalOfStation(String stationId, final String strBeginDate, final String strEndDate, String strAlarmType) {

        String strCondtion = " and STATION_ID=" + stationId;

        String yearB = strBeginDate.substring(0, 4);
        String monthB = strBeginDate.substring(5, 7);
        String dayB = strBeginDate.substring(8, 10);

        String yearE = strEndDate.substring(0, 4);
        String monthE = strEndDate.substring(5, 7);
        String dayE = strEndDate.substring(8, 10);

        String strDateB = yearB + monthB + dayB;
        String strDateE = yearE + monthE + dayE;

        boolean bFlag = false;
        String sql = null;
        String sqlHistoryAll = null;
        String sqlOperationAll = null;

        if (strAlarmType.compareTo("allAlarm") == 0) {
            bFlag = true;
            sqlHistoryAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strCondtion +" order by YEAR_MONTH_DAY";
            sqlOperationAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strCondtion +" order by YEAR_MONTH_DAY";

        } else if (strAlarmType.compareTo("switchAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strCondtion +" and TYPE=100 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("measureAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strCondtion +" and TYPE>=0 and TYPE<=6 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("operationAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strCondtion +" and TYPE=200 order by YEAR_MONTH_DAY";
        } else {
            return null;
        }

        final List<AlarmRecordData> alarmRecordDatas = new ArrayList<>();

        if (!bFlag) {
            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else {
            try {
                jdbcTemplate.query(sqlHistoryAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

            try {
                jdbcTemplate.query(sqlOperationAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return alarmRecordDatas;
    }

    public List<AlarmRecordData> getAlarmRecordIntervalOfDevice(String stationId, String deviceId, final String strBeginDate, final String strEndDate, String strAlarmType) {

        String strContion = " and STATION_ID=" + stationId + " and EQUIPMENT_ID=" + deviceId;

        String yearB = strBeginDate.substring(0, 4);
        String monthB = strBeginDate.substring(5, 7);
        String dayB = strBeginDate.substring(8, 10);

        String yearE = strEndDate.substring(0, 4);
        String monthE = strEndDate.substring(5, 7);
        String dayE = strEndDate.substring(8, 10);

        String strDateB = yearB + monthB + dayB;
        String strDateE = yearE + monthE + dayE;

        boolean bFlag = false;
        String sql = null;
        String sqlHistoryAll = null;
        String sqlOperationAll = null;

        if (strAlarmType.compareTo("allAlarm") == 0) {
            bFlag = true;
            sqlHistoryAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strContion +" order by YEAR_MONTH_DAY";
            sqlOperationAll = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strContion +" order by YEAR_MONTH_DAY";

        } else if (strAlarmType.compareTo("switchAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strContion +" and TYPE=100 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("measureAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from HISTORY_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strContion +" and TYPE>=0 and TYPE<=6 order by YEAR_MONTH_DAY";
        } else if (strAlarmType.compareTo("operationAlarm") == 0) {
            sql = "select YEAR_MONTH_DAY,HOUR_MINUTE_SEC_MS,DESCRIBE,TYPE from OPERATION_MATTERS_INFO where rownum <=" +nRownum+ " and YEAR_MONTH_DAY>=" + strDateB + " and YEAR_MONTH_DAY<=" + strDateE + strContion +" and TYPE=200 order by YEAR_MONTH_DAY";
        } else {
            return null;
        }

        final List<AlarmRecordData> alarmRecordDatas = new ArrayList<>();

        if (!bFlag) {
            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else {
            try {
                jdbcTemplate.query(sqlHistoryAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

            try {
                jdbcTemplate.query(sqlOperationAll, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        // 获取设备下所有的点
                        try {
                            AlarmRecordData recordData = new AlarmRecordData();
                            String strYMD = resultSet.getString("YEAR_MONTH_DAY");
                            String strHMSM = resultSet.getString("HOUR_MINUTE_SEC_MS");

                            String strTime = (strYMD.substring(0, 4) + "-" + strYMD.substring(4, 6) + "-" + strYMD.substring(6, 8)) + " ";

                            long hmsms = Long.parseLong(strHMSM);

                            int Hour = (int)(hmsms/(100l*100l*1000l));
                            int Min = (int)((hmsms%(100l*100l*1000l))/(100l*1000l));
                            int Sec = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))/1000l);
                            int Ms = (int)(((hmsms%(100l*100l*1000l))%(100l*1000l))%1000l);

                            strTime += String.format("%02d:%02d:%02d %03d", Hour, Min, Sec, Ms);
                            String strDescribe = resultSet.getString("DESCRIBE");
                            int nType = resultSet.getInt("TYPE");

                            recordData.setTime(strTime);
                            recordData.setDescribe(strDescribe);
                            recordData.setType(nType);

                            alarmRecordDatas.add(recordData);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return alarmRecordDatas;
    }

    public List<His_AI_Day_Display> getHistoryDataOfDay(String stationId, String deviceId, final String strDate, String strAlarmType) {

        long sdate = getDays(strDate, "1970-01-01");//1970-1-1至今的天数

        String year = strDate.substring(0, 4);
        //String month = monthReportTime.substring(5);
        //final int days = getMonthDay(monthReportTime);
        //final long sdateBegin = getDays(getLastMonthDay(monthReportTime), "1970-1-1") + 1; //本月第一天的sdate
        //final long sdateEnd = sdateBegin + days - 1;//本月最后一天的sdate


        String tableName = null;
        int stationID = Integer.parseInt(stationId);
        int deviceID = Integer.parseInt(deviceId);
        int paraID = -1;

        tableName = "HIS_AI_DS" + year;

        final List<His_AI_Day_Display> listHisAiDayDisplay = new ArrayList<>();

        final Vector<His_Ai_Day> vecHisAiDayMax = new Vector<His_Ai_Day>();

        final Vector<His_Ai_Day> vecHisAiDayMin = new Vector<His_Ai_Day>();

        String sqlTableName = "select * from user_tables where table_name = '" + tableName + "'";
        tableExist = null;
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        if(tableExist == null || tableExist.isEmpty()) {
            return null;
        }

        String sqlMax;
        String sqlMin;
        switch (strAlarmType) {
            case "electric":
                sqlMax = "select AI_ID,DATA,SAVE_TIME from " + tableName + " where STATION_ID=" + stationId + " and DEVICE_ID=" + deviceId + " and AI_ID>=1" + " and AI_ID<=3" + " and SDATE=" + sdate + " and FLAG=150 order by AI_ID";
                sqlMin = "select AI_ID,DATA,SAVE_TIME from " + tableName + " where STATION_ID=" + stationId + " and DEVICE_ID=" + deviceId + " and AI_ID>=1" + " and AI_ID<=3" + " and SDATE=" + sdate + " and FLAG=151 order by AI_ID";
                break;
            case "voltage":
                sqlMax = "select AI_ID,DATA,SAVE_TIME from " + tableName + " where STATION_ID=" + stationId + " and DEVICE_ID=" + deviceId + " and AI_ID>=5" + " and AI_ID<=7" + " and SDATE=" + sdate + " and FLAG=150 order by AI_ID";
                sqlMin = "select AI_ID,DATA,SAVE_TIME from " + tableName + " where STATION_ID=" + stationId + " and DEVICE_ID=" + deviceId + " and AI_ID>=5" + " and AI_ID<=7" + " and SDATE=" + sdate + " and FLAG=151 order by AI_ID";
                break;
            case "activeP":
                sqlMax = "select AI_ID,DATA,SAVE_TIME from " + tableName + " where STATION_ID=" + stationId + " and DEVICE_ID=" + deviceId + " and AI_ID>=9" + " and AI_ID<=11" + " and SDATE=" + sdate + " and FLAG=150 order by AI_ID";
                sqlMin = "select AI_ID,DATA,SAVE_TIME from " + tableName + " where STATION_ID=" + stationId + " and DEVICE_ID=" + deviceId + " and AI_ID>=9" + " and AI_ID<=11" + " and SDATE=" + sdate + " and FLAG=151 order by AI_ID";
                break;
            case "activeQ":
                sqlMax = "select AI_ID,DATA,SAVE_TIME from " + tableName + " where STATION_ID=" + stationId + " and DEVICE_ID=" + deviceId + " and AI_ID>=12" + " and AI_ID<=14" + " and SDATE=" + sdate + " and FLAG=150 order by AI_ID";
                sqlMin = "select AI_ID,DATA,SAVE_TIME from " + tableName + " where STATION_ID=" + stationId + " and DEVICE_ID=" + deviceId + " and AI_ID>=12" + " and AI_ID<=14" + " and SDATE=" + sdate + " and FLAG=151 order by AI_ID";
                break;
            case "activeF":
                sqlMax = "select AI_ID,DATA,SAVE_TIME from " + tableName + " where STATION_ID=" + stationId + " and DEVICE_ID=" + deviceId + " and AI_ID>=15" + " and AI_ID<=17" + " and SDATE=" + sdate + " and FLAG=150 order by AI_ID";
                sqlMin = "select AI_ID,DATA,SAVE_TIME from " + tableName + " where STATION_ID=" + stationId + " and DEVICE_ID=" + deviceId + " and AI_ID>=15" + " and AI_ID<=17" + " and SDATE=" + sdate + " and FLAG=151 order by AI_ID";
                break;
            default :
                return null;
        }

        try {
            jdbcTemplate.query(sqlMax, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    // 获取设备下所有的点
                    try {
                        His_Ai_Day aiDay = new His_Ai_Day();
                        int nID = resultSet.getInt("AI_ID");
                        Double value = resultSet.getDouble("DATA");
                        int nTime = resultSet.getInt("SAVE_TIME");
                        int nHour = nTime/60;
                        int minute = nTime%60;
                        String strTime = nHour + ":" + minute;

                        aiDay.setId(nID);
                        aiDay.setValue(value);
                        aiDay.setTime(strTime);

                        vecHisAiDayMax.add(aiDay);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        try {
            jdbcTemplate.query(sqlMin, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    // 获取设备下所有的点
                    try {
                        His_Ai_Day aiDay = new His_Ai_Day();
                        int nID = resultSet.getInt("AI_ID");
                        Double value = resultSet.getDouble("DATA");
                        int nTime = resultSet.getInt("SAVE_TIME");
                        int nHour = nTime/60;
                        int minute = nTime%60;
                        String strTime = nHour + ":" + minute;

                        aiDay.setId(nID);
                        aiDay.setValue(value);
                        aiDay.setTime(strTime);

                        vecHisAiDayMin.add(aiDay);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        for (int i = 0; i < vecHisAiDayMax.size(); i++) {
            His_Ai_Day hisAiDayMax = vecHisAiDayMax.get(i);
            int maxId = hisAiDayMax.getId();

            for (int j = 0; j < vecHisAiDayMin.size(); j++) {
                His_Ai_Day hisAiDayMin = vecHisAiDayMin.get(j);
                int minId = hisAiDayMin.getId();
                if (maxId == minId) {
                    His_AI_Day_Display hisAiDayDisplay = new His_AI_Day_Display();

                    hisAiDayDisplay.setMaxValue(hisAiDayMax.getValue());
                    hisAiDayDisplay.setMaxTime(hisAiDayMax.getTime());
                    hisAiDayDisplay.setMinValue(hisAiDayMin.getValue());
                    hisAiDayDisplay.setMinTime(hisAiDayMin.getTime());

                    listHisAiDayDisplay.add(hisAiDayDisplay);
                }
            }
        }

        return listHisAiDayDisplay;
    }

    // 获得当前时间所在月份的上个月的最后一天所在日期
    public String getLastMonthDay(String monthReportTime) {
        Calendar cal = Calendar.getInstance();
        String strMonth = monthReportTime.substring(0, 4);
        int Year = Integer.parseInt(monthReportTime.substring(0, 4));
        int Month = Integer.parseInt(monthReportTime.substring(5)) - 1; // 上个月月份
        //设置年月
        if (Month == 1) {
            Year = Year - 1;
            Month = 12;
        } else {
            Year = Year;
        }
        //设置天数
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM");
        String lastMonth = Year +"-" + Month;
        Date d = null;

        try {

            d = format.parse(lastMonth);
        } catch (ParseException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        cal.setTime(d);
        int day =cal.getActualMaximum(Calendar.DAY_OF_MONTH);
        String endDay = Year + "-" + Month + "-" + day;
        return endDay;
    }



    //得到某月的天数
    public int getMonthDay(String monthReportTime){
        //String source = "2007年12月";
        int count=30;
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM");
        try {
            Date date = format.parse(monthReportTime);
            Calendar calendar = new GregorianCalendar();
            calendar.setTime(date);
            count=calendar.getActualMaximum(Calendar.DAY_OF_MONTH);
        }catch (Exception e) {
            e.printStackTrace();
        }
        return count;
    }


    /**
     * 两个时间之间的天数
     *
     * @param enddate
     * @param begindate
     * @return
     */
    public long getDays(String enddate, String begindate) {
        if (enddate == null || enddate.equals(""))
            return 0;
        if (begindate == null || enddate.equals(""))
            return 0;
        // 转换为标准时间
        SimpleDateFormat myFormatter = new SimpleDateFormat("yyyy-MM-dd");
        java.util.Date date = null;
        java.util.Date mydate = null;
        try {
            date = myFormatter.parse(enddate);
            mydate = myFormatter.parse(begindate);
        } catch (Exception e) {
        }
        long day = (date.getTime() - mydate.getTime()) / (24 * 60 * 60 * 1000);
        return day;
    }

    private int ParseParaId(String PsrId, int beginIndex){
        //截取掉从首字母起长度为beginIndex的字符串
        int ParaId = Integer.parseInt(PsrId.substring(beginIndex));
        return ParaId;
    }


//    /**
//     * 请求报表导出的历史数据
//     */
//    public Integer getreportExportData(){
//        HSSFWorkbook wb = new HSSFWorkbook();
//        String sheetName = "test1";
//        HSSFSheet sheet = wb.createSheet(sheetName);
//        List<CellRangeAddress> titleRanges = new ArrayList<>();
//        List<String> columnNames = new ArrayList<>();
//        columnNames.add("测点\\时间");
//        columnNames.add("6:00");
//        columnNames.add("7:00");
//        columnNames.add("8:00");
//        columnNames.add("9:00");
//        columnNames.add("10:00");
//        columnNames.add("11:00");
//        columnNames.add("12:00");
//        columnNames.add("13:00");
//        columnNames.add("14:00");
//        columnNames.add("15:00");
//        columnNames.add("16:00");
//        columnNames.add("17:00");
//        columnNames.add("18:00");
//        columnNames.add("19:00");
//        columnNames.add("20:00");

//        for (int i = 0; i < 5; i++){//创建合并单元格区域
//            CellRangeAddress titleRange = new CellRangeAddress(0, 1, i, i);
//            titleRanges.add(titleRange);
//            sheet.addMergedRegion(titleRange);
//        }
//        CellRangeAddress titleRange = new CellRangeAddress(1, 1, 5, 8);
//        sheet.addMergedRegion(titleRange);
//        // 创建标题栏样式
//        HSSFCellStyle styleTitle = wb.createCellStyle();
//        styleTitle.setAlignment(HSSFCellStyle.ALIGN_CENTER);// 居中
//        HSSFFont fontTitle = wb.createFont();
//        // 创建一行
//        HSSFRow rowTitle = sheet.createRow(0);
//        // 创建第二行
//        HSSFRow rowTitleDesc = sheet.createRow(1);
//        HSSFCell cellTitleDesc = rowTitleDesc.createCell(5);
//        cellTitleDesc.setCellValue("以上数据按小时导出（时间标注峰谷平）");
//        cellTitleDesc.setCellStyle(styleTitle);
//        // 微软雅黑字体
//        fontTitle.setFontName("微软雅黑");
//        fontTitle.setFontHeight((short) 200);
//        styleTitle.setFont(fontTitle);
//        //    styleTitle.setFillPattern(HSSFCellStyle.BIG_SPOTS);//填充方式
//        styleTitle.setFillBackgroundColor(HSSFColor.LIGHT_YELLOW.index);//设置背景色
//        // 在行上创建11列,填充响应的值
//        int columnNum = columnNames.size();
//        for (int j = 0; j < columnNum; j++){
//            HSSFCell cellTitle = rowTitle.createCell(j);
//            // 列标题及样式
//            cellTitle.setCellValue(columnNames.get(j));
//            cellTitle.setCellStyle(styleTitle);
//        }
//        //设置单元格自动适应文本大小
//        for (int j = 0; j < columnNum; j++) {
//            sheet.autoSizeColumn(j, true);
//        }
//        try{
//            FileOutputStream fout = new FileOutputStream("D:/awb.xls");
//            wb.write(fout);
//            fout.close();
//            wb.close();
//            System.out.println("导出完成！");
//        }catch (Exception e){
//        }
//
//        return 1;
//
//    }


    /**
     * 根据设备ID获取设备的遥测信息
     * @param stationId 厂站id
     * @param deviceId 设备id
     * @return 遥测信息
     */
    public List<BasePSR> getAIsofDevice(String stationId,String deviceId) {
        final List<BasePSR> AIs = new ArrayList<>();
        //String sql = "select 代码 from 遥测参数表 where 设备代码='" + id + "' and 类型 = 150";
        //AI_TAG为测点单位
        String sql = "select AI_ID,DESCRIBE,AI_TAG from SCD_AI where STATION_ID='" + stationId + "' and DEVICE_ID='" + deviceId + "' order by AI_ID";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    /*BasePSR basePSR = new BasePSR(resultSet.getString("代码").trim(), resultSet.getString("描述").trim()
                            , resultSet.getString("类型").trim());*/
                    //DEVICE_TAG暂未启用
                    BasePSR AI = new BasePSR(resultSet.getString("AI_ID").trim(), resultSet.getString
                            ("DESCRIBE").trim(),resultSet.getString("AI_TAG").trim());
                    AIs.add(AI);
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        return AIs;
    }

    /**
     * 根据设备ID获取设备的电度信息
     * @param stationId 厂站id
     * @param deviceId 设备id
     * @return 电度信息
     */
    public List<BasePSR> getKWHsofDevice(String stationId,String deviceId) {
        final List<BasePSR> KWHs = new ArrayList<>();
        //String sql = "select 代码 from 遥测参数表 where 设备代码='" + id + "' and 类型 = 150";
        //DD_TAG为测点单位
        String sql = "select DD_ID,DESCRIBE,DD_TAG from SCD_DD where STATION_ID='" + stationId + "' and DEVICE_ID='" + deviceId + "' order by DD_ID";
        //String sql = "select DD_ID,DESCRIBE,DD_TAG from SCD_DD where DEVICE_ID='" + id + "'";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    /*BasePSR basePSR = new BasePSR(resultSet.getString("代码").trim(), resultSet.getString("描述").trim()
                            , resultSet.getString("类型").trim());*/
                    //DEVICE_TAG暂未启用
                    BasePSR KWH = new BasePSR(resultSet.getString("DD_ID").trim(), resultSet.getString
                            ("DESCRIBE").trim(),resultSet.getString("DD_TAG").trim());
                    KWHs.add(KWH);
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        return KWHs;
    }


    /**
     * 根据设备ID获取设备的计算量信息
     * @param stationId 厂站id
     * @param deviceId 设备id
     * @return 计算量信息
     */
    public List<BasePSR> getCALCsofDevice(String stationId,String deviceId) {
        final List<BasePSR> CALCs = new ArrayList<>();
        //String sql = "select 代码 from 遥测参数表 where 设备代码='" + id + "' and 类型 = 150";
        //AI_TAG为测点单位
        String sql = "select AI_ID,DESCRIBE,AI_TAG from SCD_CALCULATION where STATION_ID='" + stationId + "' and DEVICE_ID='" + deviceId + "' order by AI_ID";
        //String sql = "select AI_ID,DESCRIBE,AI_TAG from SCD_CALCULATION where DEVICE_ID='" + id + "'";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    /*BasePSR basePSR = new BasePSR(resultSet.getString("代码").trim(), resultSet.getString("描述").trim()
                            , resultSet.getString("类型").trim());*/
                    //DEVICE_TAG暂未启用
                    BasePSR Calc = new BasePSR(resultSet.getString("AI_ID").trim(), resultSet.getString
                            ("DESCRIBE").trim(),resultSet.getString("AI_TAG").trim());
                    CALCs.add(Calc);
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        return CALCs;
    }





    /**
     * 获取阵列或分布式厂站,取的是SCD设备参数表中的厂站
     * @return 列表
     */
    public List getArray() {
        final List arrays = new ArrayList();
        // select 代码,描述 from scd设备参数表 where 类型=52 and 站所线系代码 in (select 代码 from 站所线系参数表 where 类型=2)
        //String sql = String.format("select 代码,描述 from scd设备参数表 where 类型=%1$s and 站所线系代码 in (select 代码 from 站所线系参数表 where 类型=2)", PSRTypeConstant.ARRAY);
        String sql = String.format("select 代码,描述 from scd设备参数表 where 类型=%1$s and 站所线系代码 in (select station_id from SCD_STATION where station_id=1 and group_id=1)", PSRTypeConstant.ARRAY);

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    arrays.add(new BasePSR(resultSet.getString("代码").trim(), resultSet.getString("描述").trim()));
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return arrays;
    }

    /**
     * 获取阵列或分布式厂站中指定类型的设备
     * @param arrayID 阵列或分布式厂站
     * @return 列表
     */
    public List getPSRFromArray(String arrayID, Integer type) {
        final List ret = new ArrayList();

        String sql = String.format("select 代码, 描述 from SCD设备参数表 where 类型=%1$d and 站所线系代码 in \n" +
                "                                            (select 站所线系代码 from SCD设备参数表 where 代码='%2$s')", type, arrayID);

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    ret.add(new BasePSR(resultSet.getString("代码").trim(), resultSet.getString("描述").trim()));
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        return ret;
    }

    /**
     * 获取一个设备下挂的子设备
     * @param psrID 设备ID
     * @param subType 子设备类型
     * @return 列表
     */
    public List getChildren(String psrID, Integer subType) {
        final List ret = new ArrayList();

        String sql = String.format("select 代码,描述 from scd设备参数表 where 类型=%1$d and 所属馈线='%2$s'", subType, psrID);

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    ret.add(new BasePSR(resultSet.getString("代码").trim(), resultSet.getString("描述").trim()));
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        return ret;
    }

    public WeatherInstrument GetWeatherInstrument(){
        final List<WeatherInstrument> weatherInstruments = new ArrayList<WeatherInstrument>();
        String sql = "select * from scd设备参数表 where 类型=29 and 站所线系代码 in (select 代码 from 站所线系参数表 where 描述<>'系统站')" ;

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    WeatherInstrument weatherInstrument = new WeatherInstrument();
                    weatherInstrument.setId(resultSet.getString("代码").trim());
                    weatherInstrument.setName(resultSet.getString("描述").trim());
                    weatherInstruments.add(weatherInstrument);
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        return weatherInstruments.isEmpty() ? null : weatherInstruments.get(0);
    }


//    public List<BasePSR> GetPVArrayDevices(String arrayID){
//        final List<BasePSR> basePSRs = new ArrayList<BasePSR>();
//        final int[] typeArray = {22,27,26};
//        String sql;
//        //以箱变、逆变器、汇流箱排列
//        try {
//            for (int i = 0; i < 3; i++) {
//                sql = "select 代码,描述 from scd设备参数表 where 站所线系代码='" + arrayID + "' and 类型=" + Integer.toString(typeArray[i]) ;
//                    jdbcTemplate.query(sql, new RowCallbackHandler() {
//                        public void processRow(ResultSet resultSet) throws SQLException {
//                            BasePSR basePSR = new BasePSR();
//                            basePSR.setId(resultSet.getString("代码").trim());
//                            basePSR.setName(resultSet.getString("描述").trim());
//                            basePSRs.add(basePSR);
//                        }
//                    });
//                }
//            } catch (Exception e) {
//                e.printStackTrace();
//        }
//
//        return basePSRs;
//    }

    @Deprecated
    public PackageTransformer GetPackageTransformer(String arrayID){
        final List<PackageTransformer> packageTransformers = new ArrayList<PackageTransformer>();
        String sql = "select 代码,描述 from scd设备参数表 where 站所线系代码='" + arrayID + "' and 类型=22 and (所属馈线 is null or 所属馈线=' ')";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    PackageTransformer packageTransformer = new PackageTransformer();
                    packageTransformer.setId(resultSet.getString("代码").trim());
                    packageTransformer.setName(resultSet.getString("描述").trim());
                    packageTransformers.add(packageTransformer);
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        return packageTransformers.isEmpty() ? null : packageTransformers.get(0);
    }


//    /**
//     * 获取一个阵列,或一个分布式厂站的逆变器列表
//     * @param arrayID 阵列或厂站的ID
//     * @return
//     */
//    public List<BasePSR> getInverters(String arrayID){
//        final List<BasePSR> inverters = new ArrayList();
//        String sql = "select 代码,描述 from scd设备参数表 where 站所线系代码='" + arrayID + "' and 类型=27 order by 描述";
//
//        try {
//            jdbcTemplate.query(sql, new RowCallbackHandler() {
//                public void processRow(ResultSet resultSet) throws SQLException {
//                    Inverter inverter = new Inverter();
//                    inverter.setId(resultSet.getString("代码").trim());
//                    inverter.setName(resultSet.getString("描述").trim());
//                    inverters.add(inverter);
//                }
//            });
//
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
//
//        return inverters;
//    }


//    public List<BasePSR> GetJunctions(String arrayID){
//        final List<BasePSR> junctions = new ArrayList();
//        String sql = "select 代码,描述 from scd设备参数表 where 站所线系代码='" + arrayID + "' and 类型=26 order by 描述";
//
//        try {
//            jdbcTemplate.query(sql, new RowCallbackHandler() {
//                public void processRow(ResultSet resultSet) throws SQLException {
//                    Junction junction = new Junction();
//                    junction.setId(resultSet.getString("代码").trim());
//                    junction.setName(resultSet.getString("描述").trim());
//                    junctions.add(junction);
//                }
//            });
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
//
//        return junctions;
//    }

    public String  GetPVArray(String id){
        final List<String> pvArraies = new ArrayList<String>();
        String sql = "select 站所线系代码 from scd设备参数表 where 代码='" + id + "'";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    pvArraies.add(resultSet.getString("站所线系代码").trim());
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return pvArraies.isEmpty() ? "":pvArraies.get(0);
    }

    public String getDeviceName(String id) {
        final List<String> names = new ArrayList<String>();
        String sql = "select 描述 from SCD设备参数表 where 代码='" + id + "'";

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

    /**
     * 获取汇流箱矩阵
     * @Title:getMatrixs
     * @param:站所线系代码arrayID
     * @return:汇流箱矩阵数据
     */
    public List<MeasurementInfo> getMatrixs(final String arrayID){
        final List<MeasurementInfo> Matrixs = new ArrayList<MeasurementInfo>();
        String sql = "select 代码,描述 from 遥测参数表 where 设备代码 in (select 代码 from scd设备参数表 where 站所线系代码 = '" + arrayID + "' and 类型 = 25) order by 遥测序号" ;

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    MeasurementInfo measureInfo = new MeasurementInfo();
                    measureInfo.setPsrID(arrayID);
                    measureInfo.setMeasurementID(resultSet.getString("代码").trim());
                    measureInfo.setMeasurementType(0);
                    measureInfo.setMagic("");
                    measureInfo.setTemplateIndex(getNameNumber(resultSet.getString("描述").trim()));
                    Matrixs.add(measureInfo);
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return Matrixs;
    }
    public Integer getNameNumber(String id)
    {
        List<String> mystrList = new ArrayList<String>();
        String regex = "\\d*";
        Pattern p = Pattern.compile(regex);

        Matcher m = p.matcher(id);

        while (m.find()) {
            if (!"".equals(m.group()))
                mystrList.add(m.group());
        }
        Integer index = 0;
        if (mystrList.get(0).length() != 0 && mystrList.get(1).length() != 0)
        {
            int no1 = Integer.parseInt(mystrList.get(0));
            int no2 = Integer.parseInt(mystrList.get(1));
            index = (no1-1)*16 + no2;
        }
        return index;
    }

    public List<Junction> GetChildJunctions(String id){
        final List<Junction> junctions = new ArrayList<Junction>();
        String sql = "select 代码,描述 from scd设备参数表 where 所属馈线='" + id + "' and 类型=26 order by 描述";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    Junction junction = new Junction();
                    junction.setId(resultSet.getString("代码").trim());
                    junction.setName(resultSet.getString("描述").trim());
                    junctions.add(junction);
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return junctions;
    }

   public List<PackageTransformer> GetChildTransformers(String id){
        final List<PackageTransformer> packageTransformers = new ArrayList<PackageTransformer>();
        String sql = "select 代码,描述 from scd设备参数表 where 所属馈线='" + id + "' and 类型=22 order by 描述";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    PackageTransformer packageTransformer = new PackageTransformer();
                    packageTransformer.setId(resultSet.getString("代码").trim());
                    packageTransformer.setName(resultSet.getString("描述").trim());
                    packageTransformers.add(packageTransformer);
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return packageTransformers;
    }

    public String getEnergyId(String id){
        final List<String> ids = new ArrayList<String>();
        //String sql = "select 代码 from 电度参数表 where 设备代码 ='" + id + "' and 类型=200 order by 电度序号";
        String sql = "select DD_ID from SCD_DD where DEVICE_ID ='" + id + "' order by DD_ID";
        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    ids.add(resultSet.getString("DD_ID").trim());
                    //ids.add(resultSet.getString("代码").trim());

                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
        return ids.isEmpty() ? "":ids.get(0);
    }

    public String getIfromJunction(String id) {
        final List<String> ids = new ArrayList<String>();
        //String sql = "select 代码 from 遥测参数表 where 设备代码='" + id + "' and 类型 = 150";
        String sql = "select AI_ID from SCD_AI where DEVICE_ID='" + id + "'";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    ids.add(resultSet.getString("AI_ID").trim());
                    //ids.add(resultSet.getString("代码").trim());
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return ids.isEmpty() ? "":ids.get(0);
    }
    public String getUfromJunction(String id) {
        final List<String> ids = new ArrayList<String>();
        //String sql = "select 代码 from 遥测参数表 where 设备代码='" + id + "' and 类型 = 150";
        String sql = "select AI_ID from SCD_AI where DEVICE_ID='" + id + "'";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    ids.add(resultSet.getString("AI_ID").trim());
                    //ids.add(resultSet.getString("代码").trim());
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return ids.isEmpty() ? "":ids.get(0);
    }

    /**
     * 获取指定设备类型的并网柜记录表的指定数据类型代码
     * @Title:getCombinedCabName
     * @param:
     * @return:电度代码
     */
    public String getCombinedCabName(String stationId, int scdType, int dataType){
        final List<String> names = new ArrayList<String>();
        String sql = "select 代码 from 电度参数表 where 设备代码 in (select 代码 from scd设备参数表 where 站所线系代码 = '" + stationId + "' and 类型 = " + scdType + " ) and 类型 =  " + dataType + " order by 电度序号";
        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    names.add(resultSet.getString("代码").trim());
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
        return names.isEmpty() ? "":names.get(0);
    }

    /**
     * 获取指定厂站指定类型的设备列表
     * @Title:getCombinedCabName
     * @param:
     * @return:电度代码
     */
    public List<BasePSR> getScdDevices(String stationId, int scdType){
        final List<BasePSR> basePSRList = new ArrayList<>();
        String sql = "select * from scd设备参数表 where 站所线系代码 = '" + stationId + "' and 类型 = " + scdType ;
        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    BasePSR basePSR = new BasePSR();
                    basePSR.setId(resultSet.getString("代码").trim());
                    basePSR.setName(resultSet.getString("描述").trim());
                    basePSR.setArea("");
                    basePSRList.add(basePSR);
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
        return basePSRList;
    }
}
