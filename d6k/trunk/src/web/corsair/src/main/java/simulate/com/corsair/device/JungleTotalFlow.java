package simulate.com.corsair.device;

import java.lang.reflect.Field;

/**
 * Created by 洪祥 on 15/7/8.
 */
public class JungleTotalFlow {
    private String name;
    private float track1;
    private float track2;
    private float track3;
    private float track4;
    private float track5;
    private float track6;
    private float track7;
    private float track8;
    private float track9;
    private float track10;
    private float track11;
    private float track12;
    private float track13;
    private float track14;
    private float track15;
    private float track16;
    private float average;
    private float max;

    public JungleTotalFlow(Integer i) {
        this.name = (i + 1) + "#";
    }

    public void getValue() {
        Field fields[] = JungleTotalFlow.class.getDeclaredFields();
        float all[] = new float[16];
        int i = 0;

        for (Field field : fields) {
            if (field.getName().contains("track")) {
                float value = (float) (10 + (Math.random() - 0.5));
                all[i] = value;
                i++;

                try {
                    field.setFloat(this, value);
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

    public float getTrack1() {
        return track1;
    }

    public float getTrack2() {
        return track2;
    }

    public float getTrack3() {
        return track3;
    }

    public float getTrack4() {
        return track4;
    }

    public float getTrack5() {
        return track5;
    }

    public float getTrack6() {
        return track6;
    }

    public float getTrack7() {
        return track7;
    }

    public float getTrack8() {
        return track8;
    }

    public float getTrack9() {
        return track9;
    }

    public float getTrack10() {
        return track10;
    }

    public float getTrack11() {
        return track11;
    }

    public float getTrack12() {
        return track12;
    }

    public float getTrack13() {
        return track13;
    }

    public float getTrack14() {
        return track14;
    }

    public float getTrack15() {
        return track15;
    }

    public float getTrack16() {
        return track16;
    }

    public float getAverage() {
        return average;
    }

    public float getMax() {
        return max;
    }
}
