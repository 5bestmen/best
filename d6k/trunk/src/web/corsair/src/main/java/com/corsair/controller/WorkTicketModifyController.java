package com.corsair.controller;

import com.corsair.entity.SecurityMeasures;
import com.corsair.entity.WorkContent;
import com.corsair.entity.WorkTicket;
import com.corsair.form.WorkTicketModifyForm;
import com.corsair.service.WorkTicketService;
import com.rbac.common.UserDetail;
import com.rbac.engine.SnakerEngineFacets;
import com.rbac.entity.SysAccount;
import com.rbac.service.AccountService;
import com.rbac.util.CommonUtils;
import org.apache.commons.lang.StringUtils;
import org.snaker.engine.access.QueryFilter;
import org.snaker.engine.entity.Order;
import org.snaker.engine.entity.Task;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Created by 洪祥 on 2015/10/23.
 */
@Controller
public class WorkTicketModifyController {

    public static final SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");//设置日期格式
    @Autowired
    WorkTicketService workTicketService;

    @Autowired
    AccountService accountService;

    @Autowired
    SnakerEngineFacets facets;

    @RequestMapping(value = "/workTicketModify", method = RequestMethod.GET)
    public ModelAndView workTicketModifyGet(WorkTicketModifyForm workTicketModifyForm, HttpServletRequest request) {

        ModelAndView workTicketModel = new ModelAndView("workTicketModify");

        if (workTicketModifyForm.getId() != null) {  //判据为工作票ID非空

            WorkTicket workTicket = workTicketService.getWorkTicketById(workTicketModifyForm.getId());
            //workTicketModifyForm.setStationName(workTicket.getStationName());
            workTicketModifyForm.setWorkTicketType(revertWorkTicketType(workTicket.getWorkTicketType()));
            workTicketModifyForm.setCode(workTicket.getCode());
            workTicketModifyForm.setResponsiblePerson(getRealNameByUserName(workTicket.getResponsiblePerson()));
            workTicketModifyForm.setDepartment(workTicket.getDepartment());
            workTicketModifyForm.setWorkCondition(workTicket.getWorkCondition());
            workTicketModifyForm.setSignTime(workTicket.getSignTime());
            workTicketModifyForm.setPlannedWorkingStartTime(workTicket.getPlannedWorkingStartingTime());
            workTicketModifyForm.setPlannedWorkingEndTime(workTicket.getPlannedWorkingEndingTime());
            workTicketModifyForm.setFillWorkTicketTime(workTicket.getFillWorkTicketTime());
            List<String> crews = workTicket.getCrew();
            //String[] tempCrew = (String[])crews.toArray();
            String Crew = "";
            for (String s : crews) {
                Crew += s + ",";
            }
            Crew = Crew.substring(0, Crew.length() - 1);
            workTicketModifyForm.setCrew(Crew);
            //workTicketModifyForm.setWorkPlace(workTicket.getWorkPlace());
            workTicketModifyForm.setTask(workTicket.getTask());
            workTicketModifyForm.setRemark(workTicket.getRemark());

            List<SecurityMeasures> securityMeasureses = workTicket.getSecurityMeasureses();//安全措施
            List<WorkContent> workContents = workTicket.getWorkContents();//工作内容
            workTicketModel.addObject("securityMeasureses", securityMeasureses);
            workTicketModel.addObject("workContents", workContents);
            //workTicketModel.addObject("flag", 1);//签发人
            //workTicketModel.addObject("flag", 1);

        } else if (workTicketModifyForm.getTaskId() != null) { //判据为工作票流程任务ID非空

            WorkTicket workTicket = workTicketService.getWorkTicketByTaskId(workTicketModifyForm.getTaskId());
            if (workTicket != null) {
                workTicketModifyForm.setWorkTicketType(revertWorkTicketType(workTicket.getWorkTicketType()));
                workTicketModifyForm.setCode(workTicket.getCode());
                workTicketModifyForm.setResponsiblePerson(workTicket.getResponsiblePerson());
                workTicketModifyForm.setDepartment(workTicket.getDepartment());
                workTicketModifyForm.setWorkCondition(workTicket.getWorkCondition());
                workTicketModifyForm.setSignTime(workTicket.getSignTime());
                workTicketModifyForm.setPlannedWorkingStartTime(workTicket.getPlannedWorkingStartingTime());
                workTicketModifyForm.setPlannedWorkingEndTime(workTicket.getPlannedWorkingEndingTime());
                workTicketModifyForm.setFillWorkTicketTime(workTicket.getFillWorkTicketTime());
                List<String> crews = workTicket.getCrew();
                //String[] tempCrew = (String[])crews.toArray();
                String Crew = "";
                for (String s : crews) {
                    Crew += s + ",";
                }
                Crew = Crew.substring(0, Crew.length() - 1);
                workTicketModifyForm.setCrew(Crew);
                workTicketModifyForm.setTask(workTicket.getTask());
                workTicketModifyForm.setRemark(workTicket.getRemark());

                List<SecurityMeasures> securityMeasureses = workTicket.getSecurityMeasureses();
                List<WorkContent> workContents = workTicket.getWorkContents();//工作内容
                workTicketModel.addObject("securityMeasureses", securityMeasureses);
                workTicketModel.addObject("workContents", workContents);
            }

        }

        workTicketModel.addObject("process", workTicketModifyForm.getProcessId());
        workTicketModel.addObject("orderId", workTicketModifyForm.getOrderId());
        workTicketModel.addObject("taskId", workTicketModifyForm.getTaskId());

        workTicketModel = initDropdownBox(workTicketModel);
        workTicketModel = initWorkGroupDropdownBox(workTicketModel);
        return workTicketModel;
    }

    @RequestMapping(value = "/workTicketModify", method = RequestMethod.POST)
    public ModelAndView workTicketModifyPost(WorkTicketModifyForm workTicketModifyForm, HttpServletRequest request, HttpServletResponse response) {
        ModelAndView workTicketModel;
        String[] securityMeasuresType = request.getParameterValues("securityMeasuresType");
        if (CommonUtils.isNotBlank(workTicketModifyForm.getSubmit())) {

            workTicketModifyForm.setResponsiblePerson(getLoginUser(request));//责任人设置为当前用户
            workTicketModifyForm.setCode(UUID.randomUUID().toString());//编码自动生成
            workTicketModifyForm.setWorkTicketCreateTime(df.format(new Date()));
            workTicketModifyForm.setFillWorkTicketTime(df.format(new Date()));//填写工作票日期
            //第一步，进入流程，生成一条工作票流程任务
            Task task = process(workTicketModifyForm, request);
            //第二步，工作票相关数据的持久化
            if (task != null) {
                ticketInDb(workTicketModifyForm, task, securityMeasuresType);
            }
            workTicketModel = new ModelAndView("redirect:/ticketTaskList");
        } else {
            //永远不会走到这里，但我要保留这条语句，我不能拆散if和else，那样太残忍。
            workTicketModel = new ModelAndView("workTicketModify");
        }

        return workTicketModel;
    }

    //进入填写工作票流程，返回生成的工作票流程任务实例
    public Task process(WorkTicketModifyForm workTicketModifyForm, HttpServletRequest request) {

        String userLogin = getLoginUser(request);
        String issuedPerson = getUserNameByRealName(workTicketModifyForm.getProcessIssuedPerson());//getUserNameByRealName
        request.getSession().setAttribute("issuedPerson", issuedPerson);//保存签发人
        //保存流程实例参数
        Map<String, Object> params = new HashMap<>();
        params.put("workTicketModify.operator", userLogin);
        params.put("workTicketIssued.operator", issuedPerson);

        String processId = workTicketModifyForm.getProcessId();
        String orderId = workTicketModifyForm.getOrderId();
        String taskId = workTicketModifyForm.getTaskId();

        //启动流程，创建流程实例
        Task task = null;

        if (StringUtils.isNotEmpty(processId) && StringUtils.isEmpty(orderId) && StringUtils.isEmpty(taskId)) {
            Order order = facets.startAndExecute(processId, userLogin, params);
            //获取启动流程所生成的任务
            List<Task> tasks = facets.getEngine().query().getActiveTasks(new QueryFilter().setOrderId(order.getId()));
            task = tasks.get(0);
        } else {
            //进入这一步说明，此前许可流程结果为不通过，迭代到填写工作票流程重新填写工作票
            task = facets.execute(taskId, userLogin, params).get(0);
        }

        return task;
    }

    //工作票相关数据的持久化处理
    private void ticketInDb(WorkTicketModifyForm workTicketModifyForm, Task task, String[] type) {
        String id = workTicketModifyForm.getId();
        List<String> measures = workTicketModifyForm.getSecurityMeasure();//安全策略
        List<String> taskContentName = workTicketModifyForm.getWorkNameAndPlace();//工作地及名称
        List<String> taskContent = workTicketModifyForm.getWorkContent();//工作内容
        List<SecurityMeasures> securityMeasureses = new ArrayList<>();//安全措施
        List<String> crews = new ArrayList<String>();//工作班成员
        WorkTicket workTicket = new WorkTicket();

        if (CommonUtils.isNotBlank(id)) {
            workTicket = workTicketService.getWorkTicketById(id); //修改工作票时，首先获取初始安全措施
        } else {
            workTicket.setState(0); //新增工作票时，单独设置审核状态为未审核
        }

        initWorkTicketType(workTicket, workTicketModifyForm);
        workTicket.setCode(workTicketModifyForm.getCode());//编码
        workTicket.setFullTimePerson(workTicketModifyForm.getFullTimePerson());
        workTicket.setFullRemark(workTicketModifyForm.getFullRemark());
        workTicket.setOtherRemark(workTicketModifyForm.getOtherRemark());
        workTicket.setResponsiblePerson(workTicketModifyForm.getResponsiblePerson());//责任人
        if (!workTicketModifyForm.getCrew().isEmpty()) {
            String[] tempCrew = workTicketModifyForm.getCrew().split(",");
            for (String s : tempCrew) {
                crews.add(s);
            }
        }
        workTicket.setCrew(crews);//工作班成员
        workTicket.setDepartment(workTicketModifyForm.getDepartment());
        workTicket.setPlannedWorkingStartingTime(workTicketModifyForm.getPlannedWorkingStartTime());//保存工作票的开始时间
        workTicket.setPlannedWorkingEndingTime(workTicketModifyForm.getPlannedWorkingEndTime());//保存工作票的结束时间
        workTicket.setFillWorkTicketTime(workTicketModifyForm.getFillWorkTicketTime());//填写工作票日期
        workTicket.setSignTime(workTicketModifyForm.getSignTime());
        workTicket.setWorkCondition(workTicketModifyForm.getWorkCondition());
        workTicket.setTask(workTicketModifyForm.getTask());
        workTicket.setRemark(workTicketModifyForm.getRemark());
        workTicket.setProcessId(workTicketModifyForm.getProcessId());
        workTicket.setWorkTicketSigner(workTicketModifyForm.getProcessIssuedPerson());//流程签发人入库
        workTicket.setWorkTicketCreateTime(workTicketModifyForm.getWorkTicketCreateTime());//工作票创建时间
        workTicket.setTaskId(task.getId());
        workTicket.setState(1); //设置审核状态为审核中

           //设置安全措施
        for (int i = 0; i < measures.size(); i++) {
            SecurityMeasures securityMeasures = new SecurityMeasures();
            securityMeasures.setMeasure(measures.get(i));
            securityMeasureses.add(securityMeasures);
        }

        //设置安全类型
        if (type != null) {
            for (int i = 0; i < type.length; i++) {
                securityMeasureses.get(i).setSecurityMeasuresType(type[i]);
            }
        }

        workTicket.setSecurityMeasureses(securityMeasureses);

        //设置工作内容
        if (taskContentName.size() > 0 || taskContent.size() > 0) {
            for (int j = 0; j < taskContentName.size(); j++) {
                WorkContent work = new WorkContent();
                work.setTaskName(taskContentName.get(j));
                work.setTaskContent(taskContent.get(j));
                work.setWorkTicket(workTicket);
                workTicket.getWorkContents().add(work);
            }
        }

        workTicketService.saveOrUpdateWorkTicket(workTicket);

    }

    private ModelAndView initDropdownBox(ModelAndView model) {

        List<String> workTicketTypes = new ArrayList<>();
//        workTicketTypes.add("一级动火工作票");
//        workTicketTypes.add("二级动火工作票");
        workTicketTypes.add("电气一种工作票");
        workTicketTypes.add("电气二种工作票");
        model.addObject("workTicketTypes", workTicketTypes);

        List<SysAccount> accountList = accountService.getSysAccountList("", "");
        List<String> accoutRealNames = new ArrayList<>();
        accoutRealNames.add("---请选择---");
        for (SysAccount sysAccount : accountList) {
            accoutRealNames.add(sysAccount.getRealname());
        }
        model.addObject("accoutRealNames", accoutRealNames);

        return model;
    }

    /**
     * 工作班成员
     *
     * @param model
     * @return
     */
    private ModelAndView initWorkGroupDropdownBox(ModelAndView model) {
        List<String> workGroups = new ArrayList<>();
        workGroups.add("系统管理员");
        model.addObject("workGroupPersons", workGroups);
        return model;
    }

    private void initWorkTicketType(WorkTicket workTicket, WorkTicketModifyForm workTicketModifyForm) {
        String type = workTicketModifyForm.getWorkTicketType();
        if (type.equals("一级动火工作票")) {
            workTicket.setWorkTicketType(1);
        } else if (type.equals("二级动火工作票")) {
            workTicket.setWorkTicketType(2);
        } else if (type.equals("电气一种工作票")) {
            workTicket.setWorkTicketType(3);
        } else if (type.equals("电气二种工作票")) {
            workTicket.setWorkTicketType(4);
        }
    }

    private String revertWorkTicketType(Integer type) {
        String workTicketType = "";
        if (type == 1) {
            workTicketType = "一级动火工作票";
        } else if (type == 2) {
            workTicketType = "二级动火工作票";
        } else if (type == 3) {
            workTicketType = "电气一种工作票";
        } else if (type == 4) {
            workTicketType = "电气二种工作票";
        }
        return workTicketType;
    }


    //获取系统当前登录用户
    private String getLoginUser(HttpServletRequest request) {

        UserDetail userDetail = (UserDetail) request.getSession().getAttribute("com_user");
        SysAccount sysAccount = userDetail.getAccount();
        return sysAccount == null ? null : sysAccount.getUsername();
    }

    private String getUserNameByRealName(String realName) {
        String userName = "";
        List<SysAccount> accountList = accountService.getSysAccountList("", "");
        for (SysAccount sysAccount : accountList) {
            if (sysAccount.getRealname().equals(realName)) {
                userName = sysAccount.getUsername();
            }
        }
        return userName;
    }
    /**
     *
     * @param userName
     * @return
     */
    private String getRealNameByUserName(String userName) {
        String realName = "";
        List<SysAccount> accountList = accountService.getSysAccountList("", "");
        for (SysAccount sysAccount : accountList) {
            if (sysAccount.getUsername().equals(userName)) {
                realName = sysAccount.getRealname();
            }
        }
        return realName;
    }
}
