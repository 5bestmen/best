package com.corsair.measurement.magic;

/**
 * Created by ChengXi on 2015/7/31.
 */

import com.corsair.dao.NetElectricityDao;
import com.corsair.entity.NetElectricity;
import com.corsair.measurement.AbstractSingleMeasurement;
import com.corsair.measurement.MeasurementInfo;
import com.corsair.measurement.MeasurementValue;
import com.corsair.service.MagicValueService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

@Repository("day-grid-generated")
public class DayRridGenerated extends AbstractSingleMeasurement{

    @Autowired
    MagicValueService magicValueService;
    @Autowired
    NetElectricityDao netElectricityDao;
    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
//        Double dayGridGenerated = magicValueService.getYesterdayGenerated();
//        MeasurementValue measurementValue = null;
//        if (dayGridGenerated == null)
//        {
//            measurementValue = new MeasurementValue("N/A");
//        }else{
//            measurementValue = new MeasurementValue(dayGridGenerated);
//        }

        List<NetElectricity> netElectricities = netElectricityDao.getNetElectricityByStation("oi815");
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd");

        Double gridEnergy = 0d;
        try{
            if (!netElectricities.isEmpty()){
                Date date = dateFormat.parse(netElectricities.get(0).getNetDate());
                NetElectricity retNetElectricity = netElectricities.get(0);
                for (NetElectricity netElectricity : netElectricities){
                    Date currentDate = dateFormat.parse(netElectricity.getNetDate());
                    if (currentDate.after(date)){
                        date = currentDate;
                        retNetElectricity = netElectricity;
                    }
                }
                gridEnergy = retNetElectricity.getReverseTotalKwh() * retNetElectricity.getMultiple();
            }

        }catch (ParseException e){
            e.printStackTrace();
        }
        return  new MeasurementValue(gridEnergy);
    }


}
