package simulate.com.corsair.device;
import com.corsair.measurement.MeasurementDataFlag;
import com.corsair.measurement.MeasurementValue;

import java.lang.reflect.Field;
/**
 * Created by libh on 2015/8/13.
 */

public class JunctionMatrixFlow {
        private String name;
        private MeasurementDataFlag track1;
        private MeasurementDataFlag track2;
        private MeasurementDataFlag track3;
        private MeasurementDataFlag track4;
        private MeasurementDataFlag track5;
        private MeasurementDataFlag track6;
        private MeasurementDataFlag track7;
        private MeasurementDataFlag track8;
        private MeasurementDataFlag track9;
        private MeasurementDataFlag track10;
        private MeasurementDataFlag track11;
        private MeasurementDataFlag track12;
        private MeasurementDataFlag track13;
        private MeasurementDataFlag track14;
        private MeasurementDataFlag track15;
        private MeasurementDataFlag track16;
        private float average;
        private float max;

        public JunctionMatrixFlow(Integer i) {
            this.name = (i + 1) + "#";
        }

        public void getValue(MeasurementValue[] measurementValues) {
            Field fields[] = JunctionMatrixFlow.class.getDeclaredFields();
            float all[] = new float[16];
            int i = 0;

            for (Field field : fields) {
                if (field.getName().contains("track")) {
                    MeasurementDataFlag dataFlag = new MeasurementDataFlag();
                    boolean flag = measurementValues[i].getInvalid();
                    String strValue = measurementValues[i].getData().toString();
                   float value = Float.parseFloat(strValue);
                    dataFlag.setData(value);
                    dataFlag.setFlag(flag);

                    all[i] = value;
                    i++;

                    try {
                        field.set(this, dataFlag);
//                        field.setFloat(this,value);
                    } catch (IllegalAccessException e) {
                        e.printStackTrace();
                    }
                }
            }

            this.average = getAverage(all);
            this.max = getMax(all);
        }


        public static float getAverage(float all[]) {
            float total = 0;

            for (float i : all) {
                total += i;
            }

            return total / all.length;
        }

        public static float getMax(float all[]) {
            float max = 0;

            for (float i : all) {
                if (i > max) {
                    max = i;
                }
            }

            return max;
        }

        public String getName() {
            return name;
        }

        public MeasurementDataFlag getTrack1() {
            return track1;
        }

        public MeasurementDataFlag getTrack2() {
            return track2;
        }

        public MeasurementDataFlag getTrack3() {
            return track3;
        }

        public MeasurementDataFlag getTrack4() {
            return track4;
        }

        public MeasurementDataFlag getTrack5() {
            return track5;
        }

        public MeasurementDataFlag getTrack6() {
            return track6;
        }

        public MeasurementDataFlag getTrack7() {
            return track7;
        }

        public MeasurementDataFlag getTrack8() {
            return track8;
        }

        public MeasurementDataFlag getTrack9() {
            return track9;
        }

        public MeasurementDataFlag getTrack10() {
            return track10;
        }

        public MeasurementDataFlag getTrack11() {
            return track11;
        }

        public MeasurementDataFlag getTrack12() {
            return track12;
        }

        public MeasurementDataFlag getTrack13() {
            return track13;
        }

        public MeasurementDataFlag getTrack14() {
            return track14;
        }

        public MeasurementDataFlag getTrack15() {
            return track15;
        }

        public MeasurementDataFlag getTrack16() {
            return track16;
        }

        public float getAverage() {
            return average;
        }

        public float getMax() {
            return max;
        }
    }


