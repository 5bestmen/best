package com.corsair.dao;


import com.corsair.entity.config.DeviceTemplateDataConfig;
import org.springframework.stereotype.Repository;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import java.io.File;
import java.io.IOException;

/**
 * Created by 洪祥 on 2015/12/17.
 */
@Repository
public class ConfigDao {

    private static final String FILE_NAME = "com/corsair/config/deviceTemplateData.xml";
    private DeviceTemplateDataConfig deviceTemplateDataConfig;
    private String fileName;

    public ConfigDao() {
        fileName = this.getClass().getResource("/").getPath() + FILE_NAME;
        try {
            loadSettings();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public DeviceTemplateDataConfig getDeviceTemplateDataConfig() {
        return deviceTemplateDataConfig;
    }

    public void setDeviceTemplateDataConfig(DeviceTemplateDataConfig deviceTemplateDataConfig) {
        this.deviceTemplateDataConfig = deviceTemplateDataConfig;
    }

    public void loadSettings() throws IOException {
        try {
            File file = new File(fileName);

            if (!file.exists()) {
                return;
            }

            JAXBContext jaxbContext = JAXBContext.newInstance(DeviceTemplateDataConfig.class);
            Unmarshaller jaxbUnmarshaller = jaxbContext.createUnmarshaller();
            this.deviceTemplateDataConfig = (DeviceTemplateDataConfig) jaxbUnmarshaller.unmarshal(file);
        } catch (JAXBException e) {
            e.printStackTrace();
        }
    }
}
