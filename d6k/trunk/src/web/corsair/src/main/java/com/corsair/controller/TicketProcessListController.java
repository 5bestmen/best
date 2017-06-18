package com.corsair.controller;

import com.corsair.form.TicketProcessForm;
import com.corsair.service.TicketProcessService;

import com.rbac.engine.SnakerEngineFacets;
import com.rbac.util.CommonUtils;
import org.snaker.engine.access.Page;
import org.snaker.engine.access.QueryFilter;
import org.snaker.engine.entity.*;
import org.snaker.engine.entity.Process;
import org.snaker.engine.helper.StringHelper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.List;

/**
 * Created by 洪祥 on 2015/10/23.
 */
@Controller
public class TicketProcessListController {
    @Autowired
    TicketProcessService processService ;
    @Autowired
    private SnakerEngineFacets facets;

    @RequestMapping(value ="/ticketProcessList",method = RequestMethod.GET)
    public ModelAndView ticketProcessControl(TicketProcessForm ticketProcessForm, HttpServletRequest request,HttpServletResponse response){
        ModelAndView workTicketModel = new ModelAndView("/ticketProcessList");
        if(CommonUtils.isNotBlank(ticketProcessForm.getDel())){
            processService.deleteProcess(ticketProcessForm.getDeleteIds());
        }
        Page<Process> page = new Page<>();
        QueryFilter filter = new QueryFilter();
        facets.getEngine().process().getProcesss(page, filter);
//        List ticketProcessList = processService.getProcessList(ticketProcessForm.getName(),ticketProcessForm.getDisplayName());
        workTicketModel.addObject("page", page);
        return workTicketModel;
    }

//    public String processList(Model model, Page<Process> page, String displayName) {
//        QueryFilter filter = new QueryFilter();
//        if(StringHelper.isNotEmpty(displayName)) {
//            filter.setDisplayName(displayName);
//        }
//        facets.getEngine().process().getProcesss(page, filter);
//        model.addAttribute("page", page);
//        return "snaker/processList";
//    }
}
