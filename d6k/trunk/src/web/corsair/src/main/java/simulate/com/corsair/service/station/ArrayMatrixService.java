package simulate.com.corsair.service.station;


import com.corsair.measurement.MeasurementValue;
import simulate.com.corsair.device.JunctionMatrixFlow;

/**
 * Created by libh on 2015/8/13.
 */
public class ArrayMatrixService {
    private static int matrixCount = 12;
    private JunctionMatrixFlow MatrixFlows[] = new JunctionMatrixFlow[matrixCount];


    public  ArrayMatrixService() {
        for (int i = 0; i < matrixCount; i++) {
            MatrixFlows[i] = new JunctionMatrixFlow(i);
        }
    }

    public JunctionMatrixFlow[] getMatrixFlows(MeasurementValue [][] measurementValues) {
        for (int i = 0; i < matrixCount; i++) {
            MatrixFlows[i].getValue(measurementValues[i]);
        }
        return MatrixFlows;
    }
}
