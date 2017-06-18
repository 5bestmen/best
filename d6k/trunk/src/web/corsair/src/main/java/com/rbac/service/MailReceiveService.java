package com.rbac.service;

import com.rbac.common.MenuTree;
import com.rbac.dao.MailReceiveDao;
import com.rbac.dao.MenuDao;
import com.rbac.dao.RoleDao;
import com.rbac.entity.*;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.Date;
import java.util.List;

@Service("mailReceiveService")
public class MailReceiveService {

	@Autowired
	private MailReceiveDao mailReceiveDao;
	
	/**
	 * 查询所有邮箱接收者信息
	 * @param mailName
	 * @param mailAddress
	 * @return
	 */
	public List<MailReceiver> getMailReceives(String mailName,String mailAddress){
		return mailReceiveDao.getMailReceives(mailName,mailAddress);
	}
	/**
	 * 根据接收人id查找接收人信息
	 * @param mailReceiverId
	 * @return
	 */
	public MailReceiver getMailReceiverById(Long mailReceiverId){
		return mailReceiveDao.findById(MailReceiver.class, mailReceiverId);
	}

	/**
	 * 删除邮件人
	 * @param mailReceiverId
	 * @return
	 */
	public void delMailReceiveById(Long mailReceiverId){
		 mailReceiveDao.delMailReceiveById(mailReceiverId);
	}


	/**
	 * 保存邮件接收者实体
	 * @param mailReceiver
	 */
	public void saveOrUpdateMailReceive(MailReceiver mailReceiver){
		mailReceiveDao.saveOrUpdate(mailReceiver);
	}

}
