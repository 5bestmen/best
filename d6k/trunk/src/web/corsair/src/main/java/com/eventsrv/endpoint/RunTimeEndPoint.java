package com.eventsrv.endpoint;

import com.corsair.service.ApplicationInformationService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by ChengXi on 2016/1/14.
 */
public class RunTimeEndPoint extends TextWebSocketHandler {

    @Autowired
    private ApplicationInformationService applicationInformationService;

    @Override
    protected void handleTextMessage(WebSocketSession session,TextMessage message) throws Exception {
        super.handleTextMessage(session, message);

    }

    @Override
    public void afterConnectionEstablished(WebSocketSession session)
            throws Exception {

        String startDateTime = applicationInformationService.getRunningTime();
        String format = "yyyy-MM-dd HH:mm:ss";
        SimpleDateFormat df = new SimpleDateFormat(format);

        while (session.isOpen()) {
            String curDateTime = df.format(new Date());
            if (isValidDate(startDateTime,format)){
                String runtime = dateDiff(startDateTime,curDateTime,format, "h");
                session.sendMessage(new TextMessage(runtime));
            }
            Thread.sleep(1000*60*10); // 十分钟更新一次
        }
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session,
                                      CloseStatus status)
            throws Exception {
    }

    private String dateDiff(String startTime, String endTime,  String format, String str) {
        // 按照传入的格式生成一个simpledateformate对象
        SimpleDateFormat sd = new SimpleDateFormat(format);
        long nd = 1000 * 24 * 60 * 60;// 一天的毫秒数
        long nh = 1000 * 60 * 60;// 一小时的毫秒数
        long nm = 1000 * 60;// 一分钟的毫秒数
        long ns = 1000;// 一秒钟的毫秒数
        long diff;
        long day = 0;
        long hour = 0;
        long min = 0;
        long sec = 0;
        // 获得两个时间的毫秒时间差异
        try {
            diff = sd.parse(endTime).getTime() - sd.parse(startTime).getTime();
            day = diff / nd;// 计算差多少天
            hour = diff % nd / nh + day * 24;// 计算差多少小时
            min = diff % nd % nh / nm + day * 24 * 60;// 计算差多少分钟
            sec = diff % nd % nh % nm / ns;// 计算差多少秒
            // 输出结果
            System.out.println("时间相差：" + day + "天" + (hour - day * 24) + "小时"
                    + (min - day * 24 * 60) + "分钟" + sec + "秒。");
            System.out.println("hour=" + hour + ",min=" + min);
            if (str.equalsIgnoreCase("h")) {

                return (day + "天" + (hour - day * 24) + "小时");
            } else {
                return (day + "天" + (hour - day * 24) + "小时" + (min - day * 24 * 60) + "分钟");
            }

        } catch (ParseException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        if (str.equalsIgnoreCase("h")) {
            return (day + "天" + (hour - day * 24) + "小时");
        } else {
            return (day + "天" + (hour - day * 24) + "小时" + (min - day * 24 * 60) + "分钟");
        }
    }

    private boolean isValidDate(String str, String dateFormat) {
        boolean convertSuccess=true;
        // 指定日期格式为四位年/两位月份/两位日期，注意yyyy/MM/dd区分大小写；
        SimpleDateFormat format = new SimpleDateFormat(dateFormat);
        try {
            // 设置lenient为false. 否则SimpleDateFormat会比较宽松地验证日期，比如2007/02/29会被接受，并转换成2007/03/01
            format.setLenient(false);
            format.parse(str);
        } catch (ParseException e) {
            // e.printStackTrace();
            // 如果throw java.text.ParseException或者NullPointerException，就说明格式不对
            convertSuccess=false;
        }
        return convertSuccess;
    }

}
