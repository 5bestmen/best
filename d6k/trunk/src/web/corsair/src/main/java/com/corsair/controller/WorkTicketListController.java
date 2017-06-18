package com.corsair.controller;

import com.corsair.entity.*;
import com.corsair.form.WorkTicketForm;
import com.corsair.service.WorkTicketService;
import com.itextpdf.text.DocumentException;
import com.itextpdf.text.pdf.AcroFields;
import com.itextpdf.text.pdf.BaseFont;
import com.itextpdf.text.pdf.PdfReader;
import com.itextpdf.text.pdf.PdfStamper;
import com.rbac.entity.SysAccount;
import com.rbac.service.AccountService;
import com.rbac.util.CommonUtils;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.URLEncoder;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Created by 洪祥 on 2015/10/23.
 */
@Controller
public class WorkTicketListController {
    @Autowired
    WorkTicketService workTicketService;
    @Autowired
    AccountService accountService;

    @RequestMapping(value = "/workTicketList")
    public ModelAndView workTicketListControl(WorkTicketForm workTicketForm, HttpServletRequest request, HttpServletResponse response) {
        ModelAndView workTicketModel = new ModelAndView("/workTicketList");
        if (CommonUtils.isNotBlank(workTicketForm.getDel())) {
            workTicketService.deleteWorkTicket(workTicketForm.getDeleteIds());
        } else if (CommonUtils.isNotBlank(workTicketForm.getView())) {

            String title = workTicketGenerator(workTicketForm.getViewId(), request);
            try {
                String ticketRelativePath = "/resources/tickets/" + URLEncoder.encode(title.trim(), "UTF-8") + ".pdf";
                response.sendRedirect(ticketRelativePath);
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }
        List workTicketList = workTicketService.getWorkTicketList(workTicketForm.getWorkTicketType(), workTicketForm.getCode());
        List workTicketNewList = new ArrayList();
        WorkTicket workTicket = null;
        for (Object ticketBean:workTicketList){
            workTicket = new WorkTicket();
            workTicket = (WorkTicket)ticketBean;
            workTicket.setNewResponsiblePerson(getRealNameByUserName(workTicket.getNewResponsiblePerson()));
            workTicketNewList.add(workTicket);
        }
        //对工作票列表排序
        sortByTicketState(workTicketNewList);
        sortByTime(workTicketNewList);//根据WorkTicketCreateTime降序排列列表数据
        workTicketModel.addObject("workTicketList", workTicketNewList);
        return workTicketModel;
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
    //按照数据索引号对设备模板数据进行升序排序
    private void sortByTime(List<WorkTicket> workTickets) {

        for (int i = 0; i < workTickets.size(); i++) {

            for (int j = i + 1; j < workTickets.size(); j++) {

                String  frontTime = workTickets.get(i).getWorkTicketCreateTime();
                String  behindTime = workTickets.get(j).getWorkTicketCreateTime();
                SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                Date frontDate = null;
                Date behindDate = null;
                try {
                    if(frontTime!=null){
                        frontDate = sdf.parse(frontTime);
                    }
                   if(behindTime!=null){
                       behindDate = sdf.parse(behindTime);
                   }
                } catch (Exception e) {
                    e.printStackTrace();
                }
                if (frontDate !=null && behindDate!=null){
                    if (frontDate.getTime() < behindDate.getTime()) {
                        WorkTicket temp = workTickets.get(i);
                        workTickets.set(i, workTickets.get(j));
                        workTickets.set(j, temp);
                    }
                }
            }
        }
    }

    private String workTicketGenerator(String id, HttpServletRequest request) {

        WorkTicket workTicket = workTicketService.getWorkTicketById(id);
        String title = revertWorkTicketType(workTicket.getWorkTicketType());//workTicket.getStationName() + "-" +
        workTicket.setResponsiblePerson(getRealNameByUserName(workTicket.getResponsiblePerson()));
        try {
            String currentPath = request.getServletContext().getRealPath("\\");
            // String TemplatePDF = currentPath + "resources\\" +"tickets\\" + "WorkTicketTemplate.pdf";
            String arr;
            if (title.equals("电气一种工作票")) {
                arr = "WorkTicketTemplate1.pdf";
            } else {
                arr = "WorkTicketTemplate2.pdf";
            }
            String TemplatePDF = currentPath + "resources\\" + "tickets\\" + arr;
            PdfReader reader = new PdfReader(TemplatePDF);
            OutputStream outputstream = new FileOutputStream(request.getSession().getServletContext().getRealPath("\\") + "resources\\" + "tickets\\" + title + ".pdf");
            PdfStamper stamp = new PdfStamper(reader, outputstream);

//            PdfContentByte under = stamp.getUnderContent(1);
            BaseFont bf = BaseFont.createFont("STSong-Light", "UniGB-UCS2-H", BaseFont.NOT_EMBEDDED);
//            Font FontChinese = new Font(bf, 10, Font.NORMAL);
            AcroFields form = stamp.getAcroFields();
            form.addSubstitutionFont(bf);  //简宋体

            // form.setField("title", title);
            form.setField("code", workTicket.getCode());
            // form.setField("stationName", workTicket.getStationName());
            form.setField("permitResponsiblePerson", workTicket.getResponsiblePerson());
            form.setField("newResponsiblePerson", workTicket.getResponsiblePerson());
            form.setField("department", workTicket.getDepartment());
            form.setField("plannedWorkingStartingTime", workTicket.getPlannedWorkingStartingTime());
            form.setField("plannedWorkingEndingTime", workTicket.getPlannedWorkingEndingTime());
            //String[] crews = (String[])workTicket.getCrew().toArray();
            String Crew = "";
            if (workTicket.getCrew().size() > 0) {
                for (String s : workTicket.getCrew()) {
                    Crew += s + ",";
                }
                Crew = Crew.substring(0, Crew.length() - 1);
            }
            form.setField("crew", Crew);
            form.setField("crewNum", String.valueOf(workTicket.getCrew().size()));
            form.setField("task", workTicket.getTask());
            //form.setField("workPlace", workTicket.getWorkPlace());
            form.setField("remark", workTicket.getRemark());

            form.setField("summaryTime", workTicket.getSummaryTime());
            form.setField("workCondition", workTicket.getWorkCondition());
            form.setField("workTicketSigner", workTicket.getWorkTicketSigner());
            form.setField("signTime", workTicket.getSignTime());
            form.setField("extensionTime", workTicket.getExtensionTime());
            form.setField("addSecurityMeasures", workTicket.getAddSecurityMeasures());
            form.setField("PermittingWorkTime", workTicket.getPermittingWorkTime());
            form.setField("responsiblePersonTime", workTicket.getFillWorkTicketTime());
            form.setField("permitPersonTime", workTicket.getPermitPersonTime());
            form.setField("permitPerson", getRealNameByUserName(workTicket.getWorkPermitPerson()));
            form.setField("otherMatters", workTicket.getOtherMatters());
            form.setField("receivedWorkTicketTime", workTicket.getReceivedWorkTicketTime());
            form.setField("RunningDuty", workTicket.getRunningDuty());
            form.setField("guardian", workTicket.getFullTimePerson());
            form.setField("otherMatters", workTicket.getOtherRemark());
            form.setField("fullRemark", workTicket.getFullRemark());
           /* //注入安全措施数据
            List<SecurityMeasures> securityMeasureses = workTicket.getSecurityMeasureses();
            for (int i = 0; i < securityMeasureses.size(); i++) {
                form.setField("measure_" + Integer.toString(i + 1), i+1+")");
                form.setField("securityMeasure_" + Integer.toString(i + 1), securityMeasureses.get(i).getMeasure());
            }*/

            //注入安全措施数据
            List<SecurityMeasures> securityMeasureses = workTicket.getSecurityMeasureses();
            for (int i = 0; i < securityMeasureses.size(); i++) {
                String type = securityMeasureses.get(i).getSecurityMeasuresType();

                if (StringUtils.isNotEmpty(type)) {
                    List<SecurityMeasures> securityMeasureList = workTicketService.getSecurityMeasureListByTypeAndId(type, workTicket);
                    for (int j = 0; j < securityMeasureList.size(); j++) {

                        if (type.equals("1")) {
                            form.setField("firstMeasure_" + Integer.toString(j + 1), securityMeasureList.get(j).getMeasure());
                            form.setField("firstFinished_" + Integer.toString(j + 1), String.valueOf(securityMeasureList.get(j).getFinished()));
                            form.setField("countUnfinishedGroundLine",Integer.toString(j + 1));
                        } else if (type.equals("2")) {
                            form.setField("secondMeasure_" + Integer.toString(j + 1), securityMeasureList.get(j).getMeasure());
                            form.setField("secondFinished_" + Integer.toString(j + 1), String.valueOf(securityMeasureList.get(j).getFinished()));
                            form.setField("countUnfinishedSwitchesAndKnife",Integer.toString(j + 1));
                        } else {
                            form.setField("thirdMeasure_" + Integer.toString(j + 1), securityMeasureList.get(j).getMeasure());
                            form.setField("thirdFinished_" + Integer.toString(j + 1), String.valueOf(securityMeasureList.get(j).getFinished()));
                        }
                    }
                } else {
                    List<SecurityMeasures> securityMeasures = workTicket.getSecurityMeasureses();
                    for (int k = 0; k < securityMeasures.size(); k++) {
                        form.setField("measure_" + Integer.toString(k + 1), k + 1 + ")");
                        form.setField("securityMeasure_" + Integer.toString(k + 1), securityMeasureses.get(k).getMeasure());
                    }
                }

            }


            //注入工作任务
            List<WorkContent> workContents = workTicket.getWorkContents();
            if (workContents.size() > 0) {
                for (int i = 0; i < workContents.size(); i++) {
                    form.setField("taskId_" + Integer.toString(i + 1), i + 1 + ")");
                    form.setField("taskName_" + Integer.toString(i + 1), workContents.get(i).getTaskName());
                    form.setField("taskContent_" + Integer.toString(i + 1), workContents.get(i).getTaskContent());
                }
            }
            //注入延长期
            List<WorkTicketDelay> workTicketDelays = workTicket.getWorkTicketDelays();
            if (workTicketDelays.size() > 0) {
                for (int i = 0; i < workTicketDelays.size(); i++) {
                    form.setField("extensionTime", workTicketDelays.get(i).getEndTime());
                    form.setField("name", workTicketDelays.get(i).getName());
                    form.setField("responsiblePersonTime(delay)" , workTicket.getFillWorkTicketTime());
                    form.setField("permitPersonTime(delay)", workTicket.getPermitPersonTime());
                    form.setField("newResponsiblePerson(delay)", workTicket.getResponsiblePerson());
                }
            }

            //注入工作许可数据
            List<WorkPermit> workPermits = workTicket.getWorkPermits();
            if (workPermits.size() > 0) {
                List<String> permits = workPermits.get(0).getPermits();
                for (int i = 0; i < permits.size(); i++) {
                    form.setField("permit_" + Integer.toString(i + 1), i + 1 + ")");
                    form.setField("workPermit_" + Integer.toString(i + 1), permits.get(i));
                }
               // form.setField("permitPerson", workPermits.get(0).getPermitPerson());
               // form.setField("permitResponsiblePerson", workPermits.get(0).getResponsiblePerson());
            }

            //注入工作时间记录
            List<WorkTicketRecord> workTicketRecords = workTicket.getWorkTicketRecords();
            if (workTicketRecords.size() > 0) {
                for (int i = 0; i < workTicketRecords.size(); i++) {
                    form.setField("workStartTime_" + Integer.toString(i + 1), workTicketRecords.get(i).getStartTime());
                    form.setField("workEndTime_" + Integer.toString(i + 1), workTicketRecords.get(i).getEndTime());
                    form.setField("permitResponsiblePerson_"+ Integer.toString(i + 1) ,workTicketRecords.get(i).getResponsiblePerson());
                    form.setField("permitPerson_" + Integer.toString(i + 1),workTicketRecords.get(i).getPermitPerson());
                }
            }


            //注入补充注意事项
            List<AttenTionMeasure> attenTionMeasures = workTicket.getAttenTionMeasure();
            if (attenTionMeasures.size() > 0) {
                for (int i = 0; i < attenTionMeasures.size(); i++) {
                    form.setField("attenTionMeasures_" + Integer.toString(i + 1), attenTionMeasures.get(i).getAttenTionMeasure());
                }
            }

            //注入补充安全措施
            List<NeedAttention> needAttentions = workTicket.getNeedAttentions();
            if (needAttentions.size() > 0) {
                for (int i = 0; i < needAttentions.size(); i++) {
                    form.setField("needAttentions_" + Integer.toString(i + 1), needAttentions.get(i).getNeedAttention());
                }
            }
            //注入工作变动
            List<PersonnelChangeCondition> personnelChangeConditions = workTicket.getPersonnelChanges();
            if (personnelChangeConditions.size() > 0) {
                for (int i = 0; i < personnelChangeConditions.size(); i++) {
                    String changeName = getRealNameByUserName(personnelChangeConditions.get(i).getName());
                    form.setField("changePersonName_" + Integer.toString(i + 1), changeName);
                    form.setField("newResponsiblePerson(change)", changeName);
                    form.setField("newResponsiblePerson", changeName);
                    form.setField("PersonnelChangeTime_" + Integer.toString(i + 1), personnelChangeConditions.get(i).getStartTime());
                    form.setField("signTime(change)", workTicket.getSignTime());
                    form.setField("workTicketSigner(change)", workTicket.getWorkTicketSigner());
                    form.setField("permitResponsiblePerson(change)", workTicket.getResponsiblePerson());
                }
            }else{
                form.setField("newResponsiblePerson", workTicket.getResponsiblePerson());
            }

            //注入工作终结数据
          //  form.setField("summary", workTicket.getSummary());
          //  form.setField("workTicketEnd", workTicket.getWorkTicketEnd());

            stamp.setFormFlattening(true);
            stamp.close();
            reader.close();

        } catch (DocumentException e1) {
            e1.printStackTrace();
        } catch (IOException e2) {
            e2.printStackTrace();
        }
        return title;
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

    //按照工作票的状态进行排序，顺序依次为：未审核、审核中、审核通过和审核不通过
    private void sortByTicketState(List<WorkTicket> workTickets) {

        for (int i = 0; i < workTickets.size(); i++) {

            for (int j = i + 1; j < workTickets.size(); j++) {

                int frontState = workTickets.get(i).getState();
                int behindState = workTickets.get(j).getState();

                if (frontState > behindState) {
                    WorkTicket temp = workTickets.get(i);
                    workTickets.set(i, workTickets.get(j));
                    workTickets.set(j, temp);
                }
            }
        }
    }

}
