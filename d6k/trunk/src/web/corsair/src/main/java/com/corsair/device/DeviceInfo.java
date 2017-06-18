package com.corsair.device;

/**
 * Created by zyj on 2016/12/2.
 */

import java.util.List;

public class DeviceInfo {
    List<AiInfo>    aiList;
    List<DiInfo>    diList;
    List<CalcInfo>  calcList;

    public List<AiInfo> getAiList() {
        return aiList;
    }

    public void setAiList(List<AiInfo> aiList) {
        this.aiList = aiList;
    }

    public List<DiInfo> getDiList() {
        return diList;
    }

    public void setDiList(List<DiInfo> diList) {
        this.diList = diList;
    }

    public List<CalcInfo> getCalcList() {
        return calcList;
    }

    public void setCalcList(List<CalcInfo> calcList) {
        this.calcList = calcList;
    }
}
