package com.corsair.service;

import com.corsair.dao.DisplayAreaDao;
import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.dao.ScadaMeasurementDao;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.entity.template.display.DisplayArea;
import com.corsair.entity.template.display.DisplayTemplate;
import com.corsair.entity.template.display.DisplayTemplateData;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.Set;

/**
 * Created by 洪祥 on 15/7/15.
 */
@Service("displayAreaService")
public class DisplayAreaService {
    @Autowired
    DisplayAreaDao displayAreaDao;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;
    @Autowired
    ScadaMeasurementDao scadaMeasurementDao;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;
    @Autowired
    ScadaPSRService scadaPSRService;

    public DeviceTemplate getDeviceTemplateByPSRID(String id) {
        return psrTemplateMappingDao.getDeviceTemplateByPSRID(id);
    }

    public DisplayArea getDisplayAreaByName(String name) {
        return displayAreaDao.getDisplayAreaByName(name);
    }

    public DisplayArea getDisplayAreaByComments(String comments) {
        for(DisplayArea displayArea : getDisplayAreaList()){
            if (displayArea.getComments().equals(comments)){
                return displayArea;
            }
        }
        return null;
    }

    public DisplayTemplate getDisplayTemplate(DisplayArea area, DeviceTemplate psr) {
        Set<DisplayTemplate> displayTemplates = psr.getDisplayTemplates();

        for (DisplayTemplate template : displayTemplates) {
            if (template.getDisplayArea().equals(area)) {
                return template;
            }
        }

        return null;
    }

    public String getTagString(String displayAreaName, String psrID) {

        DisplayArea displayArea = displayAreaDao.getDisplayAreaByName(displayAreaName);

        if (displayArea == null) {
            return String.format("无法找到名为%1$s的显示区域", displayAreaName);
        }

        DeviceTemplate deviceTemplate = psrTemplateMappingDao.getDeviceTemplateByPSRID(psrID);

        if (deviceTemplate == null) {
            return String.format("无法找到ID为%1$s设备", psrID);
        }

        DisplayTemplate displayTemplate = getDisplayTemplate(displayArea, deviceTemplate);

        if (displayTemplate == null) {
            return String.format("无法为%1$s找到在%2$s上的显示模板", psrID, displayAreaName);
        }

        List<DisplayTemplateData> displayTemplateDatas = displayTemplate.getDisplayTemplateDataList();

        String finalElement = "";
        String elementsString = "";

        for (DisplayTemplateData displayTemplateData : displayTemplateDatas) {
            DeviceTemplateData deviceTemplateData = displayTemplateData.getDeviceTemplateData();

            if (deviceTemplateData == null) {
                elementsString += String.format("显示模板%1$s数据%2$s没有对应的量测模板数据", displayTemplate.getName(), displayTemplateData.getId());
                continue;
            }

            String measurementID = scadaMeasurementService.getMeasurementID(psrID, deviceTemplateData);
            elementsString += createElement(measurementID, displayTemplate, displayTemplateData, deviceTemplateData, psrID);
        }

        if (Objects.equals(displayTemplate.getNeedContainer(), Boolean.TRUE)) {
            String container = displayTemplate.getContainerStyleTemplate();

            if (container.contains("ELEMENTS_STRING")) {
                finalElement = container.replace("ELEMENTS_STRING", elementsString);
            }
        } else {
            finalElement = elementsString;
        }
        return finalElement;
    }

    private String createElement(String measurementID, DisplayTemplate displayTemplate,
                                 DisplayTemplateData displayTemplateData, DeviceTemplateData deviceTemplateData, String PSRID) {

        String template = displayTemplate.getElementStyleTemplate();

        if (template.contains("DATA_PSR_ID")) {
            template = template.replace("DATA_PSR_ID", PSRID == null ? "" : PSRID);
        }

        if (template.contains("DATA_TEMPLATE_INDEX")) {
            template = template.replace("DATA_TEMPLATE_INDEX", deviceTemplateData.getIndex().toString());
        }

        if (template.contains("DATA_MEANS_ID")) {
            template = template.replace("DATA_MEANS_ID", measurementID == null ? "" : measurementID.trim());
        }

        if (template.contains("DATA_MEANS_TYPE")) {
            template = template.replace("DATA_MEANS_TYPE", deviceTemplateData.getType().toString());
        }

        if (template.contains("DATA_MAGIC_STRING")) {
            template = template.replace("DATA_MAGIC_STRING", deviceTemplateData.getMagic() == null ? "" : deviceTemplateData.getMagic());
        }

        if (template.contains("DATA_NAME")) {
            template = template.replace("DATA_NAME", deviceTemplateData.getName() == null ? "" : deviceTemplateData.getName());
        }

        if (template.contains("DATA_CALCULATE")) {
            template = template.replace("DATA_CALCULATE", deviceTemplateData.getCalculate() == null ? "" : deviceTemplateData.getCalculate().toString());
        }

        if (template.contains("UNIT_STRING")) {
            template = template.replace("UNIT_STRING", deviceTemplateData.getUnit());
        }

        if (template.contains("TITLE_STRING")) {
            template =template.replace("TITLE_STRING", deviceTemplateData.getDesc() == null ? "" : deviceTemplateData.getDesc());
        }

        if (template.contains("ICON_STRING")) {
            template = template.replace("ICON_STRING", displayTemplateData.getIcon() == null ? "" : displayTemplateData.getIcon());
        }

        if (template.contains("EXTRA_STRING_1")) {
            template = template.replace("EXTRA_STRING_1", displayTemplateData.getExtraString1() == null ? "" : displayTemplateData.getExtraString1());
        }

        if (template.contains("EXTRA_STRING_2")) {
            template = template.replace("EXTRA_STRING_2", displayTemplateData.getExtraString2() == null ? "" : displayTemplateData.getExtraString2());
        }

        if (template.contains("EXTRA_STRING_3")) {
            template = template.replace("EXTRA_STRING_3", displayTemplateData.getExtraString3() == null ? "" : displayTemplateData.getExtraString3());
        }

        return template;
    }

    public List<DisplayArea> getDisplayAreaList() {
        return displayAreaDao.getDisplayArea();
    }

    public List<DisplayArea> getDisplayArea(String name, String comments) {
        return displayAreaDao.getDisplayArea(name, comments);
    }

    public DisplayArea getDisplayAreaById(String id){
        return displayAreaDao.getDisplayAreaById(id);
    }

    public void saveOrUpdateDisplayArea(DisplayArea displayArea){
        displayAreaDao.saveOrUpdate(displayArea);
    }

    public void deleteDisplayArea(String id){
        DisplayArea displayArea  = displayAreaDao.getDisplayAreaById(id);
        displayAreaDao.delete(displayArea);
    }

    public List<String> getDisplayAreaDescList(){
        List<String> ret = new ArrayList<>();
        for(DisplayArea displayArea : getDisplayAreaList()){
            ret.add(displayArea.getComments());
        }
        return ret;
    }

    public boolean checkNameExist(String name){
        List<DisplayArea> list = displayAreaDao.getDisplayArea();
        for (DisplayArea area : list){
            if (area.getName().equals(name)){
                return true;
            }
        }
        return false;
    }

    public boolean checkCommentsExist(String comments){
        List<DisplayArea> list = displayAreaDao.getDisplayArea();
        for (DisplayArea area : list){
            if (area.getComments().equals(comments)){
                return true;
            }
        }
        return false;
    }
}
