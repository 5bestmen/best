package com.corsair.api;

import com.corsair.device.BasePSR;
import com.corsair.entity.template.psr.*;
import com.corsair.service.DeviceTemplateService;
import com.corsair.service.ScadaPSRService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.*;

@RestController
@RequestMapping(value = "/psr")
public class ScadaPSRController {
    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    DeviceTemplateService deviceTemplateService;

    /**
     * 获取所有子厂站
     * @return 子长站列表
     */
    @RequestMapping(value = "/subStations", method = RequestMethod.GET)
    public ResponseEntity<List<BasePSR>> getSubstations() {
        return new ResponseEntity<List<BasePSR>>(scadaPSRService.getStations(), HttpStatus.OK);
    }

    /**
     * 获取一个厂站的所有设备
     * @param id 设备ID
     * @return 设备列表
     */
    @RequestMapping(value = "/subStation/{id}/devices", method = RequestMethod.GET)
    public ResponseEntity<List<BasePSR>> getDevicesOfSubStation(@PathVariable("id") String id) {
        List<BasePSR> devices = scadaPSRService.getDeviceOfStation(id);

        List<BasePSR> retList = new ArrayList<>();

        for (int i = 0; i < devices.size(); i++) {
            //过滤掉屋顶和虚厂站这类虚设备
            if (!devices.get(i).getArea().equals(PSRTypeConstant.ROOF.toString()) && !devices.get(i).getArea().equals(PSRTypeConstant.ARRAY.toString())) {
                retList.add(devices.get(i));
            }
        }
        return new ResponseEntity<>(retList, HttpStatus.OK);
    }

    /**
     * 获取一个设备实际的量测点信息
     * @param stationId 厂站ID
     * @param deviceId 设备ID
     * @return 量测点信息
     */

    @RequestMapping(value = "/devices/{stationId}_{id}/templates/curves", method = RequestMethod.GET)
    public ResponseEntity<List<BasePSR>> getParasOfDevice(@PathVariable("stationId") String stationId, @PathVariable("id")String deviceId) {
        List<BasePSR> YCs = scadaPSRService.getAIsofDevice(stationId,deviceId);
        YCs = changeBasePSRId(YCs,"AI");
        List<BasePSR> KWHs = scadaPSRService.getKWHsofDevice(stationId,deviceId);
        KWHs = changeBasePSRId(KWHs,"DD");
        List<BasePSR> Calcs = scadaPSRService.getCALCsofDevice(stationId,deviceId);
        Calcs = changeBasePSRId(Calcs,"CALC");

        List<BasePSR> retList = new ArrayList<>();

        if (deviceId == null) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }

        for(int i = 0; i < YCs.size(); i++){
            retList.add(YCs.get(i));
        }

//        for(int j = 0; j < KWHs.size(); j++){
//            retList.add(KWHs.get(j));
//        }

        for(int k = 0; k < Calcs.size(); k++){
            retList.add(Calcs.get(k));
        }

        return new ResponseEntity<>(retList, HttpStatus.OK);
    }

    //对BasePSR的ID做特殊处理
    private List<BasePSR> changeBasePSRId(List<BasePSR> list, String preStr){
        if (list.size() != 0){
            for (BasePSR basePSR : list){
                basePSR.setId(preStr + "_" + basePSR.getId());
            }
        }
        return  list;
    }


    /*
    @RequestMapping(value = "/devices/{id}/templates/curves", method = RequestMethod.GET)
    //public ResponseEntity<List<DeviceTemplateData>> getTemplateCurveDataOfDevice(@PathVariable("id") String id) {
    public ResponseEntity<List<DeviceTemplateData>> getTemplateCurveDataOfDevice(@PathVariable("id") String id) {
        DeviceTemplate template = scadaPSRService.getDeviceTemplateByPSRID(id);

        if (id == null) {
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }

        Set<DeviceTemplateData> datas = template.getDeviceTemplateDatas();
        List<DeviceTemplateData> result = new ArrayList<>();
        for (DeviceTemplateData data : datas) {
            //去掉遥信模板
            if (data.getType().equals(1)) {
                continue;
            }

            data.setDeviceTemplate(null);
            data.setDisplayTemplateDatas(null);
            result.add(data);
        }

        //测点排序
        Collections.sort(result, new Comparator<DeviceTemplateData>() {
            @Override
            public int compare(DeviceTemplateData o1, DeviceTemplateData o2) {
                if (o1.getType() > o2.getType()) {
                    return 1;
                } else if (o1.getType() < o2.getType()) {
                    return -1;
                } else {
                    if (o1.getIndex() > o2.getIndex()) {
                        return 1;
                    } else if (o1.getIndex() < o2.getIndex()) {
                        return -1;
                    } else {
                        return 0;
                    }
                }
            }
        });

        return new ResponseEntity<>(result, HttpStatus.OK);
    }
    */
}
