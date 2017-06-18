package com.corsair.controller;

import com.corsair.entity.OperateTicket;
import com.corsair.entity.OperateTicketTemplateData;
import com.corsair.form.OperateTicketListForm;
import com.corsair.service.OperateTicketService;
import com.itextpdf.text.DocumentException;
import com.itextpdf.text.Font;
import com.itextpdf.text.pdf.*;
import com.rbac.common.MvcConstant;
import com.rbac.common.UserDetail;
import com.rbac.entity.SysAccountRole;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.List;
import java.util.Set;

/**
 * Created by zjq on 2015/10/22.
 */
@Controller
public class OperateTicketListController {
    @Autowired
    OperateTicketService operateTicketService;

    @RequestMapping(value = "/operateTicketList")
    public ModelAndView operateTickettListIni(OperateTicketListForm operateTicketListForm,
                                                      HttpServletRequest request, HttpServletResponse response,HttpSession session){
        ModelAndView model;
        if (CommonUtils.isNotBlank(operateTicketListForm.getDel())) {
            operateTicketService.deleteOperateTicket(operateTicketListForm.getDeleteId());
        }else if (CommonUtils.isNotBlank(operateTicketListForm.getView())){
            ByteArrayOutputStream operateTicketStream = operateTicketGenerate(operateTicketListForm.getViewId(),request);

            try {
                response.setContentType("application/pdf");
                response.setContentLength(operateTicketStream.size());
                ServletOutputStream out = response.getOutputStream();
                operateTicketStream.writeTo(out);
                out.flush();
            }catch(IOException e){
                e.printStackTrace();
            }
            return null;
        }

        model = new ModelAndView("operateTicketList");
        UserDetail currentUserInfo = (UserDetail) session.getAttribute(MvcConstant.USER);
        Set<SysAccountRole> sysAccountRoleSet = currentUserInfo.getAccount().getSysAccountRoles();

        for (SysAccountRole sysAccountRole : sysAccountRoleSet){
            if (sysAccountRole.getSysRole().getId() == 1){//暂定管理员角色有审核权限
                model.addObject("isCheckAuthority", true);
            }
        }

        List operateTicketList = operateTicketService.getOperateTicketList();
        model.addObject("operateTicketList", operateTicketList);
        return model;
    }

    private ByteArrayOutputStream operateTicketGenerate(String  operateTicketId,HttpServletRequest request) {
        OperateTicket operateTicket = operateTicketService.getOperateTicketById(operateTicketId);
    //    String operateName = operateTicket.getName();
        ByteArrayOutputStream outputstream = null;

        try {
            String currentPath = request.getServletContext().getRealPath("\\");
            String TemplatePDF = currentPath + "resources\\" +"tickets\\" + "OperateTicketTemplate.pdf";
            PdfReader reader = new PdfReader(TemplatePDF);
         //   OutputStream outputstream = new FileOutputStream(request.getSession().getServletContext().getRealPath("\\") + "resources\\" + "tickets\\" + operateName +".pdf");
            outputstream = new ByteArrayOutputStream(4096);
            PdfStamper stamp = new PdfStamper(reader, outputstream);
            PdfContentByte under = stamp.getUnderContent(1); /*使用中文字体*/
            BaseFont bf = BaseFont.createFont("STSong-Light", "UniGB-UCS2-H", BaseFont.NOT_EMBEDDED);
            Font FontChinese = new Font(bf, 10, Font.NORMAL);
            AcroFields form = stamp.getAcroFields();
            form.addSubstitutionFont(bf);  //简宋体
            form.setField("series_num", operateTicket.getSeriesNo());
            form.setField("send_name", operateTicket.getSendName());
            form.setField("recv_name", operateTicket.getRecvName());
            form.setField("send_time", operateTicket.getSendTime());
            form.setField("begin_time", operateTicket.getBeginTime());
            form.setField("end_time", operateTicket.getEndTime());
            form.setField("guardian_name", operateTicket.getGuardianName());
            form.setField("single_operator", operateTicket.getSingleOperator());
            form.setField("repair_operator", operateTicket.getRepairOperator());
            form.setField("comment", operateTicket.getComments());
            form.setField("ticket_title", operateTicket.getName());
            form.setField("operate_task", operateTicket.getOperateTask());
            List<OperateTicketTemplateData> operateItems = operateTicket.getOperateTicketTemplateData();

            for (int i = 0; i < operateItems.size(); i++) {
                form.setField("seq_" + Integer.toString(i + 1), Integer.toString(operateItems.get(i).getIndexNumber()));
                form.setField("operate_column_" + Integer.toString(i + 1), operateItems.get(i).getContent());
            }

            stamp.setFormFlattening(true);
            stamp.close();
            reader.close();
        } catch (DocumentException e1) {
            e1.printStackTrace();
        }catch (IOException e2){
            e2.printStackTrace();
        }

        return outputstream;
    }
}




