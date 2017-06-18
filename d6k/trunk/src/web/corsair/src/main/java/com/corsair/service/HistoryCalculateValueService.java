package com.corsair.service;

import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.measurement.HistoryDataValue;
import com.corsair.measurement.ScadaConfig;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import parsii.eval.Expression;
import parsii.eval.Parser;
import parsii.eval.Scope;
import parsii.eval.Variable;
import parsii.tokenizer.ParseException;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by zjq on 2015/12/30.
 */
@Service("historyCalculateValueService")
public class HistoryCalculateValueService {
    private static final String VARIABLE_BEGIN = "v_";
    private static final String VARIABLE_END = "_v";

    @Autowired
    HistoryDataService historyDataService;
    @Autowired
    DisplayAreaService displayAreaService;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;
    @Autowired
    ScadaConfig scadaConfig;

    public List<HistoryDataValue> getCalculateValue(String psrId, String dataName, String date, int beginMinite, int endMinite, HistoryDataService.DataType type) {
        DeviceTemplate deviceTemplate = displayAreaService.getDeviceTemplateByPSRID(psrId);

        if (deviceTemplate == null) {
            return getNullValue();
        }

        DeviceTemplateData deviceTemplateData = deviceTemplate.getDeviceTemplateDataByName(dataName);
        return deviceTemplateData == null ? getNullValue() : getValidCalculateValue(psrId, dataName, date, beginMinite, endMinite, type);
    }

    private DeviceTemplateData getDeviceTemplateData(DeviceTemplate deviceTemplate, String name) {
        return deviceTemplate == null ? null : deviceTemplate.getDeviceTemplateDataByName(name);
    }

    private List<HistoryDataValue> getNullValue() {
        List<HistoryDataValue> historyDataValues = new ArrayList<>();
        HistoryDataValue historyValue = new HistoryDataValue(0, "-");
        historyValue.setInvalid(true);
        historyDataValues.add(historyValue);

        return historyDataValues;
    }

    private List<HistoryDataValue> getValidCalculateValue(String psrId, String dataName, String date, int beginMinite, int endMinite, HistoryDataService.DataType type) {
        DeviceTemplate deviceTemplate = displayAreaService.getDeviceTemplateByPSRID(psrId);
        DeviceTemplateData deviceTemplateData = deviceTemplate.getDeviceTemplateDataByName(dataName);
        String expression = deviceTemplateData.getExpression();

        List<HistoryDataValue> retHistoryDataValues = new ArrayList<>();

        if (expression == null) {
            return getNullValue();
        }

        List<String> variables = getVariableFromExpression(expression);
        List<List<HistoryDataValue>> allHistoryData = new ArrayList<>();
        int max = 0;
        int maxPos = 0;
        for (String v : variables) {
            List<HistoryDataValue> historyDataValues = historyDataService.getHistoryData(psrId, v, date, beginMinite, endMinite, HistoryDataService.DataType.REALTIME_VALUE);
            allHistoryData.add(historyDataValues);
            int currentNum = historyDataValues.size();

            if (currentNum > max) {
                max = currentNum;
                maxPos = allHistoryData.size() - 1;
            }
        }

        //进行数据缺失处理
        for (int m = 0; m < max; m++) {
            for (int n = 0; n < allHistoryData.size(); n++) {
                List<HistoryDataValue> currentHistoryValue = allHistoryData.get(n);
                int time1 = currentHistoryValue.get(m).getTime();
                int time2 = allHistoryData.get(maxPos).get(m).getTime();
                if (time1 != time2) {
                    currentHistoryValue.add(m, new HistoryDataValue(time1, "-"));
                }
            }
        }

        Scope scope = Scope.create();

        for (int k = 0; k < max; k++) {
            for (int i = 0; i < variables.size(); i++) {
                HistoryDataValue historyDataValue = allHistoryData.get(i).get(k);

                if (historyDataValue.getData() == null) {
                    return getNullValue();
                }

                Variable variable = scope.getVariable(VARIABLE_BEGIN + variables.get(i) + VARIABLE_END);
                variable.setValue(historyDataValue.getData());
            }

            int currentTime = allHistoryData.get(maxPos).get(k).getTime();

            try {
                HistoryDataValue value;
                Expression expr = Parser.parse(deviceTemplateData.getExpression(), scope);
                Double finalValue = expr.evaluate();

                if (finalValue.isInfinite()) {
                    value = new HistoryDataValue(currentTime, "Infinite");
                } else if (finalValue.isNaN()) {
                    value = new HistoryDataValue(currentTime, "-");
                } else {
                    value = new HistoryDataValue(currentTime, finalValue);
                }

                retHistoryDataValues.add(value);

            } catch (ParseException e) {
                e.printStackTrace();
                return getNullValue();
            }
        }
        return retHistoryDataValues;
    }


    private List<String> getVariableFromExpression(String expression) {
        List<String> variables = new ArrayList<>();

        int begin = expression.indexOf(VARIABLE_BEGIN);
        int end = expression.indexOf(VARIABLE_END);

        while (begin > -1 && end > -1 && (begin + 2) < end) {
            variables.add(expression.substring(begin + 2, end));

            if (expression.length() <= (end + 2)) {
                break;
            }

            expression = expression.substring(end + 2);

            begin = expression.indexOf(VARIABLE_BEGIN);
            end = expression.indexOf(VARIABLE_END);
        }

        return variables;
    }

}
