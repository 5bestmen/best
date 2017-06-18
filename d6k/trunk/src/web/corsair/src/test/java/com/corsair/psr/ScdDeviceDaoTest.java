package com.corsair.psr;

import com.corsair.dao.ScdDeviceDao;
import com.corsair.entity.template.psr.PSRTypeConstant;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.annotation.Rollback;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.TestExecutionListeners;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.DependencyInjectionTestExecutionListener;
import org.springframework.test.context.transaction.TransactionConfiguration;
import org.springframework.test.context.web.WebAppConfiguration;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * Created by 洪祥 on 16/3/1.
 */
@RunWith(SpringJUnit4ClassRunner.class)
@WebAppConfiguration
@ContextConfiguration(locations={"file:src/main/webapp/WEB-INF/applicationContext.xml", "file:src/main/webapp/WEB-INF/applicationContext-snaker.xml"})
@TestExecutionListeners(listeners = {
        DependencyInjectionTestExecutionListener.class
})
@Transactional
@TransactionConfiguration(transactionManager = "transactionManager", defaultRollback = false)
public class ScdDeviceDaoTest {
    @Autowired
    ScdDeviceDao scdDeviceDao;

    @Test
    public void getStation() {
        List lst = scdDeviceDao.getArray();
        Assert.assertEquals(1, lst.size());

        lst = scdDeviceDao.getPSRFromArray("0052ow1e8n1ln", 27);
        Assert.assertEquals(26, lst.size());

    }


    @Test
    @Rollback(value = false)
    public void getDev() {
        List list = scdDeviceDao.getPSRFromArray("ow1e", PSRTypeConstant.INVERTER);
        list.size();
    }

    @Test
    public void getChildInverter() {
        List list = scdDeviceDao.getChildren("0030ow1elanl3", PSRTypeConstant.INVERTER);
        list.size();
    }
}
