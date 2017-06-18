package com.corsair.controller;

import com.corsair.device.ElectricPSR;
import com.corsair.device.PhotovoltaicArray;
import com.corsair.entity.ElectricPrice;
import com.corsair.entity.ElectricStationInfo;
import com.corsair.form.BenefitComplexForm;
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
 * Created by libh on 2015/12/18.
 */
@Controller
public class BenefitComplexController extends BaseController {

    @Autowired
    ComplexBenefitService complexBenefitService;

    @RequestMapping(value = "/benefitComplex")
    public ModelAndView NetElectricity(BenefitComplexForm benefitComplexForm,HttpServletRequest request,HttpSession session){
        ModelAndView model;

        Date dt=new Date();
        SimpleDateFormat matter1=new SimpleDateFormat("yyyy/MM/dd");
        String curDate = matter1.format(dt);

        String selDate,stationId;
        String stationName="";
        List<PhotovoltaicArray> stationList = complexBenefitService.getStationList();

        if (CommonUtils.isNotBlank(benefitComplexForm.getSelectDate())){
            selDate = benefitComplexForm.getSelectDate();
        }else {
            selDate = curDate.substring(0,7);
            benefitComplexForm.setSelectDate(selDate);
        }

        if (CommonUtils.isNotBlank(benefitComplexForm.getStationId())){
            stationId = benefitComplexForm.getStationId();
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

            List<ElectricPSR> totalElectricList = complexBenefitService.getTotalElectricityDirectByDate(selDate, stationId);

            Map<String, ElectricPrice> nameToElectricPriceMap = complexBenefitService.getNameToElectricPriceMap();
            Double StateSubsidyPrice = (nameToElectricPriceMap.containsKey("国家补贴") ? nameToElectricPriceMap.get("国家补贴").getPrice() : 0.00);
            Double provinceSubsidyPrice = (nameToElectricPriceMap.containsKey("省补贴") ? nameToElectricPriceMap.get("省补贴").getPrice() : 0.00);
            Double citySubsidyPrice = (nameToElectricPriceMap.containsKey("市补贴") ? nameToElectricPriceMap.get("市补贴").getPrice() : 0.00);
            Double countySubsidyPrice = (nameToElectricPriceMap.containsKey("县补贴") ? nameToElectricPriceMap.get("县补贴").getPrice() : 0.00);

            //补贴结算
            List<ElectricPSR> subsidyAccountList = new ArrayList<>();

            Double totalCompanyFee = 0.00;
            Double totalElectricValue = 0.00, totalStateSubsidy = 0.00, totalprovinceSubsidy = 0.00, totalCitySubsidy = 0.00, totalCountySubsidy = 0.00;
            for (ElectricPSR electricPSR : totalElectricList) {
                totalCompanyFee += electricPSR.getFee();

                if (electricPSR.getName().contains("正向 总"))
                    continue;
                Double electricValue = electricPSR.getElectricValue();
                ElectricPSR subsidyElectricPSR = new ElectricPSR();
                subsidyElectricPSR.setName(electricPSR.getName());
                subsidyElectricPSR.setElectricValue(electricValue);
                subsidyElectricPSR.setElectricPrice(complexBenefitService.numberFormat(StateSubsidyPrice * electricValue));
                subsidyElectricPSR.setCurValue(complexBenefitService.numberFormat(provinceSubsidyPrice * electricValue));
                subsidyElectricPSR.setLastValue(complexBenefitService.numberFormat(citySubsidyPrice * electricValue));
                subsidyElectricPSR.setFee(complexBenefitService.numberFormat(countySubsidyPrice * electricValue));
                subsidyAccountList.add(subsidyElectricPSR);

                totalElectricValue += electricValue;
                totalStateSubsidy += (StateSubsidyPrice * electricValue);
                totalprovinceSubsidy += (provinceSubsidyPrice * electricValue);
                totalCitySubsidy += (citySubsidyPrice * electricValue);
                totalCountySubsidy += (countySubsidyPrice * electricValue);
            }

            ElectricPSR subsidyAccount = new ElectricPSR();
            subsidyAccount.setName("合计");
            subsidyAccount.setElectricValue(complexBenefitService.numberFormat(totalElectricValue));
            subsidyAccount.setElectricPrice(complexBenefitService.numberFormat(totalStateSubsidy));
            subsidyAccount.setCurValue(complexBenefitService.numberFormat(totalprovinceSubsidy));
            subsidyAccount.setLastValue(complexBenefitService.numberFormat(totalCitySubsidy));
            subsidyAccount.setFee(complexBenefitService.numberFormat(totalCountySubsidy));
            subsidyAccountList.add(subsidyAccount);

            Double totalSubsidyFee = totalStateSubsidy + totalprovinceSubsidy + totalCitySubsidy + totalCountySubsidy;
            totalNetElectricFee = complexBenefitService.changeValue(totalNetElectricFee);
            totalCompanyFee = complexBenefitService.changeValue(totalCompanyFee);
            totalSubsidyFee = complexBenefitService.changeValue(totalSubsidyFee);
            Double totalFee = totalNetElectricFee + totalCompanyFee + totalSubsidyFee;
            totalFee = complexBenefitService.changeValue(totalFee);

            String totalFeeUppercase = complexBenefitService.digitUppercase(totalFee);

            request.setAttribute("stationName", stationName);
            request.setAttribute("electricStationInfo", electricStationInfo);
            request.setAttribute("initDate", initDate);
            request.setAttribute("printDate", printDate);

            request.setAttribute("netElectricList", netElectricList);
            request.setAttribute("netElectricAccount", netElectricAccount);
            request.setAttribute("totalNetElectricFee", totalNetElectricFee);
            request.setAttribute("totalElectricList", totalElectricList);
            request.setAttribute("totalCompanyFee", totalCompanyFee);
            request.setAttribute("subsidyAccountList", subsidyAccountList);
            request.setAttribute("totalSubsidyFee", totalSubsidyFee);
            request.setAttribute("totalFee", totalFee);

            request.setAttribute("StateSubsidyPrice", StateSubsidyPrice);
            request.setAttribute("provinceSubsidyPrice", provinceSubsidyPrice);
            request.setAttribute("citySubsidyPrice", citySubsidyPrice);
            request.setAttribute("countySubsidyPrice", countySubsidyPrice);
            request.setAttribute("totalFeeUppercase", totalFeeUppercase);
        }
        request.setAttribute("stationList", stationList);
        model = new ModelAndView("benefitComplex");
        return model;
    }
}
