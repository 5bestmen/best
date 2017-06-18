package com.rbac.controller.system;
import com.corsair.service.ApplicationInformationService;
import com.rbac.common.BaseController;
import com.rbac.form.system.MenuListForm;
import com.rbac.service.MenuService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.List;

@Controller
public class MenuListController  extends BaseController {
    @Autowired
    MenuService menuService;
    @Autowired
    ApplicationInformationService applicationInformationService;

//    @RequestMapping(value = "/menuList", method = RequestMethod.GET)
//    public ModelAndView menuListRedirect(MenuListForm menuListForm,
//                                 HttpServletRequest request, HttpServletResponse response) {
//        ModelAndView model;
//        List menuList = menuService.getSysMenuList(menuListForm
//                .getNameQry(), menuListForm.getUrlQry());
//        request.setAttribute("menuList", menuList);
//        model = new ModelAndView("menuList");
//        model.addObject("menuList", menuList);
//        return model;
//    }

    @RequestMapping(value = "/menuList")
    public ModelAndView menuListAction(MenuListForm menuListForm,
                                HttpServletRequest request, HttpServletResponse response) {
        ModelAndView model;
        if (CommonUtils.isNotBlank(menuListForm.getDel())) {
            menuService.deleteMenu(Long.parseLong(menuListForm
                    .getDeleteIds()), super.getCurrentAccountId(request));
        }
        List menuList = menuService.getSysMenuList(menuListForm
                .getNameQry(), menuListForm.getUrlQry());
        model = new ModelAndView("menuList");
        model.addObject("menuList", menuList);
        return model;
    }

}