package com.corsair.service;

import com.corsair.dao.MaterialInformationDao;
import com.corsair.entity.MaterialInformation;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * Created by zjq on 2015/11/5.
 */
@Service("materialInformationService")
public class MaterialInformationService  {
    @Autowired
    MaterialInformationDao materialInformationDao;

    public List<MaterialInformation> getMaterialInformationsByNoAndId(String materialSeriesNo,String materialId){
        return materialInformationDao.getMaterialInformationByNoAndId(materialSeriesNo,materialId);
    }
}
