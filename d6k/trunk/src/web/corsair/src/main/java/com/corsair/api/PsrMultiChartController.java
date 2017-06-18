package com.corsair.api;

import com.corsair.chart.*;

import com.corsair.device.*;
import com.corsair.service.ScadaPSRService;
import com.github.abel533.echarts.Option;
import com.github.abel533.echarts.json.GsonUtil;
import com.rbac.common.PagerOfAlarm;
import org.apache.poi.ss.formula.functions.T;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import javax.servlet.http.HttpServletRequest;
import java.util.List;
import java.util.Vector;


@RestController
@RequestMapping(value = "/multiCharts")
public class PsrMultiChartController {

    @Autowired
    ScadaPSRService scadaPSRService;

    @Autowired
    HistoryDataLineChart historyDataLineChart;

    /*@RequestMapping(value = "chart", method = RequestMethod.POST)
    public ResponseEntity<String> getMultiChart(@RequestBody HistoryDataQuery historyDataQuery) {
        //获取页面选择的所有测点
        List<HistoryData> historyDatas = historyDataQuery.getTemplates();
        Option option = historyDataLineChart.getChart(historyDatas, null);
        if (option == null) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        } else {
            return new ResponseEntity<>(GsonUtil.format(option), HttpStatus.OK);
        }
    }*/

    @RequestMapping(value = "/historyQueryOfDay", method = RequestMethod.POST)
    public ResponseEntity<List<List<String>>> getHistoryQueryOfDay(HttpServletRequest request) {

        String strStationID = request.getParameter("stationId");
        String strDeviceID = request.getParameter("deviceId");
        String strDate = request.getParameter("date");

        return new ResponseEntity<List<List<String>>>(scadaPSRService.getHisDayValueDataOf5Minute(strStationID, strDeviceID, strDate), HttpStatus.OK);
    }

    @RequestMapping(value = "/dayReport", method = RequestMethod.POST)
    public ResponseEntity<List<HisDayValueData>> getDayReport(HttpServletRequest request) {

        String strStationID = request.getParameter("stationId");
        String strDeviceID = request.getParameter("deviceId");
        String strDate = request.getParameter("date");

        return new ResponseEntity<List<HisDayValueData>>(scadaPSRService.getHisDayValueData(strStationID, strDeviceID, strDate), HttpStatus.OK);
    }
    /*public ResponseEntity<String> getDayChart(@RequestBody HistoryDataQuery historyDataQuery) {
        //获取页面选择的所有测点
        List<HistoryData> historyDatas = historyDataQuery.getTemplates();
        Option option = historyDataLineChart.getChart(historyDatas, null);
        if (option == null) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        } else {
            return new ResponseEntity<>(GsonUtil.format(option), HttpStatus.OK);
        }
    }*/

    //HisMonthValueData
    @RequestMapping(value = "/monthReport", method = RequestMethod.POST)
    public ResponseEntity<Vector<HisMonthValueData>> getMonthReport(HttpServletRequest request) {

        String strStationID = request.getParameter("stationId");
        String strDeviceID = request.getParameter("deviceId");
        String strDate = request.getParameter("date");

        return new ResponseEntity<Vector<HisMonthValueData>>(scadaPSRService.getHisMonthValueData(strStationID, strDeviceID, strDate), HttpStatus.OK);
    }


    @RequestMapping(value = "/alarmRecordDay", method = RequestMethod.POST)
    public ResponseEntity<PagerOfAlarm> searchAlarmRecordDay(HttpServletRequest request) {

        String strStationID = request.getParameter("stationId");
        String strDeviceID = request.getParameter("deviceId");
        String strDate = request.getParameter("date");
        String strAlarmType = request.getParameter("alarmType");
        String strCurrentPage = request.getParameter("currentPage");
        String strLevel = request.getParameter("level");
        String strChooseName = request.getParameter("chooseName");

        return new ResponseEntity<PagerOfAlarm>(scadaPSRService.searchAlarmRecordDay(strStationID, strDeviceID, strDate, strAlarmType, strCurrentPage, strLevel, strChooseName), HttpStatus.OK);
    }

    @RequestMapping(value = "/alarmRecordInterval", method = RequestMethod.POST)
    public ResponseEntity<PagerOfAlarm> getAlarmRecordInterval(HttpServletRequest request) {

        String strStationID = request.getParameter("stationId");
        String strDeviceID = request.getParameter("deviceId");
        String strBeginDate = request.getParameter("beginDate");
        String strEndDate = request.getParameter("endDate");
        String strAlarmType = request.getParameter("alarmType");
        String strCurrentPage = request.getParameter("currentPage");
        String strLevel = request.getParameter("level");
        String strChooseName = request.getParameter("chooseName");

        return new ResponseEntity<PagerOfAlarm>(scadaPSRService.getAlarmRecordInterval(strStationID, strDeviceID, strBeginDate, strEndDate, strAlarmType, strCurrentPage, strLevel, strChooseName), HttpStatus.OK);
    }

    @RequestMapping(value = "/historyDataOfDay", method = RequestMethod.POST)
    public ResponseEntity<List<His_AI_Day_Display>> getHistoryDataOfDay(HttpServletRequest request) {

        String strStationID = request.getParameter("stationId");
        String strDeviceID = request.getParameter("deviceId");
        String strDate = request.getParameter("date");
        String strDataType = request.getParameter("dataType");

        return new ResponseEntity<List<His_AI_Day_Display>>(scadaPSRService.getHistoryDataOfDay(strStationID, strDeviceID, strDate, strDataType), HttpStatus.OK);
    }

    @RequestMapping(value = "/getDeviceInfo", method = RequestMethod.POST)
    public ResponseEntity<List<String>> getDeviceInfo(HttpServletRequest request) {

        String strStationID = request.getParameter("stationID");
        String strDeviceID = request.getParameter("deviceID");

        return new ResponseEntity<List<String>>(scadaPSRService.getDeviceDesc(strStationID, strDeviceID), HttpStatus.OK);
    }

//    @RequestMapping(value = "/reportExport", method = RequestMethod.POST)
//    public ResponseEntity<Integer> reportExport(HttpServletRequest request) {
//
//        /*String strStationName = request.getParameter("stationName");
//        String strDeviceName = request.getParameter("deviceName");
//        String strName = request.getParameter("name");
//        String strDate = request.getParameter("date");*/
//
//
//        return new ResponseEntity<Integer>(scadaPSRService.getreportExportData(), HttpStatus.OK);
//    }

}