package com.corsair.dao;

import com.corsair.device.CalcInfo;
import com.corsair.device.StationDeviceParaID;
import com.corsair.export.ExcelExportData;
import com.corsair.measurement.HistoryDataValue;
import com.corsair.measurement.YMD_StatisticsInfo;
import com.corsair.service.TreeDataOfAreaRelation;
import com.rbac.common.BaseDaoSupport;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowCallbackHandler;
import org.springframework.stereotype.Repository;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

/**
 * Created by zjq on 2015/11/10.
 */
@Repository("historyDataDao")
public class HistoryDataDao extends BaseDaoSupport {

    @Autowired
    private JdbcTemplate jdbcTemplate;
    private String tableExist = null;


    // add by zyj 2017/2/24
    @Autowired
    DeviceInfoDao deviceInfoDao;

    /**
     * 返回历史数据
     * @param tableName 数据库表名.
     * @param measureId 遥测或遥脉代码.
     * @param dateDays 从1970年到现在的天数.
     * @param beginMinute 开始分钟数.
     * @param endMinute 结束分钟数.
     * @param type 数据类型.具体如下
     *              1       //实时值
                    4       //死数
                    5       //终端故障
                    52		//电度表码
                    79      //日累计电量

     * @return 返回历史数据
     */
    public List<HistoryDataValue> getHistoryData(String tableName,String measureId,int dateDays,int beginMinute,int endMinute,int type){

        final List<HistoryDataValue> historyDatas = new ArrayList<>();
        String sqlTableName = "select * from user_tables where table_name = '" + tableName + "'";
        tableExist = null;
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });
        //measureId="00311c072j87575lh4b0203";
        //tableName="KWH201610";
        //dateDays = 17099;
        //endMinute= 1125;
        int stationId = 1;
        int deviceId = 56;
        int paraId = 2;
        //dateDays = 16856;
        if(tableExist != null && !tableExist.isEmpty()){
            String sql;
            /*if (dateDays == 0){
                sql = "select name,sdate,time,data from " + tableName + " where name = '" + measureId + "' and (time between " + Integer.toString(beginMinute) + " and "
                        + Integer.toString(endMinute) + ")  and flag=" + Integer.toString(type) + " order by time asc";
            }
            else {
                sql = "select name,sdate,time,data from " + tableName + " where name = '" + measureId + "' and sdate=" + Integer.toString(dateDays) + " and (time between " + Integer.toString(beginMinute) + " and "
                        + Integer.toString(endMinute) + ")  and flag=" + Integer.toString(type) + " order by time asc";
            }*/
            sql = "select * from " + tableName + " where STATION_ID=" + stationId + " and DEVICE_ID=" +deviceId + " and AI_ID=" + paraId + " and sdate=" + dateDays;
            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        //取出6：00-20:00历史库存放的第一个数据
                        for(int i=6; i<21;i++){
                            try {
                                String colname="VALUE_"+String.valueOf(i);
                                String strValue = resultSet.getString(colname).trim();
                                String value= strValue.substring(0,strValue.indexOf(","));
                                //数据为空则继续
                                if (value.isEmpty()) {
                                    continue;
                                }
                                HistoryDataValue historyDataValue = new HistoryDataValue();
                                historyDataValue.setSdate(resultSet.getInt("sdate"));
                                historyDataValue.setTime(60*i);
                                historyDataValue.setData(Double.valueOf(value));
                                historyDatas.add(historyDataValue);
                            } catch (Exception e) {
                                //数据为空出现异常时，将值设置为0
                                HistoryDataValue historyDataValue = new HistoryDataValue();
                                historyDataValue.setSdate(resultSet.getInt("sdate"));
                                historyDataValue.setTime(60*i-60);
                                historyDataValue.setData(Double.valueOf(0));
                                historyDatas.add(historyDataValue);
                            }

                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return historyDatas;
    }

    public List<HistoryDataValue> getHistoryData(String tableName,String measureId,int dateDaysEnd, int type){
        final List<HistoryDataValue> historyDatas = new ArrayList<>();

        String sqlTableName = "select * from user_tables where table_name = '" + tableName + "'";
        tableExist = null;
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }});

        if(tableExist != null && !tableExist.isEmpty()) {
            String sql = "select name,sdate,time,data from " + tableName + " where name = '" + measureId + "' and flag=" + Integer.toString(type) +" and sdate <= " + Integer.toString(dateDaysEnd) + " order by sdate ASC ";

            if (dateDaysEnd == -1) {
                sql = "select name,sdate,time,data from " + tableName + " where name = '" + measureId + "'  and flag=" + Integer.toString(type) +" order by sdate ASC";
            }

            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        HistoryDataValue historyDataValue = new HistoryDataValue();
                        historyDataValue.setName(resultSet.getString("name").trim());
                        historyDataValue.setSdate(resultSet.getInt("sdate"));
                        historyDataValue.setTime(resultSet.getInt("time"));
                        historyDataValue.setData(resultSet.getDouble("data"));
                        historyDatas.add(historyDataValue);
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return historyDatas;
    }

    public List<List<HistoryDataValue>> getRealCurveInfo(String tableName, long dateDays, int nStationID, int nDeviceID, int nParaID) {

        // 判断表是否存在
        String sqlExist = "select * from user_tables where table_name = '" + tableName + "'";
        tableExist = null;
        jdbcTemplate.query(sqlExist, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }
        });

        if(tableExist == null || tableExist.isEmpty()) {
            return null;
        }

        Calendar c = Calendar.getInstance();
        final int hour = c.get(Calendar.HOUR_OF_DAY);
        final int minute = c.get(Calendar.MINUTE);

        final List<List<HistoryDataValue>> listListHistoryDatas = new ArrayList<>();

        String sql;

        sql = "select * from " + tableName + " where STATION_ID=" + nStationID + " and DEVICE_ID=" + nDeviceID + " and AI_ID=" + nParaID + " and sdate=" + dateDays;
        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    //取出0：00-当前小时的数据
                    for(int i = 0; i < hour; i++) {
                        try {
                            String colname="VALUE_"+String.valueOf(i);
                            // 整个小时的数据
                            String strValue = resultSet.getString(colname).trim();
                            //数据为空则补零继续
                            if (strValue.isEmpty()) {
                                // 当前小时每5分钟都设置成0
                                // 每个小时的数据
                                final List<HistoryDataValue> listSignalHourData = new ArrayList<>();
                                for (int j = 0; j < 12; j++) {
                                    HistoryDataValue hisValue = new HistoryDataValue();
                                    hisValue.setSdate(resultSet.getInt("sdate"));
                                    hisValue.setTime(i);
                                    hisValue.setData(Double.valueOf(0));

                                    listSignalHourData.add(hisValue);
                                }

                                listListHistoryDatas.add(listSignalHourData);
                                continue;
                            }
                            String [] value= strValue.split(";");
                            int sizeZero = 12 - value.length;
                            // 等于0表示数据齐全
                            if (sizeZero == 0) {
                                final List<HistoryDataValue> listSignalHourData = new ArrayList<>();
                                for (int j = 0; j < value.length; j++) {
                                    HistoryDataValue hisValue = new HistoryDataValue();
                                    hisValue.setSdate(resultSet.getInt("sdate"));
                                    hisValue.setTime(i);
                                    //String value= strValue.substring(0,strValue.indexOf(","));
                                    hisValue.setData(Double.parseDouble(value[j].substring(0, value[j].indexOf(","))));

                                    listSignalHourData.add(hisValue);
                                }

                                listListHistoryDatas.add(listSignalHourData);
                            } else if (sizeZero < 0) {
                                final List<HistoryDataValue> listSignalHourData = new ArrayList<>();
                                for (int j = 0; j < 12; j++) {
                                    HistoryDataValue hisValue = new HistoryDataValue();
                                    hisValue.setSdate(resultSet.getInt("sdate"));
                                    hisValue.setTime(i);
                                    //String value= strValue.substring(0,strValue.indexOf(","));
                                    hisValue.setData(Double.parseDouble(value[j].substring(0, value[j].indexOf(","))));

                                    listSignalHourData.add(hisValue);
                                }

                                listListHistoryDatas.add(listSignalHourData);
                            } else {
                                final List<HistoryDataValue> listSignalHourData = new ArrayList<>();

                                // 写入有效数据
                                for (int k = 0; k < value.length; k++) {
                                    HistoryDataValue hisValue = new HistoryDataValue();
                                    hisValue.setSdate(resultSet.getInt("sdate"));
                                    hisValue.setTime(i);
                                    //String value= strValue.substring(0,strValue.indexOf(","));
                                    hisValue.setData(Double.parseDouble(value[k].substring(0, value[k].indexOf(","))));

                                    listSignalHourData.add(hisValue);
                                }

                                // 不足补零
                                for (int j = 0; j < sizeZero; j++) {
                                    HistoryDataValue hisValue = new HistoryDataValue();
                                    hisValue.setSdate(resultSet.getInt("sdate"));
                                    hisValue.setTime(i);
                                    hisValue.setData(Double.valueOf(0));

                                    listSignalHourData.add(hisValue);
                                }

                                listListHistoryDatas.add(listSignalHourData);
                            }
                        } catch (Exception e) {
                            //数据为空出现异常时，将值设置为0
                            final List<HistoryDataValue> listSignalHourData = new ArrayList<>();
                            for (int j = 0; j < 12; j++) {
                                HistoryDataValue hisValue = new HistoryDataValue();
                                hisValue.setSdate(resultSet.getInt("sdate"));
                                hisValue.setTime(i);
                                hisValue.setData(Double.valueOf(0));

                                listSignalHourData.add(hisValue);
                            }

                            listListHistoryDatas.add(listSignalHourData);
                        }
                    }

                    // 处理分钟的数据
                    try {
                        String colname = "VALUE_" + hour;
                        String strValue = resultSet.getString(colname).trim();
                        String[] value = strValue.split(";");
                        int sizeZero = (minute / 5 + 1) - value.length;
                        // 等于0表示数据齐全
                        if (sizeZero == 0) {
                            final List<HistoryDataValue> listSignalHourData = new ArrayList<>();
                            for (int j = 0; j < value.length; j++) {
                                HistoryDataValue hisValue = new HistoryDataValue();
                                hisValue.setSdate(resultSet.getInt("sdate"));
                                hisValue.setTime(hour);
                                //String value= strValue.substring(0,strValue.indexOf(","));
                                hisValue.setData(Double.parseDouble(value[j].substring(0, value[j].indexOf(","))));

                                listSignalHourData.add(hisValue);
                            }

                            listListHistoryDatas.add(listSignalHourData);
                        } else if (sizeZero < 0) {
                            final List<HistoryDataValue> listSignalHourData = new ArrayList<>();
                            for (int j = 0; j < (minute / 5 + 1); j++) {
                                HistoryDataValue hisValue = new HistoryDataValue();
                                hisValue.setSdate(resultSet.getInt("sdate"));
                                hisValue.setTime(hour);
                                //String value= strValue.substring(0,strValue.indexOf(","));
                                hisValue.setData(Double.parseDouble(value[j].substring(0, value[j].indexOf(","))));

                                listSignalHourData.add(hisValue);
                            }

                            listListHistoryDatas.add(listSignalHourData);
                        } else {
                            final List<HistoryDataValue> listSignalHourData = new ArrayList<>();

                            // 写入有效数据
                            for (int k = 0; k < value.length; k++) {
                                HistoryDataValue hisValue = new HistoryDataValue();
                                hisValue.setSdate(resultSet.getInt("sdate"));
                                hisValue.setTime(hour);
                                //String value= strValue.substring(0,strValue.indexOf(","));
                                hisValue.setData(Double.parseDouble(value[k].substring(0, value[k].indexOf(","))));

                                listSignalHourData.add(hisValue);
                            }

                            // 不足补零
                            for (int j = 0; j < sizeZero; j++) {
                                HistoryDataValue hisValue = new HistoryDataValue();
                                hisValue.setSdate(resultSet.getInt("sdate"));
                                hisValue.setTime(hour);
                                hisValue.setData(Double.valueOf(0));

                                listSignalHourData.add(hisValue);
                            }

                            listListHistoryDatas.add(listSignalHourData);
                        }
                    } catch (Exception e) {
                        //数据为空出现异常时，将值设置为0
                        final List<HistoryDataValue> listSignalHourData = new ArrayList<>();
                        for (int j = 0; j < (minute/5+1); j++) {
                            HistoryDataValue hisValue = new HistoryDataValue();
                            hisValue.setSdate(resultSet.getInt("sdate"));
                            hisValue.setTime(hour);
                            hisValue.setData(Double.valueOf(0));

                            listSignalHourData.add(hisValue);
                        }

                        listListHistoryDatas.add(listSignalHourData);
                    }
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        return listListHistoryDatas;
    }

    public List<HistoryDataValue> getSaveElecInfo(String tableName, long dateDays, String strLevel, String strName, int ymd) {

        final List<StationDeviceParaID> listStaDevPara = new ArrayList<StationDeviceParaID>();

        List<TreeDataOfAreaRelation> listAreaRelation = deviceInfoDao.getAreaRelation();
        // 树形结构选择省份，则查找地级市
        if (strLevel.compareTo("1") == 0) {
            for (int i = 0; i < listAreaRelation.size(); ++i) {
                if (listAreaRelation.get(i).getStrLevel().compareTo("2") == 0) {

                    String name = listAreaRelation.get(i).getStrDescribe();
                    String strStationAndDevice = listAreaRelation.get(i).getStrStationAndDeviceRelationID();

                    String [] strStaDev = strStationAndDevice.split("-");
                    if (strStaDev.length < 2) {
                        return null;
                    }

                    StationDeviceParaID stationDeviceParaID = new StationDeviceParaID();

                    stationDeviceParaID.setStationId(Integer.parseInt(strStaDev[0]));
                    stationDeviceParaID.setDeviceId(Integer.parseInt(strStaDev[1]));
                    stationDeviceParaID.setStrName(name);

                    listStaDevPara.add(stationDeviceParaID);
                }
            }
        } else if (strLevel.compareTo("2") == 0) { // 点击地级市时，需要根据STATION_ID字段从STATISTICS_STATION_INFO表中查找各个县级市的日节电量信息

            final List<SaveElecInfoOfAll> listSavElecOfAll = new ArrayList<SaveElecInfoOfAll>();

            // 获取所有厂站日节电量信息
            String sqlSaveDay = "select * from STATISTICS_STATION_INFO";
            jdbcTemplate.query(sqlSaveDay, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    SaveElecInfoOfAll saveElecInfoOfAll = new SaveElecInfoOfAll();

                    saveElecInfoOfAll.setnStationId(resultSet.getInt("STATION_ID"));
                    saveElecInfoOfAll.setStrStationDescribe(resultSet.getString("STATION_DESCRIBE").trim());

                    saveElecInfoOfAll.setnDeviceId(resultSet.getInt("DEVICE_ID"));
                    saveElecInfoOfAll.setStrDeviceDescribe(resultSet.getString("DEVICE_DESCRIBE").trim());

                    listSavElecOfAll.add(saveElecInfoOfAll);
                }});

            if (listSavElecOfAll.size() == 0) {
                return null;
            }

            String strStationMore = "";
            for (int i = 0; i < listAreaRelation.size(); ++i) {
                if (listAreaRelation.get(i).getStrDescribe().compareTo(strName) == 0 &&
                        listAreaRelation.get(i).getStrLevel().compareTo("2") == 0) {

                    strStationMore = listAreaRelation.get(i).getStrStationID();

                    break;
                }
            }

            String [] strStationArray = strStationMore.split(",");
            if (strStationArray.length == 0) {
                return null;
            }

            for (int i = 0; i < strStationArray.length; ++i) {
                for (int j = 0; j < listSavElecOfAll.size(); ++j) {
                    if (Integer.parseInt(strStationArray[i]) == listSavElecOfAll.get(j).getnStationId()) {

                        StationDeviceParaID stationDeviceParaID = new StationDeviceParaID();

                        stationDeviceParaID.setStationId(listSavElecOfAll.get(j).getnStationId());
                        stationDeviceParaID.setDeviceId(listSavElecOfAll.get(j).getnDeviceId());
                        //stationDeviceParaID.setParaId(listSavElecOfAll.get(j).getnParaId());
                        stationDeviceParaID.setStrName(listSavElecOfAll.get(j).getStrStationDescribe());

                        listStaDevPara.add(stationDeviceParaID);
                    }
                }
            }

        } else {
            return null;
        }

        // 判断指定厂站、设备、点是否配置
        if (listStaDevPara.size() == 0) {
            return null;
        }

        // 判断表是否存在
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

        // 日
        final List<HistoryDataValue> listHistoryDatas = new ArrayList<>();

        for (int i = 0; i < listStaDevPara.size(); ++i) {

            int nStationID = listStaDevPara.get(i).getStationId();
            int nDeviceID = listStaDevPara.get(i).getDeviceId();
            int nId = ymd;// 默认1为年节电量，2为月节电量，3为日节电量。
            final String name = listStaDevPara.get(i).getStrName();
            String sql;

            sql = "select DATA from " + tableName + " where STATION_ID=" + nStationID + " and DEVICE_ID=" +nDeviceID + " and CALC_ID=" + nId + " and sdate=" + dateDays + " and rownum<2";
            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {

                        try {
                            double dValue = resultSet.getDouble("DATA");

                            HistoryDataValue historyDataValue = new HistoryDataValue();

                            historyDataValue.setData(dValue);
                            historyDataValue.setName(name);

                            listHistoryDatas.add(historyDataValue);
                        } catch (Exception e) {
                            //数据为空出现异常时，将值设置为0
                            HistoryDataValue historyDataValue = new HistoryDataValue();

                            historyDataValue.setData(Double.valueOf(0));
                            historyDataValue.setName(name);

                            listHistoryDatas.add(historyDataValue);
                        }

                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return listHistoryDatas;
    }

    /*public List<HistoryDataValue> getSaveElecOfMonth(String tableName, int nYearMonth, String strLevel, String strName) {

        // 判断表是否存在
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

        final List<StationDeviceParaID> listStaDevPara = new ArrayList<StationDeviceParaID>();

        List<TreeDataOfAreaRelation> listAreaRelation = deviceInfoDao.getAreaRelation();
        // 树形结构选择省份，则查找地级市
        if (strLevel.compareTo("1") == 0) {
            for (int i = 0; i < listAreaRelation.size(); ++i) {
                if (listAreaRelation.get(i).getStrLevel().compareTo("2") == 0) {

                    String name = listAreaRelation.get(i).getStrDescribe();
                    String strStationAndDevice = listAreaRelation.get(i).getStrStationAndDeviceRelationID();

                    String [] strStaDev = strStationAndDevice.split("-");
                    if (strStaDev.length < 3) {
                        return null;
                    }

                    StationDeviceParaID stationDeviceParaID = new StationDeviceParaID();

                    stationDeviceParaID.setStationId(Integer.parseInt(strStaDev[0]));
                    stationDeviceParaID.setDeviceId(Integer.parseInt(strStaDev[1]));
                    stationDeviceParaID.setParaId(Integer.parseInt(strStaDev[2]));
                    stationDeviceParaID.setStrName(name);

                    listStaDevPara.add(stationDeviceParaID);
                }
            }
        } else if (strLevel.compareTo("2") == 0) { // 点击地级市时，需要根据STATION_ID字段从STATISTICS_STATION_INFO表中查找各个县级市的日节电量信息

            final List<SaveElecInfoOfAll> listSavElecOfAll = new ArrayList<SaveElecInfoOfAll>();

            // 获取所有厂站日节电量信息
            String sqlSaveDay = "select * from STATISTICS_STATION_INFO";
            jdbcTemplate.query(sqlSaveDay, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    SaveElecInfoOfAll saveElecInfoOfAll = new SaveElecInfoOfAll();

                    saveElecInfoOfAll.setnStationId(resultSet.getInt("STATION_ID"));
                    saveElecInfoOfAll.setStrStationDescribe(resultSet.getString("STATION_DESCRIBE").trim());

                    saveElecInfoOfAll.setnDeviceId(resultSet.getInt("DEVICE_ID"));
                    saveElecInfoOfAll.setStrDeviceDescribe(resultSet.getString("DEVICE_DESCRIBE").trim());

                    saveElecInfoOfAll.setnParaId(resultSet.getInt("PARA_ID"));
                    saveElecInfoOfAll.setStrParaDescribe(resultSet.getString("PARA_DESCRIBE").trim());

                    listSavElecOfAll.add(saveElecInfoOfAll);
                }});

            if (listSavElecOfAll.size() == 0) {
                return null;
            }

            String strStationMore = "";
            for (int i = 0; i < listAreaRelation.size(); ++i) {
                if (listAreaRelation.get(i).getStrDescribe().compareTo(strName) == 0 &&
                        listAreaRelation.get(i).getStrLevel().compareTo("2") == 0) {

                    strStationMore = listAreaRelation.get(i).getStrStationID();

                    break;
                }
            }

            String [] strStationArray = strStationMore.split(",");
            if (strStationArray.length == 0) {
                return null;
            }

            for (int i = 0; i < strStationArray.length; ++i) {
                for (int j = 0; j < listSavElecOfAll.size(); ++j) {
                    if (Integer.parseInt(strStationArray[i]) == listSavElecOfAll.get(j).getnStationId()) {

                        StationDeviceParaID stationDeviceParaID = new StationDeviceParaID();

                        stationDeviceParaID.setStationId(listSavElecOfAll.get(j).getnStationId());
                        stationDeviceParaID.setDeviceId(listSavElecOfAll.get(j).getnDeviceId());
                        stationDeviceParaID.setParaId(listSavElecOfAll.get(j).getnParaId());
                        stationDeviceParaID.setStrName(listSavElecOfAll.get(j).getStrStationDescribe());

                        listStaDevPara.add(stationDeviceParaID);
                    }
                }
            }

        } else {
            return null;
        }

        // 判断指定厂站、设备、点是否配置
        if (listStaDevPara.size() == 0) {
            return null;
        }

        final List<HistoryDataValue> historyDatas = new ArrayList<>();

        for (int i = 0; i < listStaDevPara.size(); ++i) {

            int nStationID = listStaDevPara.get(i).getStationId();
            int nDeviceID = listStaDevPara.get(i).getDeviceId();
            int nId = listStaDevPara.get(i).getParaId();
            final String name = listStaDevPara.get(i).getStrName();
            String sql;

            sql = "select VALUE from " + tableName + " where STATION_ID=" + nStationID + " and DEVICE_ID=" +nDeviceID + " and DATA_ID=" + nId + " and SDATE=" + nYearMonth + " and TYPE=79";
            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {

                        try {
                            double dValue = resultSet.getDouble("VALUE");

                            HistoryDataValue historyDataValue = new HistoryDataValue();

                            historyDataValue.setData(dValue);
                            historyDataValue.setName(name);

                            historyDatas.add(historyDataValue);
                        } catch (Exception e) {
                            //数据为空出现异常时，将值设置为0
                            HistoryDataValue historyDataValue = new HistoryDataValue();

                            historyDataValue.setData(Double.valueOf(0));
                            historyDataValue.setName(name);

                            historyDatas.add(historyDataValue);
                        }

                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return historyDatas;
    }*/

    public List<HistoryDataValue> getHistoryDataByEqual(String tableName,String measureId,int equalValue){
        final List<HistoryDataValue> historyDatas = new ArrayList<>();

        String sqlTableName = "select * from user_tables where table_name = '" + tableName + "'";
        tableExist = null;
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }});

        if(tableExist != null && !tableExist.isEmpty()) {

            String sql = "select name,sdate,time,data from " + tableName + " where name = '" + measureId + "' and sdate = " + Integer.toString(equalValue) + " and flag=79 order by sdate ASC";

            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        HistoryDataValue historyDataValue = new HistoryDataValue();
                        historyDataValue.setName(resultSet.getString("name").trim());
                        historyDataValue.setSdate(resultSet.getInt("sdate"));
                        historyDataValue.setTime(resultSet.getInt("time"));
                        historyDataValue.setData(resultSet.getDouble("data"));
                        historyDatas.add(historyDataValue);
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return historyDatas;
    }

    /**
     * 返回指定日期时间的历史数据
     * @param tableName 数据库表名.
     * @param measureId 遥测或遥脉代码.
     * @param dateDays 从1970年到现在的天数.
     * @param time 分钟数.
     * @param type 数据类型.具体如下
     *              1       //实时值
                    4       //死数
                    5       //终端故障
                    52		//电度表码
                    79      //日累计电量
     * @return 返回历史数据
     */
    public HistoryDataValue getHistoryDataByDateTime(String tableName,String measureId,int dateDays,int time, int type){
        final List<HistoryDataValue> historyDatas = new ArrayList<>();
        String sqlTableName = "select * from user_tables where table_name = '" + tableName + "'";
        tableExist = null;
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }});

        if(tableExist != null && !tableExist.isEmpty()){
            String sql = "select name,sdate,time,data from " + tableName + " where name = '" + measureId + "' and sdate=" + Integer.toString(dateDays) + " and time = " + Integer.toString(time) +
                   " and flag=" + Integer.toString(type) + " order by time asc";
            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        HistoryDataValue historyDataValue = new HistoryDataValue();
                        historyDataValue.setName(resultSet.getString("name").trim());
                        historyDataValue.setSdate(resultSet.getInt("sdate"));
                        historyDataValue.setTime(resultSet.getInt("time"));
                        historyDataValue.setData(resultSet.getDouble("data"));
                        historyDatas.add(historyDataValue);
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return (historyDatas.size() == 0)? null : historyDatas.get(0);
    }


    /**
     * 返回指定日期时间的历史数据
     * @param tableName 数据库表名.
     * @param measureIdList 遥测或遥脉代码.
     * @param dateDayList 从1970年到现在的天数.
     * @param timeList 分钟数.
     * @param type 数据类型.具体如下
     *              1       //实时值
                    4       //死数
                    5       //终端故障
                    52		//电度表码
                    79      //日累计电量
     * @return 返回历史数据
     */
    public List<HistoryDataValue> getHistoryDataByRangeDateTime(String tableName,List<String> measureIdList,List<Integer> dateDayList, List<Integer> timeList, int type){
        final List<HistoryDataValue> historyDatas = new ArrayList<>();
        String sqlTableName = "select * from user_tables where table_name = '" + tableName + "'";
        tableExist = null;
        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
            public void processRow(ResultSet rels) throws SQLException {
                tableExist = rels.getString("TABLE_NAME");
            }});

        if(tableExist != null && !tableExist.isEmpty()){
            if((measureIdList.size() < 1) || (dateDayList.size() < 1) || (timeList.size() < 1))
                return null;
            String measureIds = "";
            String dataDays = "";
            String times = "";
            for (String measureId : measureIdList){
                measureIds = measureIds + "'" + measureId + "'" + ",";
            }
            for(Integer dateDay : dateDayList){
                dataDays = dataDays + dateDay.toString() + ",";
            }
            for(Integer time : timeList){
                times = times + time.toString() + ",";
            }
            measureIds = measureIds.substring(0, measureIds.length()-1);
            dataDays = dataDays.substring(0, dataDays.length()-1);
            times = times.substring(0, times.length()-1);


            String sql = "select name,sdate,time,data from " + tableName + " where name in (" + measureIds + ") and sdate in (" + dataDays + ") and time in (" + times +
                    ") and flag=" + Integer.toString(type) + " order by sdate,time ";
            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        HistoryDataValue historyDataValue = new HistoryDataValue();
                        historyDataValue.setName(resultSet.getString("name").trim());
                        historyDataValue.setSdate(resultSet.getInt("sdate"));
                        historyDataValue.setTime(resultSet.getInt("time"));
                        historyDataValue.setData(resultSet.getDouble("data"));
                        historyDatas.add(historyDataValue);
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return  historyDatas;
    }




//    public Double getHistoryMaxData(String tableName,String measureId,int dateDays,int beginMinute,int endMinute,int type){
//
//        final Double historyDataValue = 0d;
//        String sqlTableName = "select * from user_tables where table_name = '" + tableName + "'";
//        tableExist = null;
//        jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
//            public void processRow(ResultSet rels) throws SQLException {
//                tableExist = rels.getString("TABLE_NAME");
//            }});
//
//        if(tableExist != null && !tableExist.isEmpty()){
//            String sql = "select max(data) AS data from " + tableName + " where name = '" + measureId + "' and sdate=" + Integer.toString(dateDays) + " and (time between " + Integer.toString(beginMinute) + " and "
//                    + Integer.toString(endMinute) + ")  and flag=" + Integer.toString(type);
//            try {
//                jdbcTemplate.query(sql, new RowCallbackHandler() {
//                    public void processRow(ResultSet resultSet) throws SQLException {
//                    historyDataValue = resultSet.getDouble("data");
//                    }
//                });
//            } catch (Exception e) {
//                e.printStackTrace();
//            }
//        }
//        return historyDataValue;
//    }
}
