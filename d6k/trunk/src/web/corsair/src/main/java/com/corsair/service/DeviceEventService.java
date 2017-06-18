package com.corsair.service;

import com.corsair.dao.DeviceEventDao;
import com.corsair.device.DeviceEvent;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * Created by 洪祥 on 15/7/13.
 */
@Service("deviceEventService")
public class DeviceEventService {
    @Autowired
    private DeviceEventDao deviceEventDao;

    public List<DeviceEvent> getDeviceEvent(String from, String to, String device) {
        return deviceEventDao.getDeviceEvent(from, to, device);
    }
}
