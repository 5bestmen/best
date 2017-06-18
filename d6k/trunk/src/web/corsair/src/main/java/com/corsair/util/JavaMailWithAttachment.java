package com.corsair.util;
import org.springframework.objenesis.instantiator.basic.FailingInstantiator;
import sun.plugin2.util.SystemUtil;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Properties;

import javax.activation.DataHandler;
import javax.activation.DataSource;
import javax.activation.FileDataSource;
import javax.mail.BodyPart;
import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.Multipart;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeBodyPart;
import javax.mail.internet.MimeMessage;
import javax.mail.internet.MimeMultipart;
import javax.mail.internet.MimeUtility;
/**
 * Created by Administrator on 2016/1/6.
 */
public class JavaMailWithAttachment {
    private MimeMessage message;
    private Session session;
    private Transport transport;

    private String mailHost = "";
    private String sender_username = "";
    private String sender_password = "";

    /*
     * 初始化方法
     */
    public JavaMailWithAttachment(boolean debug) {
        Properties pop = PropertiesUtils.loadPropertyFile("mail.properties");
        //Enumeration enumKeys = pop.keys();
        //while (enumKeys.hasMoreElements())
        //{
        //System.out.println(pop.getProperty("startTime"));
        //System.out.println(pop.getProperty("endTime"));
        this.mailHost = pop.getProperty("mail.smtp.host");
        this.sender_username = pop.getProperty("mail.sender.username");
        this.sender_password = pop.getProperty("mail.sender.password");

        session = Session.getInstance(pop);
        session.setDebug(debug);// 开启后有调试信息
        message = new MimeMessage(session);
    }

    /**
     * 发送邮件
     *
     * @param subject
     *            邮件主题
     * @param sendHtml
     *            邮件内容
     * @param receiveUser
     *            收件人地址
     * @param attachment
     *            附件
     */
    public void doSendHtmlEmail(String subject, String sendHtml, String receiveUser, List<File> attachment) {
        try {
            // 发件人
            InternetAddress from = new InternetAddress(sender_username);
            message.setFrom(from);

            //这里是关键
            List list = new ArrayList();//不能使用string类型的类型，这样只能发送一个收件人
            String []median=receiveUser.split(",");//对输入的多个邮件进行逗号分割
            for(int i=0;i<median.length;i++){
                list.add(new InternetAddress(median[i]));
            }
            InternetAddress[] address =(InternetAddress[])list.toArray(new InternetAddress[list.size()]);

            message.setRecipients(Message.RecipientType.TO,address);//当邮件有多个收件人时，用逗号隔开

            // 收件人  发给一个人
            //InternetAddress to = new InternetAddress(receiveUser);
            //message.setRecipient(Message.RecipientType.TO, to);

            // 邮件主题
            message.setSubject(subject);

            // 向multipart对象中添加邮件的各个部分内容，包括文本内容和附件
            Multipart multipart = new MimeMultipart();

            // 添加邮件正文
            MimeBodyPart textBodyPart = new MimeBodyPart(); // 用来设置正文的
            textBodyPart.setText(sendHtml);
            // 邮件正文
            multipart.addBodyPart(textBodyPart);
            // 添加附件的内容
            if (attachment != null){
                File file = null;
                for(int i=0;i<attachment.size();i++){
                    file = attachment.get(i);
                    BodyPart attachmentBodyPart = new MimeBodyPart();
                    DataSource source = new FileDataSource(file);
                    attachmentBodyPart.setDataHandler(new DataHandler(source));

                    // 网上流传的解决文件名乱码的方法，其实用MimeUtility.encodeWord就可以很方便的搞定
                    // 这里很重要，通过下面的Base64编码的转换可以保证你的中文附件标题名在发送时不会变成乱码
                    //sun.misc.BASE64Encoder enc = new sun.misc.BASE64Encoder();
                    //messageBodyPart.setFileName("=?GBK?B?" + enc.encode(attachment.getName().getBytes()) + "?=");

                    //MimeUtility.encodeWord可以避免文件名乱码
                    attachmentBodyPart.setFileName(MimeUtility.encodeWord(file.getName()));
                    multipart.addBodyPart(attachmentBodyPart);
                }

            }
            // 将multipart对象放到message中
            message.setContent(multipart);
            // 保存邮件
            message.saveChanges();

            transport = session.getTransport("smtp");
            // smtp验证，就是你用来发邮件的邮箱用户名密码
            transport.connect(mailHost, sender_username, sender_password);
            // 发送
            transport.sendMessage(message, message.getAllRecipients());

            System.out.println("send success!");
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (transport != null) {
                try {
                    transport.close();
                } catch (MessagingException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static void main(String[] args) {
        JavaMailWithAttachment se = new JavaMailWithAttachment(true);
        //String files = se.getMailFileName();
        ArrayList<File> affix=JavaMailWithAttachment.getListFiles("");
        se.doSendHtmlEmail("邮件主题", "邮件附件内容", "15151835907@163.com", affix);//
    }

    /***
     * 获取指定目录下的所有的文件（不包括文件夹），采用了递归
     *
     * @param obj
     * @return
     */
    public static ArrayList<File> getListFiles(Object obj) {
        File directory = null;
        if (obj instanceof File) {
            directory = (File) obj;
        } else {
            directory = new File(obj.toString());
        }
        ArrayList<File> files = new ArrayList<File>();
        if (directory.isFile()) {
            files.add(directory);
            return files;
        } else if (directory.isDirectory()) {
            File[] fileArr = directory.listFiles();
            for (int i = 0; i < fileArr.length; i++) {
                File fileOne = fileArr[i];
                files.addAll(getListFiles(fileOne));
            }
        }
        return files;
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
