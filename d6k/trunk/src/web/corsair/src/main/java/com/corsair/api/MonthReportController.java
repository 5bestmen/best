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

public class MonthReportController {

    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    HistoryDataDao historyDataDao;

    @RequestMapping(value = "monthReport", method = RequestMethod.POST)
    public ResponseEntity<String> getMonthReport(String date) {

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
        field0.put("title", "日期");
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

        String [][] retData= new String[32][dayReportCols+1];
        List<String> days = new ArrayList<>();

        for(int i=0; i<32; i++) {
            if(i!=31) {
                days.add(i + 1 + "日");
            } else {
                days.add("月累计");
            }
        }

        int   expireYear   =   Integer.parseInt(date.substring(0, 4));
        int   expireMonth   =   Integer.parseInt(date.substring(5, 7));
        int   expireDay   =   1;
        String tableName = "DAYKWH" +date.substring(0, 4)+date.substring(5, 7);

        //计算每月的第一天天数
        Calendar calendar   =   Calendar.getInstance();
        calendar.set(expireYear,   expireMonth-1,   expireDay);
        long   datas   =   calendar.getTime().getTime();
        int startDays = (int) ((datas / 1000 / 3600 + 8) / 24) ;



        List<HistoryDataValue> historyDataValueList = null;
        Map hisdates2DataMap = new HashedMap();
        double total = 0;

        for (int col=0; col<dayReportCols+1; col++) {
            if(col > 0){
                String energyId = scadaPSRService.getEnergyId(basePSRList.get(col-1).getId());
                historyDataValueList = historyDataDao.getHistoryData(tableName, energyId, startDays + 31 , 79);
                for (int i = 0; i < historyDataValueList.size(); i++){
                    hisdates2DataMap.put(historyDataValueList.get(i).getSdate(), historyDataValueList.get(i).getData());
                }
            }
            for (int row=0; row<days.size(); row++) {

                if (col==0) {
                    retData[row][0] = days.get(row);
                } else {
                    if (row!=days.size()-1) {
                        if (hisdates2DataMap.containsKey(startDays + row)){
                            retData[row][col] = String.valueOf(hisdates2DataMap.get(startDays + row));
                            total += ((Number) hisdates2DataMap.get(startDays + row)).doubleValue();
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
