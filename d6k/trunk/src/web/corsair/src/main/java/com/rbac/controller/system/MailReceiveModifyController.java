package com.rbac.controller.system;

import com.corsair.service.GetInverterDataToExcelService;
import com.corsair.util.JavaMailWithAttachment;
import com.rbac.common.BaseController;
import com.rbac.entity.MailReceiver;
import com.rbac.form.system.MailReceiveModifyForm;
import com.rbac.service.MailReceiveService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.io.File;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

/**
 * Created by 洪祥 on 2015/10/08.
 */
@Controller
public class MailReceiveModifyController extends BaseController{
    @Autowired
    MailReceiveService mailReceiveService;
    @Autowired
    GetInverterDataToExcelService getInverterDataToExcelService;

    @RequestMapping(value= "/mailReceiveModify", method= RequestMethod.GET)
    public ModelAndView ActionModify(MailReceiveModifyForm mailReceiveModifyForm,HttpServletRequest request,HttpSession session){

        ModelAndView model;
        Long id = CommonUtils.parseLong(mailReceiveModifyForm.getId());

        if(CommonUtils.isNotBlank(mailReceiveModifyForm.getId())) {
            MailReceiver receiver = mailReceiveService.getMailReceiverById(id);
            mailReceiveModifyForm.setReceiverName(receiver.getReceiverName());
            mailReceiveModifyForm.setMailAddress(receiver.getMailAddress());
            mailReceiveModifyForm.setPhone(receiver.getPhone());
            mailReceiveModifyForm.setReceiverJob(receiver.getReceiverJob());
            if (CommonUtils.isNotBlank(mailReceiveModifyForm.getId()) && "1".equals(mailReceiveModifyForm.getFlag())) {//编辑
                model = new ModelAndView("mailReceiveModify");
                return model;
            } else {
                model = new ModelAndView("mailReceiveDetail");
                return model;
            }
        }else{
            model = new ModelAndView("mailReceiveModify");
            return model;
        }
    }

    @RequestMapping(value= "/mailReceiveModify", method = RequestMethod.POST)
     public ModelAndView ActionModifyAction(MailReceiveModifyForm mailReceiveModifyForm,HttpServletRequest request,HttpSession session){
        ModelAndView model;
        if(CommonUtils.isNotBlank(mailReceiveModifyForm.getSubmit())){
            MailReceiver receiver = new MailReceiver();
            if(CommonUtils.isNotBlank(mailReceiveModifyForm.getId())){
                Long id = CommonUtils.parseLong(mailReceiveModifyForm.getId());
                receiver = mailReceiveService.getMailReceiverById(id);
                receiver.setModifierId(super.getCurrentAccountId(request));
                receiver.setModifyTime(new Date());
                receiver.setReceiverJob(mailReceiveModifyForm.getReceiverJob());
                receiver.setReceiverName(mailReceiveModifyForm.getReceiverName());
                receiver.setPhone(mailReceiveModifyForm.getPhone());
                receiver.setMailAddress(mailReceiveModifyForm.getMailAddress());
            }else {
                receiver.setCreatorId(super.getCurrentAccountId(request));
                receiver.setCreateTime(new Date());
                receiver.setReceiverJob(mailReceiveModifyForm.getReceiverJob());
                receiver.setReceiverName(mailReceiveModifyForm.getReceiverName());
                receiver.setPhone(mailReceiveModifyForm.getPhone());
                receiver.setMailAddress(mailReceiveModifyForm.getMailAddress());
            }
            mailReceiveService.saveOrUpdateMailReceive(receiver);
            model = new ModelAndView("redirect:/mailReceiveList");
            return model;
        }
        model = new ModelAndView("mailReceiveModify");
        return model;
    }

    /**
     * 实时发送
     * @param mailReceiveModifyForm
     * @param request
     * @param session
     */
    @RequestMapping(value= "/actualTimeMailReceive", method = RequestMethod.GET)
    public ModelAndView ActionActualTimeMailReceiveAction(MailReceiveModifyForm mailReceiveModifyForm,HttpServletRequest request,HttpSession session) {
        ModelAndView model;
        String mail_address = request.getParameter("mailAddress");
        String param=request.getParameter("param");//1实时发送,0停止定时发送
        String mail_id = request.getParameter("mail_id");
        getInverterDataToExcelService.GenerateExcelData(mail_address);//实时发送邮件
//        JavaMailWithAttachment se = new JavaMailWithAttachment(true);
//        ArrayList<File> affix=JavaMailWithAttachment.getListFiles(getMailFileName());
//        if(affix.size()>0) {//15151835907@163.com,zhangy@sznari.com,zhangjq@sznari.com
//            se.doSendHtmlEmail("当天逆变器数据", "附件内容是今天的逆变器数据",mail_address, affix);//
//        }
        model = new ModelAndView("redirect:/mailReceiveList");
        model.addObject("param_flag",param);
        model.addObject("mail_id",mail_id);
        return model;
    }
    /**
     * 删除记录
     * @param mailReceiveModifyForm
     * @param request
     * @param session
     */
    @RequestMapping(value= "/delMailReceiveById", method = RequestMethod.GET)
    public ModelAndView ActionDelMailReceiveById(MailReceiveModifyForm mailReceiveModifyForm,HttpServletRequest request,HttpSession session) {
        ModelAndView model;
        String id = request.getParameter("id");
        mailReceiveService.delMailReceiveById(Long.parseLong(id));
        model = new ModelAndView("redirect:/mailReceiveList");
        return model;
    }
    /**
     * 定时发送
     * @param mailReceiveModifyForm
     * @param request
     * @param session
     */
    @RequestMapping(value= "/clockingMailReceive", method = RequestMethod.GET)
    public ModelAndView ActionClockingMailReceiveAction(MailReceiveModifyForm mailReceiveModifyForm,HttpServletRequest request,HttpSession session) {
//        ModelAndView model;
//        String mail_address = request.getParameter("mailAddress");
//        String param=request.getParameter("param");//1实时发送,0停止定时发送
//        String mail_id = request.getParameter("mail_id");
//        Timer timer = null;
//        String sendUrl = getMailFileName();
//        if ("1".equals(param)){
//            timer = new Timer();
//            timer.schedule(new MyTask(mail_address,sendUrl), 15000, 50000);//在1秒后执行此任务,每次间隔2秒执行一次,如果传递一个Data参数,就可以在某个固定的时间执行这个任务.
//        }else if ("2".equals(param)){
//            timer.cancel();
//        }
//        model = new ModelAndView("redirect:/mailReceiveList");
//        model.addObject("param_flag",param);
//        model.addObject("mail_id",mail_id);
//        return model;
       // String id = request.getParameter("id");
        ModelAndView model;
        String id = request.getParameter("id");
        mailReceiveService.delMailReceiveById(Long.parseLong(id));
        model = new ModelAndView("redirect:/mailReceiveList");
        return model;
    }
    static class MyTask extends java.util.TimerTask{//定时器
        private String mailAddress;
        private String sendUrl;
        public MyTask(){}
        public MyTask(String address,String sendUrl){this.mailAddress=address;this.sendUrl=sendUrl;}
        public void run(){
            JavaMailWithAttachment se = new JavaMailWithAttachment(true);
            ArrayList<File> affix=JavaMailWithAttachment.getListFiles(this.sendUrl);
            if(affix.size()>0) {
                se.doSendHtmlEmail("当天逆变器数据", "附件内容是今天的逆变器数据", this.mailAddress, affix);//
            }
        }
    }
    public String getMailFileName() {
        //以日期作為文件名
        DateFormat date = new SimpleDateFormat("yyyy-MM-dd");
        String time = date.format(new Date());
        String path = this.getClass().getClassLoader().getResource("../../").getPath();
        String fileName = path + "resources/" +"inverterData/"+ time;// + time+"_"+model+".xls";
        return fileName;
    }
}
