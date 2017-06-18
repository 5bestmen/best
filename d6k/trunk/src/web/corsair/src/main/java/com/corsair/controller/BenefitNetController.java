package com.corsair.controller;

import com.corsair.device.ElectricPSR;
import com.corsair.device.PhotovoltaicArray;
import com.corsair.entity.ElectricPrice;
import com.corsair.entity.ElectricStationInfo;
import com.corsair.form.BenefitNetForm;
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

/**
 * Created by libh on 2015/12/24.
 */
@Controller
public class BenefitNetController extends BaseController {
    @Autowired
    ComplexBenefitService complexBenefitService;

    @RequestMapping(value = "/benefitNet")
    public ModelAndView NetElectricity(BenefitNetForm benefitNetForm,HttpServletRequest request,HttpSession session){
        ModelAndView model;

        Date dt=new Date();
        SimpleDateFormat matter1=new SimpleDateFormat("yyyy/MM/dd");
        String curDate = matter1.format(dt);

        String selDate,stationId;
        String stationName="";
        List<PhotovoltaicArray> stationList = complexBenefitService.getStationList();

        if (CommonUtils.isNotBlank(benefitNetForm.getSelectDate())){
            selDate = benefitNetForm.getSelectDate();
        }else {
            selDate = curDate.substring(0,7);
            benefitNetForm.setSelectDate(selDate);
        }

        if (CommonUtils.isNotBlank(benefitNetForm.getStationId())){
            stationId = benefitNetForm.getStationId();
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

            List<ElectricPSR> netElectricList = complexBenefitService.getNetElectricAccountByDate(selDate, stationId);
            Double totalNetElectricFee = 0.00;
            List<ElectricPSR> netElectricAccount = new ArrayList<>();
            for (ElectricPSR electricPSR : netElectricList) {
                if(!electricPSR.getName().contains("反向 总"))
                    continue;
                totalNetElectricFee += electricPSR.getFee();
                netElectricAccount.add(electricPSR);
            }

            totalNetElectricFee = complexBenefitService.changeValue(totalNetElectricFee);
            String totalFeeUppercase = complexBenefitService.digitUppercase(totalNetElectricFee);

            request.setAttribute("stationName", stationName);
            request.setAttribute("electricStationInfo", electricStationInfo);
            request.setAttribute("initDate", initDate);
            request.setAttribute("printDate", printDate);

            request.setAttribute("netElectricAccount", netElectricAccount);
            request.setAttribute("netElectricList", netElectricList);
            request.setAttribute("totalNetElectricFee", totalNetElectricFee);
            request.setAttribute("totalFeeUppercase", totalFeeUppercase);
        }
        request.setAttribute("stationList", stationList);
        model = new ModelAndView("benefitNet");
        return model;
    }
}
