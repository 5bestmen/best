package com.corsair.controller;

import com.corsair.entity.NetElectricity;
import com.corsair.form.NetElectricityForm;
import com.corsair.service.ComplexBenefitService;
import com.rbac.common.BaseController;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.util.List;

/**
 * Created by libh on 2015/12/17.
 */
@Controller
public class NetElectricityController extends BaseController {
    @Autowired
    ComplexBenefitService complexBenefitService;

    @RequestMapping(value = "/netElectricity", method = RequestMethod.GET)
         public ModelAndView NetElectricity(NetElectricityForm netElectricityForm,HttpServletRequest request,HttpSession session){
        ModelAndView model;
        List stationList = complexBenefitService.getStationList();
        request.setAttribute("stationList", stationList);

        model = new ModelAndView("netElectricity");
        return model;
    }
    @RequestMapping(value = "/netElectricity", method = RequestMethod.POST)
    public ModelAndView NetElectricityNet(NetElectricityForm netElectricityForm,HttpServletRequest request,HttpSession session){
        ModelAndView model = new ModelAndView();
        if(CommonUtils.isNotBlank(netElectricityForm.getSubmit())){
            NetElectricity netElectricity = complexBenefitService.getNetElectricityByDate(netElectricityForm.getNetDate(), netElectricityForm.getStationId());
            if (netElectricity == null){
                netElectricity = new NetElectricity();
                netElectricity.setNetDate(netElectricityForm.getNetDate());
            }
            netElectricity.setTotalKwh(Double.parseDouble(netElectricityForm.getTotalKwh()));
            netElectricity.setPeakKwh(Double.parseDouble(netElectricityForm.getPeakKwh()));
            netElectricity.setNormalKwh(Double.parseDouble(netElectricityForm.getNormalKwh()));
            netElectricity.setValleyKwh(Double.parseDouble(netElectricityForm.getValleyKwh()));
            netElectricity.setReverseTotalKwh(Double.parseDouble(netElectricityForm.getReverseTotalKwh()));
            netElectricity.setTotalKVar(Double.parseDouble(netElectricityForm.getTotalKVar()));
            netElectricity.setMultiple(Integer.parseInt(netElectricityForm.getMultiple()));
            netElectricity.setStation(netElectricityForm.getStationId());
            complexBenefitService.saveNetElectricity(netElectricity);
        }
        netElectricityForm.setTotalKwh("");
        netElectricityForm.setNormalKwh("");
        netElectricityForm.setPeakKwh("");
        netElectricityForm.setValleyKwh("");
        netElectricityForm.setReverseTotalKwh("");
        netElectricityForm.setTotalKVar("");
        netElectricityForm.setMultiple("");

        List stationList = complexBenefitService.getStationList();
        request.setAttribute("stationList", stationList);

        model.setViewName("netElectricity");
        return model;
    }
}
