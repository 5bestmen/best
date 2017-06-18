package com.corsair.entity.template.display;

import com.corsair.entity.template.psr.DeviceTemplate;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.*;

/**
 * Created by 洪祥 on 15/7/13.
 */
public class DisplayTemplate implements java.io.Serializable {
    private String id;
    private String name;
    private String desc;
    private Boolean needContainer = false;
    private String elementStyleTemplate;
    private String containerStyleTemplate;
    private DisplayElementTemplate elementTemplate;
    private DisplayElementTemplate containerTemplate;
    private DeviceTemplate deviceTemplate;
    private DisplayArea displayArea;
    private Set<DisplayTemplateData> displayTemplateDatas = new HashSet<DisplayTemplateData>();

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDesc() {
        return desc;
    }

    public void setDesc(String desc) {
        this.desc = desc;
    }

    public DeviceTemplate getDeviceTemplate() {
        return deviceTemplate;
    }

    public void setDeviceTemplate(DeviceTemplate deviceTemplate) {
        this.deviceTemplate = deviceTemplate;
    }

    public Set<DisplayTemplateData> getDisplayTemplateDatas() {
        return displayTemplateDatas;
    }

    public void setDisplayTemplateDatas(Set<DisplayTemplateData> displayTemplateDatas) {
        this.displayTemplateDatas = displayTemplateDatas;
    }

    public Boolean getNeedContainer() {
        return needContainer;
    }

    public void setNeedContainer(Boolean needContainer) {
        this.needContainer = needContainer;
    }

    public String getElementStyleTemplate() {

        if (elementTemplate != null && elementStyleTemplate == null){
            String fileName = getClass().getClassLoader().getResource("/").getPath() + "template" + File.separator + elementTemplate.getFileName() + ".conf";
            elementStyleTemplate = getConf(fileName);
        }
        return elementStyleTemplate;

    }

    public String getContainerStyleTemplate() {
        if (containerTemplate != null && containerStyleTemplate == null){
            String fileName = getClass().getClassLoader().getResource("/").getPath() + "template" + File.separator + containerTemplate.getFileName() + ".conf";
            containerStyleTemplate = getConf(fileName);
        }
        return containerStyleTemplate;
    }


    public DisplayElementTemplate getElementTemplate() {
        return elementTemplate;
    }

    public void setElementTemplate(DisplayElementTemplate elementTemplate) {
        this.elementTemplate = elementTemplate;
    }

    public DisplayElementTemplate getContainerTemplate() {
        return containerTemplate;
    }

    public void setContainerTemplate(DisplayElementTemplate containerTemplate) {
        this.containerTemplate = containerTemplate;
    }

    public DisplayArea getDisplayArea() {
        return displayArea;
    }

    public void setDisplayArea(DisplayArea displayArea) {
        this.displayArea = displayArea;
    }

    public List getDisplayTemplateDataList() {
        List<DisplayTemplateData> displayTemplateDataList = new ArrayList<DisplayTemplateData>();
        displayTemplateDataList.addAll(getDisplayTemplateDatas());
        Collections.sort(displayTemplateDataList, new Comparator<DisplayTemplateData>() {
            @Override
            public int compare(DisplayTemplateData o1, DisplayTemplateData o2) {
                int ret = 0;
                if (o1.getIndex() > o2.getIndex()) {
                    ret = 1;
                } else if (o1.getIndex() < o2.getIndex()) {
                    ret = -1;
                } else {
                    ret = 0;
                }
                return ret;
            }
        });

        return displayTemplateDataList;
    }

    private static String getConf(String fileName) {
        File file = new File(fileName);
        Long fileLength = file.length();
        byte[] fileContent = new byte[fileLength.intValue()];
        try {
            FileInputStream in = new FileInputStream(file);
            in.read(fileContent);
            in.close();

            return new String(fileContent);

        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return "";
        } catch (IOException e) {
            e.printStackTrace();
            return "";
        }
    }
}
