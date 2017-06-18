package com.corsair.controller;

import com.corsair.device.ElectricPSR;
import com.corsair.device.PhotovoltaicArray;
import com.corsair.entity.ElectricPrice;
import com.corsair.entity.ElectricStationInfo;
import com.corsair.form.BenefitCompanyForm;
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
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Map;

/**
 * Created by libh on 2015/12/24.
 */
@Controller
public class BenefitCompanyController extends BaseController {
    @Autowired
    ComplexBenefitService complexBenefitService;

    @RequestMapping(value = "/benefitCompany")
    public ModelAndView NetElectricity(BenefitCompanyForm benefitCompanyForm,HttpServletRequest request,HttpSession session){
        ModelAndView model;

        Date dt=new Date();
        SimpleDateFormat matter1=new SimpleDateFormat("yyyy/MM/dd");
        String curDate = matter1.format(dt);

        String selDate,stationId;
        String stationName="";
        List<PhotovoltaicArray> stationList = complexBenefitService.getStationList();

        if (CommonUtils.isNotBlank(benefitCompanyForm.getSelectDate())){
            selDate = benefitCompanyForm.getSelectDate();
        }else {
            selDate = curDate.substring(0,7);
            benefitCompanyForm.setSelectDate(selDate);
        }

        if (CommonUtils.isNotBlank(benefitCompanyForm.getStationId())){
            stationId = benefitCompanyForm.getStationId();
            for (PhotovoltaicArray photovoltaicArray : stationList){
                if (photovoltaicArray.getId().equals(stationId)){
                    stationName = photovoltaicArray.getName();
                }
            }
        }
        else {
            stationId = stationList.get(0).getId();
            stationName = stationList.get(0).getName();
        }

        ElectricStationInfo electricStationInfo = complexBenefitService.getStationInfoById(stationId);

        if (CommonUtils.isNotBlank(selDate)) {
            int   selYear   =   Integer.parseInt(selDate.substring(0, 4));
            int   selMonth   =   Integer.parseInt(selDate.substring(5, 7));
            String initDate = selYear + "年" + selMonth + "月";
            String printDate = complexBenefitService.getCurDate();

            List<ElectricPSR> totalElectricList = complexBenefitService.getTotalElectricityDirectByDate(selDate, stationId);

            //自用电费结算
            Double totalCompanyFee = 0.00;
            for (ElectricPSR electricPSR : totalElectricList) {
                totalCompanyFee += electricPSR.getFee();
            }

            totalCompanyFee = complexBenefitService.changeValue(totalCompanyFee);
            String totalFeeUppercase = complexBenefitService.digitUppercase(totalCompanyFee);

            request.setAttribute("stationName", stationName);
            request.setAttribute("electricStationInfo", electricStationInfo);
            request.setAttribute("initDate", initDate);
            request.setAttribute("printDate", printDate);

            request.setAttribute("totalElectricList", totalElectricList);
            request.setAttribute("totalCompanyFee", totalCompanyFee);
            request.setAttribute("totalFeeUppercase", totalFeeUppercase);
        }
        request.setAttribute("stationList", stationList);
        model = new ModelAndView("benefitCompany");
        return model;
    }
}
