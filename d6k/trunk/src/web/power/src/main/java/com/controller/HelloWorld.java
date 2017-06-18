package com.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.boot.builder.SpringApplicationBuilder;
import org.springframework.boot.web.support.SpringBootServletInitializer;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by zyj on 2017/5/19.
 */
@Controller
@EnableAutoConfiguration
public class HelloWorld {

    @RequestMapping("/")
    public String test(){
        return "/index";
    }

    @RequestMapping("/index")
    public String index(){
        return "/index";
    }

    @RequestMapping("/helloHtml")
    public String helloHtml(Map<String,Object> map){

        map.put("hello","from TemplateController.helloHtml");
        return"/zyj";
    }
}
