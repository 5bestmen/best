package com.eventsrv;

import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;

import java.io.IOException;
import java.util.Set;
import java.util.concurrent.CopyOnWriteArraySet;

/**
 * Created by ChengXi on 2015/12/16.
 */
public class SessionMgr {

    private static class SessionMgrHolder {

        private static final SessionMgr INSTANCE = new SessionMgr();

    }

    private SessionMgr (){}

    public static final SessionMgr getInstance() {

        return SessionMgrHolder.INSTANCE;

    }

    private Set<WebSocketSession> connections = new CopyOnWriteArraySet<>();

    public void addBrowser(WebSocketSession client) {
        connections.add(client);
    }

    public void removeBrowser(WebSocketSession client) {
        connections.remove(client);
    }

    public void broadcast(TextMessage msg) {
        //System.out.print(connections.size()+"\n");
        for (WebSocketSession client : connections) {
            try {
                synchronized (client) {
                    client.sendMessage(msg);
                }
            } catch (IOException e) {
                connections.remove(client);
                try {
                    client.close();
                } catch (IOException e1) {
                    // Ignore
                }
            }
        }
    }
}
