package com.corsair.service;

import com.corsair.dao.ApplicationInformationDao;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Created by 洪祥 on 15/7/1.
 */
@Service("applicationInformationService")
public class ApplicationInformationService {
    @Autowired
    ApplicationInformationDao applicationInformationDao;

    public String getAppName() {
        return applicationInformationDao.getAppName();
    }

    public String getVersion() {
        return applicationInformationDao.getVersion();
    }

    public String getCenterPoint_lng() {return  applicationInformationDao.getCenterPoint_long();}

    public String getCenterPoint_lat() {return  applicationInformationDao.getCenterPoint_lat();}

    public String getStationName() {return applicationInformationDao.getStationName();}

    public String getStationCapacity() {return applicationInformationDao.getStationCapacity();}

    public String getPowerGridCapacity() {return applicationInformationDao.getPowerGridCapacity();}

    public String getRunningTime() {return applicationInformationDao.getRunningTime();}

    public String getMenuWay() {return applicationInformationDao.getMenuWay();}

    public Float getMapZoom() {
        String zoom = applicationInformationDao.getMapZoom();
        return  Float.valueOf(zoom);}

    public boolean isZoomable() {
        String zoomable = applicationInformationDao.getZoomFlag();
        return zoomable.equals("true");
    }

    public boolean isDragable() {
        String dragable = applicationInformationDao.getDragFlag();
        return dragable.equals("true");
        }

}
