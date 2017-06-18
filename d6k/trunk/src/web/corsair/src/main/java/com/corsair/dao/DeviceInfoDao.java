package com.corsair.dao;

import com.corsair.device.*;
import com.corsair.service.TreeDataOfAreaRelation;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowCallbackHandler;
import org.springframework.stereotype.Repository;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

/**
 * Created by zyj on 2016/12/2.
 */
@Repository("DeviceInfoDao")
public class DeviceInfoDao {

    private String tableExist = null;

    // ai表名
    private String aiTableName = "SCD_AI";

    // di表名
    private String diTableName = "SCD_DI";

    // calc表名
    private String calcTableName = "SCD_CALCULATION";

    // 对应的计算量厂站ID
    int nCalcStationId = 0;

    // 对应的计算量设备ID
    int nCalcDeviceId = 0;

    // 表中data数据
    float data;

    @Autowired
    private JdbcTemplate jdbcTemplate;

    /**
     * 从数据库中查找设备信息
     *
     * @param stationId  厂站ID
     * @param deviceId   设备ID
     * @return 返回设备的ai,di,calc信息
     */
    public DeviceInfo getDeviceInfo(String stationId, String deviceId) {

        String tableName = null;
        int stationID = Integer.parseInt(stationId);
        int deviceID = Integer.parseInt(deviceId);

        DeviceInfo deviceInfo = new DeviceInfo();

        deviceInfo.setAiList(getAiInfo(stationId, deviceId));

        deviceInfo.setDiList(getDiInfo(stationId, deviceId));

        deviceInfo.setCalcList(getCalcInfo(stationId, deviceId));


        return deviceInfo;

    }

    public List<AiInfo> getAiInfo(String stationId, String deviceId) {

        int stationID = Integer.parseInt(stationId);
        int deviceID = Integer.parseInt(deviceId);

        final List<AiInfo> aiDataList = new ArrayList<>();

        String sqlTableName = "select * from user_tables where table_name = '" + aiTableName + "'";
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        // 表名不存在返回null
        if (tableExist == null || tableExist.isEmpty()) {
            return  null;
        }

        String sql = null;
        sql = "select STATION_ID, DEVICE_ID, AI_ID, AI_TAG, DESCRIBE from " + aiTableName + " where STATION_ID=" + stationID + " and DEVICE_ID=" +deviceID + " ORDER BY AI_ID";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    AiInfo info = new AiInfo();

                    try {

                        String strStationID = resultSet.getString("STATION_ID");
                        String strDeviceID = resultSet.getString("DEVICE_ID");
                        String strID = resultSet.getString("AI_ID");
                        String strUnit = resultSet.getString("AI_TAG");
                        String strDescribe = resultSet.getString("DESCRIBE").trim();

                        String strMeasurements = strStationID + "-" + strDeviceID + "-" + strID + "-" + "0200";

                        info.setName(strDescribe);
                        info.setCode(strMeasurements);
                        info.setUnit(strUnit);

                    } catch (Exception e) {
                        // log
                    }


                    aiDataList.add(info);
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return aiDataList;
    }

    public List<DiInfo> getDiInfo(String stationId, String deviceId) {

        int stationID = Integer.parseInt(stationId);
        int deviceID = Integer.parseInt(deviceId);

        final List<DiInfo> diDataList = new ArrayList<>();

        String sqlTableName = "select * from user_tables where table_name = '" + diTableName + "'";
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        // 表名不存在返回null
        if (tableExist == null || tableExist.isEmpty()) {
            return  null;
        }

        String sql = null;
        sql = "select STATION_ID, DEVICE_ID, DI_ID, DESCRIBE from " + diTableName + " where STATION_ID=" + stationID + " and DEVICE_ID=" +deviceID + " ORDER BY DI_ID";;

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    DiInfo info = new DiInfo();

                    try {
                        String strStationID = resultSet.getString("STATION_ID");
                        String strDeviceID = resultSet.getString("DEVICE_ID");
                        String strID = resultSet.getString("DI_ID");
                        String strDescribe = resultSet.getString("DESCRIBE").trim();

                        String strMeasurements = strStationID + "-" + strDeviceID + "-" + strID + "-" + "0100";

                        info.setName(strDescribe);
                        info.setCode(strMeasurements);

                    } catch (Exception e) {
                        // log
                    }


                    diDataList.add(info);
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return diDataList;
    }

    public List<CalcInfo> getCalcInfo(String stationId, String deviceId) {

        int stationID = Integer.parseInt(stationId);
        int deviceID = Integer.parseInt(deviceId);

        final List<CalcInfo> calcDataList = new ArrayList<>();

        String sqlTableName = "select * from user_tables where table_name = '" + calcTableName + "'";
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        // 表名不存在返回null
        if (tableExist == null || tableExist.isEmpty()) {
            return  null;
        }

        String sql = null;
        sql = "select STATION_ID, DEVICE_ID, AI_ID, AI_TAG, DESCRIBE from " + calcTableName + " where STATION_ID=" + stationID + " and DEVICE_ID=" +deviceID + " ORDER BY AI_ID";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    CalcInfo info = new CalcInfo();

                    try {
                        String strStationID = resultSet.getString("STATION_ID");
                        String strDeviceID = resultSet.getString("DEVICE_ID");
                        String strID = resultSet.getString("AI_ID");
                        String strUnit = resultSet.getString("AI_TAG").trim();
                        String strDescribe = resultSet.getString("DESCRIBE").trim();

                        String strMeasurements = strStationID + "-" + strDeviceID + "-" + strID + "-" + "0400";

                        info.setName(strDescribe);
                        info.setCode(strMeasurements);
                        info.setUnit(strUnit);

                    } catch (Exception e) {
                        // log
                    }


                    calcDataList.add(info);
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return calcDataList;
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

    public Total_YMD_Info getTotalCalcInfo(String strName, String strType, String strDate) {

        List<TreeDataOfAreaRelation> listArea = getAreaRelation();

        Total_YMD_Info total_ymd_info = new Total_YMD_Info();

        total_ymd_info.setDayCalcList(getTotalCalcInfoDay(strName, strType, strDate, listArea));

        // 节电量等遥测里面做了日月年统计，暂时屏蔽从月统计表里查找
        //total_ymd_info.setMonthCalcList(getTotalCalcInfoMonth(strName, strType, strDate, listArea));

        return total_ymd_info;

    }

    /**
     * 保留3位小数
     * @param:f Double
     * @return:
     */
    public Float changeValue(Float f) {
        DecimalFormat df = new DecimalFormat("#.000");
        Float value =  Float.parseFloat(df.format(f));
        return value;
    }

    public List<CalcInfo> getTotalCalcInfoDay(String strName, String strType, String strDate, List<TreeDataOfAreaRelation> listArea) {

        if (listArea.size() == 0) {
            return null;
        }

        final long sdate = getDays(strDate, "1970-01-01");//1970-1-1至今的天数

        final String strYear = strDate.substring(0, 4);
        final int nYear = Integer.parseInt(strYear);
        String strMonth = strDate.substring(5, 7);
        final int nMonth = Integer.parseInt(strMonth);
        final List<CalcInfo> calcDataList = new ArrayList<>();

        final String tableName = "HIS_CALC_DS" + strYear;

        String sqlTableName = "select * from user_tables where table_name = '" + tableName + "'";
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        // 表名不存在返回null
        if (tableExist == null || tableExist.isEmpty()) {
            return  null;
        }

        // 对应的计算量厂站ID
        nCalcStationId = 0;

        // 对应的计算量设备ID
        nCalcDeviceId = 0;

        for (int i = 0; i < listArea.size(); ++i) {
            if (listArea.get(i).getStrLevel().compareTo(strType) == 0 && listArea.get(i).getStrDescribe().compareTo(strName) == 0) {

                String strStationAndDevice = listArea.get(i).getStrStationAndDeviceRelationID();

                String [] strStaDev = strStationAndDevice.split("-");
                if (strStaDev.length < 2) {
                    return null;
                }

                nCalcStationId = Integer.parseInt(strStaDev[0]);
                nCalcDeviceId = Integer.parseInt(strStaDev[1]);

                break;
            }
        }

        String sql = null;
        sql = "select DATA,CALC_ID from " + tableName + " where STATION_ID=" + nCalcStationId + " and DEVICE_ID=" + nCalcDeviceId + " and SDATE=" + sdate + " order by CALC_ID";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    CalcInfo info = new CalcInfo();

                    try {

                        data = resultSet.getFloat("DATA");
                        int nID = resultSet.getInt("CALC_ID");
                        if (nID == 7) { // 调容开关次数(日统计)

                            String sqlDay = null;
                            sqlDay = "select SDATE,DATA,CALC_ID from " + tableName + " where sdate in (select max(sdate) from " + tableName + " where sdate<" + sdate + ") and station_id=" + nCalcStationId + " and device_id=" + nCalcDeviceId + " and CALC_ID=7";
                            try {
                                jdbcTemplate.query(sqlDay, new RowCallbackHandler() {
                                    public void processRow(ResultSet resultSet) throws SQLException {
                                        CalcInfo infoReal = new CalcInfo();

                                        try {
                                            // 获取查询的日期前一次的值
                                            float fDataPrev = resultSet.getFloat("DATA");

                                            float dataReal = data - fDataPrev;
                                            dataReal = changeValue(dataReal);

                                            infoReal.setfValue(dataReal);

                                        } catch (Exception e) {

                                            infoReal.setfValue(0);
                                            calcDataList.add(6, infoReal);
                                        }

                                        calcDataList.add(6, infoReal);
                                    }
                                });
                            } catch (Exception e) {
                                info.setfValue(0);
                                calcDataList.add(6, info);
                            }

                        } else if (nID == 10) { // 调压开关次数(日统计)

                            String sqlDay = null;
                            sqlDay = "select SDATE,DATA,CALC_ID from " + tableName + " where sdate in (select max(sdate) from " + tableName + " where sdate<" + sdate + ") and station_id=" + nCalcStationId + " and device_id=" + nCalcDeviceId + " and CALC_ID=10";
                            try {
                                jdbcTemplate.query(sqlDay, new RowCallbackHandler() {
                                    public void processRow(ResultSet resultSet) throws SQLException {
                                        CalcInfo infoReal = new CalcInfo();

                                        try {
                                            // 获取查询的日期前一次的值
                                            float fDataPrev = resultSet.getFloat("DATA");

                                            float dataReal = data - fDataPrev;
                                            dataReal = changeValue(dataReal);

                                            infoReal.setfValue(dataReal);

                                        } catch (Exception e) {

                                            infoReal.setfValue(0);
                                            calcDataList.add(9, infoReal);
                                        }

                                        calcDataList.add(9, infoReal);
                                    }
                                });
                            } catch (Exception e) {
                                info.setfValue(0);
                                calcDataList.add(9, info);
                            }

                        } else {

                            data = changeValue(data);

                            info.setfValue(data);
                            calcDataList.add(info);
                        }

                    } catch (Exception e) {
                        info.setfValue(0);
                        calcDataList.add(info);
                    }
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        // 获取指定月份的最后一天。 xxxx-xx-xx
        String strBeginMonthDay = strYear+"-"+strMonth+"-"+"01";
        String strLastDayOfMonth = getLastDayOfMonth(nYear, nMonth);

        final long beginDay = getDays(strBeginMonthDay, "1970-01-01");//1970-1-1到月初的天数
        final long endDay = getDays(strLastDayOfMonth, "1970-01-01");//1970-1-1到月低的天数

        String sqlMonth = null;
        sqlMonth = "select SDATE,DATA,CALC_ID from "+tableName+" where sdate in (select max(sdate) from "+tableName+" where sdate>="+beginDay+" and sdate<="+endDay+") and station_id="+nCalcStationId+" and device_id="+nCalcDeviceId+" order by CALC_ID";
        try {
            jdbcTemplate.query(sqlMonth, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    int nID = resultSet.getInt("CALC_ID");
                    if (nID == 2 || nID == 5 || nID == 8 || nID == 11) {
                        CalcInfo info = new CalcInfo();

                        try {
                            // 获取查询的日期的数据
                            data = resultSet.getFloat("DATA");
                            switch (nID) {
                                case 2:
                                    data = changeValue(data);

                                    info.setfValue(data);

                                    calcDataList.remove(1);
                                    calcDataList.add(1, info);
                                    break;
                                case 5:
                                    data = changeValue(data);

                                    info.setfValue(data);

                                    calcDataList.remove(4);
                                    calcDataList.add(4, info);
                                    break;
                                case 8:
                                    String strLastDayOfMonth8 = "";

                                    int nPreYear = 0;

                                    String strNewTableName;
                                    if (nMonth == 1) {
                                        nPreYear = nYear - 1;
                                        strNewTableName = "HIS_CALC_DS" + nPreYear;
                                        strLastDayOfMonth8 = getLastDayOfMonth(nPreYear, 12);
                                    } else {
                                        strLastDayOfMonth8 = getLastDayOfMonth(nYear, nMonth-1);
                                        nPreYear = nYear;
                                        strNewTableName = tableName;
                                    }

                                    String strBeginMonthDay = nPreYear+"-"+(nMonth-1)+"-"+"01";

                                    final long beginDay = getDays(strBeginMonthDay, "1970-01-01");//1970-1-1到月初的天数
                                    final long endDay = getDays(strLastDayOfMonth8, "1970-01-01");//1970-1-1到月低的天数

                                    String sqlMonthCnt8 = null;
                                    sqlMonthCnt8 = "select SDATE,DATA,CALC_ID from " + strNewTableName + " where sdate in (select max(sdate) from " + strNewTableName + " where sdate>=" + beginDay + " and sdate<=" + endDay + ") and station_id=" + nCalcStationId + " and device_id=" + nCalcDeviceId + " and CALC_ID=8";
                                    try {
                                        jdbcTemplate.query(sqlMonthCnt8, new RowCallbackHandler() {
                                            public void processRow(ResultSet resultSet) throws SQLException {
                                                CalcInfo infoReal = new CalcInfo();

                                                try {
                                                    // 获取查询的日期前一次的值
                                                    float fDataPrev = resultSet.getFloat("DATA");

                                                    float dataReal = data - fDataPrev;
                                                    dataReal = changeValue(dataReal);

                                                    infoReal.setfValue(dataReal);

                                                } catch (Exception e) {

                                                    infoReal.setfValue(0);
                                                    calcDataList.add(7, infoReal);
                                                }

                                                calcDataList.remove(7);
                                                calcDataList.add(7, infoReal);
                                            }
                                        });
                                    } catch (Exception e) {
                                        info.setfValue(0);
                                        calcDataList.add(7, info);
                                    }

                                    break;
                                case 11:
                                    String strLastDayOfMonth11 = "";

                                    int nPreYear11 = 0;

                                    String strNewTableName11 = "";
                                    if (nMonth == 1) {
                                        nPreYear11 = nYear - 1;
                                        strNewTableName11 = "HIS_CALC_DS" + nPreYear11;
                                        strLastDayOfMonth11 = getLastDayOfMonth(nPreYear11, 12);
                                    } else {
                                        strLastDayOfMonth11 = getLastDayOfMonth(nYear, nMonth-1);
                                        nPreYear11 = nYear;
                                        strNewTableName11 = tableName;
                                    }

                                    String strBeginMonthDay11 = nPreYear11+"-"+(nMonth-1)+"-"+"01";

                                    final long beginDay11 = getDays(strBeginMonthDay11, "1970-01-01");//1970-1-1到月初的天数
                                    final long endDay11 = getDays(strLastDayOfMonth11, "1970-01-01");//1970-1-1到月低的天数
                                    String sqlMonthCnt11 = null;
                                    sqlMonthCnt11 = "select SDATE,DATA,CALC_ID from " + strNewTableName11 + " where sdate in (select max(sdate) from " + strNewTableName11 + " where sdate>=" + beginDay11 + " and sdate<=" + endDay11 + ") and station_id=" + nCalcStationId + " and device_id=" + nCalcDeviceId + " and CALC_ID=11";
                                    try {
                                        jdbcTemplate.query(sqlMonthCnt11, new RowCallbackHandler() {
                                            public void processRow(ResultSet resultSet) throws SQLException {
                                                CalcInfo infoReal = new CalcInfo();

                                                try {
                                                    // 获取查询的日期前一次的值
                                                    float fDataPrev = resultSet.getFloat("DATA");

                                                    float dataReal = data - fDataPrev;
                                                    dataReal = changeValue(dataReal);

                                                    infoReal.setfValue(dataReal);

                                                } catch (Exception e) {
                                                    infoReal.setfValue(0);
                                                    calcDataList.add(10, infoReal);
                                                }

                                                calcDataList.remove(10);
                                                calcDataList.add(10, infoReal);
                                            }
                                        });
                                    } catch (Exception e) {
                                        info.setfValue(0);
                                        calcDataList.add(10, info);
                                    }

                                    break;
                                default:
                                    break;
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        String sqlYear = null;
        sqlYear = "select DATA,CALC_ID from "+tableName+" where sdate in (select max(sdate) from "+tableName+") and station_id="+nCalcStationId+" and device_id="+nCalcDeviceId+" order by CALC_ID";
        try {
            jdbcTemplate.query(sqlYear, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {

                    // 2,5,8,11
                    int nID = resultSet.getInt("CALC_ID");
                    if (nID == 3 || nID == 6 || nID == 9 || nID == 12) {

                        CalcInfo info = new CalcInfo();

                        try {
                            float fDataPrev = resultSet.getFloat("DATA");
                            fDataPrev = changeValue(fDataPrev);
                            info.setfValue(fDataPrev);

                        } catch (Exception e) {
                            info.setfValue(0);
                        }

                        switch (nID) {
                            case 3:
                                calcDataList.remove(2);
                                calcDataList.add(2, info);
                                break;
                            case 6:
                                calcDataList.remove(5);
                                calcDataList.add(5, info);
                                break;
                            case 9:
                                calcDataList.remove(8);
                                calcDataList.add(8, info);
                                break;
                            case 12:
                                calcDataList.remove(11);
                                calcDataList.add(11, info);
                                break;
                            default:
                                break;
                        }
                    }
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return calcDataList;
    }

    public Total_YMD_Info getDeviceCalcInfo(String strDate, String strStationID, String strDeviceID) {

        Total_YMD_Info total_ymd_info = new Total_YMD_Info();

        total_ymd_info.setDayCalcList(getDeviceCalcInfoDay(strDate, strStationID, strDeviceID));

        return total_ymd_info;

    }

    public List<CalcInfo> getDeviceCalcInfoDay(String strDate, String strStationID, String strDeviceID) {

        long sdate = getDays(strDate, "1970-01-01");//1970-1-1至今的天数

        String year = strDate.substring(0, 4);
        String month = strDate.substring(5, 7);
        final List<CalcInfo> calcDataList = new ArrayList<>();

        String tableName = "HIS_CALC_DS" + year;

        String sqlTableName = "select * from user_tables where table_name = '" + tableName + "'";
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        // 表名不存在返回null
        if (tableExist == null || tableExist.isEmpty()) {
            return  null;
        }

        // 对应的计算量厂站ID
        int nCalcStationId = Integer.parseInt(strStationID);

        // 对应的计算量设备ID
        int nCalcDeviceId = Integer.parseInt(strDeviceID);

        final List<String> listAiTag = new ArrayList<>();

        // 获取小容量节电量系数指标
        String strSqlSmall = "select AI_TAG from scd_calculation where station_id=" + nCalcStationId + " and device_id=" +nCalcDeviceId+ " and ai_id=1";
        try {
            jdbcTemplate.query(strSqlSmall, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    try {
                        final String strAiTag = resultSet.getString("AI_TAG").trim();
                        listAiTag.add(strAiTag);

                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        if (listAiTag.size() == 0) {
            return null;
        }

        String sql = null;
        sql = "select DATA,CALC_ID from " + tableName + " where STATION_ID=" + nCalcStationId + " and DEVICE_ID=" + nCalcDeviceId + " and SDATE=" + sdate + " order by CALC_ID";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    CalcInfo info = new CalcInfo();

                    try {
                        String strData = resultSet.getString("DATA").trim();

                        info.setValue(strData);
                        info.setCode(listAiTag.get(0));

                    } catch (Exception e) {
                        info.setValue("--");
                    }

                    calcDataList.add(info);
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        int nMonthDay = getCurrentMonthDay();
        String strBeginMonthDay = year+"-"+month+"-"+"01";
        String strMonthLastDay = year+"-"+month+"-"+nMonthDay;
        long beginDay = getDays(strBeginMonthDay, "1970-01-01");//1970-1-1到月初的天数
        long endDay = getDays(strMonthLastDay, "1970-01-01");//1970-1-1到月低的天数


        String sqlMonth = null;
        sqlMonth = "select DATA,CALC_ID from "+tableName+" where sdate in (select max(sdate) from "+tableName+" where sdate>="+beginDay+" and sdate<="+endDay+") and station_id="+nCalcStationId+" and device_id="+nCalcDeviceId+" order by CALC_ID";
        try {
            jdbcTemplate.query(sqlMonth, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    CalcInfo info = new CalcInfo();

                    try {
                        String strData = resultSet.getString("DATA").trim();
                        info.setValue(strData);

                    } catch (Exception e) {
                        info.setValue("--");
                    }

                    int nID = resultSet.getInt("CALC_ID");
                    switch (nID) {
                        case 2:
                            calcDataList.remove(1);
                            calcDataList.add(1, info);
                            break;
                        case 5:
                            calcDataList.remove(4);
                            calcDataList.add(4, info);
                            break;
                        case 11:
                            calcDataList.remove(10);
                            calcDataList.add(10, info);
                            break;
                        case 14:
                            calcDataList.remove(13);
                            calcDataList.add(13, info);
                            break;
                        case 17:
                            calcDataList.remove(16);
                            calcDataList.add(16, info);
                            break;
                        default:
                            break;
                    }

                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        String sqlYear = null;
        sqlYear = "select DATA,CALC_ID from "+tableName+" where sdate in (select max(sdate) from "+tableName+") and station_id="+nCalcStationId+" and device_id="+nCalcDeviceId+" order by CALC_ID";
        try {
            jdbcTemplate.query(sqlYear, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    CalcInfo info = new CalcInfo();

                    try {
                        String strData = resultSet.getString("DATA").trim();
                        info.setValue(strData);

                    } catch (Exception e) {
                        info.setValue("--");
                    }

                    // 2,5,8,11
                    int nID = resultSet.getInt("CALC_ID");
                    switch (nID) {
                        case 3:
                            calcDataList.remove(2);
                            calcDataList.add(2, info);
                            break;
                        case 6:
                            calcDataList.remove(5);
                            calcDataList.add(5, info);
                            break;
                        case 12:
                            calcDataList.remove(11);
                            calcDataList.add(11, info);
                            break;
                        case 15:
                            calcDataList.remove(14);
                            calcDataList.add(14, info);
                            break;
                        case 18:
                            calcDataList.remove(17);
                            calcDataList.add(17, info);
                            break;
                        default:
                            break;
                    }

                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return calcDataList;
    }

    public static int getCurrentMonthDay() {

        Calendar a = Calendar.getInstance();
        a.set(Calendar.DATE, 1);
        a.roll(Calendar.DATE, -1);
        int maxDate = a.get(Calendar.DATE);
        return maxDate;
    }

    public static int getLastMonthDay() {

        Calendar a = Calendar.getInstance();
        a.add(Calendar.MONTH, -1);    //加一个月
        a.set(Calendar.DATE, 1);
        a.roll(Calendar.DATE, -1);
        int maxDate = a.get(Calendar.DATE);
        return maxDate;
    }

    public static String getLastDayOfMonth(int year,int month)
    {
        Calendar cal = Calendar.getInstance();
        //设置年份
        cal.set(Calendar.YEAR,year);
        //设置月份
        cal.set(Calendar.MONTH, month-1);
        //获取某月最大天数
        int lastDay = cal.getActualMaximum(Calendar.DAY_OF_MONTH);
        //设置日历中月份的最大天数
        cal.set(Calendar.DAY_OF_MONTH, lastDay);
        //格式化日期
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        String lastDayOfMonth = sdf.format(cal.getTime());

        return lastDayOfMonth;
    }

    public List<CalcInfo> getTotalCalcInfoMonth(String strName, String strType, String strDate, List<TreeDataOfAreaRelation> listArea) {

        if (listArea.size() == 0) {
            return null;
        }

        String year = strDate.substring(0, 4);
        String month = strDate.substring(5,7);

        int nSdate = Integer.parseInt(year+month);

        final List<CalcInfo> calcDataList = new ArrayList<>();

        String tableName = "HIS_MS" + year;

        String sqlTableName = "select * from user_tables where table_name = '" + tableName + "'";
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        // 表名不存在返回null
        if (tableExist == null || tableExist.isEmpty()) {
            return  null;
        }

        // 对应的计算量厂站ID
        int nCalcStationId = 0;

        // 对应的计算量设备ID
        int nCalcDeviceId = 0;

        for (int i = 0; i < listArea.size(); ++i) {
            if (listArea.get(i).getStrLevel().compareTo(strType) == 0 && listArea.get(i).getStrDescribe().compareTo(strName) == 0) {

                String strStationAndDevice = listArea.get(i).getStrStationAndDeviceRelationID();

                String [] strStaDev = strStationAndDevice.split("-");
                if (strStaDev.length < 2) {
                    return null;
                }

                nCalcStationId = Integer.parseInt(strStaDev[0]);
                nCalcDeviceId = Integer.parseInt(strStaDev[1]);

                break;
            }
        }

        String sql = null;
        sql = "select VALUE from " + tableName + " where STATION_ID=" + nCalcStationId + " and DEVICE_ID=" + nCalcDeviceId + " and SDATE=" + nSdate + " and DATA_FLAG=3 and TYPE=79" + " order by DATA_ID";

        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    CalcInfo info = new CalcInfo();

                    try {
                        String strData = resultSet.getString("VALUE").trim();

                        info.setCode(strData);

                    } catch (Exception e) {
                        // log
                    }


                    calcDataList.add(info);
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return calcDataList;
    }

    public DeviceRunningInfo getDeviceRunningInfo(String strName, String strType) {

        String tableName = "SCD_DEVICE";

        String sqlTableName = "select * from user_tables where table_name = '" + tableName + "'";
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        // 表名不存在返回null
        if (tableExist == null || tableExist.isEmpty()) {
            return  null;
        }

        // 对应的计算量厂站ID
        String strCalcStationId = "";

        // 对应的计算量设备ID
        String strCalcDeviceId = "";

        final DeviceRunningInfo deviceRunningInfo = new DeviceRunningInfo();

        if (strType.compareTo("1") == 0) {

            // 从区域关系表中，根据name和type查找设备ID
            final List<TreeDataOfAreaRelation> listArea = new ArrayList<TreeDataOfAreaRelation>();
            String sqlArea = String.format("select * from AREA_RELATION");
            try {
                jdbcTemplate.query(sqlArea, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        TreeDataOfAreaRelation area = new TreeDataOfAreaRelation();

                        area.setStrDescribe(resultSet.getString("DESCRIBE").trim());
                        area.setStrLevel(resultSet.getString("AREA_LEVEL").trim());
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

            // 厂站个数
            String sqlScdStation = String.format("select count(*) from scd_station");
            try {
                jdbcTemplate.query(sqlScdStation, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {

                        int num = resultSet.getInt("count(*)");
                        deviceRunningInfo.setnDeviceNumOfTotal(num);
                    }
                });

            } catch (Exception e) {
                e.printStackTrace();
            }

            if (deviceRunningInfo.getnDeviceNumOfTotal() == 0) {
                return null;
            }

            for (int i = 0; i < listArea.size(); ++i) {
                if (listArea.get(i).getStrLevel().compareTo(strType) == 0 && listArea.get(i).getStrDescribe().compareTo(strName) == 0) {

                    String strStationAndDevice = listArea.get(i).getStrStationAndDeviceRelationID();

                    String [] strStaDev = strStationAndDevice.split("-");
                    if (strStaDev.length < 2) {
                        return null;
                    }

                    strCalcStationId = strStaDev[0];
                    strCalcDeviceId = strStaDev[1];

                    break;
                }
            }

            // 默认虚拟厂站ID为1
            String sqlDeviceNum = "select count(*) from scd_device  where station_id != 1";
            try {
                jdbcTemplate.query(sqlDeviceNum, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {

                        try {
                            int nDeviceNum = resultSet.getInt("count(*)");

                            int nStationNum = deviceRunningInfo.getnDeviceNumOfTotal() - 1;

                            deviceRunningInfo.setnDeviceNumOfTotal(nDeviceNum - nStationNum);

                        } catch (Exception e) {
                            // log
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

        } else if (strType.compareTo("2") == 0) {
            // 从区域关系表中，根据name和type查找设备ID
            final List<TreeDataOfAreaRelation> listArea = new ArrayList<TreeDataOfAreaRelation>();
            String sqlArea = String.format("select * from AREA_RELATION");
            try {
                jdbcTemplate.query(sqlArea, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        TreeDataOfAreaRelation area = new TreeDataOfAreaRelation();

                        area.setStrStationID(resultSet.getString("STATION_id").trim());
                        area.setStrDescribe(resultSet.getString("DESCRIBE").trim());
                        area.setStrLevel(resultSet.getString("AREA_LEVEL").trim());
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

            for (int i = 0; i < listArea.size(); ++i) {
                if (listArea.get(i).getStrLevel().compareTo(strType) == 0 && listArea.get(i).getStrDescribe().compareTo(strName) == 0) {

                    String strStationID = listArea.get(i).getStrStationID();

                    final String [] strStaID = strStationID.split(",");
                    if (strStaID.length == 0) {
                        return null;
                    }

                    String strStationAndDevice = listArea.get(i).getStrStationAndDeviceRelationID();

                    String [] strStaDev = strStationAndDevice.split("-");
                    if (strStaDev.length < 2) {
                        return null;
                    }

                    strCalcStationId = strStaDev[0];
                    strCalcDeviceId = strStaDev[1];


                    String sqlDeviceNum = "select count(*) from scd_device where STATION_ID=";

                    for (int j = 0; j < strStaID.length; j++) {
                        int nStationID = Integer.parseInt(strStaID[j]);
                        sqlDeviceNum += nStationID;

                        if (j != strStaID.length - 1) {
                            sqlDeviceNum += " or STATION_ID=";
                        }
                    }

                    try {
                        jdbcTemplate.query(sqlDeviceNum, new RowCallbackHandler() {
                            public void processRow(ResultSet resultSet) throws SQLException {
                                int nDeviceNum = resultSet.getInt("count(*)");

                                deviceRunningInfo.setnDeviceNumOfTotal(nDeviceNum - strStaID.length);
                            }
                        });

                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                    break;
                }
            }

        } else {
            return null;
        }

        // 在线数计算量获取
        List<CalcInfo> listCalcInfo = getCalcInfo(strCalcStationId, strCalcDeviceId);
        if (listCalcInfo.size() >= 15) {
            deviceRunningInfo.setStrDeviceNumOfRunning(listCalcInfo.get(12).getCode());

            deviceRunningInfo.setStrDeviceNumOfStoping(listCalcInfo.get(13).getCode());

            deviceRunningInfo.setStrPercent(listCalcInfo.get(14).getCode());
        }

        return deviceRunningInfo;
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
}
