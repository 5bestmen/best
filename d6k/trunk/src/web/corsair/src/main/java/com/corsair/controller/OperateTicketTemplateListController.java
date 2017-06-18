package com.corsair.controller;

import com.corsair.form.OperateTicketTemplateListForm;
import com.corsair.service.OperateTicketService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.List;

/**
 * Created by zjq on 2015/10/19.
 */
@Controller
public class OperateTicketTemplateListController {
    @Autowired
    OperateTicketService operateTicketService;

    @RequestMapping(value = "/operateTicketTemplateList")
    public ModelAndView operateTicketGenerateControll(OperateTicketTemplateListForm operateTicketTemplateListForm,
                                                      HttpServletRequest request, HttpServletResponse response){
        ModelAndView model;

        if (CommonUtils.isNotBlank(operateTicketTemplateListForm.getDel())) {
            operateTicketService.deleteOperateTicketTemplate(operateTicketTemplateListForm.getDeleteIds());
        }

        List commonTicketTemplateList = operateTicketService.getCommonTicketTemplateList();
        model = new ModelAndView("operateTicketTemplateList");
        model.addObject("commonTicketList", commonTicketTemplateList);
        return model;
    }
}
