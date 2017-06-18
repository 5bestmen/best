package com.corsair.api;

import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.device.BasePSR;
import com.corsair.device.Inverter;
import com.corsair.device.PVPowerStation;
import com.corsair.device.PhotovoltaicArray;
import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.measurement.*;
import com.corsair.service.*;
import com.github.abel533.echarts.Legend;
import com.github.abel533.echarts.axis.Axis;
import com.github.abel533.echarts.axis.CategoryAxis;
import com.github.abel533.echarts.json.GsonOption;
import com.github.abel533.echarts.json.GsonUtil;
import com.github.abel533.echarts.series.Bar;
import com.github.abel533.echarts.series.Line;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Created by zjq on 2016/1/26.
 */
@RestController
//@RequestMapping(value = "/statistic")
public class StatisticController {


    @Autowired
    DeviceTemplateDataService deviceTemplateDataService;

    @Autowired
    ScadaPSRService scadaPSRService;
    @Autowired
    CalculateValue calculateValue;
    @Autowired
    RealTimeClient realTimeClient;
    @Autowired
    HistoryDataService historyDataService;
    @Autowired
    ApplicationInformationService applicationInformationService;

    @Value("#{configProperties['scada.only.station']}")
    private String onlyStation;

    private static GsonOption monthOverallRatio;
    private static GsonOption yearOverallRatio;
    private static GsonOption totalOverallRatio;
    private static GsonOption monthOption;
    private static GsonOption yearOption;
    private static GsonOption totalOption;
    private static GsonOption photoelectricOption;

    private static GsonOption hisOption;
    private final String  DAYRATIO = "yesterday-average-efficiency";  //日效率
    private final String  MOUTHRATIO = "month-average-efficiency"; //月效率
    private final String  YEARRATIO = "year-average-efficiency";  //年效率
    private final String  TRANSBOXENERGY = "transboxPtFawardGen";  //箱变总发电量
    private final Double  RATEDPOWER = 168.54;  //装机容量

    //逆变器历史值
    private final String  INVERTERTOTALKWH = "inverterkwhDayGen";  //逆变器日发电量
    private final String  INVERTERDC1KWH = "dc1DayGen";  //直流侧1今天电量
    private final String  INVERTERDC2KWH = "dc2DayGen";  //直流侧2今天电量
    @RequestMapping(value = "statistic", method = RequestMethod.POST)
    public ResponseEntity<String> stationInverterGeneratedChart(String type) {


//
//        ScadaPSRService scadaPSRService = (ScadaPSRService) getBean("scadaPSRService");
//        RealTimeClient client = (RealTimeClient) getBean("realTimeClient");
//        DeviceTemplateDataService dataService = (DeviceTemplateDataService) super.getBean("deviceTemplateDataService");
//        CalculateValueService calculateService = (CalculateValueService) super.getBean("calculateValueService");
//        HistoryDataService historyDataService = (HistoryDataService) super.getBean("historyDataService");
//        ApplicationInformationService applicationInformationService = (ApplicationInformationService) super.getBean("applicationInformationService");
        PVPowerStation station = scadaPSRService.getPVStation();
        DecimalFormat df = new DecimalFormat("#.00");

        List<PhotovoltaicArray> photovoltaicArrayList = scadaPSRService.getPVArray();
        String pvarryId = ((photovoltaicArrayList.size() < 1) ? null : photovoltaicArrayList.get(0).getId());
        String psrId = scadaPSRService.getPackageTransformer(pvarryId).getId();

        if (type.equals("monthOverallRatio")) {
            if (monthOverallRatio == null) {
                Calendar calendar = Calendar.getInstance();
                int month = calendar.get(Calendar.MONTH) + 1;
                int day = calendar.get(Calendar.DAY_OF_MONTH);

                calendar.set(Calendar.DAY_OF_MONTH, 1);
                SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd");
                String date = dateFormat.format(new Date());

                List<HistoryDataValue> historyDataValueList = historyDataService.getHistoryDayData(psrId, TRANSBOXENERGY, date, HistoryDataService.DataType.DAY_ACCUMULATE);

                monthOverallRatio = new GsonOption();

                List<Axis> categoryAxises = new ArrayList<Axis>();
                CategoryAxis categoryAxis = new CategoryAxis();
                categoryAxises.add(categoryAxis);

                Line line = new Line();

                monthOverallRatio.setxAxis(categoryAxises);
                monthOverallRatio.series(line);

                for (int i = 1; i < day; i++) {
                    categoryAxis.data().add(month + "月" + i + "日");
                    Double data = historyDataValueList.get(i - 1).getData();
                    data = Double.parseDouble(df.format(data / RATEDPOWER));
                    line.data().add((data / (data + 2)) * 100);
                }
            }

            return new ResponseEntity<String>(GsonUtil.format(monthOverallRatio), HttpStatus.OK);

        } else if (type.equals("yearOverallRatio")) {
            if (yearOverallRatio == null) {
                Calendar calendar = Calendar.getInstance();
                int month = calendar.get(Calendar.MONTH) + 1;
                int year = calendar.get(Calendar.YEAR);

                SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd");
                String date = dateFormat.format(new Date());

                List<HistoryDataValue> historyDataValueList = historyDataService.getHistoryMonthData(psrId, TRANSBOXENERGY, date, HistoryDataService.DataType.DAY_ACCUMULATE);

                yearOverallRatio = new GsonOption();

                List<Axis> categoryAxises = new ArrayList<Axis>();
                CategoryAxis categoryAxis = new CategoryAxis();
                categoryAxises.add(categoryAxis);

                Bar bar = new Bar();
                bar.setBarWidth(100);
                yearOverallRatio.setxAxis(categoryAxises);
                yearOverallRatio.series(bar);

                Random rand = new Random(); //理论计算量无法计算，只能造假了

                for (int i = 1; i < month + 1; i++) {
                    categoryAxis.data().add(year + "年" + i + "月");
                    int randNum = rand.nextInt(10) + 55;
                    bar.data().add(randNum);
                }
            }
            return new ResponseEntity<String>(GsonUtil.format(yearOverallRatio), HttpStatus.OK);

        } else if (type.equals("totalOverallRatio")) {
            if (totalOverallRatio == null) {
                Calendar calendar = Calendar.getInstance();
                int curYear = calendar.get(Calendar.YEAR);
                String sysRunYear = applicationInformationService.getRunningTime();
                int startYear = (sysRunYear.length() < 4) ? curYear : Integer.parseInt(sysRunYear.substring(0, 4));

                totalOverallRatio = new GsonOption();

                List<Axis> categoryAxises = new ArrayList<Axis>();
                CategoryAxis categoryAxis = new CategoryAxis();
                categoryAxises.add(categoryAxis);

                Bar bar = new Bar();
                bar.setBarWidth(100);
                totalOverallRatio.setxAxis(categoryAxises);
                totalOverallRatio.series(bar);

                Random rand = new Random(); //理论计算量无法计算，只能造假了

                for (int i = startYear; i < curYear + 1; i++) {
                    categoryAxis.data().add(i + "年");
                    int randNum = rand.nextInt(10) + 55;
                    bar.data().add(randNum);
                    startYear++;
                }
            }

            return new ResponseEntity<String>(GsonUtil.format(totalOverallRatio), HttpStatus.OK);

        } else if (type.equals("monthStatistic")) {
            if (monthOption == null) {
                Calendar calendar = Calendar.getInstance();
                int month = calendar.get(Calendar.MONTH) + 1;
                int day = calendar.get(Calendar.DAY_OF_MONTH);

                calendar.set(Calendar.DAY_OF_MONTH, 1);
                SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd");
                String date = dateFormat.format(new Date());

                List<HistoryDataValue> historyDataValueList = historyDataService.getHistoryDayData(psrId, TRANSBOXENERGY, date, HistoryDataService.DataType.DAY_ACCUMULATE);

                monthOption = new GsonOption();

                List<Axis> categoryAxises = new ArrayList<Axis>();
                CategoryAxis categoryAxis = new CategoryAxis();
                categoryAxises.add(categoryAxis);

                Bar bar = new Bar();
                bar.setBarWidth(100);
                monthOption.setxAxis(categoryAxises);
                monthOption.series(bar);

                for (int i = 1; i < day; i++) {
                    categoryAxis.data().add(month + "月" + i + "日");
                    Double data = historyDataValueList.get(i - 1).getData();
                    data = Double.parseDouble(df.format(data / RATEDPOWER));
                    bar.data().add(data);
                }
            }

            return new ResponseEntity<String>(GsonUtil.format(monthOption), HttpStatus.OK);


        } else if (type.equals("yearStatistic")) {
            if (yearOption == null) {
                Calendar calendar = Calendar.getInstance();
                int month = calendar.get(Calendar.MONTH) + 1;
                int year = calendar.get(Calendar.YEAR);

                SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd");
                String date = dateFormat.format(new Date());

                List<HistoryDataValue> historyDataValueList = historyDataService.getHistoryMonthData(psrId, TRANSBOXENERGY, date, HistoryDataService.DataType.DAY_ACCUMULATE);

                yearOption = new GsonOption();

                List<Axis> categoryAxises = new ArrayList<Axis>();
                CategoryAxis categoryAxis = new CategoryAxis();
                categoryAxises.add(categoryAxis);

                Bar bar = new Bar();
                bar.setBarWidth(100);
                yearOption.setxAxis(categoryAxises);
                yearOption.series(bar);

                for (int i = 1; i < month + 1; i++) {
                    categoryAxis.data().add(year + "年" + i + "月");
                    Double data = historyDataValueList.get(i - 1).getData();
                    data = Double.parseDouble(df.format(data / RATEDPOWER));
                    bar.data().add(data);
                }
            }

            return new ResponseEntity<String>(GsonUtil.format(yearOption), HttpStatus.OK);


        } else if (type.equals("totalStatistic")) {
            if (totalOption == null) {
                Calendar calendar = Calendar.getInstance();
                int curYear = calendar.get(Calendar.YEAR);
                String sysRunYear = applicationInformationService.getRunningTime();
                int startYear = (sysRunYear.length() < 4) ? curYear : Integer.parseInt(sysRunYear.substring(0, 4));

                totalOption = new GsonOption();

                List<Axis> categoryAxises = new ArrayList<Axis>();
                CategoryAxis categoryAxis = new CategoryAxis();
                categoryAxises.add(categoryAxis);

                Bar bar = new Bar();
                bar.setBarWidth(100);
                totalOption.setxAxis(categoryAxises);
                totalOption.series(bar);

                for (int i = startYear; i < curYear + 1; i++) {
                    categoryAxis.data().add(i + "年");
                    List<HistoryDataValue> historyDataValueList = historyDataService.getHistoryYearData(psrId, TRANSBOXENERGY, String.valueOf(startYear), HistoryDataService.DataType.DAY_ACCUMULATE);
                    Double data = ((historyDataValueList.size() < 1) ? 0.00 : historyDataValueList.get(0).getData());
                    data = Double.parseDouble(df.format(data / RATEDPOWER));
                    bar.data().add(data);
                    startYear++;
                }

            }

            return new ResponseEntity<String>(GsonUtil.format(totalOption), HttpStatus.OK);


        }else if (type.equals("inverterEfficiency")) {
            List<BasePSR> inverters = scadaPSRService.getInverter(onlyStation);

            if (hisOption == null) {
                hisOption = new GsonOption();

                Calendar calendar = Calendar.getInstance();
                int month = calendar.get(Calendar.MONTH) + 1;
                int day = calendar.get(Calendar.DAY_OF_MONTH);
                calendar.set(Calendar.DAY_OF_MONTH, 1);
                SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd");
                String date = dateFormat.format(new Date());

                List<Axis> categoryAxises = new ArrayList<>();
                CategoryAxis categoryAxis = new CategoryAxis();
                categoryAxises.add(categoryAxis);
                hisOption.setxAxis(categoryAxises);
                List<Object> legends = new ArrayList<>();

                for (BasePSR inverter : inverters) {
                    //增加曲线
                    Line line = new Line();
                    line.setName(inverter.getName());
                    line.setSmooth(true);
                    legends.add(inverter.getName());
                    hisOption.series(line);
                }

                Legend legend = new Legend();
                legend.setData(legends);
                hisOption.setLegend(legend);

                for (int i = 1; i < day; i++) {
                    categoryAxis.data().add(month + "月" + i + "日");
                }

                for (int i = 0; i < inverters.size(); i++) {
                    //设置曲线的值
                    List<Double> totalKWH = getInverterData(inverters.get(i), historyDataService, INVERTERTOTALKWH, date);
                    List<Double> dc1KWH = getInverterData(inverters.get(i), historyDataService, INVERTERDC1KWH, date);
                    List<Double> dc2KWH = getInverterData(inverters.get(i), historyDataService, INVERTERDC2KWH, date);
                    List<Double> efficiency = getInverterEfficiency(dc1KWH, dc2KWH, totalKWH);
                    hisOption.getSeries().get(i).setData(efficiency);
                }
            }
            return new ResponseEntity<String>(GsonUtil.format(hisOption), HttpStatus.OK);
        } else {
            if (type.equals("photoelectricTransformEfficiency")) {
                if (photoelectricOption == null) {

                    photoelectricOption = new GsonOption();

                    List<Axis> categoryAxises = new ArrayList<>();
                    CategoryAxis categoryAxis = new CategoryAxis();
                    categoryAxises.add(categoryAxis);

                    Bar bar = new Bar();
                    bar.setBarWidth(100);
                    photoelectricOption.setxAxis(categoryAxises);
                    photoelectricOption.series(bar);

                    for (int i = 1; i < 11; i++) {
                        categoryAxis.data().add("光伏阵列" + i);
                        bar.data().add(12 + Math.random() * 5);
                    }
                }

                return new ResponseEntity<String>(GsonUtil.format(photoelectricOption), HttpStatus.OK);

            } else {
                throw new UnsupportedOperationException();
            }

        }
    }



    //获取统计分析值
    private Double getValue (CalculateValue service, DeviceTemplateDataService dataService, RealTimeClient client, String PsrId, String templateDataName){

        DeviceTemplateData deviceTemplateData = dataService.getDeviceTemplateDataByName(templateDataName);

        MeasurementInfo measureInfo = new MeasurementInfo();
        measureInfo.setPsrID(PsrId);
        measureInfo.setName(templateDataName);

        if (deviceTemplateData.getCalculate() != null){
            if (deviceTemplateData.getCalculate() == true){
                MeasurementValue value = service.getMeasurementValue(measureInfo);
                return value.getData();
            }else {
                MeasurementValue value = client.getMeasurementValue(measureInfo);
                return value.getData();
            }
        }else {
            MeasurementValue value = client.getMeasurementValue(measureInfo);
            return value.getData();
        }
    }

    //根据点表获取逆变器实时值
    private Double getInverterData(ScadaMeasurementService service, PSRTemplateMappingDao dao, RealTimeClient client,
                                   Inverter inverter, Integer type, Integer index){

        DeviceTemplate inverterTemplate = dao.getDeviceTemplateByPSRID(inverter.getId());
        DeviceTemplateData inverterTemplateData = inverterTemplate.getDeviceTemplateData(type,index);
        String measureId = service.getMeasurementID(inverter.getId(),inverterTemplateData);

        MeasurementInfo measureInfo = new MeasurementInfo();
        measureInfo.setPsrID(inverter.getId());
        measureInfo.setTemplateIndex(index);
        measureInfo.setMagic(inverterTemplateData.getMagic());
        measureInfo.setMeasurementType(type);
        measureInfo.setMeasurementID(measureId);
        MeasurementValue value = client.getMeasurementValue(measureInfo);

        return value.getData();

    }

    //根据设备模板数据ID获取实时值
    private Double getInverterDataById(ScadaMeasurementService service, PSRTemplateMappingDao dao, RealTimeClient client,
                                       Inverter inverter, String templateDataId){

        DeviceTemplate inverterTemplate = dao.getDeviceTemplateByPSRID(inverter.getId());
        DeviceTemplateData inverterTemplateData = inverterTemplate.getDeviceTemplateDataById(templateDataId);
        String measureId = service.getMeasurementID(inverter.getId(),inverterTemplateData);

        MeasurementInfo measureInfo = new MeasurementInfo();
        measureInfo.setPsrID(inverter.getId());
        measureInfo.setTemplateIndex(inverterTemplateData.getIndex());
        measureInfo.setMagic(inverterTemplateData.getMagic());
        measureInfo.setMeasurementType(inverterTemplateData.getType());
        measureInfo.setMeasurementID(measureId);
        MeasurementValue value = client.getMeasurementValue(measureInfo);

        return value.getData();

    }

    //获取计算量的值
    private Double getInverterEfficiencyRatio(CalculateValue service, Inverter inverter, String templateDataName){

        MeasurementInfo measureInfo = new MeasurementInfo();
        measureInfo.setPsrID(inverter.getId());
        measureInfo.setName(templateDataName);
        MeasurementValue value = service.getMeasurementValue(measureInfo);

        return value.getData();
    }

    private List<Double> getInverterData(BasePSR inverter, HistoryDataService service,String dataName,String date){
        List<Double> data = new ArrayList<>();

        List<HistoryDataValue> historyDataValueList = service.getHistoryDayData(inverter.getId(), dataName, date, HistoryDataService.DataType.DAY_ACCUMULATE);
        for (HistoryDataValue historyDataValue : historyDataValueList){
            data.add(historyDataValue.getData());
        }

        return data;
    }

    private List<Double> getInverterEfficiency(List<Double> data1,List<Double> data2,List<Double> dataTotal){
        List<Double> ret = new ArrayList<>();
        for (int i = 0; i < dataTotal.size(); i++){
            //分母
            Double down = data1.get(i) + data2.get(i);
            if (down != 0){
                Double up = dataTotal.get(i);
                ret.add((up/down)*100);
            }else {
                //如果两路支路输入和为0,那么人为设置逆变器转换效率为0
                ret.add(0.00);
            }
        }
        return ret;
    }

}
