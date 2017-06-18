package com.corsair.taglib;
import com.corsair.entity.*;
import com.corsair.service.DisplayAreaService;
import com.corsair.service.ScadaMeasurementService;

import javax.servlet.jsp.JspException;
import java.io.IOException;
import java.util.List;

/**
 * Created by 洪祥 on 15/7/11.
 */
public class MeasurementTag extends BaseTagSupport {
    //这两项是需要传入的参数
    private String displayAreaName; //显示区域
    private String PSRID; //电力系统资源ID

    public void setPSRID(String PSRID) {
        this.PSRID = PSRID;
    }

    public void setDisplayAreaName(String displayAreaName) {
        this.displayAreaName = displayAreaName;
    }

    public int doStartTag() throws JspException {
        DisplayAreaService displayAreaService = (DisplayAreaService)super.getBean("displayAreaService");

        try {
            this.pageContext.getOut().print(displayAreaService.getTagString(displayAreaName, PSRID));
        } catch (IOException e) {
            e.printStackTrace();
        }

        return SKIP_BODY;
    }


}
