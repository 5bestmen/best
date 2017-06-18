package com.corsair.controller;

import com.corsair.entity.*;
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
 * Created by 洪祥 on 2015/11/18.
 */
@Controller
public class WorkTicketIssuedController {

    public static final String SWITCHANDKNIFE = "应拉断路器（开关）、隔离开关（刀闸）";
    public static final String GROUNDLINENUMBER = "应装接地线、应合接地刀闸（注明确实地点、名称及接地线编号）";
    public static final String OTHERMEASURES = "应设遮栏、应挂标示牌及防止二次回路误碰等措施";
    public static final SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");//设置日期格式
    @Autowired
    WorkTicketService workTicketService;

    @Autowired
    AccountService accountService;

    @Autowired
    SnakerEngineFacets facets;

    @RequestMapping(value ="/workTicketIssued", method = RequestMethod.GET)
    public ModelAndView workTicketIssuedGet(WorkTicketModifyForm workTicketModifyForm, HttpServletRequest request){

        ModelAndView workTicketModel = new ModelAndView("workTicketIssued");

        if (workTicketModifyForm.getTaskId() != null){

            WorkTicket workTicket = workTicketService.getWorkTicketByTaskId(workTicketModifyForm.getTaskId());
            if(workTicket!=null){
                //workTicketModifyForm.setStationName(workTicket.getStationName());
                workTicketModifyForm.setWorkTicketType(revertWorkTicketType(workTicket.getWorkTicketType()));
                workTicketModifyForm.setCode(workTicket.getCode());
                if (null!=workTicket.getIsChangePerson() && workTicket.getIsChangePerson()==true){//更换责任人
                    workTicketModifyForm.setSelectResponsiblePerson("1");
                    PersonnelChangeCondition changePerson = new PersonnelChangeCondition();//工作人员变动情况
                    List sortChangePersonNewList = sortPersonnelChangeList(workTicket.getPersonnelChanges());
                    if(sortChangePersonNewList.size()>0){
                        changePerson = workTicket.getPersonnelChanges().get(0);
                    }
                    workTicketModifyForm.setResponsiblePerson(getRealNameByUserName(changePerson.getName()));
                }else{
                    workTicketModifyForm.setSelectResponsiblePerson("0");
                    workTicketModifyForm.setResponsiblePerson(getRealNameByUserName(workTicket.getResponsiblePerson()));
                }
                //workTicketModifyForm.setResponsiblePerson(workTicket.getResponsiblePerson());
                workTicketModifyForm.setDepartment(workTicket.getDepartment());
                workTicketModifyForm.setSignTime(workTicket.getSignTime());
                workTicketModifyForm.setWorkCondition(workTicket.getWorkCondition());
                workTicketModifyForm.setPermitPersonTime(workTicket.getPermitPersonTime());
                if(null!=workTicket.getIsDelay() &&  workTicket.getIsDelay()==true){//延期
                    workTicketModifyForm.setSelectdelayTime("1");
                    WorkTicketDelay workDelay = new WorkTicketDelay();//工作票延期
                    List sortNewList = sortList(workTicket.getWorkTicketDelays());
                    if(sortNewList.size()>0) {
                        workDelay = workTicket.getWorkTicketDelays().get(0);
                    }
                    workTicketModifyForm.setPlannedWorkingStartTime(workDelay.getStartTime());//计划工作开始时间
                    workTicketModifyForm.setPlannedWorkingEndTime(workDelay.getEndTime());//计划工作结束时间
                }else{
                    workTicketModifyForm.setSelectdelayTime("0");
                    workTicketModifyForm.setPlannedWorkingStartTime(workTicket.getPlannedWorkingStartingTime());//计划工作开始时间
                    workTicketModifyForm.setPlannedWorkingEndTime(workTicket.getPlannedWorkingEndingTime());//计划工作结束时间
                }
                List<String> crews = workTicket.getCrew();
                //String[] tempCrew = (String[])crews.toArray();
                String Crew = "";
                for (String s:crews){
                    Crew +=s+",";
                }
                Crew=Crew.substring(0,Crew.length()-1);
                workTicketModifyForm.setCrew(Crew);//工作班成员
                //workTicketModifyForm.setWorkPlace(workTicket.getWorkPlace());
                workTicketModifyForm.setTask(workTicket.getTask());
                workTicketModifyForm.setRemark(workTicket.getRemark());
                workTicketModifyForm.setFullRemark(workTicket.getFullRemark());
                workTicketModifyForm.setOtherRemark(workTicket.getOtherRemark());
                workTicketModifyForm.setFullTimePerson(workTicket.getFullTimePerson());
                List<SecurityMeasures> securityMeasureses = workTicket.getSecurityMeasureses();

                for (int i = 0; i < securityMeasureses.size(); i++) {
                    SecurityMeasures securityMeasures = securityMeasureses.get(i);
                    if( StringUtils.isNotBlank(securityMeasures.getSecurityMeasuresType())){
                        securityMeasures.setSecurityMeasuresType(revertSecurityMeasuresType(securityMeasures.getSecurityMeasuresType()));
                    }
                }

                List<WorkContent> workContents = workTicket.getWorkContents();//工作内容

                workTicketModel.addObject("securityMeasureses", securityMeasureses);
                workTicketModel.addObject("workContents", workContents);
                workTicketModel.addObject("workTicketType",  workTicket.getWorkTicketType());

            }
        }

        workTicketModel = initDropdownBox(workTicketModel);
        workTicketModel = initWorkGroupDropdownBox(workTicketModel);
        return workTicketModel;
    }

    /**
     * 根据时间降序排列list集合数据
     * @param list
     * @return
     */
    public static List sortList(List<WorkTicketDelay> list){

        // 按点击数倒序
        Collections.sort(list, new Comparator<WorkTicketDelay>() {
            public int compare(WorkTicketDelay arg0, WorkTicketDelay arg1) {
                int flag=0;
                try{
                    long hits0 = df.parse(arg0.getCreateTime()).getTime();
                    long hits1 = df.parse(arg1.getCreateTime()).getTime();
                    if (hits1 > hits0) {
                        flag = 1;
                    } else if (hits1 == hits0) {
                        flag=0;
                    } else {
                        flag=-1;
                    }
                }catch(Exception e){
                    e.printStackTrace();
                }
                return flag;
            }
        });
        return list;
    }
    public static List sortPersonnelChangeList(List<PersonnelChangeCondition> list){

        // 按点击数倒序
        Collections.sort(list, new Comparator<PersonnelChangeCondition>() {
            public int compare(PersonnelChangeCondition arg0, PersonnelChangeCondition arg1) {
                int flag=0;
                try{
                    long hits0 = df.parse(arg0.getCreateTime()).getTime();
                    long hits1 = df.parse(arg1.getCreateTime()).getTime();
                    if (hits1 > hits0) {
                        flag = 1;
                    } else if (hits1 == hits0) {
                        flag=0;
                    } else {
                        flag=-1;
                    }
                }catch(Exception e){
                    e.printStackTrace();
                }
                return flag;
            }
        });
        return list;
    }
    @RequestMapping(value = "/workTicketIssued", method = RequestMethod.POST)
    public ModelAndView workTicketIssuedPost(WorkTicketModifyForm workTicketModifyForm,HttpServletRequest request, HttpServletResponse response) {

        String nextPage = "";
        List<String> needs = workTicketModifyForm.getAttention();
        List<NeedAttention> attentions = new ArrayList<>();
        //设置安全措施
        for (int i = 0; i < needs.size(); i++) {
            NeedAttention needAttention  = new NeedAttention();
            needAttention.setNeedAttention(needs.get(i));
            attentions.add(needAttention);
        }
       // workTicketModifyForm.setResponsiblePerson(getLoginUser(request));//责任人设置为当前用户
        //workTicketModifyForm.setCode(UUID.randomUUID().toString());//编码自动生成
        workTicketModifyForm.setSignTime(df.format(new Date()));//签发日期
        request.getSession().setAttribute("PermitPerson",workTicketModifyForm.getProcessPermitPerson());
//        if ("系统管理员".equals(workTicketModifyForm.getProcessPermitPerson())){
//            workTicketModifyForm.setProcessPermitPerson("admin");
//        }
        Task task = process(workTicketModifyForm,request);
        //完成签发流程，会生成下一条任务，更新工作票任务ID
        if (task != null && CommonUtils.isNotBlank(workTicketModifyForm.getIssued())){
            WorkTicket workTicket = workTicketService.getWorkTicketByTaskId(workTicketModifyForm.getTaskId());
            if (workTicket!=null){
                workTicket.setTaskId(task.getId());
                workTicket.setNeedAttentions(attentions);
                workTicket.setWorkPermitPerson(workTicketModifyForm.getProcessPermitPerson());//流程许可人
                workTicket.setSignTime(workTicketModifyForm.getSignTime());//签发时间入库
                workTicketService.saveOrUpdateWorkTicket(workTicket);
            }
            nextPage = "redirect:/ticketTaskList";
        }else {
            nextPage = "redirect:/ticketProcessList";
        }

        return new ModelAndView(nextPage);

    }

    //签发工作票流程
    public Task process(WorkTicketModifyForm workTicketModifyForm, HttpServletRequest request) {

        String userLogin = getLoginUser(request);
        String permitPerson = getUserNameByRealName(workTicketModifyForm.getProcessPermitPerson());//工作票审结者 getUserNameByRealName(
        String taskId = workTicketModifyForm.getTaskId();
        Map<String, Object> params = new HashMap<>();
        params = facets.getEngine().query().getTask(taskId).getVariableMap();
        params.put("workTicketPermit.operator",permitPerson);
        Task task = null;

        if (CommonUtils.isNotEmpty(taskId) && CommonUtils.isNotBlank(workTicketModifyForm.getIssued())){
            //获取完成签发流程所生成的下一个任务
            task = facets.execute(taskId, userLogin, params).get(0);
        }else if (CommonUtils.isNotEmpty(taskId) && CommonUtils.isNotBlank(workTicketModifyForm.getQuit())){
            facets.executeAndJump(taskId, userLogin, params, request.getParameter("nodeName"));
        }

        return task;
    }

    private ModelAndView initDropdownBox(ModelAndView model){

        List<String> workTicketTypes = new ArrayList<>();
        workTicketTypes.add("一级动火工作票");
        workTicketTypes.add("二级动火工作票");
        workTicketTypes.add("电气一种工作票");
        workTicketTypes.add("电气二种工作票");
        model.addObject("workTicketTypes", workTicketTypes);

        List<SysAccount> accountList = accountService.getSysAccountList("","");
        List<String> accoutRealNames = new ArrayList<>();
        accoutRealNames.add("---请选择---");
        for (SysAccount sysAccount : accountList){
            accoutRealNames.add(sysAccount.getRealname());
        }
        model.addObject("accoutRealNames", accoutRealNames);

        return model;
    }
    /**
     * 工作班成员
     * @param model
     * @return
     */
    private ModelAndView initWorkGroupDropdownBox(ModelAndView model){
        List<String> workGroups = new ArrayList<>();
        workGroups.add("系统管理员");
        model.addObject("workGroupPersons", workGroups);
        return model;
    }
    private String revertWorkTicketType(Integer type){
        String workTicketType = "";
        if(type == 1){
            workTicketType = "一级动火工作票";
        }else if (type == 2){
            workTicketType = "二级动火工作票";
        }else if (type == 3){
            workTicketType = "电气一种工作票";
        }else if (type == 4){
            workTicketType = "电气二种工作票";
        }
        return workTicketType;
    }

    private String revertSecurityMeasuresType(String type) {
        String securityMeasuresType = "";
        if (type.equals("1")) {
            securityMeasuresType = SWITCHANDKNIFE;
        } else if (type.equals("2")) {
            securityMeasuresType = GROUNDLINENUMBER;
        } else if (type.equals("3")) {
            securityMeasuresType = OTHERMEASURES;
        }
        return securityMeasuresType;
    }
    //获取系统当前登录用户
    private String getLoginUser(HttpServletRequest request){

        UserDetail userDetail = (UserDetail)request.getSession().getAttribute("com_user");
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
