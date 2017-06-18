package com.corsair.api;

import com.corsair.measurement.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * Created by 洪祥 on 16/1/5.
 */
@RestController
@RequestMapping(value = "/measurement")
public class MeasurementController {
    @Autowired
    CalculateValue calculateValue;
    @Autowired
    RealTimeClient realTimeClient;
    @Autowired
    MagicValue magicValue;

    @RequestMapping(value = "calculate", method = RequestMethod.POST)
    public ResponseEntity<List<MeasurementValue>> getCalculateValues(@RequestBody List<MeasurementInfo> measurementInfos) {
        List<MeasurementValue> values = calculateValue.getMeasurementValue(measurementInfos);
        return new ResponseEntity<List<MeasurementValue>>(values, HttpStatus.OK);
    }

    @RequestMapping(value = "normal", method = RequestMethod.POST)
    public ResponseEntity<List<MeasurementValue>> getNormalValues(@RequestBody List<MeasurementInfo> measurementInfos) {
        List<MeasurementValue> values = realTimeClient.getMeasurementValue(measurementInfos);
        return new ResponseEntity<List<MeasurementValue>>(values, HttpStatus.OK);
    }

    @RequestMapping(value = "magic", method = RequestMethod.POST)
    public ResponseEntity<List<MeasurementValue>> getMagicValues(@RequestBody List<MeasurementInfo> measurementInfos) {
        List<MeasurementValue> values = magicValue.getMeasurementValue(measurementInfos);
        return new ResponseEntity<List<MeasurementValue>>(values, HttpStatus.OK);
    }

}
