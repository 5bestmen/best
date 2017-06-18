package com.corsair.device;

import java.util.List;

/**
 * Created by zyj on 2017/2/22.
 */
public class Total_YMD_Info {

    List<CalcInfo> dayCalcList;

    List<CalcInfo>  monthCalcList;

    public List<CalcInfo> getMonthCalcList() {
        return monthCalcList;
    }

    public void setMonthCalcList(List<CalcInfo> monthCalcList) {
        this.monthCalcList = monthCalcList;
    }

    public List<CalcInfo> getDayCalcList() {
        return dayCalcList;
    }

    public void setDayCalcList(List<CalcInfo> dayCalcList) {
        this.dayCalcList = dayCalcList;
    }
}
