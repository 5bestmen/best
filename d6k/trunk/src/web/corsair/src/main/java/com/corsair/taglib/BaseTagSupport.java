package com.corsair.taglib;

import org.springframework.context.ApplicationContext;
import org.springframework.web.context.support.WebApplicationContextUtils;

import javax.servlet.jsp.tagext.TagSupport;

/**
 * Created by 洪祥 on 15/7/11.
 */
public class BaseTagSupport extends TagSupport{
    private ApplicationContext getApplicationContext() {
        return WebApplicationContextUtils.getRequiredWebApplicationContext(pageContext.getServletContext());
    }
    protected Object getBean(String name) {
        return getApplicationContext().getBean(name);
    }
}
