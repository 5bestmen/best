package com.eventsrv.service;

import com.eventsrv.dao.EventFilterDao;
import com.eventsrv.entity.SysEventFilter;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * Created by ChengXi on 2016/1/29.
 */
@Service("eventFilterService")
public class EventFilterService {

    @Autowired
    private EventFilterDao eventFilterDao;

    public boolean isFilter(short type) {

        List<SysEventFilter> list = eventFilterDao.getEventFilter();
        for (SysEventFilter ef : list) {
            if(ef.getType() == type) {
                return true;
            }
        }

        return false;
    }
}
