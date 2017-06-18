package com.eventsrv.service;

import com.eventsrv.dao.EventDao;
import com.eventsrv.entity.SysEvent;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * Created by ChengXi on 2015/12/30.
 */
@Service("eventService")
public class EventService {

    @Autowired
    private EventDao eventDao;

    public void saveEvent(SysEvent se) {
        eventDao.save(se);
    }

    public void updateEvent(SysEvent se) {
        eventDao.update(se);
    }

    public List<SysEvent> getUnRead() {
        return eventDao.getUnRead();
    }

}
