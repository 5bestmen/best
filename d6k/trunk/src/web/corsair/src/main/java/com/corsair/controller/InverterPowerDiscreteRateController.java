package com.corsair.controller;

import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.device.Junction;
import com.corsair.entity.template.psr.PSRTemplateMapping;
import com.corsair.measurement.HistoryDataValue;
import com.corsair.service.HistoryDataService;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Created by ChengXi on 2015/9/29.
 */
@Controller
public class InverterPowerDiscreteRateController {
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;
    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    HistoryDataService historyDataService;

    @RequestMapping(value = "/inverterPowerDiscreteRate", method = RequestMethod.GET)
    public ModelAndView redirect() {
        ModelAndView modelAndView = new ModelAndView("inverterPowerDiscreteRate");
//        List<String> data = getData();
//        modelAndView.addObject("avgs",data);
        return modelAndView;
    }

    public List<String> getData() {

        //计算某一时刻该逆变器下所带组串电流的平均值
        //计算某一时刻该逆变器下所带组串电流的标准差
        List avgs = new ArrayList();
        //获得公式中的X——某个时刻该逆变器下所带汇流箱第i组串的电流
        String dataName = "Idc";
        PSRTemplateMapping psrTemplateMapping = psrTemplateMappingDao.getPSRTemplateMappingById("3#方阵逆变器");
        String inverterPSRId = psrTemplateMapping.getPSRID();
        List<Junction> junctions = scadaPSRService.getChildJunction(inverterPSRId);
        SimpleDateFormat df = new SimpleDateFormat("yyyy/MM/dd");
        String currentTime = df.format(new Date());
        int[] hours = new int[24];
        for (int i = 0; i < 24; i++) {
            hours[i] = i;
        }
        Double sum=0.0;
        int n = junctions.size();//获得N——为该逆变器下所带电池组串的总数量
        Double avg=0.0;
        for (int j = 0; j < hours.length-1; j++) {
            for (int i = 0; i < junctions.size(); i++) {
                String meterPSRID = junctions.get(i).getId();
                //可以获得每一个小时内该逆变器下所带汇流箱第i组串的所有电流值
                List<HistoryDataValue> historyDatas = historyDataService.getHistoryData(meterPSRID, dataName, currentTime, hours[j]*60, hours[j + 1]*60, HistoryDataService.DataType.REALTIME_VALUE);
                for (int k = 0; k < historyDatas.size(); k++) {
                    Double num = historyDatas.get(k).getData();
                    sum = sum+num;
                }
            }
            avg = sum/n;//计算某一时刻该逆变器下所带组串电流的平均值
            avgs.add(avg);
        }

        return avgs;
    }
}
