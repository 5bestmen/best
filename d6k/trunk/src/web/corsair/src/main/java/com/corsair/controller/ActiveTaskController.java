package com.corsair.controller;

import com.rbac.engine.SnakerEngineFacets;
import org.snaker.engine.access.Page;
import org.snaker.engine.access.QueryFilter;
import org.snaker.engine.entity.HistoryOrder;
import org.snaker.engine.entity.WorkItem;
import org.snaker.engine.model.TaskModel;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

import java.util.List;

/**
 * Created by 洪祥 on 2015/11/04.
 */
@Controller
public class ActiveTaskController {
    @Autowired
    private SnakerEngineFacets facets;

    @RequestMapping(value = "/activeTask", method = RequestMethod.GET)
    public String homeTaskList(Model model) {
        String[] assignees = new String[]{"admin"};

        Page<WorkItem> majorPage = new Page<WorkItem>(5);
        Page<WorkItem> aidantPage = new Page<WorkItem>(3);
        Page<HistoryOrder> ccorderPage = new Page<HistoryOrder>(3);
        List<WorkItem> majorWorks = facets.getEngine()
                .query()
                .getWorkItems(majorPage, new QueryFilter()
                        .setOperators(assignees)
                        .setTaskType(TaskModel.TaskType.Major.ordinal()));
        List<WorkItem> aidantWorks = facets.getEngine()
                .query()
                .getWorkItems(aidantPage, new QueryFilter()
                        .setOperators(assignees)
                        .setTaskType(TaskModel.TaskType.Aidant.ordinal()));
        List<HistoryOrder> ccWorks = facets.getEngine()
                .query()
                .getCCWorks(ccorderPage, new QueryFilter()
                        .setOperators(assignees)
                        .setState(1));

        model.addAttribute("majorWorks", majorWorks);
        model.addAttribute("majorTotal", majorPage.getTotalCount());
        model.addAttribute("aidantWorks", aidantWorks);
        model.addAttribute("aidantTotal", aidantPage.getTotalCount());
        model.addAttribute("ccorderWorks", ccWorks);
        model.addAttribute("ccorderTotal", ccorderPage.getTotalCount());
        return "snaker/activeTask";
    }
}
