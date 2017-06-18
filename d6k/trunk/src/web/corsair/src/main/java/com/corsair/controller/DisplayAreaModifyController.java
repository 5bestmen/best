package com.corsair.controller;

import com.corsair.entity.template.display.DisplayArea;
import com.corsair.form.DisplayAreaModifyForm;
import com.corsair.service.DisplayAreaService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Created by 洪祥 on 2015/12/15.
 */
@Controller
public class DisplayAreaModifyController {
    @Autowired
    DisplayAreaService displayAreaService ;

    @RequestMapping(value ="/displayAreaModify", method = RequestMethod.GET)
    public ModelAndView displayAreaModifyGetControl(DisplayAreaModifyForm displayAreaModifyForm,
                                              HttpServletRequest request,HttpServletResponse response){

        ModelAndView model = new ModelAndView("/displayAreaModify");

        if(CommonUtils.isNotBlank(displayAreaModifyForm.getId())){

            String id = displayAreaModifyForm.getId();
            DisplayArea displayArea = displayAreaService.getDisplayAreaById(id);
            displayAreaModifyForm.setName(displayArea.getName());
            displayAreaModifyForm.setComments(displayArea.getComments());
        }
        return model;
    }

    @RequestMapping(value ="/displayAreaModify", method = RequestMethod.POST)
    public ModelAndView displayAreaModifyPostControl(DisplayAreaModifyForm displayAreaModifyForm,
                                                       HttpServletRequest request,HttpServletResponse response){
        //保存
        if (CommonUtils.isNotBlank(displayAreaModifyForm.getSubmit())){
            String id = displayAreaModifyForm.getId();
            if (CommonUtils.isNotBlank(id)){    //修改区域
                DisplayArea displayArea = displayAreaService.getDisplayAreaById(id);
                displayArea.setName(displayAreaModifyForm.getName());
                displayArea.setComments(displayAreaModifyForm.getComments());
                displayAreaService.saveOrUpdateDisplayArea(displayArea);
            }else { //新增区域
                DisplayArea displayArea = new DisplayArea();
                displayArea.setName(displayAreaModifyForm.getName());
                displayArea.setComments(displayAreaModifyForm.getComments());
                displayAreaService.saveOrUpdateDisplayArea(displayArea);
            }
        }

        return new ModelAndView("redirect:/displayArea");
    }
}
