package com.corsair.controller;

import com.corsair.entity.MaterialStore;
import com.corsair.form.MaterialStoreForm;
import com.corsair.service.MaterialStoreService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.List;

/**
 * Created by zjq on 2015/11/16.
 */
public class materialStoreInEditController  {
    @Autowired
    MaterialStoreService materialStoreService;

    @RequestMapping(value = "/materialStoreInEdit")
    public ModelAndView materialStoreInEditIni(MaterialStoreForm materialStoreForm,
                                                       HttpServletRequest request, HttpServletResponse response){
        ModelAndView model = new ModelAndView("materialStore");
        String materialInfoId = materialStoreForm.getMaterialInfoId();
        List<MaterialStore> materialStores = materialStoreService.getMaterialStoresByInfoId(materialInfoId);
        model.addObject("materialStores", materialStores);

        return model;
    }
}
