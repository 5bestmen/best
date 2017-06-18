package com.eventsrv.endpoint;

import com.alibaba.fastjson.JSON;
import com.eventsrv.Constant;
import com.eventsrv.SessionMgr;
import com.eventsrv.entity.SysEvent;
import com.eventsrv.service.EventFilterService;
import com.eventsrv.service.EventService;
import org.apache.commons.collections4.MapUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;

import java.util.List;

/**
 * Created by ChengXi on 2015/12/15.
 */
public class EventEndPoint extends TextWebSocketHandler {

    @Autowired
    private EventService eventService;

    @Autowired
    private EventFilterService eventFilterService;

    @Override
    protected void handleTextMessage(WebSocketSession session,TextMessage message) throws Exception {
        super.handleTextMessage(session, message);

        int type = MapUtils.getInteger(session.getAttributes(), "type");

        if (type == Constant.TYPE_EVENT_SRV) { // eventsrv发过来的消息存盘后进行转发
            String str = message.getPayload();
            try {
                SysEvent se = JSON.parseObject(str, SysEvent.class);
                if(!eventFilterService.isFilter(se.getType())) {
                    eventService.saveEvent(se);
                    SessionMgr.getInstance().broadcast(new TextMessage(JSON.toJSONString(se)));
                }
            } catch (Exception e) {
                e.getMessage();
            }


        } else if (type == Constant.TYPE_BROWSER) { // 浏览器端发过来的消息进行已读存盘处理

            String str = message.getPayload();
            try {
                SysEvent se = JSON.parseObject(str, SysEvent.class);
                se.setRead((short)1);
                eventService.updateEvent(se);
            } catch (Exception e) {
                e.getMessage();
            }
        }
    }

    @Override
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {

        session.getAttributes();
        int type = MapUtils.getInteger(session.getAttributes(), "type");
        if (type == Constant.TYPE_BROWSER) {
            SessionMgr.getInstance().addBrowser(session);

            // 发送未读消息
            List<SysEvent> list = eventService.getUnRead();
            for (SysEvent se : list) {
                if (se.getObject()==null) {
                    se.setObject("");
                }
                session.sendMessage(new TextMessage(JSON.toJSONString(se)));
            }
        }

    }

    @Override
    public void afterConnectionClosed(WebSocketSession session,
                                      CloseStatus status) throws Exception {
        SessionMgr.getInstance().removeBrowser(session);
    }

}
