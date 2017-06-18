package com.corsair.controller;

import com.corsair.entity.OperateTicketProcessRecord;
import com.corsair.entity.OperateTicketTemplate;
import com.corsair.entity.OperateTicketTemplateData;
import com.corsair.form.OperateTicketTemplateEditForm;
import com.corsair.service.OperateTicketService;
import com.rbac.common.MvcConstant;
import com.rbac.common.UserDetail;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

/**
 * Created by zjq on 2015/10/21.
 */
@Controller
public class OperateTicketTemplateEditController {
    @Autowired
    OperateTicketService operateTicketService;

    @RequestMapping(value = "/operateTicketTemplateEdit", method = RequestMethod.GET)
    public ModelAndView operateTicketTemplateEditIni(OperateTicketTemplateEditForm operateTicketTemplateEditForm,
                                                     HttpServletRequest request, HttpServletResponse response) {
        ModelAndView model;
        model = new ModelAndView("operateTicketTemplateEdit");

        if (CommonUtils.isNotBlank(operateTicketTemplateEditForm.getId())) {
            OperateTicketTemplate operateTicketTemplate = operateTicketService.getOperateTicketTemplateById(operateTicketTemplateEditForm.getId());
            operateTicketTemplateEditForm.setOperateTask(operateTicketTemplate.getOperateTask());
            operateTicketTemplateEditForm.setOperateTicketTemplateName(operateTicketTemplate.getName());
            operateTicketTemplateEditForm.setComment(operateTicketTemplate.getComments() == null ? "" : operateTicketTemplate.getComments());
            List<OperateTicketTemplateData> operateTicketTemplateDatas = operateTicketTemplate.getOperateTicketTemplateData();
            model.addObject("operateTicketTemplateDatas", operateTicketTemplateDatas);
        }

        return model;
    }

    @RequestMapping(value = "/operateTicketTemplateEdit", method = RequestMethod.POST)
    public ModelAndView operateTicketTemplateEditControll(OperateTicketTemplateEditForm operateTicketTemplateEditForm,
                                                          HttpServletRequest request, HttpServletResponse response,HttpSession session) {
        ModelAndView model;

        if (CommonUtils.isNotBlank(operateTicketTemplateEditForm.getSubmit())) {
            String id = operateTicketTemplateEditForm.getId();
            List<String> contents = operateTicketTemplateEditForm.getOperateColumn();
            List<OperateTicketTemplateData> operateTicketTemplateDatas = new ArrayList<>();
            OperateTicketTemplate operateTicketTemplate = new OperateTicketTemplate();
            OperateTicketProcessRecord operateTicketProcessRecord = new OperateTicketProcessRecord();//新建一条操作记录

            if (CommonUtils.isNotBlank(id)) {
                operateTicketTemplate = operateTicketService.getOperateTicketTemplateById(id);
                operateTicketTemplateDatas = operateTicketTemplate.getOperateTicketTemplateData();
                operateTicketProcessRecord.setOperateType(1);
            }else{
                operateTicketProcessRecord.setOperateType(0);
            }

            operateTicketTemplate.setName(operateTicketTemplateEditForm.getOperateTicketTemplateName().trim());
            operateTicketTemplate.setOperateTask(operateTicketTemplateEditForm.getOperateTask());
            operateTicketTemplate.setComments(operateTicketTemplateEditForm.getComment());

            int contentSize = contents.size();
            int operateTicketTemplateDatasSize = operateTicketTemplateDatas.size();
            for (int i = 0; i < contentSize; i++) {

                if (i < operateTicketTemplateDatasSize){
                    OperateTicketTemplateData operateTicketTemplateData = operateTicketTemplateDatas.get(i);
                 //   operateTicketTemplateData.setIndexNumber(i + 1);
                    operateTicketTemplateData.setContent(contents.get(i));
                 //   operateTicketTemplateData.setBaseTicket(operateTicketTemplate);
                }else{
                    OperateTicketTemplateData operateTicketTemplateData  = new OperateTicketTemplateData();
                 //   operateTicketTemplateData.setIndexNumber(i + 1);
                    operateTicketTemplateData.setContent(contents.get(i));
                 //   operateTicketTemplateData.setBaseTicket(operateTicketTemplate);
                    operateTicketTemplateDatas.add(operateTicketTemplateData);
                }
            }

            for (int m = contentSize; m < operateTicketTemplateDatasSize; m++){
               if (operateTicketTemplateDatas.get(contentSize) != null){
                   operateTicketTemplateDatas.remove(contentSize);
               }
            }

            operateTicketTemplate.setOperateTicketTemplateData(operateTicketTemplateDatas);
            UserDetail currentUserInfo = (UserDetail) session.getAttribute(MvcConstant.USER);
            operateTicketProcessRecord.setOperator(currentUserInfo.getAccount());
            Calendar nowTime = Calendar.getInstance();
            operateTicketProcessRecord.setOperateTime(nowTime.getTime());
            operateTicketProcessRecord.setBaseTicket(operateTicketTemplate);
            operateTicketService.saveOrUpdateTicketTemplate(operateTicketTemplate);
            operateTicketService.saveOrUpdateTicketProcessRecord(operateTicketProcessRecord);
            model = new ModelAndView("redirect:/operateTicketTemplateList");
        } else {
            model = new ModelAndView("operateTicketTemplateEdit");
        }

        return model;
    }
}
