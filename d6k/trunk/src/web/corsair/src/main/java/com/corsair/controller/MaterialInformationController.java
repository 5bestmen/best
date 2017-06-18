package com.corsair.controller;

import com.corsair.entity.MaterialInformation;
import com.corsair.form.MaterialInformationForm;
import com.corsair.service.MaterialInformationService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.List;

/**
 * Created by zjq on 2015/11/5.
 */
@Controller
public class MaterialInformationController {
    @Autowired
    MaterialInformationService materialInformationService;
    static List<MaterialInformation> materialInformations;
    private final int DISPLAYNUM = 15;
    @RequestMapping(value = "/materialInformation")
    public ModelAndView materialInformationGetControll(MaterialInformationForm materialInformationForm,
                                                      HttpServletRequest request, HttpServletResponse response){
        ModelAndView model = new ModelAndView("materialInformation");;
        String materialSeriesNo = materialInformationForm.getMaterialSeriesNo();
        String materialId = materialInformationForm.getMaterialId();
        List<MaterialInformation> materialInformationsDisplay = null;
//        int displayNum = 0;
//        int displayPage = 0;
//
//        if (CommonUtils.isNotBlank(materialInformationForm.getNext())){
//            displayPage = Integer.parseInt(materialInformationForm.getCurrentPage()) - 1;
//            displayNum = Integer.parseInt(materialInformationForm.getDisplayNum());
//        }else if (CommonUtils.isNotBlank(materialInformationForm.getPrev())){
//            displayPage = Integer.parseInt(materialInformationForm.getCurrentPage()) - 2;
//            displayNum = Integer.parseInt(materialInformationForm.getDisplayNum());
//        }else{
            materialInformations = materialInformationService.getMaterialInformationsByNoAndId(materialSeriesNo,materialId);
//            displayNum = DISPLAYNUM;
//            materialInformationForm.setCurrentPage(Integer.toString(1));
//        }
//
//        for (int i = displayPage*displayNum; i < materialInformations.size() && i < displayNum*(displayPage+1); i++){
//            materialInformationsDisplay.add(materialInformations.get(i));
//        }
        model.addObject("materialInformations", materialInformationsDisplay);

        return model;
    }
}
