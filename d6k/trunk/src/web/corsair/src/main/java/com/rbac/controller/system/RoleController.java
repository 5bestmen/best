package com.rbac.controller.system;

import com.rbac.form.system.LoginForm;
import com.rbac.form.system.RoleListForm;
import com.rbac.service.RoleService;
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
 * Created by 洪祥 on 2015/9/28.
 */
@Controller
public class RoleController {
    @Autowired
    RoleService roleService;

    @RequestMapping(value = "/roleList")
    public ModelAndView RoleListAction(RoleListForm roleListForm, HttpServletRequest request, HttpSession session) {

        if (CommonUtils.isNotBlank(roleListForm.getDel())) {
            roleService.deleteRole(Long.parseLong(roleListForm.getDeleteIds()));
        }

        List roleList = roleService.getSysRoleList(roleListForm.getRoleNameQry(), roleListForm.getRoleDescQry());
        ModelAndView model = new ModelAndView("roleList");
        model.addObject("roleList", roleList);

        return model;
    }

}
