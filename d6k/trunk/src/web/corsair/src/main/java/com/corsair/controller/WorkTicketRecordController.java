package com.corsair.controller;

import com.corsair.entity.WorkTicket;
import com.corsair.entity.WorkTicketRecord;
import com.corsair.form.WorkTicketRecordForm;
import com.corsair.service.WorkTicketService;
import com.rbac.common.UserDetail;
import com.rbac.engine.SnakerEngineFacets;
import com.rbac.entity.SysAccount;
import com.rbac.service.AccountService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Created by guo_zhiqi on 2015/10/23.
 */
@Controller
public class WorkTicketRecordController {

    @Autowired
    WorkTicketService workTicketService;

    @Autowired
    AccountService accountService;

    @Autowired
    private SnakerEngineFacets facets;

    @RequestMapping(value = "/workTicketRecord", method = RequestMethod.GET)
    public ModelAndView workTicketModifyGet(WorkTicketRecordForm workTicketRecordForm, String id) {
        ModelAndView workTicketModel = new ModelAndView("/workTicketRecord");
        workTicketModel.addObject("id", id);
        workTicketModel = initDropdownBox(workTicketModel);
        workTicketModel = initWorkGroupDropdownBox(workTicketModel);
        return workTicketModel;
    }

    @RequestMapping(value = "/workTicketRecordList", method = RequestMethod.GET)
    public ModelAndView workTicketModifyGet(WorkTicketRecordForm workTicketRecordForm,HttpServletRequest request, String id) {
        ModelAndView workTicketModel = new ModelAndView("/workTicketRecordList");
        workTicketModel.addObject("id", id);
        workTicketModel = initWorkGroupDropdownBox(workTicketModel);
        WorkTicket workTicket = workTicketService.getWorkTicketById(workTicketRecordForm.getId());
        List<WorkTicketRecord> WorkTicketRecords = workTicketService.getWorkTicketRecordList(workTicket);
        List<WorkTicketRecord> newRecords = new ArrayList<>();
        WorkTicketRecord record = null;
        for (int i = 0; i <WorkTicketRecords.size() ; i++) {
            record = WorkTicketRecords.get(i);
            String dateStr = record.getStartTime().substring(0,10);
            String startTime = record.getStartTime().substring(10,19);
            String endTime = record.getEndTime().substring(10, 19);
            record.setRecordDate(dateStr);
            record.setStartTime(startTime);
            record.setEndTime(endTime);
            newRecords.add(record);
        }
        sortByTime(newRecords);
        workTicketModel.addObject("workTicketRecord",newRecords);
        return workTicketModel;
    }

    @RequestMapping(value = "/workTicketRecord", method = RequestMethod.POST)
    public ModelAndView workTicketModifyPost(WorkTicketRecordForm workTicketRecordForm, HttpServletRequest request, HttpServletResponse response,String id) {
        ModelAndView workTicketModel;
       // workTicketRecordForm.setResponsiblePerson(getLoginUser(request));//责任人设置为当前用户
       // workTicketRecordForm.setPermitPerson(getLoginUser(request));
        //第二步，工作票相关数据的持久化
        ticketInDb(workTicketRecordForm);
        workTicketModel = new ModelAndView("redirect:/workTicketRecordList");
        workTicketModel.addObject("id", id);
        return workTicketModel;
    }

    //工作票相关数据的持久化处理
    private void ticketInDb(WorkTicketRecordForm workTicketRecordForm) {
        WorkTicket workTicket = workTicketService.getWorkTicketById(workTicketRecordForm.getId());
        WorkTicketRecord workTicketRecord = new WorkTicketRecord();
        workTicketRecord.setStartTime(workTicketRecordForm.getStartTime());//保存开工始时间
        workTicketRecord.setEndTime(workTicketRecordForm.getEndTime());//保存收工时间
        workTicketRecord.setResponsiblePerson(workTicketRecordForm.getResponsiblePerson());//保存工作责任人
        workTicketRecord.setPermitPerson(workTicketRecordForm.getPermitPerson());//保存工作许可人
        workTicketRecord.setWorkTicket(workTicket);
        workTicket.getWorkTicketRecords().add(workTicketRecord);
        workTicketService.saveOrUpdateWorkTicket(workTicket);
    }

    private ModelAndView initDropdownBox(ModelAndView model) {
        List<SysAccount> accountList = accountService.getSysAccountList("", "");
        List<String> accoutRealNames = new ArrayList<>();
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

    //获取系统当前登录用户
    private String getLoginUser(HttpServletRequest request) {

        UserDetail userDetail = (UserDetail) request.getSession().getAttribute("com_user");
        SysAccount sysAccount = userDetail.getAccount();
        return sysAccount == null ? null : sysAccount.getUsername();
    }

    //按照数据索引号对设备模板数据进行升序排序
    private void sortByTime(List<WorkTicketRecord> workTicketRecords) {

        for (int i = 0; i < workTicketRecords.size(); i++) {

            for (int j = i + 1; j < workTicketRecords.size(); j++) {

                String  frontTime = workTicketRecords.get(i).getRecordDate();
                String  behindTime = workTicketRecords.get(j).getRecordDate();
                SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
                Date frontDate = null;
                Date behindDate = null;
                try {
                    frontDate = sdf.parse(frontTime);
                    behindDate = sdf.parse(behindTime);
                } catch (ParseException e) {
                    e.printStackTrace();
                }
                if (frontDate.getTime() > behindDate.getTime()) {
                    WorkTicketRecord temp = workTicketRecords.get(i);
                    workTicketRecords.set(i, workTicketRecords.get(j));
                    workTicketRecords.set(j, temp);
                }
            }
        }
    }



}
