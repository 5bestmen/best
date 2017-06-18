package com.corsair.service;

import com.corsair.dao.MaterialStoreDao;
import com.corsair.entity.MaterialStore;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * Created by zjq on 2015/11/6.
 */
@Service("MaterialStoreService")
public class MaterialStoreService {
    @Autowired
    MaterialStoreDao materialStoreDao;

    public List<MaterialStore> getMaterialStoresByInfoId(String materialInfoId){
        return materialStoreDao.getMaterialStoresByInfoId(materialInfoId);
    }
}
