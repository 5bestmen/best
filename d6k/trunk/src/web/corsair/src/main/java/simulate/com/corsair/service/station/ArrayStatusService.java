package simulate.com.corsair.service.station;

import simulate.com.corsair.device.JungleTotalFlow;

/**
 * Created by 洪祥 on 15/7/8.
 */

public class ArrayStatusService {
    private static int jungleCount = 12;
    private JungleTotalFlow jungleFlows[] = new JungleTotalFlow[jungleCount];


    public ArrayStatusService() {
        for (int i = 0; i < jungleCount; i++) {
            jungleFlows[i] = new JungleTotalFlow(i);
        }
    }

    public JungleTotalFlow[] getJungleFlows() {

        for (int i = 0; i < jungleCount; i++) {
            jungleFlows[i].getValue();
        }

        return jungleFlows;
    }
}
