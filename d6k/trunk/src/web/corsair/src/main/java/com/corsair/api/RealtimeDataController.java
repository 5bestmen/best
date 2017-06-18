package com.corsair.api;

import com.corsair.device.*;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.http.HttpStatus;
import javax.servlet.http.HttpServletRequest;
import java.util.List;

/**
 * Created by zyj on 2016/11/30.
 */

@RestController
@RequestMapping(value = "/realtimeCharts")
public class RealtimeDataController {

    @Autowired
    ScadaPSRService scadaPSRService;

    @RequestMapping(value = "/rdbData", method = RequestMethod.POST)
    public ResponseEntity<DeviceInfo> getAiDiCalc(HttpServletRequest request) {

        String strStationID = request.getParameter("stationId");
        String strDeviceID = request.getParameter("deviceId");

        return new ResponseEntity<DeviceInfo>(scadaPSRService.getDeviceInfo(strStationID, strDeviceID), HttpStatus.OK);
    }

    @RequestMapping(value = "/rdbDataAI", method = RequestMethod.POST)
    public ResponseEntity<List<AiInfo>> getAiInfo(HttpServletRequest request) {

        String strStationID = request.getParameter("stationId");
        String strDeviceID = request.getParameter("deviceId");

        return new ResponseEntity<List<AiInfo>>(scadaPSRService.getAiInfo(strStationID, strDeviceID), HttpStatus.OK);
    }

    @RequestMapping(value = "/rdbDataDI", method = RequestMethod.POST)
    public ResponseEntity<List<DiInfo>> getDiInfo(HttpServletRequest request) {

        String strStationID = request.getParameter("stationId");
        String strDeviceID = request.getParameter("deviceId");

        return new ResponseEntity<List<DiInfo>>(scadaPSRService.getDiInfo(strStationID, strDeviceID), HttpStatus.OK);
    }

    @RequestMapping(value = "/rdbDataCalc", method = RequestMethod.POST)
    public ResponseEntity<List<CalcInfo>> getCalcInfo(HttpServletRequest request) {

        String strStationID = request.getParameter("stationId");
        String strDeviceID = request.getParameter("deviceId");

        return new ResponseEntity<List<CalcInfo>>(scadaPSRService.getCalcInfo(strStationID, strDeviceID), HttpStatus.OK);
    }

    @RequestMapping(value = "/totalDataCalc", method = RequestMethod.POST)
    public ResponseEntity<Total_YMD_Info> getTotalCalcInfo(HttpServletRequest request) {

        String strName = request.getParameter("name");
        String strType = request.getParameter("type");
        String strDate = request.getParameter("date");

        return new ResponseEntity<Total_YMD_Info>(scadaPSRService.getTotalCalcInfo(strName, strType, strDate), HttpStatus.OK);
    }

    // 获取节能分析数据
    @RequestMapping(value = "/energySavingAnalysis", method = RequestMethod.POST)
    public ResponseEntity<Total_YMD_Info> getEnergySavingAnalysisInfo(HttpServletRequest request) {

        String strDate = request.getParameter("date");
        String strStationID = request.getParameter("stationId");
        String strDeviceID = request.getParameter("deviceId");

        return new ResponseEntity<Total_YMD_Info>(scadaPSRService.getDeviceCalcInfo(strDate, strStationID, strDeviceID), HttpStatus.OK);
    }

    @RequestMapping(value = "/rdbDeviceRunningInfo", method = RequestMethod.POST)
    public ResponseEntity<DeviceRunningInfo> getDeviceRunningInfo(HttpServletRequest request) {

        String strName = request.getParameter("name");
        String strType = request.getParameter("type");

        return new ResponseEntity<DeviceRunningInfo>(scadaPSRService.getDeviceRunningInfo(strName, strType), HttpStatus.OK);
    }
}
