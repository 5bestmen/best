package com.corsair.controller;

import com.corsair.form.DisplayAreaForm;
import com.corsair.service.DisplayAreaService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.List;

/**
 * Created by 洪祥 on 2015/12/12.
 */
@Controller
public class DisplayAreaController {
    @Autowired
    DisplayAreaService displayAreaService ;

    @RequestMapping(value ="/displayArea")
    public ModelAndView displayAreaControl(DisplayAreaForm displayAreaForm, HttpServletRequest request,HttpServletResponse response){
        ModelAndView model = new ModelAndView("/displayArea");
        if(CommonUtils.isNotBlank(displayAreaForm.getDel())){
            displayAreaService.deleteDisplayArea(displayAreaForm.getDeleteIds());
        }
        List displayAreaList = displayAreaService.getDisplayArea(displayAreaForm.getName(), displayAreaForm.getComments());
        model.addObject("displayAreaList", displayAreaList);
        return model;
    }
}
