package com.rbac.controller.system;

import com.rbac.common.BaseController;
import com.rbac.form.system.ActionListForm;
import com.rbac.service.ActionService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.util.List;

/**
 * Created by lujs on 2015/9/28.
 */
@Controller
public class ActionListController  extends BaseController {
    @Autowired
    ActionService actionService;

    @RequestMapping(value = "/actionList", method = RequestMethod.GET)
    public String ActionListRedirect(ActionListForm actionListForm,HttpServletRequest request,HttpSession session){
        List actionList = actionService.getSysActionList(actionListForm
                .getNameQry(), actionListForm.getUrlQry());
        request.setAttribute("actionList", actionList);
        return "actionList";
    }

    @RequestMapping(value = "/actionList", method = RequestMethod.POST)
    public ModelAndView ActionListAction(ActionListForm actionListForm,HttpServletRequest request,HttpSession session){
        ModelAndView model;

        if(CommonUtils.isNotBlank(actionListForm.getDel())){
            actionService.deleteAction(Long.parseLong(actionListForm.
                    getDeleteIds()),super.getCurrentAccountId(request));
        }
        List actionList = actionService.getSysActionList(actionListForm.getNameQry(),
                actionListForm.getUrlQry());
        session.setAttribute("actionList",actionList);
        model = new ModelAndView("actionList");
        return model;
    }
}
