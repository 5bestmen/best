package com.eventsrv;

import org.springframework.http.server.ServerHttpRequest;
import org.springframework.http.server.ServerHttpResponse;
import org.springframework.http.server.ServletServerHttpRequest;
import org.springframework.web.socket.WebSocketHandler;
import org.springframework.web.socket.server.support.HttpSessionHandshakeInterceptor;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;

/**
 * Created by ChengXi on 2015/12/11.
 */
public class HandshakeInterceptor extends HttpSessionHandshakeInterceptor {

    @Override
    public boolean beforeHandshake(ServerHttpRequest request,
                                   ServerHttpResponse response, WebSocketHandler wsHandler,
                                   Map<String, Object> attributes) throws Exception {

        ServletServerHttpRequest servletsrvRequest = (ServletServerHttpRequest) request;
        HttpServletRequest httpsrvRequest = servletsrvRequest.getServletRequest();
        String type = httpsrvRequest.getParameter("type");
        if (type != null) {
            attributes.put("type", type);
        } else {
            attributes.put("type", "0");
        }

        //System.out.println("Before Handshake");
        return super.beforeHandshake(request, response, wsHandler, attributes);
    }

    @Override
    public void afterHandshake(ServerHttpRequest request,
                               ServerHttpResponse response, WebSocketHandler wsHandler,
                               Exception ex) {
        //System.out.println("After Handshake");
        super.afterHandshake(request, response, wsHandler, ex);
    }
}
