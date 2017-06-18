package com.rbac.controller.system;

import com.rbac.common.BaseController;
import com.rbac.form.system.AccountListForm;
import com.rbac.service.AccountService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import java.util.List;

/**
 * Created by chris on 2015/9/28.
 */
@Controller
public class AccountListController extends BaseController{
    @Autowired
    AccountService accountService;

    @RequestMapping(value ="/accountList")
    public ModelAndView accountListControll(AccountListForm accountListForm, HttpServletRequest request){
        ModelAndView accountModel = new ModelAndView("/accountList");
        if(CommonUtils.isNotBlank(accountListForm.getDel())){
            accountService.deleteAccount(Long.parseLong(accountListForm.getDeleteIds()), super.getCurrentAccountId(request));
        }
        List accountList = accountService.getSysAccountList(accountListForm.getUsernameQry(), accountListForm.getRealnameQry());
        accountModel.addObject("accountList", accountList);
        return accountModel;
    }

}
