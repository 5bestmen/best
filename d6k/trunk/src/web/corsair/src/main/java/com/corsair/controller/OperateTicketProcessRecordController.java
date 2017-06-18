package com.corsair.controller;

import com.corsair.entity.OperateTicket;
import com.corsair.entity.OperateTicketTemplate;
import com.corsair.service.OperateTicketService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.util.Set;

/**
 * Created by zjq on 2015/10/30.
 */
@Controller
public class OperateTicketProcessRecordController{
@Autowired
OperateTicketService operateTicketService;

        @RequestMapping(value = "/operateTicketProcessRecord")
        public ModelAndView operateTicketProcessRecordIni(String id,String type,
                                                  HttpServletRequest request, HttpServletResponse response,HttpSession session){
            ModelAndView model = new ModelAndView("operateTicketProcessRecord");
            OperateTicket operateTicket = null;
            OperateTicketTemplate operateTicketTemplate = null;
            Set processRecords = null;

            if (CommonUtils.isNotBlank(id)) {
                if (type == null){
                    operateTicket = operateTicketService.getOperateTicketById(id);
                    model.addObject("ticketName", operateTicket.getName());
                    processRecords = operateTicket.getOperateTicketProcessRecords();
                }else {
                    operateTicketTemplate = operateTicketService.getOperateTicketTemplateById(id);
                    model.addObject("ticketName", operateTicketTemplate.getName());
                    processRecords = operateTicketTemplate.getOperateTicketProcessRecords();
                }
            }

            model.addObject("processRecords", processRecords);
            return model;
        }
}
