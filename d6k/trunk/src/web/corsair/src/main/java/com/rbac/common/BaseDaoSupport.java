package com.rbac.common;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.orm.hibernate4.support.HibernateDaoSupport;
import org.springframework.stereotype.Component;

import java.io.Serializable;

@Component("baseDaoSupport")
public class BaseDaoSupport extends HibernateDaoSupport implements BaseDao {

	private static final Log log = LogFactory.getLog(BaseDaoSupport.class);

	public BaseDaoSupport() {
	}

	/**
	 * 保存实体
	 */
	public <T> void saveOrUpdate(T t) {
		getHibernateTemplate().saveOrUpdate(t);
	}

	/**
	 * 删除实体
	 */
	public <T> void delete(T t) {
		getHibernateTemplate().delete(t);
	}

	/**
	 * 根据主键查找实体
	 */
	public <T> T findById(Class clazz, Serializable id) {
		return (T)getHibernateTemplate().get(clazz, id);
	}

}