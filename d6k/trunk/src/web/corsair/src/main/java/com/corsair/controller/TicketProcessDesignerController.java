package com.corsair.controller;

import com.corsair.service.TicketProcessService;
import com.rbac.engine.SnakerEngineFacets;
import com.rbac.engine.SnakerHelper;
import org.apache.commons.lang.StringUtils;
import org.snaker.engine.access.QueryFilter;
import org.snaker.engine.entity.*;
import org.snaker.engine.helper.AssertHelper;
import org.snaker.engine.model.ProcessModel;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import java.util.List;

/**
 * Created by 洪祥 on 2015/11/19.
 */
@Controller
public class TicketProcessDesignerController {
    @Autowired
    TicketProcessService processService ;
    @Autowired
    private SnakerEngineFacets facets;

    @RequestMapping(value ="/ticketProcessDesigner",method = RequestMethod.GET)
    public ModelAndView ticketProcessDesigner(String processId) {

        ModelAndView model = new ModelAndView("ticketProcessDesigner");

        if(StringUtils.isNotEmpty(processId)) {
            org.snaker.engine.entity.Process process = facets.getEngine().process().getProcessById(processId);
            AssertHelper.notNull(process);
            ProcessModel processModel = process.getModel();
            if(processModel != null) {
                String json = SnakerHelper.getModelJson(processModel);
                model.addObject("process", json);
            }
            model.addObject("processId", processId);
        }

        return model;
    }
}
