package com.corsair.controller;

import com.corsair.entity.*;
import com.corsair.form.MaterialModifyForm;
import com.corsair.service.MaterialService;
import com.rbac.common.BaseController;

import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.util.List;


/**
 * Created by libh on 2015/10/27.
 */
@Controller
public class MaterialModifyController extends BaseController {
    @Autowired
    MaterialService materialService;

    @RequestMapping(value= "/materialModify", method= RequestMethod.GET)
    public ModelAndView ActionModify(MaterialModifyForm materialModifyForm,HttpServletRequest request,HttpSession session) throws UnsupportedEncodingException {
        ModelAndView model;
        model = new ModelAndView("materialModify");

        String mType = materialModifyForm.getmType();
        model.addObject("material_Type", mType);
        if (CommonUtils.isBlank(mType)){return model;}

       if(mType.equals("materialModel")){
           List typeList = materialService.getMaterialTypes();
           List modelLableList = materialService.getAllModelLableList();

           model.addObject("typeList", typeList);
           model.addObject("modelLableList", modelLableList);

           if (CommonUtils.isNotBlank(materialModifyForm.getEditId())){
               MaterialModel materialModel = materialService.getMaterialModelById(materialModifyForm.getEditId());
               materialModifyForm.setCodeId(materialModel.getCodeID());
               materialModifyForm.setMaterialTypeId(materialModel.getMaterialType().getMaterialTypeID());
               String modelLabelSelList = URLDecoder.decode(materialModifyForm.getMaterialModelLables(), "UTF-8");
               model.addObject("modelLabelSelList", modelLabelSelList);
               model.addObject("nodeIdSelect", materialModifyForm.getNodeId());

           }
       }
        else {
           List extendLableList = materialService.getAllExtendLableList();
           model.addObject("extendLableList", extendLableList);

           if(CommonUtils.isNotBlank(materialModifyForm.getId())){
               MaterialModel materialModel = materialService.getMaterialModelById(materialModifyForm.getId());
               List modelList = materialService.getMaterialModelList(materialModel.getMaterialType().getMaterialTypeID());
               model.addObject("modelList", modelList);
           }

           if (CommonUtils.isNotBlank(materialModifyForm.getEditId())){
               Material material = materialService.getMaterialById(materialModifyForm.getEditId());
               materialModifyForm.setFixedId(material.getFixedID());
               materialModifyForm.setMaterialModelId(material.getMaterialModel().getMaterialModelID());
               String extendLabelSelList = URLDecoder.decode(materialModifyForm.getMaterialExtendLables(), "UTF-8");
               model.addObject("extendLabelSelList", extendLabelSelList);
           }
       }
        return model;
    }

    @RequestMapping(value= "/materialModify", method = RequestMethod.POST)
    public ModelAndView ActionModifyAction(MaterialModifyForm materialModifyForm,HttpServletRequest request,HttpSession session) {
        ModelAndView model;
        if(CommonUtils.isNotBlank(materialModifyForm.getSubmit())){
            if(materialModifyForm.getmType().equals("materialModel")){
                MaterialModel materialModel = new MaterialModel();
                if (CommonUtils.isNotBlank(materialModifyForm.getEditId()))
                {
                    materialModel = materialService.getMaterialModelById(materialModifyForm.getEditId());
                }
                materialModel.setCodeID(materialModifyForm.getCodeId());
                if(CommonUtils.isNotBlank(materialModifyForm.getMaterialTypeId())){
                    MaterialType materialType = materialService.getMaterialTypeById(materialModifyForm.getMaterialTypeId());
                    materialModel.setMaterialType(materialType);
                }
                String label;
                if(CommonUtils.isNotBlank(materialModifyForm.getMaterialModelLables())){
                    label = materialModifyForm.getMaterialModelLables() + "," +materialModifyForm.getAddModelLables();
                }else {
                    label = materialModifyForm.getAddModelLables();
                }
                materialModel.setModelLables(label);
                materialService.saveOrUpdateMaterialModel(materialModel);
            }
            else {
                Material material = new Material();
                if (CommonUtils.isNotBlank(materialModifyForm.getEditId()))
                {
                    material = materialService.getMaterialById(materialModifyForm.getEditId());
                }
                material.setFixedID(materialModifyForm.getFixedId());
                if(CommonUtils.isNotBlank(materialModifyForm.getMaterialModelId())){
                    MaterialModel materialModel = materialService.getMaterialModelById(materialModifyForm.getMaterialModelId());
                    material.setMaterialModel(materialModel);
                }
                String label;
                if(CommonUtils.isNotBlank(materialModifyForm.getMaterialExtendLables())){
                    label = materialModifyForm.getMaterialExtendLables() + "," +materialModifyForm.getAddExtendLables();
                }else {
                    label = materialModifyForm.getAddExtendLables();
                }
                material.setExtendLabels(label);

                materialService.saveOrUpdateMaterial(material);

            }
            session.setAttribute("nodeSelected", materialModifyForm.getNodeId());

            model = new ModelAndView("redirect:/materialSort");
            return model;
        }
        model = new ModelAndView("materialModify");
        return model;
    }
}
