package com.rbac.controller.system;

import com.corsair.service.ApplicationInformationService;
import com.rbac.common.BaseController;
import com.rbac.entity.SysAction;
import com.rbac.entity.SysMenu;
import com.rbac.entity.SysMenuAction;
import com.rbac.form.system.MenuModifyForm;
import com.rbac.service.ActionService;
import com.rbac.service.MenuService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.Date;
import java.util.List;

@Controller
public class MenuModifyController  extends BaseController {
    @Autowired
    MenuService menuService;
    @Autowired
    ActionService actionService;
    @Autowired
    ApplicationInformationService applicationInformationService;

    @RequestMapping(value = "/menuModify",method = RequestMethod.POST)
    public ModelAndView menuModifyAction(MenuModifyForm menuModifyForm,
                                 HttpServletRequest request, HttpServletResponse response) {
        ModelAndView model;
        if(CommonUtils.isNotBlank(menuModifyForm.getSubmit())){
            SysMenu menu = new SysMenu();
            if(CommonUtils.isNotBlank(menuModifyForm.getId())){
                Long id = CommonUtils.parseLong(menuModifyForm.getId());
                menu = menuService.getMenuById(id);
                menu.setModifierId(super.getCurrentAccountId(request));
                menu.setModifyTime(new Date());
            }
            else{
                menu.setCreatorId(super.getCurrentAccountId(request));
                menu.setCreateTime(new Date());
                menu.setModifierId(super.getCurrentAccountId(request));
                menu.setModifyTime(new Date());
            }
            menu.setName(menuModifyForm.getName());
            menu.setUrl(menuModifyForm.getUrl());
            menu.setParentId(CommonUtils.parseLong(menuModifyForm.getParentId()));
            menu.setOrderSeq(CommonUtils.parseInteger(menuModifyForm.getOrderSeq()));
            menu.setActionIds(menuModifyForm.getActionId());
            menuService.saveOrUpdateMenu(menu);
            model = new ModelAndView("redirect:/menuList");

        }else{
            model = new ModelAndView("menuModify");
        }

        return model;
    }


    @RequestMapping(value = "/menuModify", method = RequestMethod.GET)
    public ModelAndView menuModifyRedirect(MenuModifyForm menuModifyForm,
                                           HttpServletRequest request, HttpServletResponse response) {
        ModelAndView model;
        List<SysMenu> menuList = menuService.getSysMenuList("", "");
        List<SysAction> actionList = actionService.getSysActionList("","");

        if(CommonUtils.isNotBlank(menuModifyForm.getId())) {
            Long id = CommonUtils.parseLong(menuModifyForm.getId());
            SysMenu menu = menuService.getMenuById(id);
                List<SysMenuAction> menuAction = menuService.getSysMenuActionByMenuId(id);
            Long current_action_id = -1L;
            if (menuAction != null && !menuAction.isEmpty()){
                current_action_id = menuAction.get(0).getSysAction().getId();
            }

            menuModifyForm.setName(menu.getName());
            menuModifyForm.setUrl(menu.getUrl());
            menuModifyForm.setParentId(menu.getParentId() == null ? "" : menu.getParentId().toString());
            menuModifyForm.setOrderSeq(menu.getOrderSeq() == null ? "" : menu.getOrderSeq().toString());
            menuModifyForm.setActionId(current_action_id == -1 ? "" : current_action_id.toString());

        }
        model = new ModelAndView("menuModify");
        model.addObject("menuList", menuList);
        model.addObject("actionList", actionList);
        return model;
    }
}