package com.rbac.controller.system;

import com.corsair.service.ApplicationInformationService;
import com.rbac.common.MvcConstant;
import com.rbac.common.UserDetail;
import com.rbac.form.system.LoginForm;
import com.rbac.service.LoginService;
import com.rbac.util.CommonUtils;
import com.rbac.util.PasswordHash;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;
import sun.misc.BASE64Encoder;
import sun.security.provider.MD5;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;

/**
 * Created by chris on 2015/9/23.
 */
@Controller
public class LoginController {
    @Autowired
    LoginService loginService;
    @Autowired
    ApplicationInformationService applicationInformationService;
    @RequestMapping(value = "/login", method = RequestMethod.GET)
    public String LoginRedirect(HttpServletRequest request,HttpServletResponse response){
        HttpSession session = request.getSession();
        Cookie Cookie = new Cookie("user", null);
        Cookie.setMaxAge(0); //使cookie失效
        Cookie.setPath("/");   //这个不能少
        response.addCookie(Cookie);
        session.invalidate();
        return "login";
    }

    @RequestMapping(value = "/logout",method = RequestMethod.GET)
    public String LogoutRedirect(HttpServletRequest request,HttpServletResponse response){
        HttpSession session = request.getSession();
        session.invalidate();
        Cookie Cookie = new Cookie("user", null);
        Cookie.setMaxAge(0); //使cookie失效
        Cookie.setPath("/");   //这个不能少
        response.addCookie(Cookie);
        return "login";
    }

    @RequestMapping(value = "/login", method = RequestMethod.POST)
    public ModelAndView LoginAction(LoginForm loginForm, HttpServletRequest request,HttpServletResponse response, HttpSession session){
        ModelAndView model;

        if (CommonUtils.isNotBlank(loginForm.getPassword())) {
            UserDetail userDetail = loginService.login(loginForm.getUsername(), loginForm.getPassword());
            String saveLogin = loginForm.getRemember();
            if (userDetail != null) {
                session.setAttribute(MvcConstant.USER, userDetail);
                session.setAttribute("APP_NAME", applicationInformationService.getAppName());
                session.setAttribute("STATION_NAME",applicationInformationService.getStationName());
                session.setAttribute("STATION_CAPACITY",applicationInformationService.getStationCapacity());
                session.setAttribute("POWER_GRID_CAPACITY",applicationInformationService.getPowerGridCapacity());
                session.setAttribute("RUNNING_TIME",applicationInformationService.getRunningTime());
                session.setAttribute("MENU_WAY",applicationInformationService.getMenuWay());
                //session.setAttribute("username",loginForm.getUsername());
                Cookie cookie = null;
                //如果选择保存登陆信息
                if ("Remember Me".equals(saveLogin)) {
                    cookie = new Cookie("user", loginForm.getUsername()+"=="+loginForm.getPassword());
                    cookie.setPath("/"); //cookie路径问题，在我的其他文章里有专门的讲解
                    cookie.setMaxAge(60 * 60 * 24 );//有效期1天
                    response.addCookie(cookie);
                    saveLogin = null;
                }else{
                    cookie = new Cookie("user", null);
                    cookie.setPath("/"); //cookie路径问题，在我的其他文章里有专门的讲解
                    cookie.setMaxAge(0);
                    response.addCookie(cookie);
                    saveLogin = null;
                }
                // admin存入到session
                //request.getSession().setAttribute("user", userDetail);
                model = new ModelAndView("redirect:/mainIndex");
//                model = new ModelAndView("redirect:/index-snaker");
                return model;
            }
        }
        model = new ModelAndView("login");
        model.addObject(MvcConstant.ERROR_MSG, "用户名或密码错误！");
        return model;
    }

}
