package com.rbac.common;

import javax.servlet.http.HttpServletRequest;

/**
 * Created by chris on 2015/9/29.
 */
public class BaseController {
    /**
     * 得到当前登录用户id
     * @param request
     * @return
     */
    protected Long getCurrentAccountId(HttpServletRequest request){
        UserDetail userDetail = this.getCurrentUserDetail(request);
        if(userDetail!=null && userDetail.getAccount()!=null){
            return userDetail.getAccount().getId();
        }
        return null;
    }

    /**
     * 得到当前登录用户信息
     * 可以使用 ThreadLocal 管理用户session
     * 参考http://blog.csdn.net/nan_jiang_/article/details/5055221
     * @param request
     * @return
     */
    protected UserDetail getCurrentUserDetail(HttpServletRequest request){
        UserDetail userDetail = (UserDetail)request.getSession(true).getAttribute(MvcConstant.USER);
        if(userDetail!=null){
            return userDetail;
        }
        return null;
    }
}
