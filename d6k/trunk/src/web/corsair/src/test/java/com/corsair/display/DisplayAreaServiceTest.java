package com.corsair.display;

import com.corsair.service.DisplayAreaService;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.TestExecutionListeners;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.DependencyInjectionTestExecutionListener;
import org.springframework.test.context.web.WebAppConfiguration;

/**
 * Created by 洪祥 on 16/3/2.
 */
@RunWith(SpringJUnit4ClassRunner.class)
@WebAppConfiguration
@ContextConfiguration(locations={"file:src/main/webapp/WEB-INF/applicationContext.xml", "file:src/main/webapp/WEB-INF/applicationContext-snaker.xml"})
@TestExecutionListeners(listeners = {
        DependencyInjectionTestExecutionListener.class
})
public class DisplayAreaServiceTest {
    @Autowired
    DisplayAreaService displayAreaService;

    @Test
    public void makeTagStringTest() {
        String ret = displayAreaService.getTagString("index-station-info", "0052ow1e8n1ln");
        System.out.println(ret);

    }

}
