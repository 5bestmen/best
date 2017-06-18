package com.rbac.controller.system;

import com.rbac.common.UserDetail;
import com.rbac.service.LoginService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.context.support.WebApplicationContextUtils;
import org.springframework.web.context.support.XmlWebApplicationContext;

import javax.servlet.*;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;

/**
 * Created by Administrator on 2016/2/19.
 */
public class LoginFilter implements Filter {
    private LoginService loginService;
    public void destroy() {
        // TODO Auto-generated method stub

    }

    public void doFilter(ServletRequest req, ServletResponse res, FilterChain chain) throws IOException, ServletException {

        // 造型对象
        HttpServletRequest request = (HttpServletRequest) req;
        HttpServletResponse response = (HttpServletResponse) res;
        String flag = request.getParameter("flag");
        if(null!=flag&&"0".equals(flag)){
            // 跳转到成功登录的界面
            request.getRequestDispatcher("/login").forward(request, response);
            return;
        }
        // 1.首先判断sesion中有没有admin
//        Object object = request.getSession().getAttribute("user");
//        // 如果session中有用户
//        if (object != null) {
//            // 跳转到成功登录的界面
//            request.getRequestDispatcher("/mainIndex").forward(request, response);
//            return;
//        }

        Cookie[] cookies = request.getCookies();
        String[] cooks = null;
        String username = null;
        String password = null;
        if (cookies != null) {
            for (Cookie coo : cookies) {
                String aa = coo.getValue();
                cooks = aa.split("==");
                if (cooks.length == 2) {
                    username = cooks[0];
                    password = cooks[1];
                }
            }
        }
        UserDetail userDetail = null;
        if (username!=null&&password!=null){
            userDetail = loginService.login(username, password);
        }
        if (userDetail!=null) {
            //request.getSession().setAttribute("user", userDetail);
            response.sendRedirect("/mainIndex");
            //request.getRequestDispatcher("/main.jsp").forward(request, response);
        } else {
            chain.doFilter(request, response);
        }

    }

    public void init(FilterConfig arg0) throws ServletException {
        // TODO Auto-generated method stub
        ServletContext sc = arg0.getServletContext();
        XmlWebApplicationContext cxt = (XmlWebApplicationContext) WebApplicationContextUtils.getWebApplicationContext(sc);
        if(cxt != null && cxt.getBean("loginService") != null && loginService == null)
            loginService = (LoginService) cxt.getBean("loginService");
    }
}
