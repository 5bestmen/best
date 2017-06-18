package com.rbac.service;

import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.corsair.service.MenuInjectorService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.rbac.common.MenuTree;
import com.rbac.common.UserDetail;
import com.rbac.dao.LoginDao;
import com.rbac.entity.SysAccount;
import com.rbac.entity.SysAction;
import com.rbac.entity.SysMenu;
import com.rbac.util.CommonUtils;
import com.rbac.util.PasswordHash;
//import com.rbac.entity.test;

@Service("loginService")
public class LoginService {

	@Autowired
	private LoginDao loginDao;

	@Autowired
	private MenuInjectorService menuInjectorService;

	/**
	 * 登录功能
	 * @param username
	 * @param password
	 * @return UserDetail
	 */
	public UserDetail login(String username, String password) {
		// 就算根据用户名没找到用户，也要去调用验证密码方法，防止别人根据返回时间猜测用户名
		SysAccount account = loginDao.getSysAccountByUsername(username);

		Long id = 111L;

		//List<test> name = loginDao.getNameFromId(id);
		String userhash = "11";
		String usersalt = "11";
		if (account != null && CommonUtils.isNotBlank(account.getPassword())
				&& CommonUtils.isNotBlank(account.getSalt())) {
			userhash = account.getPassword();
			usersalt = account.getSalt();
		}
		StringBuilder s = new StringBuilder();
		s.append(PasswordHash.PBKDF2_ITERATIONS).append(":").append(usersalt)
				.append(":").append(userhash);

		String strPwdSql = account.getPassword();
		if (strPwdSql.compareTo(password) == 0) {
			UserDetail userDetail = new UserDetail();
			userDetail.setAccount(account);

			userDetail.setMenuList(MenuTree.getMenu(menuInjectorService.inject(loginDao.getMenuListByAccountId(account.getId())),false));
			userDetail.setPermitActionSet(this.getPermitActionSet(account.getId()));
			return userDetail;
		}

		return null;
	}
	/*public UserDetail login(String username, String password) {
		// 就算根据用户名没找到用户，也要去调用验证密码方法，防止别人根据返回时间猜测用户名
		SysAccount account = loginDao.getSysAccountByUsername(username);

		Long id = 111L;

		//List<test> name = loginDao.getNameFromId(id);
		String userhash = "11";
		String usersalt = "11";
		if (account != null && CommonUtils.isNotBlank(account.getPassword())
				&& CommonUtils.isNotBlank(account.getSalt())) {
			userhash = account.getPassword();
			usersalt = account.getSalt();
		}
		StringBuilder s = new StringBuilder();
		s.append(PasswordHash.PBKDF2_ITERATIONS).append(":").append(usersalt)
				.append(":").append(userhash);
		try {
			if (PasswordHash.validatePassword(password, s.toString())) {
				UserDetail userDetail = new UserDetail();
				userDetail.setAccount(account);

                userDetail.setMenuList(MenuTree.getMenu(menuInjectorService.inject(loginDao.getMenuListByAccountId(account.getId())),false));
				userDetail.setPermitActionSet(this.getPermitActionSet(account.getId()));
				return userDetail;
			}
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		} catch (InvalidKeySpecException e) {
			e.printStackTrace();
		}

		return null;
	}*/
	
	/**
	 * 获取用户权限，包括菜单自身权限和菜单相关权限
	 * @param accountId
	 * @return
	 */
	private Set<String> getPermitActionSet(Long accountId){
		Set<String> permitActionSet = new HashSet<String>();
		List<SysMenu> menuList = loginDao.getMenuListByAccountId(accountId);
		List<SysAction> actionList = loginDao.getActionListByAccountId(accountId);
		for(SysMenu menu : menuList){
			permitActionSet.add(menu.getUrl());
		}
		for(SysAction action : actionList){
			permitActionSet.add(action.getUrl());
		}
		return permitActionSet;
	}
}
