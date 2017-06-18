package com.corsair.api;

import com.alibaba.fastjson.JSONObject;
import com.corsair.dao.HistoryDataDao;
import com.corsair.device.BasePSR;
import com.corsair.device.Inverter;
import com.corsair.device.PackageTransformer;
import com.corsair.device.PhotovoltaicArray;
import com.corsair.measurement.HistoryDataValue;
import com.corsair.service.ScadaPSRService;
import net.sf.json.JSONArray;
import org.apache.commons.collections.map.HashedMap;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;
import java.util.Map;

/**
 * Created by zjq on 2016/1/26.
 */
@RestController

public class YearReportController {

    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    HistoryDataDao historyDataDao;

    @RequestMapping(value = "yearReport", method = RequestMethod.POST)
    public ResponseEntity<String> getYearReport(String date) {



        //适用于站级监控，数据中心从其他方式获取；获取厂站的所有设备
        List<BasePSR> basePSRList = new ArrayList<>();

        List<PhotovoltaicArray> photovoltaicArrayList = scadaPSRService.getPVArray();
        for (PhotovoltaicArray photovoltaicArray : photovoltaicArrayList){
            List<BasePSR> inverterList = scadaPSRService.getInverter(photovoltaicArray.getId());
            basePSRList.addAll(inverterList);
            PackageTransformer packageTransformerList = scadaPSRService.getPackageTransformer(photovoltaicArray.getId());
            BasePSR basePSR = new BasePSR();
            basePSR.setId(packageTransformerList.getId());
            basePSR.setName(packageTransformerList.getName());
            basePSRList.add(basePSR);
        }

        int dayReportCols = basePSRList.size();

        JSONObject jsonObject = new JSONObject();
        JSONArray fieldArray = new JSONArray();

        JSONObject field0 = new JSONObject();
        field0.put("id", "0");
        field0.put("title", "月份");
        field0.put("type", "string");
        field0.put("columnClass", "text-center");
        fieldArray.add(field0);
        for (int i = 0; i < dayReportCols; i++){
            JSONObject field = new JSONObject();
            field.put("id", i+1);
            field.put("title", basePSRList.get(i).getName()+"发电量(kWh)");
            field.put("type", "number");
            field.put("format", "###.##");
            field.put("columnClass", "text-center");
            fieldArray.add(field);
        }

        String [][] retData= new String[13][dayReportCols+1];
        List<String> months = new ArrayList<>();

        for(int i=0; i<13; i++) {
            if(i!=12) {
                months.add(i + 1 + "月");
            } else {
                months.add("年累计");
            }
        }



        int   expireYear   =   Integer.parseInt(date.substring(0, 4));
        String tableName = "MONTHKWH" +expireYear;

        Calendar calendar   =   Calendar.getInstance();
        int curMonth=calendar.get(Calendar.MONTH)+1;
        int currentMonths = (calendar.get(Calendar.YEAR) - 1970) * 12 + calendar.get(Calendar.MONTH)+1;
        int startMonths = currentMonths - curMonth + 1;

        double total = 0;
        List<HistoryDataValue> historyDataValueList = null;
        Map hisdates2DataMap = new HashedMap();

        for (int col=0; col<dayReportCols+1; col++) {
            if(col > 0){
                String energyId = scadaPSRService.getEnergyId(basePSRList.get(col-1).getId());
                historyDataValueList = historyDataDao.getHistoryData(tableName, energyId, startMonths + 12 , 79);
                for (int i = 0; i < historyDataValueList.size(); i++){
                    hisdates2DataMap.put(historyDataValueList.get(i).getSdate(), historyDataValueList.get(i).getData());
                }
            }

            for (int row=0; row<months.size(); row++) {
                if (col==0) {
                    retData[row][0] = months.get(row);
                } else {
                    if (row!=months.size()-1) {
                        if (hisdates2DataMap.containsKey(startMonths + row)){
                            retData[row][col] = String.valueOf(hisdates2DataMap.get(startMonths + row));
                            total += ((Number) hisdates2DataMap.get(startMonths + row)).doubleValue();
                        }
                        else {
                            retData[row][col] = String.valueOf(0);
                        }
                    } else {
                        retData[row][col] = String.valueOf(total);
                        total = 0;
                    }
                }
            }
        }

        jsonObject.put("data", retData);
        jsonObject.put("FieldName", fieldArray);
        return  new ResponseEntity<String>(jsonObject.toString(), HttpStatus.OK);
    }

}
