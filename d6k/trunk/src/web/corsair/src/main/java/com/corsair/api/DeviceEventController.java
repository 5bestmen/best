package com.corsair.api;

import com.corsair.device.DeviceEvent;
import com.corsair.service.DeviceEventService;
import net.sf.json.JSONArray;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * Created by zjq on 2016/1/26.
 */
@RestController
public class DeviceEventController {
    @Autowired
    DeviceEventService deviceEventService;

    @RequestMapping(value = "deviceEvent", method = RequestMethod.POST, produces = "text/html;charset=UTF-8")
    public ResponseEntity<String> getDeviceEvent(String from, String to, String device){

        List<DeviceEvent> deviceEventList = deviceEventService.getDeviceEvent(from, to, device);
        JSONArray jsonArray = JSONArray.fromObject(deviceEventList);
        return new ResponseEntity<>(jsonArray.toString(), HttpStatus.OK);
    }
}
