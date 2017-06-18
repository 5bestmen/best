package com.corsair.controller;

import com.rbac.engine.SnakerEngineFacets;
import org.snaker.engine.access.Page;
import org.snaker.engine.access.QueryFilter;
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
public class ProcessListController {
    @Autowired
    private SnakerEngineFacets facets;

    @RequestMapping(value = "/processList", method = RequestMethod.GET)
         public String processList(Model model, Page<Process> page, String displayName) {
        QueryFilter filter = new QueryFilter();
        if(StringHelper.isNotEmpty(displayName)) {
            filter.setDisplayName(displayName);
        }
        facets.getEngine().process().getProcesss(page, filter);
        model.addAttribute("page", page);
        return "snaker/processList";
    }
}
