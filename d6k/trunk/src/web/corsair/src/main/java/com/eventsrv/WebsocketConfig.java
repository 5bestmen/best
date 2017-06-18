package com.eventsrv;

import com.eventsrv.endpoint.EventEndPoint;
import com.eventsrv.endpoint.MeasurementEndPoint;
import com.eventsrv.endpoint.RunTimeEndPoint;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.socket.WebSocketHandler;
import org.springframework.web.socket.config.annotation.EnableWebSocket;
import org.springframework.web.socket.config.annotation.WebSocketConfigurer;
import org.springframework.web.socket.config.annotation.WebSocketHandlerRegistry;

/**
 * Created by ChengXi on 2015/12/14.
 */
@Configuration
@EnableWebSocket
public class WebsocketConfig implements WebSocketConfigurer {
    @Override
    public void registerWebSocketHandlers(WebSocketHandlerRegistry registry) {
        // 事项告警服务URL
        registry.addHandler(eventHandler(), "/event")
                .setAllowedOrigins("*")
                .addInterceptors(eventInterceptors());

        // 预留为实时量测服务URL
        registry.addHandler(measurementHandler(), "/measurement")
                .setAllowedOrigins("*");

        // 并网运行时间
        registry.addHandler(runtimeHandler(), "/runtime")
                .setAllowedOrigins("*");


    }

    @Bean
    public WebSocketHandler eventHandler() {
        return new EventEndPoint();
    }

    @Bean
    public WebSocketHandler measurementHandler() {
        return new MeasurementEndPoint();
    }

    @Bean
    public WebSocketHandler runtimeHandler() {
        return new RunTimeEndPoint();
    }

    @Bean
    public HandshakeInterceptor eventInterceptors() {
        return new HandshakeInterceptor();
    }
}
