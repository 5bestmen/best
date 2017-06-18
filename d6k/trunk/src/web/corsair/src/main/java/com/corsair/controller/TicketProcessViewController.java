package com.corsair.controller;

import com.corsair.service.TicketProcessService;
import com.rbac.engine.SnakerEngineFacets;
import org.snaker.engine.access.QueryFilter;
import org.snaker.engine.entity.HistoryOrder;
import org.snaker.engine.entity.HistoryTask;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;
import java.util.List;

/**
 * Created by 洪祥 on 2015/11/11.
 */
@Controller
public class TicketProcessViewController {
    @Autowired
    TicketProcessService processService ;
    @Autowired
    private SnakerEngineFacets facets;

    @RequestMapping(value ="/ticketProcessView",method = RequestMethod.GET)
    public ModelAndView display(String processId, String orderId) {
        ModelAndView model = new ModelAndView("ticketProcessView");
        HistoryOrder order = facets.getEngine().query().getHistOrder(orderId);
        model.addObject("processId", processId);
        model.addObject("order", order);
        List<HistoryTask> tasks = facets.getEngine().query().getHistoryTasks(new QueryFilter().setOrderId(orderId));
        model.addObject("tasks", tasks);
        return model;
    }
}
