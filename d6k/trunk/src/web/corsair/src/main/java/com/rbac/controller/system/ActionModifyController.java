package com.rbac.controller.system;

import com.rbac.common.BaseController;
import com.rbac.entity.SysAction;
import com.rbac.form.system.ActionModifyForm;
import com.rbac.service.ActionService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.util.Date;

/**
 * Created by lujs on 2015/9/29.
 */
@Controller
public class ActionModifyController  extends BaseController {
    @Autowired
    ActionService actionService;

    @RequestMapping(value= "/actionModify", method=RequestMethod.GET)
    public ModelAndView ActionModify(ActionModifyForm actionModifyForm,HttpServletRequest request,HttpSession session){
        ModelAndView model;
        SysAction action = new SysAction();
        if(CommonUtils.isNotBlank(actionModifyForm.getId())){
            Long id = CommonUtils.parseLong(actionModifyForm.getId());

            action = actionService.getActionById(id);

            actionModifyForm.setName(action.getName());
            actionModifyForm.setUrl(action.getUrl());


            model = new ModelAndView("actionModify");
            return model;
        }
        else{
            model = new ModelAndView("actionModify");
            return model;
        }
    }

    @RequestMapping(value= "/actionModify", method = RequestMethod.POST)
    public ModelAndView ActionModifyAction(ActionModifyForm actionModifyForm,HttpServletRequest request,HttpSession session){
        ModelAndView model;
        if(CommonUtils.isNotBlank(actionModifyForm.getSubmit())){
            SysAction action = new SysAction();
            if(CommonUtils.isNotBlank(actionModifyForm.getId())){
                Long id = CommonUtils.parseLong(actionModifyForm.getId());
                action = actionService.getActionById(id);
                action.setModifierId(super.getCurrentAccountId(request));
                action.setModifyTime(new Date());
            }
            else{
                action.setCreatorId(super.getCurrentAccountId(request));
                action.setCreateTime(new Date());
            }
            action.setName(actionModifyForm.getName());
            action.setUrl(actionModifyForm.getUrl());
            actionService.saveOrUpdateAction(action);
            model = new ModelAndView("redirect:/actionList");
            return model;
        }
        model = new ModelAndView("actionModify");
        return model;
    }
}
