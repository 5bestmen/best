package com.corsair.service;

import com.corsair.dao.*;
import com.corsair.entity.template.display.DisplayTemplate;
import com.corsair.entity.template.display.DisplayTemplateData;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * Created by 洪祥 on 2016/1/7.
 */
@Service("displayTemplateDataService")
public class DisplayTemplateDataService {
    @Autowired
    private DisplayTemplateDataDao displayTemplateDataDao;

    @Autowired
    private DisplayTemplateDao displayTemplateDao;


    public void saveOrUpdateDisplayTemplateData(DisplayTemplateData displayTemplateData){
        displayTemplateDataDao.saveOrUpdate(displayTemplateData);
    }

    public DisplayTemplateData getDisplayTemplateDataById(String id){
        return displayTemplateDataDao.findById(DisplayTemplateData.class, id);
    }

    //根据显示模板ID查找该模板下的模板数据
    public List<DisplayTemplateData> getDisplayTemplateDatas(String id){
        return displayTemplateDataDao.getDisplayTemplateDataById(id);
    }


    public void deleteDisplayTemplateData(String id){
        DisplayTemplateData displayTemplateData  = this.getDisplayTemplateDataById(id);
        displayTemplateDataDao.delete(displayTemplateData);
    }

    public Boolean checkIndexExist(String displayTemplate, String index){
        List<DisplayTemplate> displayTemplateList = displayTemplateDao.getDisplayTemplate();
        for (DisplayTemplate template : displayTemplateList){
            //首先找到所选的显示模板
            if (template.getDesc().equals(displayTemplate)){
                //然后获取显示模板下的所有显示模板数据
                for (DisplayTemplateData displayTemplateData : template.getDisplayTemplateDatas()){
                    //判断index是否已经存在
                    if (displayTemplateData.getIndex().toString().equals(index)){
                        return true;
                    }
                }
            }
        }
        return false;
    }
}
