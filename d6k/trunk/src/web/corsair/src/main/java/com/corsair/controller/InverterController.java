package com.corsair.controller;

import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.device.Junction;
import com.corsair.device.PackageTransformer;
import com.corsair.entity.DeviceFormat;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.PSRSpec;
import com.corsair.entity.template.psr.PSRTemplateMapping;
import com.corsair.form.InverterForm;
import com.corsair.service.ScadaPSRService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

/**
 * Created by libh on 2015/9/28.
 */
@Controller
public class InverterController {
    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;

    @RequestMapping(value = "/inverter", method = RequestMethod.GET)
    public ModelAndView redirect(InverterForm inverterForm, HttpServletRequest request, HttpSession session) {
        ModelAndView modelAndView = new ModelAndView("inverter");

        //todo,张云,此处的实现需要简化并重新调试
        String id = inverterForm.getInverterID();
        if (id != null) {
            String name = scadaPSRService.getDeivceName(id);
            String arrayID = scadaPSRService.getPVArrayId(id);
            String arrayName = scadaPSRService.getPVArrayName(arrayID);
            List<Junction> childJunctions = scadaPSRService.getChildJunction(id);
            List<PackageTransformer> childTransformers = scadaPSRService.getChildTransformers(id);

            PSRTemplateMapping psrTemplateMapping = psrTemplateMappingDao.getPSRTemplateMapping(id);
//            if (psrTemplateMapping != null){
//                if (CommonUtils.isNotBlank(psrTemplateMapping.getDevEncode())){
//                    request.setAttribute("devEncode", psrTemplateMapping.getDevEncode());
//                }
//            }
            DeviceTemplate deviceTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID(id);
//            if ( deviceTemplate != null) {
//                request.setAttribute("devModel", deviceTemplate.getModel());
//                request.setAttribute("devFactory", deviceTemplate.getFactory());
//                request.setAttribute("deviceFormats", deviceFormats);
//                request.setAttribute("deviceFormatNums", deviceFormatNums);
//            }
//            request.setAttribute("id", id);
//            request.setAttribute("name", name);
//            request.setAttribute("arrayID", arrayID);
//            request.setAttribute("arrayName", arrayName);
//            request.setAttribute("childJunctions", childJunctions);
//            request.setAttribute("childTransformers", childTransformers);

            modelAndView.addObject("device", deviceTemplate);
        }
        return modelAndView;
    }
}
