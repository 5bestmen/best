package com.corsair.controller;

import com.corsair.form.TicketProcessForm;
import com.corsair.form.TicketTaskForm;
import com.corsair.service.TicketProcessService;
import com.rbac.common.UserDetail;
import com.rbac.engine.SnakerEngineFacets;
import com.rbac.entity.SysAccount;
import com.rbac.util.CommonUtils;
import org.snaker.engine.access.Page;
import org.snaker.engine.access.QueryFilter;
import org.snaker.engine.entity.HistoryOrder;
import org.snaker.engine.entity.HistoryTask;
import org.snaker.engine.entity.Process;
import org.snaker.engine.entity.WorkItem;
import org.snaker.engine.model.TaskModel;
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
 * Created by 洪祥 on 2015/11/09.
 */
@Controller
public class TicketTaskListController {
    @Autowired
    TicketProcessService processService ;
    @Autowired
    private SnakerEngineFacets facets;

    @RequestMapping(value ="/ticketTaskList",method = RequestMethod.GET)
    public ModelAndView ticketTaskControl(TicketTaskForm ticketTaskForm, HttpServletRequest request,HttpServletResponse response){

        ModelAndView workTicketModel = new ModelAndView("/ticketTaskList");

        String userLogin = getLoginUser(request);
        String[] assignees = new String[]{userLogin};
        Page<WorkItem> majorPage = new Page<WorkItem>();
        QueryFilter filter = new QueryFilter();

        List<WorkItem> majorTasks = facets.getEngine().query().getWorkItems(majorPage, filter
                .setOperators(assignees)
                .setTaskType(TaskModel.TaskType.Major.ordinal()));

        workTicketModel.addObject("majorTasks", majorTasks);
        workTicketModel.addObject("majorTotal", majorTasks.size());

        return workTicketModel;
    }

    @RequestMapping(value = "historyTicketOrderList", method= RequestMethod.GET)
    public ModelAndView historyTicketOrderControl(HttpServletRequest request,HttpServletResponse response) {

        ModelAndView workTicketModel = new ModelAndView("/historyTicketOrderList");

        List<HistoryOrder> historyOrders = facets.getEngine().query().
                getHistoryOrders(new Page<HistoryOrder>(), new QueryFilter());

        workTicketModel.addObject("historyOrders", historyOrders);
        workTicketModel.addObject("majorTotal",historyOrders.size());

        return workTicketModel;
    }

    //获取系统当前登录用户
    private String getLoginUser(HttpServletRequest request){

        UserDetail userDetail = (UserDetail)request.getSession().getAttribute("com_user");
        SysAccount sysAccount = userDetail.getAccount();
        return sysAccount == null ? null : sysAccount.getUsername();
    }
}
