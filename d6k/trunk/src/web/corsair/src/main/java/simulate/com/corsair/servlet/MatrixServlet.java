package simulate.com.corsair.servlet;

import com.corsair.measurement.MeasurementInfo;
import com.corsair.measurement.MeasurementValue;
import com.corsair.measurement.RealTimeClient;
import com.corsair.service.ScadaPSRService;
import net.sf.json.JSONArray;

import simulate.com.corsair.service.station.ArrayMatrixService;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by libh on 2015/8/11.
 */
@WebServlet(name="MatrixServlet", urlPatterns="/MatrixServlet")
public class MatrixServlet extends BaseServlet {
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

        ArrayMatrixService arrayMartixService = (ArrayMatrixService)super.getBean("arrayMatrix");

        List<MeasurementInfo> matrixs = new ArrayList<MeasurementInfo>();
        String parryid = request.getParameter("id");
        ScadaPSRService scadaPSRService = (ScadaPSRService)super.getBean("scadaPSRService");
        matrixs = scadaPSRService.getMatrixs(parryid);

//        Map idNameMap = new HashMap();
        Map nameidMap = new HashMap();
        for (MeasurementInfo measurementInfo : matrixs)
        {

//            idNameMap.put(measurementInfo.getMeasurementID(), measurementInfo.getTemplateIndex());
            nameidMap.put(measurementInfo.getTemplateIndex(), measurementInfo.getMeasurementID());
        }

        List<MeasurementValue> measurementValues = new ArrayList<>();;
        RealTimeClient rtclient = (RealTimeClient)getBean("realTimeClient");
        measurementValues = rtclient.getMeasurementValue(matrixs);

        Map<String, MeasurementValue> nameMeaValueMap = new HashMap<String, MeasurementValue>();
        for (MeasurementValue measurementValue : measurementValues)
        {
            nameMeaValueMap.put(measurementValue.getKey(), measurementValue);
        }

        MeasurementValue [][] fillMeasurementValues = new MeasurementValue[12][16];

        int n = 0;
        for (int i = 0; i < 12; i++ )
        {
            for (int j = 0; j < 16; j++)
            {
                int tmpNo = i*16 + j + 1;
                String nameKey;
                if(nameidMap.containsKey(tmpNo))
                {
                    nameKey = nameidMap.get(tmpNo).toString().trim();
                    if (nameKey != null && nameMeaValueMap.containsKey(nameKey))
                    {
                        Object meaValue = nameMeaValueMap.get(nameKey);
                        if (meaValue != null )
                        {
                            if (meaValue instanceof MeasurementValue)
                            {
                                fillMeasurementValues[i][j] = (MeasurementValue) meaValue;
                                continue;
                            }
                        }
                    }
                }

//                if((measurementValues != null) && (n < measurementValues.size()))
//                {
//                    String idkey = measurementValues.get(n).getKey();
//                    String strNo = idNameMap.get(idkey).toString().trim();
//                    if (strNo.length() != 0)
//                    {
//                        int no = Integer.parseInt(strNo);
//                        int tmpNo = i*16 + j + 1;
//                        if (no == tmpNo)
//                        {
//                            fillMeasurementValues[i][j] = measurementValues.get(n);
//                            n++;
//                            continue;
//                        }
//                    }
//                }

                fillMeasurementValues[i][j] = new MeasurementValue(
                        "",
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

        JSONArray array = JSONArray.fromObject(arrayMartixService.getMatrixFlows(fillMeasurementValues));
        response.getOutputStream().write(array.toString().getBytes());
    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

    }
}
