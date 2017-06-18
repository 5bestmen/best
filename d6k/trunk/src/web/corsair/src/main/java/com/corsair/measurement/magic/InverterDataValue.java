package com.corsair.measurement.magic;

/**
 * Created by zjq on 2016/1/5.
 */
public class InverterDataValue {


    Double[][] commonValues = new Double[13][24];
//    Double[] pv1I = new Double[24];
//    Double[] pv1U = new Double[24];
//    Double[] Pv1P = new Double[24];
//    Double[] pv2I = new Double[24];
//    Double[] pv2U = new Double[24];
//    Double[] Pv2P = new Double[24];
//    Double[] commonpower = new Double[24];
//    Double[] dc1DayGenerated= new Double[24];
//    Double[] dc2DayGenerated= new Double[24];
//    Double[] commonEfficiency = new Double[24];
//    Double[] dayGenerated = new Double[24];
//    Integer[] workTime = new Integer[24];
//    Double[] accumulateGenerated = new Double[24];

//    Double[][] KWHSubsidies = new Double[6][24];
//以下为3号逆变器独有
//6路直流电压和电流和效率
      Double[][][] DcValues = new Double[3][6][24];
//    Double[][] DcU = new Double[6][24];
//    Double[][] DcI = new Double[6][24];
//    Double[][] efficiency = new Double[6][24];

//    Double[][] DcP = new Double[6][24];//暂时不用

      Double[][] specialValues = new Double[4][24];
//    Double[] outVoltage = new Double[24];
//    Double[] outCurrent = new Double[24];
//    Double[] outGenerated = new Double[24];

//    Double[] outPower = new Double[24];//内部计算用，无需显示
      Double[] inverterEfficiency = new Double[24];//逆变器效率，内部计算得到

    public void setInverterEfficiency(Double[] inverterEfficiency) {
        this.inverterEfficiency = inverterEfficiency;
    }

    public Double[] getInverterEfficiency() {
        for (int k = 0; k < inverterEfficiency.length; k++){
            if (inverterEfficiency[k] != null){
                return inverterEfficiency;
            }
        }

        for (int h = 0; h < specialValues[3].length; h++){
            Double inputDcPower = 0d;
            if (specialValues[3][h] != null){
                for (int i = 0; i < 6; i++){
                    inputDcPower += DcValues[0][i][h] * DcValues[1][i][h];
                }
                if (inputDcPower == 0){
                    inverterEfficiency[h] = 0d;
                }else{
                    inverterEfficiency[h] = specialValues[3][h]/inputDcPower*100;
                }
            }
        }

        return inverterEfficiency;
    }

    public Double[][] getCommonValues() {
        return commonValues;
    }

    public void setCommonValues(Double[][] commonValues) {
        this.commonValues = commonValues;
    }


    public Double[][][] getDcValues() {
        return DcValues;
    }

    public void setDcValues(Double[][][] dcValues) {
        DcValues = dcValues;
    }

    public Double[][] getSpecialValues() {
        return specialValues;
    }

    public void setSpecialValues(Double[][] specialValues) {
        this.specialValues = specialValues;
    }
}
