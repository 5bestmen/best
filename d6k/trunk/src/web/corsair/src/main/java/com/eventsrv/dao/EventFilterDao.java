package com.eventsrv.dao;

import com.eventsrv.entity.SysEventFilter;
import org.hibernate.Criteria;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.criterion.Restrictions;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by ChengXi on 2016/1/29.
 */
@Repository("EventFilterDao")
public class EventFilterDao {

    @Autowired
    protected SessionFactory sessionFactory;

    public Session getNewSession() {
        return sessionFactory.openSession();
    }

    public List <SysEventFilter> getEventFilter() {
        Session session = getNewSession();
        Criteria criteria = session.createCriteria(SysEventFilter.class);
        criteria.add(Restrictions.eq("filter", (short) 1));

        List<SysEventFilter> list = criteria.list();

        session.clear();
        session.close();

        return list;
    }
}
