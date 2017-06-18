package com.corsair.controller;

import com.corsair.entity.OperateTicket;
import com.corsair.entity.OperateTicketProcessRecord;
import com.corsair.entity.OperateTicketTemplate;
import com.corsair.entity.OperateTicketTemplateData;
import com.corsair.form.OperateTicketEditForm;
import com.corsair.service.OperateTicketService;
import com.rbac.common.MvcConstant;
import com.rbac.common.UserDetail;
import com.rbac.entity.SysAccount;
import com.rbac.service.AccountService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

/**
 * Created by zjq on 2015/10/22.
 */
@Controller
public class OperateTicketEditController {
    @Autowired
    OperateTicketService operateTicketService;
    @Autowired
    AccountService accountService;

    @RequestMapping(value = "/operateTicketEdit", method = RequestMethod.GET)
    public ModelAndView operateTicketEditIni(OperateTicketEditForm operateTicketEditForm,
                                                     HttpServletRequest request, HttpServletResponse response) {
        ModelAndView model;
        model = new ModelAndView("operateTicketEdit");
        String operateTicketTemplateId = operateTicketEditForm.getOperateTicketTemplateId();
        String operateTicketId = operateTicketEditForm.getId();
        if (CommonUtils.isNotBlank(operateTicketTemplateId)) {
            OperateTicketTemplate operateTicketTemplate = operateTicketService.getOperateTicketTemplateById(operateTicketTemplateId);
            operateTicketEditForm.setOperateTask(operateTicketTemplate.getOperateTask());
            List<OperateTicketTemplateData> operateTicketTemplateDatas = operateTicketTemplate.getOperateTicketTemplateData();
            model.addObject("operateTicketTemplateDatas", operateTicketTemplateDatas);
        }else if (CommonUtils.isNotBlank(operateTicketId)){
            OperateTicket operateTicket = operateTicketService.getOperateTicketById(operateTicketId);
            operateTicketEditForm.setOperateTicketName(operateTicket.getName());
            operateTicketEditForm.setSendName(operateTicket.getSendName());
            operateTicketEditForm.setRecvName(operateTicket.getRecvName());
            operateTicketEditForm.setSendTime(operateTicket.getSendTime());
            operateTicketEditForm.setBeginTime(operateTicket.getBeginTime());
            operateTicketEditForm.setEndTime(operateTicket.getEndTime());
            operateTicketEditForm.setGuardianName(operateTicket.getGuardianName());
            operateTicketEditForm.setSingleOperator(operateTicket.getSingleOperator());
            operateTicketEditForm.setRepairOperator(operateTicket.getRepairOperator());
            operateTicketEditForm.setComment(operateTicket.getComments());
            operateTicketEditForm.setOperateTask(operateTicket.getOperateTask());
            List<OperateTicketTemplateData> operateTicketTemplateDatas = operateTicket.getOperateTicketTemplateData();
            model.addObject("operateTicketTemplateDatas", operateTicketTemplateDatas);
        }

        List<SysAccount> accountList = accountService.getSysAccountList("","");
        List<String> accoutRealNames = new ArrayList<>();

        for (SysAccount sysAccount : accountList){
            accoutRealNames.add(sysAccount.getRealname());
        }

        model.addObject("accoutRealNames", accoutRealNames);
        return model;
    }

    @RequestMapping(value = "/operateTicketEdit", method = RequestMethod.POST)
    public ModelAndView operateTicketEditControll(OperateTicketEditForm operateTicketEditForm,
                                                          HttpServletRequest request, HttpServletResponse response,HttpSession session) {
        ModelAndView model;

        String id = operateTicketEditForm.getId();
        List<String> contents = operateTicketEditForm.getOperateColumn();
        List<OperateTicketTemplateData> operateTicketTemplateDatas = new ArrayList<>();
        OperateTicket operateTicket = new OperateTicket();

        OperateTicketProcessRecord operateTicketProcessRecord = new OperateTicketProcessRecord();//新建一条操作记录

        if (CommonUtils.isNotBlank(id)) {
            operateTicket = operateTicketService.getOperateTicketById(id);
            operateTicketTemplateDatas = operateTicket.getOperateTicketTemplateData();
            operateTicketProcessRecord.setOperateType(1);
        }else {
            SimpleDateFormat dateParse = new SimpleDateFormat("yyyyMMddHHmmssSSS");
            Calendar today = Calendar.getInstance();
            String seriesNum = dateParse.format(today.getTime());
            operateTicket.setSeriesNo(seriesNum);
            operateTicket.setState(0);
            operateTicketProcessRecord.setOperateType(0);
        }

        if (CommonUtils.isNotBlank(operateTicketEditForm.getSubmit())) {
            operateTicket.setName(operateTicketEditForm.getOperateTicketName().trim());
            operateTicket.setSendName(operateTicketEditForm.getSendName());
            operateTicket.setRecvName(operateTicketEditForm.getRecvName());
            operateTicket.setSendTime(operateTicketEditForm.getSendTime());
            operateTicket.setBeginTime(operateTicketEditForm.getBeginTime());
            operateTicket.setEndTime(operateTicketEditForm.getBeginTime());
            operateTicket.setGuardianName(operateTicketEditForm.getGuardianName());
            operateTicket.setSingleOperator(operateTicketEditForm.getSingleOperator());
            operateTicket.setRepairOperator(operateTicketEditForm.getRepairOperator());
            operateTicket.setOperateTask(operateTicketEditForm.getOperateTask());
            operateTicket.setComments(operateTicketEditForm.getComment());
            int contentSize = contents.size();
            int operateTicketTemplateDatasSize = operateTicketTemplateDatas.size();

            for (int i = 0; i < contentSize; i++) {
                if (i < operateTicketTemplateDatasSize){
                    OperateTicketTemplateData operateTicketTemplateData = operateTicketTemplateDatas.get(i);
                    operateTicketTemplateData.setContent(contents.get(i));
                }else{
                    OperateTicketTemplateData operateTicketTemplateData  = new OperateTicketTemplateData();
                    operateTicketTemplateData.setContent(contents.get(i));
                    operateTicketTemplateDatas.add(operateTicketTemplateData);
                }
            }

            for (int m = contentSize; m < operateTicketTemplateDatasSize; m++){
                if (operateTicketTemplateDatas.get(contentSize) != null){
                    operateTicketTemplateDatas.remove(contentSize);
                }
            }

            operateTicket.setOperateTicketTemplateData(operateTicketTemplateDatas);
        }else if (CommonUtils.isNotBlank(operateTicketEditForm.getCheckSuccess())){
            operateTicket.setState(1);
            operateTicketProcessRecord.setOperateType(2);
        }else if (CommonUtils.isNotBlank(operateTicketEditForm.getCheckFailed())){
            operateTicket.setState(2);
            operateTicketProcessRecord.setOperateType(3);
        }

        UserDetail currentUserInfo = (UserDetail) session.getAttribute(MvcConstant.USER);
        operateTicketProcessRecord.setOperator(currentUserInfo.getAccount());
        Calendar nowTime = Calendar.getInstance();
        operateTicketProcessRecord.setOperateTime(nowTime.getTime());
        operateTicketProcessRecord.setBaseTicket(operateTicket);
        operateTicketService.saveOrUpdateTicket(operateTicket);
        operateTicketService.saveOrUpdateTicketProcessRecord(operateTicketProcessRecord);
        model = new ModelAndView("redirect:/operateTicketList");
        return model;
    }
}
