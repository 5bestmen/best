package com.dao;

import com.entity.Area;
import org.apache.ibatis.annotations.Param;

/**
 * Created by zyj on 2017/5/19.
 */
public interface AreaDao {
    Area getById(@Param("id") Integer id);
}
