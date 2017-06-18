package com.rbac.controller.system;

import com.rbac.common.BaseController;
import com.rbac.entity.SysRole;
import com.rbac.form.system.RoleModifyForm;
import com.rbac.service.RoleService;
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
 * Created by 洪祥 on 2015/10/08.
 */
@Controller
public class RoleModifyController extends BaseController{
    @Autowired
    RoleService roleService;

    @RequestMapping(value= "/roleModify", method= RequestMethod.GET)
    public ModelAndView ActionModify(RoleModifyForm roleModifyForm,HttpServletRequest request,HttpSession session){

        ModelAndView model;

        if(CommonUtils.isNotBlank(roleModifyForm.getId())){
            Long id = CommonUtils.parseLong(roleModifyForm.getId());
            SysRole role = roleService.getRoleById(id);

            roleModifyForm.setRoleName(role.getRoleName());
            roleModifyForm.setRoleDesc(role.getRoleDesc());
            roleModifyForm.setMenuIds(role.getMenuIds());

            model = new ModelAndView("roleModify");
            model.addObject("menuJson",roleService.getCheckedMenuTree(Long.parseLong(roleModifyForm.getId())));
            return model;
        }
        else{
            model = new ModelAndView("roleModify");
            model.addObject("menuJson",roleService.getCheckedMenuTree(null));
            return model;
        }
    }

    @RequestMapping(value= "/roleModify", method = RequestMethod.POST)
    public ModelAndView ActionModifyAction(RoleModifyForm roleModifyForm,HttpServletRequest request,HttpSession session){
        ModelAndView model;
        if(CommonUtils.isNotBlank(roleModifyForm.getSubmit())){
            SysRole role = new SysRole();
            if(CommonUtils.isNotBlank(roleModifyForm.getId())){
                Long id = CommonUtils.parseLong(roleModifyForm.getId());
                role = roleService.getRoleById(id);
                role.setModifierId(super.getCurrentAccountId(request));
                role.setModifyTime(new Date());
            }else {
                role.setCreatorId(super.getCurrentAccountId(request));
                role.setCreateTime(new Date());
                role.setModifierId(super.getCurrentAccountId(request));
                role.setModifyTime(new Date());
            }
            role.setRoleDesc(roleModifyForm.getRoleDesc());
            role.setRoleName(roleModifyForm.getRoleName());
            role.setMenuIds(roleModifyForm.getMenuIds());
            roleService.saveOrUpdateRole(role);
            model = new ModelAndView("redirect:/roleList");
            return model;
        }
        model = new ModelAndView("roleModify");
        return model;
    }
}
