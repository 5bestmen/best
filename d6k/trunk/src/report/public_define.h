#ifndef  PUBLIC_DEFINE_H
#define  PUBLIC_DEFINE_H


#include <QString>


#define  HISDATA_REAL           1       //实时值  
#define  HISDATA_LOCALSET       2       //本地人工置入
#define  HISDATA_REMOTESET      3       //远程人工置入
#define  HISDATA_DEADVAL        4       //死数
#define  HISDATA_TERMINALERROR  5       //终端故障
//#define  HISDATA_FTUERROR       6       //FTU故障
#define  HISDATA_NETERROR       7       //网络故障
#define  HISDATA_OVERHIGHTLMT   8       //遥测上越限
#define  HISDATA_OVERLOWLMT     9       //遥测下越限
#define  HISDATA_OVERVOLUP      10      //电压越不合格上限
#define  HISDATA_OVERVOLDOWN    11      //电压越不合格下限
#define  HISDATA_MANUPDATE	    12      //人工修改
#define  HISDATA_MANINSERT	    13      //人工插入

#define	 HISDATA_PLAN			51		//计划值
#define  HISDATA_KWHTABCODE		52		//电度表码
#define  HISDATA_KWHRAWDATA		53		//电度原始数据

#define  KWH_DATA               54      //每日电度值

#define  HISDATA_REALMAX        55      //最大值
#define  HISDATA_REALMIN        56      //最小值
#define  HISDATA_REALHGL        57      //合格率

#define  HISDATA_AVERAGE        58      //平均值
#define  HISDATA_PQRATE         59      //负荷率

#define  HISDATA_MAXTIME        60      //最大时 暂时未用
#define  HISDATA_MINTIME        61      //最小时

#define  HISDATA_OVHHCOUNT      62      //越上上限次数
#define  HISDATA_OVHCOUNT       63      //越上限次数
#define  HISDATA_DOWNLCOUNT     64      //越下限次数
#define  HISDATA_DOWNLLCOUNT    65      //越下下限次数
#define  HISDATA_OVHHTIME       66      //越上上限时间
#define  HISDATA_OVHTIME        67      //越上限时间
#define  HISDATA_DOWNLTIME		68      //越下限时间
#define  HISDATA_DOWNLLTIME		69      //越下下限时间
#define  HISDATA_DAYKWHSD1      70      //1时段日电量(平)
#define  HISDATA_DAYKWHSD2      71      //2时段日电量(峰)
#define  HISDATA_DAYKWHSD3      72      //3时段日电量(谷)
#define  HISDATA_DAYKWHSD4      73      //4时段日电量(尖)
#define  HISDATA_DAYKWHSD5      74      //5时段日电量(脊谷)
#define  HISDATA_DAYKWHSD6      75      //6时段日电量

#define  HISDATA_DAYKWHTTL      79      //日累计电量

#define  HISDATA_DAYDEADTIME    80      //日死数时间
#define  HISDATA_DAYSTOPTIME    81      //日停运时间

//注意： 83---90  由PAS使用
#define  HISDATA_FORECAST		83		//负荷预报数据

//以下用于历史统计存盘量的标志
#define  HISDATA_DAYS_MAX        150      //日统计最大值
#define  HISDATA_DAYS_MIN        151      //日统计最小值
#define  HISDATA_DAYS_HGL        152      //日统计合格率
#define  HISDATA_DAYS_AVERAGE    153      //日统计平均值
#define  HISDATA_DAYS_PQRATE     154      //日统计负荷率
#define  HISDATA_DAYS_MAXTIME    155      //日统计最大时 暂时未用
#define  HISDATA_DAYS_MINTIME    156      //日统计最小时

// 157到160是根据存盘点与遥测参数表中的上下限值进行统计
#define  HISDATA_DAYS_OVHHCOUNT       157 //日统计越上上限点数（暂时未用）
#define  HISDATA_DAYS_OVHCOUNT        158 //日统计越上限点数
#define  HISDATA_DAYS_DOWNLCOUNT      159 //日统计越下限点数
#define  HISDATA_DAYS_DOWNLLCOUNT     160 //日统计越下下限点数（暂时未用）
#define  HISDATA_DAYS_OVHHTIME        161 //日统计越上上限时间（暂时未用）
#define  HISDATA_DAYS_OVHTIME         162 //日统计越上限时间
#define  HISDATA_DAYS_DOWNLTIME		  163 //日统计越下限时间
#define  HISDATA_DAYS_DOWNLLTIME	  164 //日统计越下下限时间（暂时未用）

// 165到169是根据电压的存盘点与电压限值表中的上下限值进行统计
#define  HISDATA_DAYS_VQOHCOUNT       165 //日电压质量越上限点数
#define  HISDATA_DAYS_VQOLCOUNT       166 //日电压质量越下限点数
#define  HISDATA_DAYS_VQOHTIME        167 //日电压质量越上限时间
#define  HISDATA_DAYS_VQOLTIME        168 //日电压质量越下限时间
#define  HISDATA_DAYS_VQHGL           169 //日电压质量合格率
#define	 HISDATA_DAYS_JLDL            170 //日奖励电量
#define  HISDATA_DAYS_KFDL            171 //日扣罚电量
#define  HISDATA_DAYS_JFDL			  172 //日奖罚电量
#define  HISDATA_DAYS_ZQL			  180 //日计划准确率

#define  HISDATA_UPPERTDDATA	      200 // 计划上限
#define  HISDATA_LOWERTDDATA	      201 // 计划下限


#define HISDATA_DAYRUNTIME            300  //日运行时间
#define HISDATA_DAYSTOPCOUNT          301  //日停止次数
#define HISDATA_DAYTOTALRUNTIME       302  //总运行时间
#define HISDATA_DAYTITALSTOPCOUNT     303  //总停止次数

#define HISDATA_TOTALRUNTIME          304  //总运行时间
#define HISDATA_TOTALSTOPCOUNT        305  //总停止次数
#define HISDATA_MAXSTOPCOUNT          306  //最大停止次数

#define VAL_IS_NOT_VALID              -1

namespace ParaseDefines
{
   QString GetDefineDes(const int index);
   int GetDefineInt(const QString &des);
}


#endif
