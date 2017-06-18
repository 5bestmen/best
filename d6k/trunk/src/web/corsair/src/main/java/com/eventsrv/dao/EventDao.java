package com.eventsrv.dao;

import com.eventsrv.entity.SysEvent;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.Criteria;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.criterion.Restrictions;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by ChengXi on 2015/12/30.
 */
@Repository("eventDao")
public class EventDao extends BaseDaoSupport {

    @Autowired
    protected SessionFactory sessionFactory;

    public Session getNewSession() {
        return sessionFactory.openSession();
    }

    public void save(Object bean) {
        try {
            Session session = getNewSession();
            session.save(bean);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void update(Object bean) {

        try {
            Session session = getNewSession();
            session.update(bean);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public List<SysEvent> getUnRead () {

        Session session = getNewSession();
        Criteria criteria = session.createCriteria(SysEvent.class);
        criteria.add(Restrictions.not(Restrictions.eq("read", (short) 0)));
        criteria.setMaxResults(100);

        List<SysEvent> list = criteria.list();

        session.clear();
        session.close();

        return list;
    }
}
