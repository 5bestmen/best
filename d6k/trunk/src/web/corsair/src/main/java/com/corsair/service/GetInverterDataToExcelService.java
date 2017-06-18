package com.corsair.service;

import com.corsair.dao.HistoryDataDao;
import com.corsair.dao.PSRTemplateMappingDao;
import com.corsair.export.ExcelExportData;
import com.corsair.export.InverterData;

import com.corsair.measurement.HistoryDataValue;
import com.corsair.measurement.magic.InverterDataValue;
import com.corsair.util.ExcelUtil;
import com.corsair.util.FileSearcher;
import com.corsair.util.JavaMailWithAttachment;
import com.rbac.entity.MailReceiver;
import com.rbac.service.MailReceiveService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

import java.io.File;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Created by zjq on 2015/12/29.
 */
@Service("getInverterDataToExcelService")
public class GetInverterDataToExcelService {

    @Autowired
    ScadaMeasurementService scadaMeasurementService;
    @Autowired
    PSRTemplateMappingDao psrTemplateMappingDao;
    @Autowired
    HistoryDataService historyDataService;
    @Autowired
    MailReceiveService mailReceiveService;
    @Autowired
    HistoryDataDao historyDataDao;//计算度电补贴用

    @Scheduled(cron="0 10 20 * * ?" )//每天20:00触发
//     @Scheduled(fixedDelay = 15000)
     public void GenerateExcelData(){

         getDataToExcel();
         //定时发送邮件
         List<MailReceiver> listReceives = mailReceiveService.getMailReceives("", "");
         String mail_address="";//收件人地址 第一次加载给所有人发邮件
         for (MailReceiver mailReceiver:listReceives){
             mail_address+=mailReceiver.getMailAddress()+",";
         }
         mail_address = mail_address.substring(0,mail_address.length()-1);
         JavaMailWithAttachment se = new JavaMailWithAttachment(true);
         File folder = new File(getMailFileName());// 默认目录
         DateFormat tem_date = new SimpleDateFormat("yyyy-MM-dd");
         String time = tem_date.format(new Date());
         String keyword = time;
         if (!folder.exists()) {// 如果文件夹不存在
             System.out.println("目录不存在：" + folder.getAbsolutePath());
             return;
         }
         File[] result = FileSearcher.searchFile(folder, keyword);// 调用方法获得文件数组
         ArrayList<File> affix = null;
         ArrayList<File> files = new ArrayList<>();
         List<String> filePath = new ArrayList<>();
         for (int i = 0; i < result.length; i++) {// 循环显示文件
             File file = result[i];
             System.out.println(file.getAbsolutePath() + " ");// 显示文件绝对路径
             filePath.add(file.getAbsolutePath());
         }
         for (String url:filePath){
             affix=JavaMailWithAttachment.getListFiles(url);
             files.addAll(affix);
         }
         if(files.size()>0) {//15151835907@163.com,zhangy@sznari.com,zhangjq@sznari.com
             se.doSendHtmlEmail("当天逆变器数据", "附件内容是今天的逆变器数据", mail_address, files);//
         }
    }

    private void getDataToExcel(){
        String[] inverterPsrIds = {"0027oi815kl9ce","0027oi815m2kbh","0027oi815q1ak7","0027oi8159gp05","0027oi8159vjnj"};
        String[] dataNames = {"inverterIpv1","inverterUpv1","inverterPpv1","inverterIpv2","inverterUpv2","inverterPpv2","inverterOutPower","dc1DayGen","dc2DayGen","inverterkwhDayGen","inverterEfficiency","inverterkwhTotalTime","inverterkwhTotalGen"};
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        Calendar todayNow = Calendar.getInstance();
        int beginMinutes = 0;
        int endMinutes = todayNow.get(Calendar.HOUR_OF_DAY) * 60 + todayNow.get(Calendar.MINUTE);
        String date = dateFormat.format(todayNow.getTime());
        List<InverterDataValue> inverterDataValues = new ArrayList<>();


        int currentHours = 0;
        for (int i = 0; i < inverterPsrIds.length; i++){
            InverterDataValue inverterDataValue = new InverterDataValue();
            Double[][] hValue = new Double[13][24];

            for (int j = 0; j < 7; j++){
                hValue[j] = getHourDatas(inverterPsrIds[i], dataNames[j], date, beginMinutes, endMinutes, HistoryDataService.DataType.REALTIME_VALUE);
            }

            Double normalPower = 31.8d;
            if (i < 3){
                normalPower = 34.98;
            }

//计算电度量数据
            for (int k = hValue[5].length - 1; k >= 0; k--){
                if (hValue[5][k] != null){
                    currentHours = k;
                    hValue[7][k] = getTotalDatas(inverterPsrIds[i],"dc1DayGen",date, beginMinutes, endMinutes, HistoryDataService.DataType.KWH_VALUE);
                    hValue[8][k] = getTotalDatas(inverterPsrIds[i],"dc2DayGen",date, beginMinutes, endMinutes, HistoryDataService.DataType.KWH_VALUE);
                    hValue[9][k] = getTotalDatas(inverterPsrIds[i],"inverterkwhDayGen",date, beginMinutes, endMinutes, HistoryDataService.DataType.KWH_VALUE);//当日发电量

                    hValue[10][k] = hValue[9][k]/(hValue[7][k] + hValue[8][k]) * 100;//逆变器效率
                    hValue[11][k] = hValue[9][k]/normalPower;//子系统当天有效小时数
                    hValue[12][k] = getTotalDatas(inverterPsrIds[i],"inverterkwhTotalGen",date, beginMinutes, endMinutes, HistoryDataService.DataType.KWH_VALUE);//累计量

                    break;
                }
            }

            inverterDataValue.setCommonValues(hValue);

            if (i == 2){
                //读取直流表的值
                String[] energyWatchPsrIds = {"0026oi8150q6lg","0026oi815ki547","0026oi8153js7f","0026oi8151sf5j","0026oi815vkobc","0026oi8159jehn"};
                String[] dcWatchDataNames = {"Udc","Idc","efficiency"};//删掉,"Pdc"
                Double[][][] dcValues = new Double[3][6][24];

                for (int j = 0; j < energyWatchPsrIds.length; j++){
                    for (int k = 0; k < dcWatchDataNames.length; k++){
                        dcValues[k][j] = getHourDatas(energyWatchPsrIds[j], dcWatchDataNames[k], date, beginMinutes, endMinutes, HistoryDataService.DataType.REALTIME_VALUE);
                    }
                }

                inverterDataValue.setDcValues(dcValues);
                String[] inverterWatchDataNames = {"transboxUaver","transboxIaver","transboxPtFawardGen","transboxPtotal"};
                Double[][] specialValues = new Double[4][24];
                //读取逆变器9N表的值
                for (int k = 0; k < inverterWatchDataNames.length; k++){
                    specialValues[k] = getHourDatas("0022oi815spi7l", inverterWatchDataNames[k], date, beginMinutes, endMinutes, HistoryDataService.DataType.REALTIME_VALUE);
                }
                inverterDataValue.setSpecialValues(specialValues);
            }

            inverterDataValues.add(inverterDataValue);
        }
//获取度电补贴
        Double[][][] kwhValues = getKWHSubsidiesDatas("0052oi815tbsbl",currentHours,date,beginMinutes,endMinutes);


        //转换成Excel
        convertToFirstExcel(inverterDataValues,"firstModel",kwhValues,currentHours);
        convertToSecondExcel(inverterDataValues,"secondModel");

    }

    public void GenerateExcelData(String mail_address){
         getDataToExcel();
        //定时发送邮件
//        List<MailReceiver> listReceives = mailReceiveService.getMailReceives("", "");
//        for (MailReceiver mailReceiver:listReceives){
//            mail_address+=mailReceiver.getMailAddress()+",";
//        }
//        mail_address = mail_address.substring(0,mail_address.length()-1);
        JavaMailWithAttachment se = new JavaMailWithAttachment(true);
        File folder = new File(getMailFileName());// 默认目录
        DateFormat tem_date = new SimpleDateFormat("yyyy-MM-dd");
        String time = tem_date.format(new Date());
        String keyword = time;
        if (!folder.exists()) {// 如果文件夹不存在
            System.out.println("目录不存在：" + folder.getAbsolutePath());
            return;
        }
        File[] result = FileSearcher.searchFile(folder, keyword);// 调用方法获得文件数组
        ArrayList<File> affix = null;
        ArrayList<File> files = new ArrayList<>();
        List<String> filePath = new ArrayList<>();
        for (int i = 0; i < result.length; i++) {// 循环显示文件
            File file = result[i];
            System.out.println(file.getAbsolutePath() + " ");// 显示文件绝对路径
            filePath.add(file.getAbsolutePath());
        }
        for (String url:filePath){
            affix=JavaMailWithAttachment.getListFiles(url);
            files.addAll(affix);
        }
        if(files.size()>0) {//15151835907@163.com,zhangy@sznari.com,zhangjq@sznari.com
            se.doSendHtmlEmail("当天逆变器数据", "附件内容是今天的逆变器数据", mail_address, files);//
        }
    }


    public void convertToFirstExcel(List<InverterDataValue> inverterDataValues,String modelName,Double[][][] kwhValues,int currentHours) {
        List<String[]> columNames = new ArrayList<String[]>();
        List<String[]> fieldNames = new ArrayList<String[]>();

        LinkedHashMap<String, List<?>> map = new LinkedHashMap<String, List<?>>();
      /*  SimpleDateFormat format=new SimpleDateFormat("HH");
        String currentTime=format.format(new Date());
        int hours = Integer.parseInt(currentTime);*/
        for (int k = 0; k < currentHours+1; k++) {
            columNames.add(new String[]{"逆变器编号及额定功率", "逆变器MPPT回路", "逆变器端口号", "光伏串", "光伏串编号", "电流I(A)","电压V(V)","输入功率P(W)","输出功率P(W)","直流侧输入电量(kwh)","当日发电量（kwh）","逆变器效率(%)","子系统当天有效小时数（h）","累计发电量（kwh）"});
            fieldNames.add(new String[]{"InverterNumAndNominalPower", "MPPTInverterCircuit","InverterPort","PVString","PVStringNumber","current","voltage","inverterInPower","inverterOutPower","DCSideInputPower","power","inverterEfficiency","effectiveNumber","total"});
            int count = k+1;
            map.put("第"+count+"小時"+getPeriodByTime(count)+"逆变器数据", addData(inverterDataValues,k,kwhValues));

        }

        makeExcel(columNames, fieldNames, map,modelName);
    }



    private List<InverterData> addData(List<InverterDataValue> inverterDataValues,int k,Double[][][] kwhValues) {
        String[] title ={"正向有功总(kwh)","正向有功峰(kwh)","正向有功平(kwh)","正向有功谷(kwh)","无功总(kvarh)"};
        List<InverterData> data = new ArrayList<InverterData>();
        List datas = new ArrayList();
        for (int j = 1; j < inverterDataValues.size()+1; j++) {
            InverterDataValue inverter =  inverterDataValues.get(j - 1);
            Double[][] commonValues =  inverter.getCommonValues();
            for (int i = 1; i <7 ; i++) {

                if(j<4){
                    getContent(k, data, j, commonValues, i,"(33kw)");
                }else{
                    getContent(k, data, j, commonValues, i,"(30kw)");
                }

            }
            datas.add(data);
        }
        for (int l = 0; l < title.length; l++) {
            data.add(new InverterData("度电补贴表",null,title[l],title[l],"",String.valueOf(kwhValues[l][0][k]),String.valueOf(kwhValues[l][1][k]),String.valueOf(kwhValues[l][2][k]),String.valueOf(kwhValues[l][3][k]),String.valueOf(kwhValues[l][4][k]),String.valueOf(kwhValues[l][5][k]),String.valueOf(kwhValues[l][6][k]),String.valueOf(kwhValues[l][7][k]),String.valueOf(kwhValues[l][8][k])));
        }
        datas.add(data);
        return data;
    }

    private void getContent(int k, List<InverterData> data, int j, Double[][] commonValues, int i,String num) {
        if(i<4){
            data.add( new InverterData("IN0"+j+num,"IN0"+j+"-MPPTA",String.valueOf(i),j+"#子系统 第"+i+"串","IN0"+j+"-A0"+i,String.valueOf(commonValues[0][k]),String.valueOf(commonValues[1][k]),String.valueOf(commonValues[2][k]),String.valueOf(commonValues[6][k]),String.valueOf(commonValues[7][k]),String.valueOf(commonValues[9][k]),String.valueOf(commonValues[10][k]),String.valueOf(commonValues[11][k]),String.valueOf(commonValues[12][k])));
        }else{
            data.add( new InverterData("IN0"+j+num,"IN0"+j+"-MPPTB",String.valueOf(i),j+"#子系统 第"+i+"串","IN0"+j+"-A0"+i,String.valueOf(commonValues[3][k]),String.valueOf(commonValues[4][k]),String.valueOf(commonValues[5][k]),String.valueOf(commonValues[6][k]),String.valueOf(commonValues[8][k]),String.valueOf(commonValues[9][k]),String.valueOf(commonValues[10][k]),String.valueOf(commonValues[11][k]),String.valueOf(commonValues[12][k])));
        }
    }

    private  void convertToSecondExcel(List<InverterDataValue> inverterDataValues,String modelName) {
        List<String[]> columNames = new ArrayList<String[]>();
        List<String[]> fieldNames = new ArrayList<String[]>();

        LinkedHashMap<String, List<?>> map = new LinkedHashMap<String, List<?>>();
        SimpleDateFormat format=new SimpleDateFormat("HH");
        String currentTime=format.format(new Date());
        int hours = Integer.parseInt(currentTime);
        for (int k = 0; k < hours; k++) {
            columNames.add(new String[]{"逆变器编号及额定功率", "逆变器MPPT回路", "逆变器端口号", "光伏串", "光伏串编号", "倾角均值","最佳工作电流Imp(A)均值","最佳工作电压Vmp(V)均值","最大输出功率Pmax(W)均值","每个光伏串直流电压（V）","每个光伏串直流电流（A）","逆变器交流输出电压（V）","逆变器交流输出电流（A）","逆变器输出电量（KW.H）","每串组件实际转换效率（%）","逆变器实际转换效率（%）","等效发电小时数（h）"});
            fieldNames.add(new String[]{"InverterNumAndNominalPower", "MPPTInverterCircuit","InverterPort","PVString","PVStringNumber","averageInclination","ImpAverage","currentAverage","powerMax","eachStringVoltage","eachStringCurrent","inverterOutVoltage","inverterOutCurrent","inverterOutputPower","eachStringConversionEfficiency","inverterConversionEfficiency","EquivalentPowerHours"});
            int count = k+1;
            map.put("第"+count+"小時"+getPeriodByTime(count)+"3#逆变器实验数据", getData(inverterDataValues, k));
        }
        makeExcel(columNames, fieldNames, map,modelName);
    }

    private  List<InverterData> getData(List<InverterDataValue> inverterDataValues,int k) {
        //假数据
        Double[] data1 = {16.5181818181818,15.9727272727273,15.4795454545455,14.7386363636364,14.7818181818182,16.0409090909091};
        Double[] data2 = {8.67136363636364,8.63909090909091,8.67272727272727,8.65227272727273,8.63090909090909,8.65772727272727};
        Double[] data3 = {30.8913636363636,30.8736363636364,30.9222727272727,30.965,30.9327272727273,30.8386363636364};
        Double[] data4 = {267.895454545455,266.713636363636,268.154545454545,267.904545454545,266.968181818182,266.981818181818};

        List<InverterData> data = new ArrayList<InverterData>();
            InverterDataValue inverter =  inverterDataValues.get(2);
            Double[][]  commonValues=  inverter.getCommonValues();
            Double[][][] dcValues =  inverter.getDcValues();
            Double[][] specialValues =  inverter.getSpecialValues();
            Double[] inverterEfficiency = inverter.getInverterEfficiency();
                for (int i = 1; i < 7; i++) {
                    if(i<4){
                        data.add(new InverterData("IN03(33kw)","IN03-MPPTA",String.valueOf(i),"3#子系统 第"+i+"串","IN03-A0"+i,String.valueOf(data1[i-1]),String.valueOf(data2[i-1]),String.valueOf(data3[i-1]),String.valueOf(data4[i-1]),String.valueOf(dcValues[0][i-1][k]),String.valueOf(dcValues[1][i-1][k]),String.valueOf(specialValues[0][k]),String.valueOf(specialValues[1][k]),String.valueOf(specialValues[2][k]),String.valueOf(dcValues[2][i-1][k]),String.valueOf(inverterEfficiency[k]),String.valueOf(commonValues[7][k])));
                    }else{
                        data.add(new InverterData("IN03(33kw)","IN03-MPPTB",String.valueOf(i),"3#子系统 第"+i+"串","IN03-A0"+i,String.valueOf(data1[i-1]),String.valueOf(data2[i-1]),String.valueOf(data3[i-1]),String.valueOf(data4[i-1]),String.valueOf(dcValues[0][i-1][k]),String.valueOf(dcValues[1][i-1][k]),String.valueOf(specialValues[0][k]),String.valueOf(specialValues[1][k]),String.valueOf(specialValues[2][k]),String.valueOf(dcValues[2][i-1][k]),String.valueOf(inverterEfficiency[k]),String.valueOf(commonValues[7][k])));
                    }
                }
        data.add(new InverterData("公式说明", "", "", "", "", "", "用于观测组件工作状态", "用于观测组件工作状态", "用于观测组件工作状态", "直流表读取数据", "直流表读取数据", "屋顶交流汇流柜内多功能表读数", "屋顶交流汇流柜内多功能表读数", "屋顶交流汇流柜内多功能表读数", "(每串直流电压*电流)/(22*1.638*0.982*1000)*100", "测量交流值/6串直流功率总和", "电量值/组件额定功率"));

        return data;
    }

    private  String getPeriodByTime(int count){
        SimpleDateFormat df=new SimpleDateFormat("yyyy-MM-dd HH:mm");
        SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd");
        String current = sdf.format(new Date());
        String[] periodTime = {"07:00","09:00","11:30","14:00","16:30","19:00","21:00","23:00"};
        if(count<7||count>23){
            return "(谷)";

        }
        //定义区间值
        Date dateAfter= null;
        Date dateBefore= null;
        Date now = null;
        for (int i = 0; i <periodTime.length-1; i++) {
            try {
                dateAfter = df.parse(current+" "+periodTime[i]);
                dateBefore = df.parse(current+" "+periodTime[i+1]);
                now = df.parse(current+" "+count+":00:00");
            } catch (ParseException e) {
                e.printStackTrace();
            }
            //判断time是否在XX之后，并且 在XX之前
            if(now.before(dateBefore)&&now.after(dateAfter)){
                // System.out.println(df.format(now)+i+"高峰期");
                return i%2==0?"(平)":"(峰)";
            }else if(now.equals(dateAfter)||count==23){
                return i%2==0?"(平,谷)":"(峰,平)";
            }else if(count==21){
                return "(峰,平)";
            }
            continue;
        }
        return "";
    }

    private void makeExcel(List<String[]> columNames, List<String[]> fieldNames, LinkedHashMap<String, List<?>> map,String modelName) {
        ExcelExportData setInfo = new ExcelExportData();
        setInfo.setDataMap(map);
        setInfo.setFieldNames(fieldNames);
        setInfo.setColumnNames(columNames);

        // 将需要导出的数据输出到文件
        try {
            ExcelUtil.export2File(setInfo, getFileName(modelName), modelName);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public String getFileName(String model) {
        //以日期作為文件名
        DateFormat date = new SimpleDateFormat("yyyy-MM-dd");
        String time = date.format(new Date());
        String path = this.getClass().getClassLoader().getResource("../../").getPath();
        String fileName = path + "resources/" +"inverterData/" + time+"_"+model+".xls";
        return fileName;
    }

    public String getMailFileName() {
        //以日期作為文件名
        DateFormat date = new SimpleDateFormat("yyyy-MM-dd");
        String time = date.format(new Date());
        String path = this.getClass().getClassLoader().getResource("../../").getPath();
        String fileName = path + "resources/" +"inverterData/";// + time+"_"+model+".xls";
        return fileName;
    }

    private Double[] getHourDatas(String psrId, String dataName, String date, int beginMinite, int endMinite, HistoryDataService.DataType dataType){
        Double[] retData = new Double[24];
        List<HistoryDataValue> historyDataValues = historyDataService.getHistoryData(psrId,dataName,date,beginMinite,endMinite,HistoryDataService.DataType.REALTIME_VALUE);
        int valueNum = 0;
        Double totalValue = 0d;

        for (HistoryDataValue historyDataValue : historyDataValues){//计算每一小时的平均值
            totalValue += historyDataValue.getData();
            valueNum++;
            if (historyDataValue.getTime()%60 == 0 || valueNum == 12){
                retData[historyDataValue.getTime()/60 - 1] = totalValue/valueNum;
                valueNum = 0;
                totalValue = 0d;
            }
        }
        return retData;
    }


    private Double getTotalDatas(String psrId, String dataName, String date, int beginMinite, int endMinite, HistoryDataService.DataType dataType){
        Double[] retData = new Double[24];
        List<HistoryDataValue> historyDataValues = historyDataService.getHistoryData(psrId,dataName,date,beginMinite,endMinite,HistoryDataService.DataType.KWH_VALUE);
        int valueNum = 0;
        Double totalValue = 0d;
        if (!historyDataValues.isEmpty()){
            totalValue = historyDataValues.get(historyDataValues.size()-1).getData();
        }
        return totalValue;
    }

    //读取度电补贴量
    private Double[][][] getKWHSubsidiesDatas(String psrId,int currentHours,String date, int beginMinite, int endMinite){

        Double normalPower = 168.54;
        Double[] iniValue = {10.58,7.82,0.67,2.07,4.22};
        Double[][][] retValues = new Double[5][9][24];

        String[] kwhNames = {"transboxMeaForwardPTotal","transboxMeaForwardPPeak","transboxMeaForwardPNormal","transboxMeaForwardPValley","transboxMeaForwardQTotal"};

        for (int i = 0; i < kwhNames.length; i++){
            List<HistoryDataValue> historyDataValues = historyDataService.getHistoryData(psrId, kwhNames[i], date, beginMinite, endMinite, HistoryDataService.DataType.KWH_VALUE);
            if (!historyDataValues.isEmpty()){
                Double currentValue = historyDataValues.get(historyDataValues.size()-1).getData();
                retValues[i][8][currentHours] = currentValue - iniValue[i] * 60;//累计量
                retValues[i][5][currentHours] = currentValue - historyDataValues.get(0).getData();//当日电量
                if (i == 0){
                    retValues[i][7][currentHours] = retValues[i][5][currentHours] / normalPower;//只有正向总有功才有值
                }
            }
        }
        return retValues;
    }
}
