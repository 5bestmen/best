package com.corsair.controller;

import com.corsair.entity.PersonnelChangeCondition;
import com.corsair.entity.SecurityMeasures;
import com.corsair.entity.WorkPermit;
import com.corsair.entity.WorkTicket;
import com.corsair.form.WorkTicketModifyForm;
import com.corsair.service.WorkTicketService;
import com.rbac.common.UserDetail;
import com.rbac.engine.SnakerEngineFacets;
import com.rbac.entity.SysAccount;
import com.rbac.service.AccountService;
import com.rbac.util.CommonUtils;
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
 * Created by 洪祥 on 2015/11/19.
 */
@Controller
public class WorkTicketEndController {

    @Autowired
    WorkTicketService workTicketService;

    @Autowired
    AccountService accountService;

    @Autowired
    SnakerEngineFacets facets;

    public static final SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");//设置日期格式

    @RequestMapping(value ="/workTicketEnd", method = RequestMethod.GET)
    public ModelAndView workTicketPermitGet(WorkTicketModifyForm workTicketModifyForm, HttpServletRequest request){

        ModelAndView workTicketModel = new ModelAndView("workTicketEnd");
        WorkTicket workTicket = workTicketService.getWorkTicketByTaskId(workTicketModifyForm.getTaskId());
        workTicketModifyForm.setWorkTicketCreateTime(workTicket.getWorkTicketCreateTime());
        workTicketModel.addObject("createTime",workTicketModifyForm.getWorkTicketCreateTime());
        workTicketModel = initDropdownBox(workTicketModel);
        workTicketModel = initWorkGroupDropdownBox(workTicketModel);
        return workTicketModel;
    }

    @RequestMapping(value = "/workTicketEnd", method = RequestMethod.POST)
    public ModelAndView workTicketPermitPost(WorkTicketModifyForm workTicketModifyForm,HttpServletRequest request, HttpServletResponse response) {

        String nextPage = "";
        process(workTicketModifyForm,request);

        if(CommonUtils.isNotBlank(workTicketModifyForm.getSubmit())){

            WorkTicket workTicket = workTicketService.getWorkTicketByTaskId(workTicketModifyForm.getTaskId());

            //保存工作票终结流程增加的工作终结和工作票终结数据
            workTicket.setSummary("工作终结时间：" + workTicketModifyForm.getWorkEndTime() + "; 工作终结办理人：" + workTicket.getWorkPermitPerson());
            if (null!=workTicket.getIsChangePerson() && workTicket.getIsChangePerson()==true){//更换责任人
                //workTicketModifyForm.setSelectResponsiblePerson("1");
                PersonnelChangeCondition changePerson = new PersonnelChangeCondition();//工作人员变动情况
                List sortChangePersonNewList = sortPersonnelChangeList(workTicket.getPersonnelChanges());
                if(sortChangePersonNewList.size()>0) {
                    changePerson = workTicket.getPersonnelChanges().get(0);
                }
                workTicketModifyForm.setResponsiblePerson(getRealNameByUserName(changePerson.getName()));
                workTicket.setWorkTicketEnd("值班负责人：" + workTicketModifyForm.getResponsiblePerson());
            }else{
                workTicketModifyForm.setResponsiblePerson(getRealNameByUserName(workTicket.getResponsiblePerson()));
                workTicket.setWorkTicketEnd("值班负责人：" + workTicketModifyForm.getResponsiblePerson());
            }
            workTicket.setSummaryTime(workTicketModifyForm.getWorkEndTime());
            workTicketService.saveOrUpdateWorkTicket(workTicket);

            nextPage = "redirect:/ticketTaskList";

        }else {
            nextPage = "redirect:/ticketProcessList";
        }
        return new ModelAndView(nextPage);

    }
    public static List sortPersonnelChangeList(List<PersonnelChangeCondition> list){

        // 按点击数倒序
        Collections.sort(list, new Comparator<PersonnelChangeCondition>() {
            public int compare(PersonnelChangeCondition arg0, PersonnelChangeCondition arg1) {
                int flag = 0;
                try {
                    long hits0 = df.parse(arg0.getCreateTime()).getTime();
                    long hits1 = df.parse(arg1.getCreateTime()).getTime();
                    if (hits1 > hits0) {
                        flag = 1;
                    } else if (hits1 == hits0) {
                        flag = 0;
                    } else {
                        flag = -1;
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
                return flag;
            }
        });
        return list;
    }
    //工作票终结流程
    public void process(WorkTicketModifyForm workTicketModifyForm, HttpServletRequest request) {

        String userLogin = getLoginUser(request);

        String taskId = workTicketModifyForm.getTaskId();
       // String ticketEndPerson = getUserNameByRealName(workTicketModifyForm.getWorkEndPerson());//工作票终结者getUserNameByRealName(
        Map<String, Object> params = new HashMap<>();
        params = facets.getEngine().query().getTask(taskId).getVariableMap();
        //params.put("workTicketEnd .operator",ticketEndPerson);
        if (CommonUtils.isNotBlank(workTicketModifyForm.getSubmit()) && CommonUtils.isNotEmpty(taskId)){
            //获取完成工作票终结流程所生成的下一个任务
            facets.execute(taskId, userLogin, params);
        }
    }

    private ModelAndView initDropdownBox(ModelAndView model){

        List<SysAccount> accountList = accountService.getSysAccountList("","");
        List<String> accoutRealNames = new ArrayList<>();
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
