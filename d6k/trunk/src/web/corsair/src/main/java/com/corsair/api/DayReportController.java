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
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

/**
 * Created by zjq on 2016/1/26.
 */
@RestController

public class DayReportController {

    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    HistoryDataDao historyDataDao;

    @RequestMapping(value = "dayReport", method = RequestMethod.POST)
    public ResponseEntity<String> stationInverterGeneratedChart(String date) {

        SimpleDateFormat dateParse = new SimpleDateFormat("yyyy/MM/dd");


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
        String [][] retData= new String[25][dayReportCols+1];

        JSONObject field0 = new JSONObject();
        field0.put("id", "0");
        field0.put("title", "时间");
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

        List<String> times = new ArrayList<>();

        for(int i=0; i<25; i++) {
            if(i!=24) {
                times.add(i + 1 + ":00");
            } else {
                times.add("日累计");
            }
        }


        double total = 0;
        Calendar c   =   Calendar.getInstance();
        int   expireYear   =   Integer.parseInt(date.substring(0, 4));
        int   expireMonth   =   Integer.parseInt(date.substring(5, 7));
        int   expireDay   =   Integer.parseInt(date.substring(8,10));
        try {
            Date requestDate = dateParse.parse((date));

            c.setTime(requestDate);
        } catch (ParseException e) {
            return new ResponseEntity<String>(HttpStatus.NOT_FOUND);
        }

        long   datas   =   c.getTime().getTime();
        int days = (int) ((datas / 1000 / 3600 + 8) / 24);
        String tableName = "KWH" +date.substring(0, 4)+date.substring(5, 7);

        List<HistoryDataValue> historyDataValueList = null;

        for (int col=0; col<dayReportCols+1; col++) {
            if(col > 0){
                String energyId = scadaPSRService.getEnergyId(basePSRList.get(col-1).getId());
                historyDataValueList = historyDataDao.getHistoryData(tableName,energyId,days,0,1440,52);
            }
            for (int row=0; row<times.size(); row++) {
                if (col==0) {
                    retData[row][0] = times.get(row);
                } else {

                    if (row!=times.size()-1){
                        Double beforeValue = 0.0;
                        Double afterValue = -1.0;
                        boolean beforeValueFlag = false;
                        boolean afterValueFlag = false;
                        for (int i = 0; i < historyDataValueList.size(); i++) {
                            int oldDayMinuteTime = historyDataValueList.get(i).getTime();
                            Double hourPointVale = historyDataValueList.get(i).getData();
                            if (hourPointVale == 0) continue;
                            if (row == 0){
                                if ((oldDayMinuteTime > (row * 60 ))&&(oldDayMinuteTime <= (row * 60 + 15)) ){
                                    beforeValue = hourPointVale;
                                    beforeValueFlag = true;
                                }
                                if ((oldDayMinuteTime > ((row+1) * 60 ))&&(oldDayMinuteTime <= ((row+1) * 60 + 15)) ){
                                    afterValue = hourPointVale;
                                    afterValueFlag = true;
                                }
                            }
                            else {
                                if ((oldDayMinuteTime >= (row * 60 ))&&(oldDayMinuteTime < (row * 60 + 15)) ){
                                    beforeValue = hourPointVale;
                                    beforeValueFlag = true;
                                }
                                if ((oldDayMinuteTime >= ((row+1) * 60 ))&&(oldDayMinuteTime < ((row+1) * 60 + 15)) ){
                                    afterValue = hourPointVale;
                                    afterValueFlag = true;
                                }
                            }

                            if (beforeValueFlag && afterValueFlag){
                                retData[row][col] = String.valueOf(afterValue-beforeValue);
                                total += (afterValue-beforeValue);
                                break;
                            }
                        }
                        if(!beforeValueFlag || !afterValueFlag)
                        {
                            retData[row][col] = String.valueOf(0);
                        }
                    }
                    else {
                        retData[row][col] = String.valueOf(total);
                        total = 0;
                    }

//                    if (row!=times.size()-1) {
//                        if (row<7||row>20) {
//                            random = Math.random()+2 ;
//                            total += random;
//                            retData[row][col] = String.valueOf(random);
//                        } else {
//                            random = Math.random() * 10 + (7 - Math.abs(row - 14)) * 10;
//                            total += random;
//                            retData[row][col] = String.valueOf(random);
//                        }
//                    } else {
//                        retData[row][col] = String.valueOf(total);
//                        total = 0;
//                    }
                }
            }
        }

        jsonObject.put("data", retData);
        jsonObject.put("FieldName", fieldArray);
        return  new ResponseEntity<>(jsonObject.toString(), HttpStatus.OK);
    }

}
