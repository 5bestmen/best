package com.corsair.measurement;

import com.corsair.entity.template.psr.DeviceTemplate;
import com.corsair.entity.template.psr.DeviceTemplateData;
import com.corsair.service.DisplayAreaService;
import com.corsair.service.ScadaMeasurementService;
import com.corsair.util.StringUtil;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Repository;
import parsii.eval.Expression;
import parsii.eval.Parser;
import parsii.eval.Scope;
import parsii.eval.Variable;
import parsii.tokenizer.ParseException;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by 洪祥 on 2015/11/26.
 */
@Repository
public class CalculateValue extends AbstractSingleMeasurement {
    private static final String VARIABLE_BEGIN = "v_";
    private static final String VARIABLE_END = "_v";

    @Autowired
    RealTimeClient realTimeClient;
    @Autowired
    DisplayAreaService displayAreaService;
    @Autowired
    ScadaMeasurementService scadaMeasurementService;

    @Value("#{configProperties['scada.measurement.simulate']}")
    private Boolean simulate;

    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
        DeviceTemplate deviceTemplate = displayAreaService.getDeviceTemplateByPSRID(measurement.getPsrID());

        if (deviceTemplate == null) {
            return getNullValue();
        }

        DeviceTemplateData deviceTemplateData = deviceTemplate.getDeviceTemplateDataByName(measurement.getName());
        return deviceTemplateData == null ? getNullValue() : getValidCalculateValue(deviceTemplate, deviceTemplateData, measurement.getPsrID());
    }

    private DeviceTemplateData getDeviceTemplateData(DeviceTemplate deviceTemplate, String name) {
        return deviceTemplate == null ? null : deviceTemplate.getDeviceTemplateDataByName(name);
    }

    private MeasurementValue getNullValue() {
        MeasurementValue measurementValue = new MeasurementValue();
        measurementValue.setText("N/A");
        measurementValue.setInvalid(true);

        return measurementValue;
    }

    private MeasurementValue getValidCalculateValue(DeviceTemplate deviceTemplate, DeviceTemplateData deviceTemplateData, String psrID) {
        String  expression = deviceTemplateData.getExpression();

        if (expression == null) {
            return getNullValue();
        }

        List<String> variables = getVariableFromExpression(expression);
        List<MeasurementValue> measurementValueList = new ArrayList<>();

        for (String v : variables) {
            DeviceTemplateData data = deviceTemplate.getDeviceTemplateDataByName(v);

            if (data != null) {
                String measurementID = scadaMeasurementService.getMeasurementID(psrID, data);


                if (StringUtil.empty(measurementID)) {
                    return getNullValue();
                }

                MeasurementValue value;
                MeasurementInfo info = new MeasurementInfo(measurementID);

                value = realTimeClient.getMeasurementValue(info);
                measurementValueList.add(value);
            }
        }

        Scope scope = Scope.create();


        for (int i = 0; i < variables.size(); i++) {
            MeasurementValue value = measurementValueList.get(i);

            if (value.getData() == null) {
                return getNullValue();
            }

            Variable variable = scope.getVariable(VARIABLE_BEGIN + variables.get(i) + VARIABLE_END);
            variable.setValue(value.getData());
        }

        try {
            MeasurementValue value;
            Expression expr = Parser.parse(deviceTemplateData.getExpression(), scope);
            Double finalValue = expr.evaluate();

            if (finalValue.isInfinite()) {
                value = new MeasurementValue("Infinite");
            } else if (finalValue.isNaN()) {
                value = new MeasurementValue("NaN");
            } else {
                value = new MeasurementValue(finalValue);
            }

            return value;
        } catch (ParseException e) {
            e.printStackTrace();
            return getNullValue();
        }
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
