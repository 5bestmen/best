package com.corsair.dao;

import com.corsair.device.PVPowerStation;
import com.corsair.service.TreeDataOfAreaRelation;
import com.corsair.service.TreeDataOfChannel;
import com.corsair.service.TreeDataOfStation;
import com.corsair.service.station.TreeDataOfDevice;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowCallbackHandler;
import org.springframework.stereotype.Repository;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by zyj on 2017/2/6.
 */
@Repository("getTreeDataDao")
public class GetTreeDataDao{
    @Autowired
    private JdbcTemplate jdbcTemplate;
    private String tableName = null;

    public String getTreeData() {
        String sqlArea = String.format("select AREA_LEVEL,DESCRIBE,STATION_ID from AREA_RELATION order by ID");
        String sqlStation = String.format("select STATION_ID,DESCRIBE from SCD_STATION");
        String sqlDevice = String.format("select DEVICE_ID,DESCRIBE,STATION_ID from SCD_DEVICE");
        //String strTreeData = "[{\"id\":1,\"text\":\"My Documents\",\"children\":[{\"id\":11,\"text\":\"Photos\",\"state\":\"closed\",\"children\":[{\"id\":111,\"text\":\"Friend123\"}]}]}]";

        StringBuilder treeJson = new StringBuilder();
       // treeJson.append("[{\"id\":\"" + 1 + "\",\"text\":\"" + "河南省" + "\",\"state\":\"open\",\"children\":[");

        final List<TreeDataOfAreaRelation> listArea = new ArrayList<TreeDataOfAreaRelation>();
        try {
            jdbcTemplate.query(sqlArea, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    TreeDataOfAreaRelation area = new TreeDataOfAreaRelation();
                    area.setStrLevel(resultSet.getString("AREA_LEVEL").trim());
                    area.setStrStationID(resultSet.getString("STATION_ID").trim());
                    area.setStrDescribe(resultSet.getString("DESCRIBE").trim());
                    listArea.add(area);
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        final List<TreeDataOfDevice> listDevice = new ArrayList<TreeDataOfDevice>();
        try {
            jdbcTemplate.query(sqlDevice, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    TreeDataOfDevice device = new TreeDataOfDevice();
                    device.setStrDeviceID(resultSet.getString("DEVICE_ID").trim());
                    device.setStrDescribe(resultSet.getString("DESCRIBE").trim());
                    device.setStrStationID(resultSet.getString("STATION_ID").trim());
                    listDevice.add(device);
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        final List<TreeDataOfStation> listStation = new ArrayList<TreeDataOfStation>();
        try {
            jdbcTemplate.query(sqlStation, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    TreeDataOfStation station = new TreeDataOfStation();
                    station.setStrID(resultSet.getString("STATION_ID").trim());
                    station.setStrDescribe(resultSet.getString("DESCRIBE").trim());
                    listStation.add(station);
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        if (listStation.isEmpty()) {
            return "";
        }

        int id = 1;
        treeJson.append("[");
        for (int i = 0; i < listArea.size(); ++i) {
            if (listArea.get(i).getStrLevel().compareTo("1") == 0) {

                treeJson.append("{\"id\":\"" + id
                        + "\",\"text\":\"" + listArea.get(i).getStrDescribe()
                        + "\",\"state\":\"open\""
                        + ",\"level\":\"1\""
                        + ",\"children\":[");

                id++;

                break;
            }
        }

        Boolean areaflag = false;

        for (int i = 0; i < listArea.size(); ++i) {
            if (listArea.get(i).getStrLevel().compareTo("2") == 0) {

                if (areaflag) {
                    treeJson.append("},");
                }

                treeJson.append("{\"id\":\"" + id
                        + "\",\"text\":\"" + listArea.get(i).getStrDescribe()
                        + "\",\"state\":\"closed\""
                        + ",\"level\":\"2\""
                        + ",\"children\":[");

                areaflag = true;

                id++;

                String strStationID = listArea.get(i).getStrStationID();
                String [] strIdSplit = strStationID.split(",");

                Boolean Stationflag = false;

                for (int j = 0; j < strIdSplit.length; j++) {

                    for (int m = 0; m < listStation.size(); ++m) {

                        if (strIdSplit[j].compareTo(listStation.get(m).getStrID()) == 0) {

                            if (Stationflag) {
                                treeJson.append("},");
                            }
                            treeJson.append("{\"id\":\"" + id
                                    + "\",\"text\":\"" + listStation.get(m).getStrDescribe()
                                    + "\",\"state\":\"closed\""
                                    + ",\"stationID\":\"" + strIdSplit[j]
                                    + "\",\"level\":\"3\""
                                    + ",\"children\":[");

                            Stationflag = true;

                            id++;

                            Boolean Deviceflag = false;

                            for (int k = 0; k < listDevice.size(); ++k) {

                                // 跳过第一个虚拟设备
                                if (listDevice.get(k).getStrDeviceID().compareTo("1") == 0) {
                                    continue;
                                }

                                if (strIdSplit[j].compareTo(listDevice.get(k).getStrStationID()) == 0) {

                                    if (Deviceflag) {
                                        treeJson.append("},");
                                    }

                                    treeJson.append("{\"id\":\"" + id
                                            + "\",\"text\":\"" + listDevice.get(k).getStrDescribe()
                                            + "\",\"stationID\":\"" + listDevice.get(k).getStrStationID()
                                            + "\",\"stationName\":\"" + listStation.get(m).getStrDescribe()
                                            + "\",\"deviceID\":\"" + listDevice.get(k).getStrDeviceID()
                                            + "\",\"level\":\"4\"");

                                    Deviceflag = true;

                                    id++;
                                }
                            }

                            treeJson.append("}]");

                            break;
                        }
                    }
                }

                treeJson.append("}]");
            }
        }

        treeJson.append("}]");
        treeJson.append("}]");

        return treeJson.toString();
    }
}
