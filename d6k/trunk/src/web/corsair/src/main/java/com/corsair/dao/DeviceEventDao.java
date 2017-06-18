package com.corsair.dao;

import com.corsair.device.DeviceEvent;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowCallbackHandler;
import org.springframework.stereotype.Repository;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.*;
import java.text.*;

/**
 * Created by 洪祥 on 15/7/13.
 */
@Repository("deviceEventDao")
public class DeviceEventDao {
    @Autowired
    private JdbcTemplate jdbcTemplate;
    private String tableName = null;

    public List<DeviceEvent> getDeviceEvent(String from, String to, String device) {
        //todo 从库中获取设备相关的事件
        final List<DeviceEvent> deviceEvents = new LinkedList<DeviceEvent>();
        from = from.replace(" ", "").replace("/", "").replace(":", "") + "000";
        to = to.replace(" ", "").replace("/", "").replace(":", "") + "000";

        //开始时间迟于结束时间，返回null
        final int result = to.compareTo(from);
        if (result < 0) {
            return null;
        }

        //取出开始时间到结束时间期间的年月;
        List<String> ymList = new ArrayList<String>();
        DateFormat format1 = new SimpleDateFormat("yyyyMM");
        try {
            Date d1 = format1.parse(from.substring(0, 6));//定义起始日期
            Date d2 = format1.parse(to.substring(0, 6));//定义结束日期
            Calendar dd = Calendar.getInstance();//定义日期实例

            dd.setTime(d2);//设置日期结束时间
            while (!dd.getTime().before(d1)) {//判断是否到起始日期
                SimpleDateFormat sdf = new SimpleDateFormat("yyyyMM");
                String str = sdf.format(dd.getTime());
                dd.add(Calendar.MONTH, -1);//进行当前日期月份减1
                ymList.add(str);
            }
//            dd.setTime(d1);//设置日期起始时间
//            while (!dd.getTime().after(d2)) {//判断是否到结束日期
//                SimpleDateFormat sdf = new SimpleDateFormat("yyyyMM");
//                String str = sdf.format(dd.getTime());
//                dd.add(Calendar.MONTH, 1);//进行当前日期月份加1
//                ymList.add(str);
//            }
        } catch (ParseException e) {
            e.printStackTrace();
        }
        //事项类型号到事项类型名的映射
        final Map<Integer, String>eventTypeToDescMap = new HashMap();
        String eventTypeSql = "select * from 事项类型表";
        try {
            jdbcTemplate.query(eventTypeSql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    eventTypeToDescMap.put(resultSet.getInt("类型号"), resultSet.getString("类型名"));
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }

        //按月份搜索数据
        for (String ymStr : ymList) {
            //判断数据库中该表是否存在
            String sqlTableName = "select * from user_tables where table_name = '电力历史事项表" + ymStr + "'";
            tableName = null;
            jdbcTemplate.query(sqlTableName, new RowCallbackHandler() {
                public void processRow(ResultSet rels) throws SQLException {
                tableName = rels.getString("TABLE_NAME");
            }});
            if(tableName == null || tableName.length() <= 0){
                continue;
            }

            String sqlCondition = null;
            if (ymStr.equals(from.substring(0, 6)) && ymStr.equals(to.substring(0, 6))) {
                sqlCondition = " and ((年月日=" + from.substring(0, 8) + " and 时分秒毫秒>=" + from.substring(8, 17) +
                        " and 时分秒毫秒<=235959999)" + " or (年月日=" + to.substring(0, 8) + " and 时分秒毫秒>=0" +
                        " and 时分秒毫秒<=" + to.substring(8, 17) + ") or (年月日>" + from.substring(0, 8) +
                        " and 年月日<" + to.substring(0, 8) + ")) order by 年月日 desc,时分秒毫秒 desc";
                ;
            } else if (ymStr.equals(from.substring(0, 6))) {
                sqlCondition = " and ((年月日=" + from.substring(0, 8) + " and 时分秒毫秒>=" + from.substring(8, 17) +
                        " and 时分秒毫秒<=235959999)" + " or (年月日>" + from.substring(0, 8) + ")) order by 年月日 desc,时分秒毫秒 desc";
            } else if (ymStr.equals(to.substring(0, 6))) {
                sqlCondition = " and ((年月日=" + to.substring(0, 8) + " and 时分秒毫秒>=0" +
                        " and 时分秒毫秒<=" + to.substring(8, 17) + ") or (年月日<" + to.substring(0, 8) + ")) order by 年月日 desc,时分秒毫秒 desc";
            } else {
                sqlCondition = null;
            }
            String sql;
            if (sqlCondition != null && sqlCondition.length() > 0) {
                sql = "select * from 电力历史事项表" + ymStr + " where 成员3 = '" + device + "'" + sqlCondition;
            } else {
                sql = "select * from 电力历史事项表" + ymStr + " where 成员3 = '" + device + "'" + "order by 年月日 desc,时分秒毫秒 desc";
            }
            try {
                jdbcTemplate.query(sql, new RowCallbackHandler() {
                    public void processRow(ResultSet resultSet) throws SQLException {
                        DeviceEvent event = new DeviceEvent();
                        event.setDeviceName(resultSet.getString("成员3"));
                        event.setEvent(resultSet.getString("文字"));
                        event.setValue(resultSet.getDouble("值"));
                        if (eventTypeToDescMap.containsKey(resultSet.getInt("类型"))){
                            event.setType(eventTypeToDescMap.get(resultSet.getInt("类型")).toString().trim());
                        }
                        else {
                            event.setType(Integer.toString(resultSet.getInt("类型")));
                        }

                        String strYmd = Integer.toString(resultSet.getInt("年月日"));
                        String strHms = Integer.toString(resultSet.getInt("时分秒毫秒"));
                        Integer hms = Integer.parseInt(strHms);
                        int hour = (int)(hms/(100l*100l*1000l));
                        int min = (int)((hms%(100l*100l*1000l))/(100l*1000l));
                        int sec = (int)(((hms%(100l*100l*1000l))%(100l*1000l))/1000l);
                        int ms = (int)(((hms%(100l*100l*1000l))%(100l*1000l))%1000l);
                        String strTime = strYmd.substring(0, 4) + "/" + strYmd.substring(4, 6) + "/" + strYmd.substring(6, 8) +
                                " " + hour + ":" + min + ":" + sec + ":" + ms;
                        event.setEventTime(strTime);
                        deviceEvents.add(event);
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return deviceEvents;
    }
}
