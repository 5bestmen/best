package com.corsair.controller;

import com.corsair.entity.template.display.DisplayTemplate;
import com.corsair.entity.template.display.DisplayTemplateData;
import com.corsair.form.DisplayTemplateDataForm;
import com.corsair.service.DisplayTemplateDataService;
import com.corsair.service.DisplayTemplateService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.List;

/**
 * Created by 洪祥 on 2016/1/7.
 */
@Controller
public class DisplayTemplateDataController {
    @Autowired
    DisplayTemplateDataService displayTemplateDataService ;

    @Autowired
    DisplayTemplateService displayTemplateService;

    @RequestMapping(value ="/displayTemplateData")
    public ModelAndView displayTemplateDataControl(DisplayTemplateDataForm displayTemplateDataForm,
                                                  HttpServletRequest request,HttpServletResponse response){

        ModelAndView model = new ModelAndView("/displayTemplateData");

        if(CommonUtils.isNotBlank(displayTemplateDataForm.getDel())){

            //首先通过即将被删除的模板数据，找到它所对应的显示模板
            DisplayTemplate displayTemplate = displayTemplateDataService.getDisplayTemplateDataById(displayTemplateDataForm.getDeleteIds()).getDisplayTemplate();
            String displayTemplateId = displayTemplate.getId();
            ModelAndView modelAndView = new ModelAndView("redirect:/displayTemplateData?id=" + displayTemplateId);

            displayTemplateDataService.deleteDisplayTemplateData(displayTemplateDataForm.getDeleteIds());

            return modelAndView;
        }

        if (CommonUtils.isNotBlank(displayTemplateDataForm.getId())){

            List displayTemplateDataList = displayTemplateDataService.getDisplayTemplateDatas(displayTemplateDataForm.getId());
            String titleDesc = displayTemplateService.getDisplayTemplateById(displayTemplateDataForm.getId()).getDesc();

            //显示模板有数据
            if(displayTemplateDataList.size() > 0){
                //按照索引对模板数据进行升序排序
                sortByIndex(displayTemplateDataList);
            }

            model.addObject("displayTemplateDataList", displayTemplateDataList);
            model.addObject("titleDesc", titleDesc + "-");

        }

        return model;
    }

    //按照数据索引对显示模板数据进行升序排序
    private void sortByIndex(List<DisplayTemplateData> displayTemplateDataList) {

        for (int i = 0; i < displayTemplateDataList.size(); i++) {

            for (int j = i + 1; j < displayTemplateDataList.size(); j++) {

                int frontIndex= displayTemplateDataList.get(i).getIndex();
                int behindIndex = displayTemplateDataList.get(j).getIndex();

                if (frontIndex > behindIndex) {
                    DisplayTemplateData temp = displayTemplateDataList.get(i);
                    displayTemplateDataList.set(i, displayTemplateDataList.get(j));
                    displayTemplateDataList.set(j, temp);
                }
            }
        }
    }
}
