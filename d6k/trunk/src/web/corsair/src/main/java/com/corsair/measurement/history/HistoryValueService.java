package com.corsair.measurement.history;

import com.corsair.service.ScadaMeasurementService;
import org.apache.commons.lang3.tuple.ImmutablePair;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Created by 洪祥 on 16/3/3.
 */
@Service
public class HistoryValueService {
    @Autowired
    HistoryMeasurementDao historyMeasurementDao;
    @Autowired
    HistoryKwhDao historyKwhDao;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;

    /**
     * 获取一个对象的数据点在一天中的首个值
     * @param psrID 对象ID
     * @param name 数据点名称
     * @param day 日期格式"20160101"
     * @return 值,找不到则为NULL
     */
    HistoryValue getDayFirstData(String psrID, String name, String day) {
        ImmutablePair<String, Integer> idAndType = scadaMeasurementService.getMeasurementID(psrID, name);

        if (idAndType == null) {
            return null;
        }

        return null;
    }
}
