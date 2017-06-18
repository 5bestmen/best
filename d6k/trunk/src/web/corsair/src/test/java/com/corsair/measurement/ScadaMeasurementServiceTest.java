package com.corsair.measurement;

import com.corsair.service.ScadaMeasurementService;
import org.apache.commons.lang3.tuple.ImmutablePair;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.TestExecutionListeners;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.DependencyInjectionTestExecutionListener;
import org.springframework.test.context.web.WebAppConfiguration;

/**
 * Created by 洪祥 on 16/3/3.
 */
@RunWith(SpringJUnit4ClassRunner.class)
@WebAppConfiguration
@ContextConfiguration(locations={"file:src/main/webapp/WEB-INF/applicationContext.xml", "file:src/main/webapp/WEB-INF/applicationContext-snaker.xml"})
@TestExecutionListeners(listeners = {
        DependencyInjectionTestExecutionListener.class
})
public class ScadaMeasurementServiceTest {
    @Autowired
    ScadaMeasurementService scadaMeasurementService;

    @Test
    public void getMeasurementPairTest() {
        ImmutablePair<String, Integer> data = scadaMeasurementService.getMeasurementID("0052ow1e8n1ln", "TotalForwardActivePower");

        Assert.assertEquals(data.getLeft(), "0052ow1e8n1ln4jsk00206");
        Assert.assertEquals(data.getRight(), 3, 0);
    }


}
