package com.rbac.controller.system;

import com.rbac.common.BaseController;
import com.rbac.entity.SysAccount;
import com.rbac.entity.SysAccountRole;
import com.rbac.entity.SysRole;
import com.rbac.form.system.AccountModifyForm;
import com.rbac.service.AccountService;
import com.rbac.service.RoleService;
import com.rbac.util.CommonUtils;
import com.rbac.util.PasswordHash;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;
import java.util.Date;
import java.util.List;

/**
 * Created by chris on 2015/9/28.
 */
@Controller
public class AccountModifyController extends BaseController{

    @Autowired
    AccountService accountService;
    @Autowired
    RoleService roleService;

    @RequestMapping("accountModify")
    public ModelAndView accountModify(AccountModifyForm accountModifyForm , HttpServletRequest request) {

        ModelAndView model = new ModelAndView();
        if(CommonUtils.isNotBlank(accountModifyForm.getSubmit())){
            SysAccount account = new SysAccount();
            if(CommonUtils.isNotBlank(accountModifyForm.getId())){
                Long id = CommonUtils.parseLong(accountModifyForm.getId());
                account = accountService.getAccountById(id);
                account.setModifierId(super.getCurrentAccountId(request));
                //就算在mysql中加了current stamp on update，也需要配置hibernate后才能正常运行
                account.setModifyTime(new Date());
            }
            else{
                account.setCreatorId(super.getCurrentAccountId(request));
                account.setCreateTime(new Date());
                account.setModifierId(super.getCurrentAccountId(request));
                account.setModifyTime(new Date());
            }

            account.setUsername(accountModifyForm.getUsername());
            account.setRealname(accountModifyForm.getRealname());
            account.setRoleIds(accountModifyForm.getRoleId());

            if(CommonUtils.isNotBlank(accountModifyForm.getPassword())){
                try {
                    String pwdHash = PasswordHash.createHash(accountModifyForm.getPassword().trim());
                    if(CommonUtils.isNotBlank(pwdHash)){
                        String[] pwd = pwdHash.split(":");
                        if(pwd!=null && pwd.length==3){
                            account.setPassword(pwd[2]);
                            account.setSalt(pwd[1]);
                        }
                    }
                } catch (NoSuchAlgorithmException e) {
                    e.printStackTrace();
                } catch (InvalidKeySpecException e) {
                    e.printStackTrace();
                }
            }
            accountService.saveOrUpdateAccount(account);
            model.setViewName("forward:/accountList");

        }
        else {
            List roleList = roleService.getSysRoleList(null, null);
            model.setViewName("accountModify");
            model.addObject("roleList",roleList);
//add by zjq 2016.1.22
            if(CommonUtils.isNotBlank(accountModifyForm.getId())){
                Long id = Long.parseLong(accountModifyForm.getId());
                SysAccount account = accountService.getAccountById(id);
                if(account!=null){
                    accountModifyForm.setUsername(account.getUsername());
                    accountModifyForm.setRealname(account.getRealname());
                    List<SysAccountRole> accountRoleList = accountService.getSysAccountRoleByAccountId(id);
                    if(accountRoleList.size()>0){
                        SysRole role = accountRoleList.get(0).getSysRole();
                        if(role!=null){
                            accountModifyForm.setRoleId(role.getId().toString());
                        }
                    }
                }
            }
        }
        return model;
    }

}
