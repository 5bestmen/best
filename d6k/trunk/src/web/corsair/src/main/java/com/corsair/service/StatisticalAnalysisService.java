package com.corsair.service;

import com.corsair.measurement.ScadaConfig;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Created by 洪祥 on 2016/01/04.
 */
@Service
public class StatisticalAnalysisService {

    @Autowired
    ScadaMeasurementService scadaMeasurementService;
    @Autowired
    ScadaConfig scadaConfig;

}
