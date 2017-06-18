package com.rbac.Interceptor;

import com.rbac.common.MvcConstant;
import com.rbac.common.UserDetail;
import com.rbac.util.CommonUtils;
import org.springframework.stereotype.Repository;
import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.util.Set;

/**
 * Created by chris on 2015/9/23.
 */
@Repository
public class LoginInterceptor extends HandlerInterceptorAdapter {
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response,
                             Object handler) throws Exception {
        // 获得请求路径的uri
        String uri = request.getRequestURI();
        HttpSession session = request.getSession();

        // 如果没有登录，进行提示并返回到错误页面
        if (request.getSession() != null && request.getSession().getAttribute(MvcConstant.USER) == null) {
            request.setAttribute(MvcConstant.ERROR_MSG, "请先登录系统");
            response.sendRedirect(request.getContextPath() + "/login");
            return false;
        }

        //检查登录用户是否有权限访问该路径
        UserDetail user = (UserDetail)session.getAttribute(MvcConstant.USER);
        Set<String> permitActionSet = user.getPermitActionSet();
        if(CommonUtils.isBlank(uri) || !permitActionSet.contains(uri.substring(1))){
            request.setAttribute(MvcConstant.ERROR_MSG, "你没有权限访问该功能");
            response.sendRedirect(request.getContextPath() + "/login");
            return false;
        }

        // 其他情况判断session中是否有key，有的话继续用户的操作
        if(request.getSession() != null && request.getSession().getAttribute(MvcConstant.USER) != null) {
            return true;
        }

        return false;
    }
}
