package com.rbac.controller.system;

import com.rbac.common.BaseController;
import com.rbac.entity.SysAccount;
import com.rbac.form.user.PwdModifyForm;
import com.rbac.service.AccountService;
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

/**
 * Created by chris on 2015/9/30.
 */
@Controller
public class PwdModifyController extends BaseController {
    @Autowired
    AccountService accountService;

    @RequestMapping("/pwdModify")
    public ModelAndView pwdModifyControll(PwdModifyForm pwdModifyForm, HttpServletRequest request) {
        ModelAndView model = new ModelAndView("pwdModify");

        if(CommonUtils.isNotBlank(pwdModifyForm.getSubmit())){
            if(CommonUtils.isNotBlank(pwdModifyForm.getId())){
                Long accountId = CommonUtils.parseLong(pwdModifyForm.getId());
                SysAccount account = accountService.getAccountById(accountId);
                account.setModifierId(this.getCurrentAccountId(request));
                account.setModifyTime(new Date());
                if(CommonUtils.isNotBlank(pwdModifyForm.getPassword())){
                    try {
                        String pwdHash = PasswordHash.createHash(pwdModifyForm.getPassword().trim());
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
                model.addObject("msg", "密码修改成功");
            }
        }
        else {
            Long id = super.getCurrentAccountId(request);
            pwdModifyForm.setId(id.toString());
        }
        return model;
    }
}
