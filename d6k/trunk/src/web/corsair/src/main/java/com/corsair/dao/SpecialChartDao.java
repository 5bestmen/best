package com.corsair.dao;

import com.corsair.entity.SpecialChart;
import com.rbac.common.BaseDaoSupport;
import com.rbac.util.CommonUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Restrictions;
import org.springframework.stereotype.Repository;

import java.util.List;

/**
 * Created by 洪祥 on 15/12/16.
 */
@Repository("specialChartDao")
public class SpecialChartDao extends BaseDaoSupport {

    public List<SpecialChart> getSpecialChart() {
        Criteria criteria = super.currentSession().createCriteria(SpecialChart.class);

        return criteria.list();
    }

    /**
     * 根据图表名和图表描述查找特殊图表
     * @param name
     * @param comments
     * @return
     */
    public List<SpecialChart> getSpecialChart(String name, String comments) {
        Criteria criteria = super.currentSession().createCriteria(SpecialChart.class);
        if(CommonUtils.isNotEmpty(name)){
            criteria.add(Restrictions.ilike("name", name, MatchMode.ANYWHERE));
        }
        if(CommonUtils.isNotEmpty(comments)){
            criteria.add(Restrictions.ilike("comments", comments, MatchMode.ANYWHERE));
        }
        return criteria.list();
    }

    public List<SpecialChart> getSpecialChartByName(String name) {
        Criteria criteria = super.currentSession().createCriteria(SpecialChart.class);
        if(CommonUtils.isNotEmpty(name)){
            criteria.add(Restrictions.eq("name", name));
        }
        return criteria.list();
    }

    /**
     * 根据ID查找特殊图表
     * @param id
     * @return
     */
    public SpecialChart getSpecialChartById(String id){
        return findById(id);
    }

    public SpecialChart findById(String id){
        return super.findById(SpecialChart.class, id);
    }
}
