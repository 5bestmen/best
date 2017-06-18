package simulate.com.corsair.servlet;

import org.springframework.context.ApplicationContext;
import org.springframework.web.context.support.WebApplicationContextUtils;

import javax.servlet.http.HttpServlet;

/**
 * Created by 洪祥 on 15/7/8.
 */
public class BaseServlet extends HttpServlet {
    private ApplicationContext getApplicationContext() {
        return WebApplicationContextUtils.getRequiredWebApplicationContext(this.getServletContext());
    }

    protected Object getBean(String name) {
        return getApplicationContext().getBean(name);
    }
}
