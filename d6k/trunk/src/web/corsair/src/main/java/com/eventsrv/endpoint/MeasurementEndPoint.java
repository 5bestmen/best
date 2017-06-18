package com.eventsrv.endpoint;

import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;

/**
 * Created by ChengXi on 2015/12/11.
 */
public class MeasurementEndPoint extends TextWebSocketHandler {


    @Override
    protected void handleTextMessage(WebSocketSession session,TextMessage message) throws Exception {
        super.handleTextMessage(session, message);
        //TextMessage returnMessage = new TextMessage(message.getPayload()+" received at server");
        //SessionMgr.getInstance().broadcast(message);

    }

    @Override
    public void afterConnectionEstablished(WebSocketSession session)
            throws Exception {
        //SessionMgr.getInstance().addBrowser(session);
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session,
                                      CloseStatus status)
            throws Exception {
        //SessionMgr.getInstance().removeBrowser(session);
    }
}
