package com.corsair.service;

import com.corsair.dao.*;
import com.corsair.device.ElectricPSR;
import com.corsair.device.PhotovoltaicArray;
import com.corsair.entity.*;
import com.corsair.entity.template.psr.PSRTemplateMapping;
import com.corsair.measurement.HistoryDataValue;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.text.DecimalFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Created by libh on 2015/12/17.
 * 用于电费结算
 */
@Service("complexBenefitService")
public class ComplexBenefitService {
    @Autowired
    private NetElectricityDao netElectricityDao;
    @Autowired
    private ElectricPriceDao electricPriceDao;
    @Autowired
    private ElectricTimeDao electricTimeDao;
    @Autowired
    private ScdDeviceDao scdDeviceDao;
    @Autowired
    HistoryDataDao historyDataDao;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;
    @Autowired
    StationDao stationDao;

    public enum DataType{
        REALTIME_VALUE(1),//实时值
        DEAD_VALUE(4),//死数
        TERMINAL_ERROR(5),//终端故障
        KWH_VALUE(52),//电度表码
        DAY_ACCUMULATE(79);//日累计电量

        private int value;
        private DataType(int value) {
            this.value = value;
        }
        public int getValue(){
            return  value;
        }
    }

    public enum DeviceDataType{
        SCD_TYPE_STATION(52),    //设备类型为厂站
        FORWARD_PKWH_TOTAL(200),   //正向有功总电量
        FORWARD_PKWH_TIP(201),     //正向有功尖电量
        FORWARD_PKWH_PEAK(202),    //正向有功峰电量
        FORWARD_PKWH_NORMAL(203),  //正向有功平电量
        FORWARD_PKWH_VALLEY(204);  //正向有功谷电量

        private int value;
        private DeviceDataType(int value) {
            this.value = value;
        }
        public int getValue(){
            return  value;
        }
    }

    public enum HisDataTime{
        FIRST_TIME(15),    //历史表每日开始存盘时间
        END_TIME(1440);    //历史表每日结束存盘时间

        private int value;
        private HisDataTime(int value) {
            this.value = value;
        }
        public int getValue(){
            return  value;
        }
    }

    /**
     * 获取所有的厂站阵列
     * @param
     * @return
     */
    public List<PhotovoltaicArray> getStationList(){
        return stationDao.getPhotovoltaics();
    }

    /**
     * 根据指定日期和厂站寻找上网电量数据
     * @param netDate
     * @param station
     * @return
     */
    public NetElectricity getNetElectricityByDate(String netDate, String station){
        return netElectricityDao.getNetElectricityByDate(netDate, station);
    }

    /**
     * 保存或更新上网电量
     * @param netElectricity
     */
    public void saveNetElectricity(NetElectricity netElectricity){
        netElectricityDao.saveOrUpdate(netElectricity);
    }

    /**
     * 检查上网电量的录入日期是否已经存在
     * @param netDate
     * @return
     */
    public boolean checkExistNetDate(String netDate){
        NetElectricity netElectricity = netElectricityDao.getNetElectricityByDate(netDate);
        if(netElectricity != null){
            return true;
        }
        return false;
    }

    /**
     * 获取描述与电价的映射
     * @param
     * @return
     */
    public Map<String, ElectricPrice> getNameToElectricPriceMap(){
        Map nameToElectricPriceMap = new HashMap();
        List<ElectricPrice> electricPriceList = new ArrayList<>();
        electricPriceList = electricPriceDao.getElectricPriceList();
        for (ElectricPrice electricPrice : electricPriceList){
            nameToElectricPriceMap.put(electricPrice.getName(), electricPrice);
        }
        return nameToElectricPriceMap;
    }

    /**
     * 获取指定月份的上网电量结算
     * @param date 指定年月
     * @param station 指定厂站
     * @return
     */
    public List<ElectricPSR> getNetElectricAccountByDate(String date, String station){
        List<ElectricPSR> netElectricSettleList = new ArrayList<>();
        String LastDayOfMonth = getLastDayOfMonth(date);
        String LastDayOfLastMonth = getLastDayOfLastMonth(date);
        NetElectricity curNetElectricity = getNetElectricityByDate(LastDayOfMonth, station);
        NetElectricity lastNetElectricity = getNetElectricityByDate(LastDayOfLastMonth, station);
        Double curTotalkWh = 0.00, lastTotalkWh = 0.00, curPeakKwh = 0.00, lastPeakKwh = 0.00;
        Double curNormalKwh = 0.00, lastNormalKwh = 0.00, curValleyKwh = 0.00, lastValleyKwh = 0.00;
        int multiple = 1;
        multiple = ((curNetElectricity==null) ? 1 : curNetElectricity.getMultiple());
        Double price = getPriceByName("上网电价");

        curTotalkWh = ((curNetElectricity==null) ? 0.00 : curNetElectricity.getTotalKwh());
        lastTotalkWh = ((lastNetElectricity==null) ? 0.00 : lastNetElectricity.getTotalKwh());
        Double electricValue = (curTotalkWh-lastTotalkWh) * multiple;

        ElectricPSR totalElectricPSR = new ElectricPSR();
        totalElectricPSR.setName("正向 总");
        totalElectricPSR.setCurValue(numberFormat(curTotalkWh));
        totalElectricPSR.setLastValue(numberFormat(lastTotalkWh));
        totalElectricPSR.setMultiple(multiple);
        totalElectricPSR.setElectricValue(numberFormat(electricValue));
        totalElectricPSR.setElectricPrice(price);
        totalElectricPSR.setFee(numberFormat(electricValue * price));
        netElectricSettleList.add(totalElectricPSR);

        curPeakKwh = ((curNetElectricity==null) ? 0.00 : curNetElectricity.getPeakKwh());
        lastPeakKwh = ((lastNetElectricity==null) ? 0.00 : lastNetElectricity.getPeakKwh());
        electricValue = (curPeakKwh-lastPeakKwh) * multiple;
        ElectricPSR peakElectricPSR = new ElectricPSR();
        peakElectricPSR.setName("正向 峰");
        peakElectricPSR.setCurValue(numberFormat(curPeakKwh));
        peakElectricPSR.setLastValue(numberFormat(lastPeakKwh));
        peakElectricPSR.setMultiple(multiple);
        peakElectricPSR.setElectricValue(numberFormat(electricValue));
        peakElectricPSR.setElectricPrice(price);
        peakElectricPSR.setFee(numberFormat(electricValue * price));
        netElectricSettleList.add(peakElectricPSR);

        curNormalKwh = ((curNetElectricity==null) ? 0.00 : curNetElectricity.getNormalKwh());
        lastNormalKwh = ((lastNetElectricity==null) ? 0.00 : lastNetElectricity.getNormalKwh());
        electricValue = (curNormalKwh-lastNormalKwh) * multiple;
        ElectricPSR normalElectricPSR = new ElectricPSR();
        normalElectricPSR.setName("正向 平");
        normalElectricPSR.setCurValue(numberFormat(curNormalKwh));
        normalElectricPSR.setLastValue(numberFormat(lastNormalKwh));
        normalElectricPSR.setMultiple(multiple);
        normalElectricPSR.setElectricValue(numberFormat(electricValue));
        normalElectricPSR.setElectricPrice(price);
        normalElectricPSR.setFee(numberFormat(electricValue * price));
        netElectricSettleList.add(normalElectricPSR);

        curValleyKwh = ((curNetElectricity==null) ? 0.00 : curNetElectricity.getValleyKwh());
        lastValleyKwh = ((lastNetElectricity==null) ? 0.00 : lastNetElectricity.getValleyKwh());
        electricValue = (curValleyKwh-lastValleyKwh) * multiple;
        ElectricPSR valleyElectricPSR = new ElectricPSR();
        valleyElectricPSR.setName("正向 谷");
        valleyElectricPSR.setCurValue(numberFormat(curValleyKwh));
        valleyElectricPSR.setLastValue(numberFormat(lastValleyKwh));
        valleyElectricPSR.setMultiple(multiple);
        valleyElectricPSR.setElectricValue(numberFormat(electricValue));
        valleyElectricPSR.setElectricPrice(price);
        valleyElectricPSR.setFee(numberFormat(electricValue * price));
        netElectricSettleList.add(valleyElectricPSR);

        Double curReverseTotalKwh = 0.00, lastReverseTotalKwh = 0.00;
        curReverseTotalKwh = ((curNetElectricity==null) ? 0.00 : curNetElectricity.getReverseTotalKwh());
        lastReverseTotalKwh = ((lastNetElectricity==null) ? 0.00 : lastNetElectricity.getReverseTotalKwh());
        electricValue = (curReverseTotalKwh-lastReverseTotalKwh) * multiple;
        ElectricPSR reverseTotalElectricPSR = new ElectricPSR();
        reverseTotalElectricPSR.setName("反向 总");
        reverseTotalElectricPSR.setCurValue(numberFormat(curReverseTotalKwh));
        reverseTotalElectricPSR.setLastValue(numberFormat(lastReverseTotalKwh));
        reverseTotalElectricPSR.setMultiple(multiple);
        reverseTotalElectricPSR.setElectricValue(numberFormat(electricValue));
        reverseTotalElectricPSR.setElectricPrice(price);
        reverseTotalElectricPSR.setFee(numberFormat(electricValue * price));
        netElectricSettleList.add(reverseTotalElectricPSR);

        return netElectricSettleList;
    }

    /**
     * 获取指定月份的光伏电站总发电量,峰谷平数据直接从表中读取
     * @param date 指定年月
     * @param stationId 指定厂站
     * @return
     */
    public List<ElectricPSR> getTotalElectricityDirectByDate(String date, String stationId){
        List<ElectricPSR> totalElectricityList = new ArrayList<>();

        String tableName = "KWH" + date.substring(0, 4) + date.substring(5, 7);
        String totalKwhName = scdDeviceDao.getCombinedCabName(stationId, DeviceDataType.SCD_TYPE_STATION.getValue(), DeviceDataType.FORWARD_PKWH_TOTAL.getValue());
        String peakKwhName = scdDeviceDao.getCombinedCabName(stationId, DeviceDataType.SCD_TYPE_STATION.getValue(), DeviceDataType.FORWARD_PKWH_PEAK.getValue());
        String normalKwhName = scdDeviceDao.getCombinedCabName(stationId, DeviceDataType.SCD_TYPE_STATION.getValue(), DeviceDataType.FORWARD_PKWH_NORMAL.getValue());
        String valleyKwhName = scdDeviceDao.getCombinedCabName(stationId, DeviceDataType.SCD_TYPE_STATION.getValue(), DeviceDataType.FORWARD_PKWH_VALLEY.getValue());

        int multiple = 1;

        String firstDayOfMonth = getFirstDayOfMonth(date);
        String LastDayOfMonth = getLastDayOfMonth(date);
        int startDays = getDaysByDate(firstDayOfMonth);
        int endDays = getDaysByDate(LastDayOfMonth);
        if (startDays > endDays) return null;

        //判断是否当前月,如果是,则总发电量为前一天24点的数据
        Date dt=new Date();
        SimpleDateFormat matter1=new SimpleDateFormat("yyyy/MM/dd");
        String curDate = matter1.format(dt);
        if (curDate.contains(date)){
            endDays = endDays -1;
        }

        List<String> nameList = new ArrayList<>();
        List<Integer> dataDayList = new ArrayList<>();
        List<Integer> timeList = new ArrayList<>();
        nameList.add(totalKwhName);
        nameList.add(peakKwhName);
        nameList.add(normalKwhName);
        nameList.add(valleyKwhName);

        dataDayList.add(startDays);
        dataDayList.add(endDays);

        timeList.add(HisDataTime.FIRST_TIME.getValue());
        timeList.add(HisDataTime.END_TIME.getValue());

        //获取本期和上期的总、峰、平、谷电量数据
        Double totalLastValue = 0.00, totalCurValue = 0.00, peakLastValue = 0.00, peakCurValue = 0.00;
        Double normalLastValue = 0.00, normalCurValue = 0.00, valleyLastValue = 0.00, valleyCurValue = 0.00;

        List<HistoryDataValue> historyDataValueList = historyDataDao.getHistoryDataByRangeDateTime(tableName, nameList, dataDayList, timeList, DataType.KWH_VALUE.getValue());
        if(historyDataValueList.size() > 0){
            for (HistoryDataValue historyDataValue : historyDataValueList){
                if(historyDataValue.getName().equals(totalKwhName)){
                    if ((historyDataValue.getSdate() == startDays) &&
                            (historyDataValue.getTime() == HisDataTime.FIRST_TIME.getValue())){
                        totalLastValue =((historyDataValue ==null) ? 0.00 : historyDataValue.getData());
                    }
                    if ((historyDataValue.getSdate() == endDays) &&
                            (historyDataValue.getTime() == HisDataTime.END_TIME.getValue())){
                        totalCurValue =((historyDataValue ==null) ? 0.00 : historyDataValue.getData());
                    }
                }
                if(historyDataValue.getName().equals(peakKwhName)){
                    if ((historyDataValue.getSdate() == startDays) &&
                            (historyDataValue.getTime() == HisDataTime.FIRST_TIME.getValue())){
                        peakLastValue =((historyDataValue ==null) ? 0.00 : historyDataValue.getData());
                    }
                    if ((historyDataValue.getSdate() == endDays) &&
                            (historyDataValue.getTime() == HisDataTime.END_TIME.getValue())){
                        peakCurValue =((historyDataValue ==null) ? 0.00 : historyDataValue.getData());
                    }
                }
                if(historyDataValue.getName().equals(normalKwhName)){
                    if ((historyDataValue.getSdate() == startDays) &&
                            (historyDataValue.getTime() == HisDataTime.FIRST_TIME.getValue())){
                        normalLastValue =((historyDataValue ==null) ? 0.00 : historyDataValue.getData());
                    }
                    if ((historyDataValue.getSdate() == endDays) &&
                            (historyDataValue.getTime() == HisDataTime.END_TIME.getValue())){
                        normalCurValue =((historyDataValue ==null) ? 0.00 : historyDataValue.getData());
                    }
                }
                if(historyDataValue.getName().equals(valleyKwhName)){
                    if ((historyDataValue.getSdate() == startDays) &&
                            (historyDataValue.getTime() == HisDataTime.FIRST_TIME.getValue())){
                        valleyLastValue =((historyDataValue ==null) ? 0.00 : historyDataValue.getData());
                    }
                    if ((historyDataValue.getSdate() == endDays) &&
                            (historyDataValue.getTime() == HisDataTime.END_TIME.getValue())){
                        valleyCurValue =((historyDataValue ==null) ? 0.00 : historyDataValue.getData());
                    }
                }
            }
        }


        //获取峰平谷电价
        Map<String, ElectricPrice> nameToElectricPriceMap = getNameToElectricPriceMap();
        ElectricPrice electricPrice;
        Double peakPrice = 0.00, normalPrice = 0.00, valleyPrice = 0.00, processPrice = 0.00;
        if(nameToElectricPriceMap.containsKey("峰值电价"))
        {
            electricPrice = nameToElectricPriceMap.get("峰值电价");
            peakPrice = electricPrice.getPrice();
        }
        if(nameToElectricPriceMap.containsKey("平期电价"))
        {
            electricPrice = nameToElectricPriceMap.get("平期电价");
            normalPrice = electricPrice.getPrice();
        }
        if(nameToElectricPriceMap.containsKey("谷值电价"))
        {
            electricPrice = nameToElectricPriceMap.get("谷值电价");
            valleyPrice = electricPrice.getPrice();
        }
        if(nameToElectricPriceMap.containsKey("燃油加工费"))
        {
            electricPrice = nameToElectricPriceMap.get("燃油加工费");
            processPrice = electricPrice.getPrice();
        }

        ElectricPSR totalElectricPSR = new ElectricPSR();
        totalElectricPSR.setName("正向 总");
        totalElectricPSR.setCurValue(numberFormat(totalCurValue));
        totalElectricPSR.setLastValue(numberFormat(totalLastValue));
        totalElectricPSR.setMultiple(multiple);
        totalElectricPSR.setElectricValue(numberFormat(multiple * (totalCurValue - totalLastValue)));
        totalElectricPSR.setElectricPrice(processPrice);
        totalElectricPSR.setFee(numberFormat((multiple * (totalCurValue - totalLastValue)) * processPrice));
        totalElectricityList.add(totalElectricPSR);

        ElectricPSR peakElectricPSR = new ElectricPSR();
        peakElectricPSR.setName("正向 峰");
        peakElectricPSR.setCurValue(numberFormat(peakCurValue));
        peakElectricPSR.setLastValue(numberFormat(peakLastValue));
        peakElectricPSR.setMultiple(multiple);
        peakElectricPSR.setElectricValue(numberFormat((peakCurValue - peakLastValue) * multiple));
        peakElectricPSR.setElectricPrice(peakPrice);
        peakElectricPSR.setFee(numberFormat(((peakCurValue - peakLastValue) * multiple) * peakPrice));
        totalElectricityList.add(peakElectricPSR);

        ElectricPSR NormalElectricPSR = new ElectricPSR();
        NormalElectricPSR.setName("正向 平");
        NormalElectricPSR.setCurValue(numberFormat(normalCurValue));
        NormalElectricPSR.setLastValue(numberFormat(normalLastValue));
        NormalElectricPSR.setMultiple(multiple);
        NormalElectricPSR.setElectricValue(numberFormat((normalCurValue - normalLastValue) *multiple));
        NormalElectricPSR.setElectricPrice(normalPrice);
        NormalElectricPSR.setFee(numberFormat(((normalCurValue - normalLastValue) * multiple)* normalPrice));
        totalElectricityList.add(NormalElectricPSR);

        ElectricPSR valleyElectricPSR = new ElectricPSR();
        valleyElectricPSR.setName("正向 谷");
        valleyElectricPSR.setCurValue(numberFormat(valleyCurValue));
        valleyElectricPSR.setLastValue(numberFormat(valleyLastValue));
        valleyElectricPSR.setMultiple(multiple);
        valleyElectricPSR.setElectricValue(numberFormat((valleyCurValue - valleyLastValue) * multiple));
        valleyElectricPSR.setElectricPrice(valleyPrice);
        valleyElectricPSR.setFee(numberFormat(((valleyCurValue - valleyLastValue) * multiple) * valleyPrice));
        totalElectricityList.add(valleyElectricPSR);

        return totalElectricityList;
    }

    /**
     * 获取指定月份的光伏电站总发电量,峰谷平数据通过总发电量计算获取;
     * @param date 指定年月
     * @param stationId 指定厂站
     * @return
     */
    public List<ElectricPSR> getTotalElectricityByDate(String date, String stationId){
        List<ElectricPSR> totalElectricityList = new ArrayList<>();

        String tableName = "KWH" + date.substring(0, 4) + date.substring(5, 7);
        String name = scdDeviceDao.getCombinedCabName(stationId, DeviceDataType.SCD_TYPE_STATION.getValue(), DeviceDataType.FORWARD_PKWH_TOTAL.getValue());

        String firstDayOfMonth = getFirstDayOfMonth(date);
        String LastDayOfMonth = getLastDayOfMonth(date);
        int startDays = getDaysByDate(firstDayOfMonth);
        int endDays = getDaysByDate(LastDayOfMonth);
        if (startDays > endDays) return null;

        //判断是否当前月,如果是,则总发电量为前一天24点的数据
        Date dt=new Date();
        SimpleDateFormat matter1=new SimpleDateFormat("yyyy/MM/dd");
        String curDate = matter1.format(dt);
        if (curDate.contains(date)){
            endDays = endDays -1;
        }

        List<ElectricTime> peakElectricTimeList = new ArrayList<>();
        List<ElectricTime> normalElectricTimeList = new ArrayList<>();
        List<ElectricTime> valleyElectricTimeList = new ArrayList<>();
        Map<String, ElectricPrice> nameToElectricPriceMap = getNameToElectricPriceMap();
        ElectricPrice electricPrice;
        Double peakPrice = 0.00, normalPrice = 0.00, valleyPrice = 0.00;
        if(nameToElectricPriceMap.containsKey("峰值电价"))
        {
            electricPrice = nameToElectricPriceMap.get("峰值电价");
            peakElectricTimeList.addAll(electricPrice.getElectricTimes());
            peakPrice = electricPrice.getPrice();
        }
        if(nameToElectricPriceMap.containsKey("平期电价"))
        {
            electricPrice = nameToElectricPriceMap.get("平期电价");
            normalElectricTimeList.addAll(electricPrice.getElectricTimes());
            normalPrice = electricPrice.getPrice();
        }
        if(nameToElectricPriceMap.containsKey("谷值电价"))
        {
            electricPrice = nameToElectricPriceMap.get("谷值电价");
            valleyElectricTimeList.addAll(electricPrice.getElectricTimes());
            valleyPrice = electricPrice.getPrice();
        }

        List<String> nameList = new ArrayList<>();
        List<Integer> dataDayList = new ArrayList<>();
        List<Integer> timeList = new ArrayList<>();
        nameList.add(name);
        for (int tmpDays = startDays; tmpDays <= endDays; tmpDays++){
            dataDayList.add(tmpDays);
        }

        List<ElectricTime> electricTimeList = electricTimeDao.getElectricTimeList();
        for (ElectricTime electricTime : electricTimeList){
            int startTime = Integer.parseInt(electricTime.getStartTime());
            int endTime = Integer.parseInt(electricTime.getEndTime());
            if (!timeList.contains(startTime)){
                timeList.add(startTime);
            }
            if (!timeList.contains(endTime)){
                timeList.add(endTime);
            }
        }

        List<HistoryDataValue> electricDataList = historyDataDao.getHistoryDataByRangeDateTime(tableName,nameList, dataDayList,timeList,DataType.KWH_VALUE.getValue());
//        List<HistoryDataValue> electricDataList = historyDataDao.getHistoryDataByDuringDateTime(tableName, name, startDays,endDays,duringTimes,52);

        Map<String,HistoryDataValue> datetimeToHistoryDataMap = new HashMap();
        if (electricDataList.size() > 0){
            for (HistoryDataValue historyDataValue : electricDataList){
                datetimeToHistoryDataMap.put((historyDataValue.getSdate().toString()+historyDataValue.getTime().toString()),historyDataValue);
            }
        }

        Double totalLastElectricValue =0.00;
        Double totalCurElectricValue = 0.00;

        String totalLastTime = String.valueOf(startDays) +String.valueOf( HisDataTime.FIRST_TIME.getValue());
        String totalCurTime = String.valueOf(endDays) + String.valueOf(HisDataTime.END_TIME.getValue());
        if (datetimeToHistoryDataMap.containsKey(totalLastTime)){
            totalLastElectricValue = datetimeToHistoryDataMap.get(totalLastTime).getData();
        }
         if (datetimeToHistoryDataMap.containsKey(totalCurTime)){
            totalCurElectricValue = datetimeToHistoryDataMap.get(totalCurTime).getData();
        }

        ElectricPSR totalElectricPSR = new ElectricPSR();
        totalElectricPSR.setName("总发电量");
        totalElectricPSR.setCurValue(numberFormat(totalCurElectricValue));
        totalElectricPSR.setLastValue(numberFormat(totalLastElectricValue));
        totalElectricPSR.setElectricValue(numberFormat(totalCurElectricValue - totalLastElectricValue));
        totalElectricPSR.setElectricPrice(null);
        totalElectricPSR.setFee(null);
        totalElectricityList.add(totalElectricPSR);

        Double peakElectricValue = 0.00;
        Double normalElectricValue = 0.00;
        Double valleyElectricValue = 0.00;
        int i = 0;
        for (i = startDays; i <= endDays; i++ ){
            for (ElectricTime peakElectricTime : peakElectricTimeList){
                Double startPeakValue = 0.00;
                Double endPeakValue = 0.00;
                String startPeakTime = i + peakElectricTime.getStartTime();
                String endPeakTime = i + peakElectricTime.getEndTime();
                if (datetimeToHistoryDataMap.containsKey(startPeakTime)){
                    startPeakValue = datetimeToHistoryDataMap.get(startPeakTime).getData();
                }
                if (datetimeToHistoryDataMap.containsKey(endPeakTime)){
                    endPeakValue = datetimeToHistoryDataMap.get(endPeakTime).getData();
                }
                peakElectricValue += (endPeakValue - startPeakValue);
            }
            for (ElectricTime normalElectricTime : normalElectricTimeList){
                Double startNormalValue = 0.00;
                Double endNormalValue = 0.00;
                String startNormalTime = i + normalElectricTime.getStartTime();
                String endNormalTime = i + normalElectricTime.getEndTime();
                if (datetimeToHistoryDataMap.containsKey(startNormalTime)){
                    startNormalValue = datetimeToHistoryDataMap.get(startNormalTime).getData();
                }
                if (datetimeToHistoryDataMap.containsKey(endNormalTime)){
                    endNormalValue = datetimeToHistoryDataMap.get(endNormalTime).getData();
                }
                normalElectricValue += (endNormalValue - startNormalValue);
            }
            for (ElectricTime valleyElectricTime : valleyElectricTimeList){
                Double startValleyValue = 0.00;
                Double endValleyValue = 0.00;
                String startValleyTime = i + valleyElectricTime.getStartTime();
                String endValleyTime = i + valleyElectricTime.getEndTime();
                if (datetimeToHistoryDataMap.containsKey(startValleyTime)){
                    startValleyValue = datetimeToHistoryDataMap.get(startValleyTime).getData();
                }
                if (datetimeToHistoryDataMap.containsKey(endValleyTime)){
                    endValleyValue = datetimeToHistoryDataMap.get(endValleyTime).getData();
                }
                valleyElectricValue += (endValleyValue - startValleyValue);
            }
        }
        ElectricPSR peakElectricPSR = new ElectricPSR();
        peakElectricPSR.setName("峰值电量");
        peakElectricPSR.setCurValue(0.000);
        peakElectricPSR.setLastValue(0.000);
        peakElectricPSR.setElectricValue(numberFormat(peakElectricValue));
        peakElectricPSR.setElectricPrice(peakPrice);
        peakElectricPSR.setFee(numberFormat(peakElectricValue * peakPrice));
        totalElectricityList.add(peakElectricPSR);

        ElectricPSR NormalElectricPSR = new ElectricPSR();
        NormalElectricPSR.setName("平期电量");
        NormalElectricPSR.setCurValue(0.000);
        NormalElectricPSR.setLastValue(0.000);
        NormalElectricPSR.setElectricValue(numberFormat(normalElectricValue));
        NormalElectricPSR.setElectricPrice(normalPrice);
        NormalElectricPSR.setFee(numberFormat(normalElectricValue * normalPrice));
        totalElectricityList.add(NormalElectricPSR);

        ElectricPSR valleyElectricPSR = new ElectricPSR();
        valleyElectricPSR.setName("谷值电量");
        valleyElectricPSR.setCurValue(0.000);
        valleyElectricPSR.setLastValue(0.000);
        valleyElectricPSR.setElectricValue(numberFormat(valleyElectricValue));
        valleyElectricPSR.setElectricPrice(valleyPrice);
        valleyElectricPSR.setFee(numberFormat(valleyElectricValue * valleyPrice));
        totalElectricityList.add(valleyElectricPSR);

        return totalElectricityList;
    }

    /**
     * 获取指定月上月的最后一天
     * @Title:getLastDayOfMonth
     * @Description:
     * @param:date
     * @return:String
     */
    public static String getLastDayOfLastMonth(String date)
    {
        int   year   =   Integer.parseInt(date.substring(0, 4));
        int   month   =   Integer.parseInt(date.substring(5, 7));
        Calendar cal = Calendar.getInstance();
        //设置年份
        cal.set(Calendar.YEAR,year);
        //设置月份
        cal.set(Calendar.MONTH, month-2);
        //获取某月最大天数
        int lastDay = cal.getActualMaximum(Calendar.DAY_OF_MONTH);
        //设置日历中月份的最大天数
        cal.set(Calendar.DAY_OF_MONTH, lastDay);
        //格式化日期
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy/MM/dd");
        String lastDayOfMonth = sdf.format(cal.getTime());

        return lastDayOfMonth;
    }

    /**
     * 获取指定月的第一天
     * @Title:getFirstDayOfMonth
     * @Description:
     * @param:date
     * @return:String
     */
    public static String getFirstDayOfMonth(String date){
        int   year   =   Integer.parseInt(date.substring(0, 4));
        int   month   =   Integer.parseInt(date.substring(5, 7));
        Calendar cal = Calendar.getInstance();
        //设置年份
        cal.set(Calendar.YEAR,year);
        //设置月份
        cal.set(Calendar.MONTH, month-1);
        //设置天数
        cal.set(Calendar.DAY_OF_MONTH, 1);
        //格式化日期
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy/MM/dd");
        String firstDayOfMonth = sdf.format(cal.getTime());

        return firstDayOfMonth;
    }

    /**
     * 获取指定月的最后一天
     * @Title:getLastDayOfMonth
     * @Description:
     * @param:date
     * @return:String
     */
    public static String getLastDayOfMonth(String date)
    {
        Date dt=new Date();
        SimpleDateFormat matter1=new SimpleDateFormat("yyyy/MM/dd");
        String curDate = matter1.format(dt);
        if (curDate.contains(date)){
            return curDate;
        }

        int   year   =   Integer.parseInt(date.substring(0, 4));
        int   month   =   Integer.parseInt(date.substring(5, 7));
        Calendar cal = Calendar.getInstance();
        //设置年份
        cal.set(Calendar.YEAR,year);
        //设置月份
        cal.set(Calendar.MONTH, month-1);
        //获取某月最大天数
        int lastDay = cal.getActualMaximum(Calendar.DAY_OF_MONTH);
        //设置日历中月份的最大天数
        cal.set(Calendar.DAY_OF_MONTH, lastDay);
        //格式化日期
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy/MM/dd");
        String lastDayOfMonth = sdf.format(cal.getTime());

        return lastDayOfMonth;
    }

    /**
     * 获取指定日期的天数,天数从1970年到现在的.
     * @Title:getDaysByDate
     * @Description:
     * @param:date
     * @return:天数
     */
    public static int getDaysByDate(String date){
        SimpleDateFormat dateParse = new SimpleDateFormat("yyyy/MM/dd");
        Calendar   c   =   Calendar.getInstance();
        try {
            Date tmpDate = dateParse.parse((date));
            c.setTime(tmpDate);
        } catch (ParseException e) {
            return 0;
        }
        long   tmpDates   =   c.getTime().getTime();
        int days = (int) ((tmpDates / 1000 / 3600 + 8) / 24);
        return days;
    }

    /**
     * 获取当前日期
     * @Title:getCurDate
     * @param:
     * @return:yyyy年mm月dd日
     */
    public String getCurDate(){
        Date dt=new Date();
        SimpleDateFormat matter1=new SimpleDateFormat("yyyy/MM/dd");
        String curDate = matter1.format(dt);
        int   curYear   =   Integer.parseInt(curDate.substring(0, 4));
        int   curMonth   =   Integer.parseInt(curDate.substring(5, 7));

        String printDate = curYear + "年" + curMonth + "月";

        return printDate;
    }

    /**
     * 获取指定的电价
     * @Title:getPriceByName
     * @param:name 电价描述
     * @return:电价
     */
    public Double getPriceByName(String name){
        Map<String, ElectricPrice> nameToElectricPriceMap = getNameToElectricPriceMap();
        ElectricPrice electricPrice;
        Double price;
        if(nameToElectricPriceMap.containsKey(name))
        {
            electricPrice = nameToElectricPriceMap.get(name);
            price = electricPrice.getPrice();
        }
        else {
            price = 0.00;
        }
        return  price;
    }

    /**
     * 保留2位小数
     * @param:f Double
     * @return:
     */
    public Double changeValue(Double f) {
        DecimalFormat df = new DecimalFormat("#.00");
        Double value =  Double.parseDouble(df.format(f));
        return value;
    }

   /**
     * 保留3位小数
     * @param:f Double
     * @return:
     */
    public Double numberFormat(Double f) {
        DecimalFormat df = new DecimalFormat("#.000");
        Double value =  Double.parseDouble(df.format(f));
        return value;
    }

    /**
     * 金额小写转换大写
     * @Title:getPriceByName
     * @param:n Double
     * @return:大写金额字符串
     */
    public static String digitUppercase(Double n){
        String fraction[] = {"角", "分"};
        String digit[] = { "零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖" };
        String unit[][] = {{"元", "万", "亿"},
                {"", "拾", "佰", "仟"}};

        String head = n < 0? "负": "";
        n = Math.abs(n);

        String s = "";
        for (int i = 0; i < fraction.length; i++) {
            s += (digit[(int)(Math.floor(n * 10 * Math.pow(10, i)) % 10)] + fraction[i]).replaceAll("(零.)+", "");
        }
        if(s.length()<1){
            s = "整";
        }
        int integerPart = (int)Math.floor(n);

        for (int i = 0; i < unit[0].length && integerPart > 0; i++) {
            String p ="";
            for (int j = 0; j < unit[1].length && n > 0; j++) {
                p = digit[integerPart%10]+unit[1][j] + p;
                integerPart = integerPart/10;
            }
            s = p.replaceAll("(零.)*零$", "").replaceAll("^$", "零") + unit[0][i] + s;
        }
        return head + s.replaceAll("(零.)*零元", "元").replaceFirst("(零.)+", "").replaceAll("(零.)+", "零").replaceAll("^整$", "零元整");
    }

    /**
     * 获取厂站详细信息
     * @param:psrId 厂站代码
     * @return:
     */
    public ElectricStationInfo getStationInfoById(String psrId){
        PSRTemplateMapping psrTemplateMapping = psrTemplateMappingDao.getPSRTemplateMapping(psrId);
        ElectricStationInfo electricStationInfo = psrTemplateMapping.getElectricStationInfo();
        return  electricStationInfo == null ? null : electricStationInfo;
    }
}
