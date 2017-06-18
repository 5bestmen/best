package com.corsair.controller;

import com.rbac.engine.SnakerEngineFacets;
import org.snaker.engine.access.Page;
import org.snaker.engine.access.QueryFilter;
import org.snaker.engine.entity.HistoryOrder;
import org.snaker.engine.entity.Process;
import org.snaker.engine.helper.StringHelper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

/**
 * Created by 洪祥 on 2015/11/04.
 */
@Controller
public class OrderController {
    @Autowired
    private SnakerEngineFacets facets;

    @RequestMapping(value = "/order", method = RequestMethod.GET)
    public String order(Model model, Page<HistoryOrder> page) {
        facets.getEngine().query().getHistoryOrders(page, new QueryFilter());
        model.addAttribute("page", page);
        return "snaker/order";
    }
}
