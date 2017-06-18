package com.measurement;

import com.webService.CGetDataService;
import org.apache.axis2.AxisFault;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Repository;

import java.rmi.RemoteException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by zyj on 17/6/12.
 */
@Repository("realTimeClient")
public class RealTimeClient implements IMeasurement {
    //@Value("#{configProperties['scada.realtime.service.url']}")
    private String url="http://127.0.0.1:7790/GetDataService";

    //@Value("#{configProperties['scada.measurement.simulate']}")
    private Boolean simulate=false;

    //@Value("#{configProperties['scada.simulate.value']}")
    private Double simulateValue=10000.0;

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }


    public CGetDataService.CommonValuesType[] getMeasurementValues(List<MeasurementInfo> measurementInfoList) {
        List<String> measurementIDList = new ArrayList<>();

        for (MeasurementInfo measurementInfo : measurementInfoList) {
            measurementIDList.add(measurementInfo.getMeasurementID());
        }

        return getValues(measurementIDList);
    }

    public CGetDataService.CommonValuesType[] getValues(List<String> measurements) {

        CGetDataService.RequestMessage message = new CGetDataService.RequestMessage();
        CGetDataService.RequestMessageType messageType = new CGetDataService.RequestMessageType();
        CGetDataService.HeaderType headerType = new CGetDataService.HeaderType();
        CGetDataService.RequestType requestType = new CGetDataService.RequestType();
        CGetDataService.ID_type1 id_type;

        headerType.setVerb(CGetDataService.Verb_type1.get);
        headerType.setNoun("SimpleValues");
        messageType.setHeader(headerType);

        for (String meaid : measurements) {
            id_type = new CGetDataService.ID_type1();
            id_type.setString(meaid);
            requestType.addID(id_type);
        }
        messageType.setRequest(requestType);

        message.setRequestMessage(messageType);
        try {
            CGetDataService stub = new CGetDataService(url);

            CGetDataService.ResponseMessage responseMessage = stub.request(message);

            return responseMessage.getResponseMessage().getPayload().getPayloadTypeChoice_type0().getCommonValues();

        } catch (AxisFault axisFault) {
            axisFault.printStackTrace();
        } catch (RemoteException e) {
            e.printStackTrace();
        }

        return null;
    }

    @Override
    public MeasurementValue getMeasurementValue(MeasurementInfo measurement) {
        List<String> measurementList = new ArrayList<>();
        measurementList.add(measurement.getMeasurementID());

        if (simulate) {
            return new MeasurementValue(
                    measurement.getMeasurementID(),
                    simulateValue,
                    "",
                    false,
                    false,
                    false,
                    false,
                    false,
                    false,
                    false);
        } else {
            CGetDataService.CommonValuesType[] commonValues = getValues(measurementList);

            if (commonValues != null && commonValues[0] != null && commonValues[0].getValue() != null) {
                CGetDataService.CommonValuesType commonValue = commonValues[0];

                return new MeasurementValue(
                        measurement.getMeasurementID(),
                        Double.parseDouble(commonValue.getValue()),
                        "",
                        commonValue.getDead(),
                        commonValue.getInvalid(),
                        commonValue.getUpperLimit(),
                        commonValue.getUpperUpperLimit(),
                        commonValue.getLowerLimit(),
                        commonValue.getLowerLowerLimit(),
                        commonValue.getSuspicious());
            } else {
                return new MeasurementValue(
                        measurement.getMeasurementID(),
                        0d,
                        "N/A",
                        Boolean.FALSE,
                        Boolean.FALSE,
                        Boolean.FALSE,
                        Boolean.FALSE,
                        Boolean.FALSE,
                        Boolean.FALSE,
                        Boolean.FALSE
                );
            }
        }
    }

    @Override
    public List<MeasurementValue> getMeasurementValue(List<MeasurementInfo> measurements) {
        List<String> measurementIDList = new ArrayList<>();

        for (MeasurementInfo measurementInfo : measurements) {
            measurementIDList.add(measurementInfo.getMeasurementID());
        }

        CGetDataService.CommonValuesType[] commonValues = null;
        List<MeasurementValue> values = new ArrayList<>();

        if (!simulate) {
            commonValues = getValues(measurementIDList);
        } else {
            //直接返回默认值供调试用
            for (MeasurementInfo measurement : measurements) {
                values.add(
                        new MeasurementValue(
                                measurement.getMeasurementID(),
                                simulateValue,
                                "",
                                Boolean.FALSE,
                                Boolean.FALSE,
                                Boolean.FALSE,
                                Boolean.FALSE,
                                Boolean.FALSE,
                                Boolean.FALSE,
                                Boolean.FALSE
                        )
                );
            }

            return values;
        }

        if (commonValues == null) {
            for (MeasurementInfo measurement : measurements) {
                values.add(
                        new MeasurementValue(
                                measurement.getMeasurementID(),
                                0d,
                                "N/A",
                                Boolean.FALSE,
                                Boolean.FALSE,
                                Boolean.FALSE,
                                Boolean.FALSE,
                                Boolean.FALSE,
                                Boolean.FALSE,
                                Boolean.FALSE
                        )
                );
            }
        } else {
            for (Integer i = 0, number = commonValues.length; i < number; i++) {
                CGetDataService.CommonValuesType commonValue = commonValues[i];

                Double value;

                if (commonValue.getValue() == null) {
                    value = 0d;
                } else {
                    value = Double.parseDouble(commonValue.getValue());
                }

                values.add(
                        new MeasurementValue(
                                measurements.get(i).getMeasurementID(),
                                value,
                                "",
                                commonValue.getDead(),
                                commonValue.getInvalid(),
                                commonValue.getUpperLimit(),
                                commonValue.getUpperUpperLimit(),
                                commonValue.getLowerLimit(),
                                commonValue.getLowerLowerLimit(),
                                commonValue.getSuspicious()
                        )
                );
            }
        }

        return values;
    }
}
