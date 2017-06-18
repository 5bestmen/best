package com.controller.measurement;

import com.measurement.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * Created by zyj on 17/6/12.
 */
@RestController
@RequestMapping(value = "/measurement")
public class MeasurementController {
    @Autowired
    RealTimeClient realTimeClient;

    @RequestMapping(value = "/normal", method = RequestMethod.POST)
    @ResponseBody
    public ResponseEntity<List<MeasurementValue>> getNormalValues(@RequestBody List<MeasurementInfo> measurementInfos) {
        List<MeasurementValue> values = realTimeClient.getMeasurementValue(measurementInfos);
        return new ResponseEntity<List<MeasurementValue>>(values, HttpStatus.OK);
    }

}
