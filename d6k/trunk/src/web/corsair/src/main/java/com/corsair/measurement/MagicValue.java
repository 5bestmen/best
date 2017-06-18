package com.corsair.measurement;

import org.springframework.stereotype.Repository;

import javax.annotation.Resource;
import java.util.Map;

/**
 * Created by 洪祥 on 2015/11/26.
 */
@Repository
public class MagicValue extends AbstractSingleMeasurement {
    @Resource(name = "magicMap")
    Map<String, IMeasurement> magicMap;

    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
        IMeasurement magic = getMagicBean(measurement.getMagic());
        MeasurementValue value;

        if (magic == null) {
            value = new MeasurementValue("无计算类");
        } else {
            value = magic.getMeasurementValue(measurement);
        }

        return value;
    }

    private IMeasurement getMagicBean(String name) {
        return magicMap.get(name);
    }
}
