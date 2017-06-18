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
public class WorkTicketPermitController {

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

    public String getIsDelay() {
        return isDelay;
    }

    public void setIsDelay(String isDelay) {
        this.isDelay = isDelay;
    }

    public String getIsChangePerson() {
        return isChangePerson;
    }

    public void setIsChangePerson(String isChangePerson) {
        this.isChangePerson = isChangePerson;
    }

    private String isDelay;
    private String isChangePerson;

    @RequestMapping(value ="/workTicketPermit", method = RequestMethod.GET)
    public ModelAndView workTicketPermitGet(WorkTicketModifyForm workTicketModifyForm, HttpServletRequest request){

        ModelAndView workTicketModel = new ModelAndView("workTicketPermit");

        if (workTicketModifyForm.getTaskId() != null){

            WorkTicket workTicket = workTicketService.getWorkTicketByTaskId(workTicketModifyForm.getTaskId());
            //workTicketModifyForm.setStationName(workTicket.getStationName());
            if (workTicket!=null){
                workTicketModifyForm.setWorkTicketType(revertWorkTicketType(workTicket.getWorkTicketType()));
                workTicketModifyForm.setCode(workTicket.getCode());
                if (null!=workTicket.getIsChangePerson() && workTicket.getIsChangePerson()==true){//更换责任人
                    //workTicketModifyForm.setSelectResponsiblePerson("1");
                    PersonnelChangeCondition changePerson = new PersonnelChangeCondition();//工作人员变动情况
                    List sortChangePersonNewList = sortPersonnelChangeList(workTicket.getPersonnelChanges());
                    if(sortChangePersonNewList.size()>0) {
                        changePerson = workTicket.getPersonnelChanges().get(0);
                    }
                    workTicketModifyForm.setResponsiblePerson(getRealNameByUserName(changePerson.getName()));
                }else{
                    workTicketModifyForm.setResponsiblePerson(getRealNameByUserName(workTicket.getResponsiblePerson()));
                    workTicketModifyForm.setSelectResponsiblePerson("0");
                }
                workTicketModifyForm.setDepartment(workTicket.getDepartment());
                if(null!=workTicket.getIsDelay() && workTicket.getIsDelay()==true){//延期
                    //workTicketModifyForm.setSelectdelayTime("1");
                    WorkTicketDelay workDelay = new WorkTicketDelay();//工作票延期
                    List sortNewList = sortDelayList(workTicket.getWorkTicketDelays());
                    if(sortNewList.size()>0) {
                        workDelay = workTicket.getWorkTicketDelays().get(0);
                    }
                    workTicketModifyForm.setPlannedWorkingStartTime(workDelay.getStartTime());//计划工作开始时间
                    workTicketModifyForm.setPlannedWorkingEndTime(workDelay.getEndTime());//计划工作结束时间
                }else{
                    workTicketModifyForm.setPlannedWorkingStartTime(workTicket.getPlannedWorkingStartingTime());//计划工作开始时间
                    workTicketModifyForm.setPlannedWorkingEndTime(workTicket.getPlannedWorkingEndingTime());//计划工作结束时间
                    workTicketModifyForm.setSelectdelayTime("0");
                }
                //workTicketModifyForm.setCrew(workTicket.getCrew());//工作班成员
                //workTicketModifyForm.setWorkPlace(workTicket.getWorkPlace());
                workTicketModifyForm.setTask(workTicket.getTask());
                workTicketModifyForm.setPermitPersonTime(workTicket.getPermitPersonTime());
                workTicketModifyForm.setRemark(workTicket.getRemark());
                workTicketModifyForm.setFullRemark(workTicket.getFullRemark());
                workTicketModifyForm.setOtherRemark(workTicket.getOtherRemark());
                workTicketModifyForm.setFullTimePerson(workTicket.getFullTimePerson());
                List<String> crews = workTicket.getCrew();
                //String[] tempCrew = (String[])crews.toArray();
                String Crew = "";
                for (String s:crews){
                    Crew +=s+",";
                }
                Crew=Crew.substring(0,Crew.length()-1);
                workTicketModifyForm.setCrew(Crew);//工作班成员
                List<SecurityMeasures> securityMeasureses = workTicket.getSecurityMeasureses();
                for (int i = 0; i < securityMeasureses.size(); i++) {
                    SecurityMeasures securityMeasures = securityMeasureses.get(i);
                    if( StringUtils.isNotBlank(securityMeasures.getSecurityMeasuresType())){
                        securityMeasures.setSecurityMeasuresType(revertSecurityMeasuresType(securityMeasures.getSecurityMeasuresType()));
                    }
                }
                workTicketModel.addObject("securityMeasureses", securityMeasureses);
                List<AttenTionMeasure> attenTionMeasure = workTicket.getAttenTionMeasure();
                workTicketModel.addObject("attenTionMeasure", attenTionMeasure);

                List<WorkPermit> workPermitses = workTicket.getWorkPermits();
                workTicketModel.addObject("workPermitses", workPermitses);
                List<NeedAttention> needAttentions = workTicket.getNeedAttentions();
                workTicketModel.addObject("needAttentions", needAttentions);
                workTicketModel.addObject("workTicketType",  workTicket.getWorkTicketType());
                if(workTicket.getIsChangePerson()==null){workTicket.setIsChangePerson(false);}
                if(workTicket.getIsDelay()==null){workTicket.setIsDelay(false);}
                workTicketModel.addObject("isChangePerson",  workTicket.getIsChangePerson());//是否更换负责人
                workTicketModel.addObject("isChangeDelay",  workTicket.getIsDelay());//是否延期
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
    public static List sortDelayList(List<WorkTicketDelay> list){

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

    @RequestMapping(value = "/workTicketPermit", method = RequestMethod.POST)
    public ModelAndView workTicketPermitPost(WorkTicketModifyForm workTicketModifyForm,HttpServletRequest request, HttpServletResponse response) {

        String nextPage = "";
        if("1".equals(workTicketModifyForm.getSelectResponsiblePerson())) {//更换负责人
            workTicketModifyForm.setResponsiblePerson(getUserNameByRealName(workTicketModifyForm.getResponsiblePerson()));//变更负责人
        }else{
            workTicketModifyForm.setResponsiblePerson(getUserNameByRealName(workTicketModifyForm.getResponseResetPerson()));
            if(!"1".equals(workTicketModifyForm.getSelectdelayTime())){
                workTicketModifyForm.setPlannedWorkingStartTime(workTicketModifyForm.getChangeStartTime());
                workTicketModifyForm.setPlannedWorkingEndTime(workTicketModifyForm.getChangeEndTime());
            }
        }
        workTicketModifyForm.setPermitPersonTime(df.format(new Date()));//工作票审结日期
        Task task = process(workTicketModifyForm,request);
        if(task != null){
            //许可流程产生的工作票许可数据入库
            permitInDb(workTicketModifyForm, task);
            nextPage = "redirect:/ticketTaskList";
        }else {
            nextPage = "redirect:/ticketProcessList";
        }
        return new ModelAndView(nextPage);

    }

    //许可工作票流程
    public Task process(WorkTicketModifyForm workTicketModifyForm, HttpServletRequest request) {

        String userLogin = getLoginUser(request);

        String taskId = workTicketModifyForm.getTaskId();
        Map<String, Object> params = new HashMap<>();
        String endPerson = getUserNameByRealName(workTicketModifyForm.getPermitPerson());//工作票终结者getUserNameByRealName(
        Task taskTemp = facets.getEngine().query().getTask(taskId);
        if (taskTemp!=null){
            params = taskTemp.getVariableMap();
            params.put("workTicketEnd.operator",endPerson);
        }
        String agree = workTicketModifyForm.getAgree();
        String disagree = workTicketModifyForm.getDisagree();
        String delay = workTicketModifyForm.getSelectdelayTime();//工作票延期
        String changeLeader = workTicketModifyForm.getSelectResponsiblePerson();//更换负责人
        Task task = null;
        WorkTicket workTicket = workTicketService.getWorkTicketByTaskId(workTicketModifyForm.getTaskId());
        if (CommonUtils.isNotEmpty(taskId) && CommonUtils.isNotBlank(agree) && "0".equals(delay) && "0".equals(changeLeader) ){
            //获取完成许可流程所生成的下一个任务
            workTicket.setIsDelay(false);
            workTicket.setIsChangePerson(false);
            params.put("result","agree");
            task = facets.execute(taskId, userLogin, params).get(0);
        }else if (CommonUtils.isNotEmpty(taskId) && CommonUtils.isNotBlank(disagree)  && "0".equals(delay) && "0".equals(changeLeader) ){
            workTicket.setIsDelay(false);
            workTicket.setIsChangePerson(false);
            params.put("result","disagree");
            task = facets.execute(taskId, userLogin, params).get(0);
        }else if (CommonUtils.isNotEmpty(taskId) && "1".equals(delay)){//延期
            WorkTicketDelay workDelay = new WorkTicketDelay();//工作票延期
            //workDelay.setName(request.getSession().getAttribute("PermitPerson").toString());//工作许可人workTicketModifyForm.getProcessPermitPerson()
            workDelay.setName(workTicket.getWorkPermitPerson());//工作许可人workTicketModifyForm.getProcessPermitPerson()
            workDelay.setStartTime(workTicketModifyForm.getPlannedWorkingStartTime());
            workDelay.setEndTime(workTicketModifyForm.getPlannedWorkingEndTime());
            workDelay.setCreateTime(df.format(new Date()));//默认获取系统当前时间入库
            workTicket.setIsDelay(true);//延期
            workTicket.setIsChangePerson(false);//更换负责人
            workTicket.getWorkTicketDelays().add(workDelay);
            workTicketService.saveOrUpdateWorkTicket(workTicket);//保存延期
            params.put("result","extension");
            task = facets.execute(taskId, userLogin, params).get(0);
        }else if (CommonUtils.isNotEmpty(taskId) && "1".equals(changeLeader)){//更换负责人
            PersonnelChangeCondition changePerson = new PersonnelChangeCondition();//工作人员变动情况
            changePerson.setName(workTicketModifyForm.getResponsiblePerson());//变更的工作负责人
            //changePerson.setWorkTicketIsurePerson(request.getSession().getAttribute("issuedPerson").toString());//工作签发人workTicketModifyForm.getProcessIssuedPerson()
            changePerson.setWorkTicketIsurePerson(workTicket.getWorkTicketSigner());//工作签发人workTicketModifyForm.getProcessIssuedPerson()
            changePerson.setStartTime(df.format(new Date()));
            changePerson.setEndTime(df.format(new Date()));
            changePerson.setCreateTime(df.format(new Date()));
            workTicket.setIsChangePerson(true);//更换负责人
            workTicket.setIsDelay(false);//延期
            workTicket.getPersonnelChanges().add(changePerson);
            workTicketService.saveOrUpdateWorkTicket(workTicket);//保存变更负责人信息
            params.put("result","alter");
            task = facets.execute(taskId, userLogin, params).get(0);
        }
        return task;
    }

    private void permitInDb(WorkTicketModifyForm workTicketModifyForm,Task task){

        List<String> workPermits = workTicketModifyForm.getWorkPermit();
        List<WorkPermit> workPermitList = new ArrayList<>();
        WorkTicket workTicket = workTicketService.getWorkTicketByTaskId(workTicketModifyForm.getTaskId());
        workTicket.setPermitPersonTime(workTicketModifyForm.getPermitPersonTime());
        List<SecurityMeasures> measures= workTicket.getSecurityMeasureses();//获取所有可执行的安全措施项
        List<String> attentionMeasuer = workTicketModifyForm.getAttentionMeasureses();
        List<AttenTionMeasure> attenTionMeasures = new ArrayList<>();
        //设置安全措施
        for (int i = 0; i < attentionMeasuer.size(); i++) {
            AttenTionMeasure attenTionMeasure  = new AttenTionMeasure();
            attenTionMeasure.setAttenTionMeasure(attentionMeasuer.get(i));
            attenTionMeasures.add(attenTionMeasure);
        }
        workTicket.setAttenTionMeasure(attenTionMeasures);
        //保存许可流程审核产生的下一条任务ID
        workTicket.setTaskId(task.getId());
        //保存许可流程增加的许可项
        if(workPermits.size() > 0){
            WorkPermit workPermit  = new WorkPermit();
            workPermit.setPermits(workPermits);
            workPermit.setPermitPerson(workTicketModifyForm.getPermitPerson());
            workPermit.setResponsiblePerson(workTicketModifyForm.getResponsiblePermitPerson());
            workPermitList.add(workPermit);
        }
        workTicket.setWorkPermits(workPermitList);
        workTicket.setState(task.getVariableMap().get("result").equals("agree") ? 3 : 4); //设置审核结果为审核
        workTicket.setNewResponsiblePerson(workTicketModifyForm.getResponsiblePerson());//更改的工作负责人
        workTicket.setWorkPermitPerson(getUserNameByRealName(workTicketModifyForm.getPermitPerson()));//工作票终结人
        if (measures.size()>0){
            for (SecurityMeasures obj:measures){
                obj.setFinished(true);//一执行
            }
        }
        workTicketService.saveOrUpdateWorkTicket(workTicket);
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
