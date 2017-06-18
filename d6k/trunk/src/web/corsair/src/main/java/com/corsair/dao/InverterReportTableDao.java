package com.corsair.dao;

import com.corsair.entity.InverterReportTable;
import com.rbac.common.BaseDaoSupport;
import org.hibernate.Session;
import org.hibernate.Transaction;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowCallbackHandler;
import org.springframework.stereotype.Repository;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

/**
 * Created by zjq on 2015/8/24.
 */

@Repository("inverterReportTableDao")
public class InverterReportTableDao extends BaseDaoSupport {
    @Autowired
    private JdbcTemplate jdbcTemplate;

    public void addRecords(){
        Session session = super.currentSession();
        Transaction tx = session.beginTransaction();
        Calendar cl = Calendar.getInstance();


        String[] pvArrayNames = {"光伏阵列1","光伏阵列2","光伏阵列3","光伏阵列4","光伏阵列5","光伏阵列6","光伏阵列7","光伏阵列8","光伏阵列9","光伏阵列10"};
        String[] inverterNames = {"逆变器1","逆变器2"};
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 2; j++) {
                cl.set(Calendar.YEAR,2015);
                cl.set(Calendar.MONTH,0);
                cl.set(Calendar.DATE,1);
                cl.set(Calendar.HOUR,0);
                cl.set(Calendar.MINUTE,0);
                cl.set(Calendar.SECOND,0);
                for (int k = 0; k < 240; k++) {
                    InverterReportTable record = new InverterReportTable();
                    record.setPsrID(pvArrayNames[i] + inverterNames[j]);
                    record.setDate(cl.getTime());
                    record.setElectricity(Math.random() * 30 + 50);
                    session.save(record);
                    cl.set(Calendar.DAY_OF_YEAR,cl.get(Calendar.DAY_OF_YEAR) + 1);
                }
            }
        }
        tx.commit();
        session.close();
        return;
    }

    public List<InverterReportTable> getRecords(Date date1,Date date2,String inverterName) {
//        Criteria criteria = super.getSession().createCriteria(InverterReportTable.class);
//        criteria.add(Restrictions.between("sdate",date1,date2));
//        criteria.add(Restrictions.eq("psrID", inverterName));
//  //      criteria.addOrder(Order.asc("sdate"));
//        List list = criteria.list();
//        return  list;
        SimpleDateFormat dateParse = new SimpleDateFormat("yyyy-MM-dd");
        String dateBegin = dateParse.format(date1);
        String dateEnd = dateParse.format(date2);

        final List<InverterReportTable> inverterReportTables = new ArrayList<InverterReportTable>();
        String sql = "select *  from INVERTERREPORTTABLE where psrid='" + inverterName + "' and  sdate between to_date('" + dateBegin + "','yyyy-mm-dd') and to_date('" + dateEnd + "','yyyy-mm-dd') order by sdate asc";
        try {
            jdbcTemplate.query(sql, new RowCallbackHandler() {
                public void processRow(ResultSet resultSet) throws SQLException {
                    InverterReportTable inverterReportTable = new InverterReportTable();
                    inverterReportTable.setId(resultSet.getString("id").trim());
                    inverterReportTable.setPsrID(resultSet.getString("psrid").trim());
                    inverterReportTable.setDate(resultSet.getDate("sdate"));
                    inverterReportTable.setElectricity(resultSet.getDouble("ELECTRICITY"));
                    inverterReportTables.add(inverterReportTable);
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
        return inverterReportTables;
    }

}