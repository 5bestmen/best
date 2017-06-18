package com.rbac.dao;

import com.rbac.common.BaseDaoSupport;
import com.rbac.entity.MailReceiver;
import com.rbac.entity.SysRoleMenu;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.List;

@Repository("mailReceiveDao")
public class MailReceiveDao extends BaseDaoSupport {
	
	/**
	 * 查询所有邮箱接收者信息
	 * @return
	 */
	public List<MailReceiver> getMailReceives(String mailName,String mailAddress){
		Criteria crit = super.currentSession().createCriteria(MailReceiver.class);
		if(CommonUtils.isNotEmpty(mailName)){
			crit.add(Restrictions.ilike("receiverName", mailName, MatchMode.ANYWHERE));
		}
		if(CommonUtils.isNotEmpty(mailAddress)){
			crit.add(Restrictions.ilike("mailAddress", mailAddress, MatchMode.ANYWHERE));
		}
		return crit.list();
	}
	/**
	 * 删除邮件接收人
	 * @return
	 */
	public void delMailReceiveById(Long mailId){
		MailReceiver receive= this.findById(MailReceiver.class, mailId);
		this.delete(receive);
	}
}
