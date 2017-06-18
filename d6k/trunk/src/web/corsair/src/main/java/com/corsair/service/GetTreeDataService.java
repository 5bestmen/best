package com.corsair.service;

import com.corsair.dao.GetTreeDataDao;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * Created by zyj on 2017/2/6.
 */
@Service("getTreeDataService")
public class GetTreeDataService {
    @Autowired
    private GetTreeDataDao getTreeDataDao;
    public String getTreeDataEvent() {

        return getTreeDataDao.getTreeData();
    }
}
