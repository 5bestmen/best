
/*! @file qcell.h
<PRE>
********************************************************************************
模块名       :
文件名       :  qcell.h
文件实现功能 :  报表单元格
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   报表单元格
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/


#ifndef QCELL_H
#define QCELL_H

#include <QTableWidgetItem>
#include <QObject>
#include <QVariant>
#include <QColor>
#include <QList>
#include <QDate>
#include <string>
#include <stdlib.h>
#include "configinfo.h"

class QCell:public QTableWidgetItem
{
public:
      QCell(const CCellData&data );

      QCell(){ setBackgroundColor(QColor(255,255,255));}

      QTableWidgetItem *clone()const;

      void SetData(int role, const QVariant &value);

      QVariant data(int role)const;

      void SetFormula(const QString &formula);

      QString Formula()const;

      void SetDirty();

      CCellData GetFormatData();

      void SetFormatData(const CCellData&);

      void SetMyTextAlign(int);

      void SetSpanData(CMergeInfo &);

      void SetSpanClear();

      void SetDate(const QDate& date)
	  {
		  m_Date=date;
	  }
private:
 
      QVariant value()const;

      QVariant EvalExdission(const QString &str,int &pos)const;

      QVariant EvalTerm(const QString &str,int &pos)const;

      QVariant EvalFactor(const QString &str,int &pos)const;

      QVariant EvalSql(const QString &str,int &pos)const;

	  QVariant EvalDateTime(const QString& date)const;

      QVariant EvalMax(const QString& str,int& pos)const;

      QVariant EvalMin(const QString& str,int& pos)const;

      QVariant EvalAverage(const QString& str,int& pos)const;

      QVariant EvalSum(const QString& str,int& pos)const;

//add by xzb to deal with the toolbox for cal
      QVariant EvalPlus(const QString &str,int& pos)const;

      QVariant EvalMinus(const QString &str,int& pos)const;

      QVariant EvalMultipy(const QString&str,int& pos)const;

      QVariant EvalDivide(const QString& str,int& pos)const;

      QVariantList &Eval(const QString &str)const;

      QString EvalData(const QString& str,const int& time)const;

      QVariant DealDReport(int dataInfo,int station_id,int device_id,int ai_id,int date,int time)const;
	  //处理日报表中计算量问题
	  QVariant DealDCalReport(int dataInfo,int station_id,int device_id,int ai_id,int date,int time)const;
	  //处理日报表中电度问题
	  QVariant DealDKwhReport(int dataInfo,int station_id,int device_id,int ai_id,int date,int time)const;

      QVariant DealMReport(int dataInfo,int station_id,int device_id,int ai_id,int date,int time)const;

	  QVariant DealMCalReport(int dataInfo,int station_id,int device_id,int ai_id,int date,int time)const;

	  QVariant DealDDReport(int dataInfo,int station_id,int device_id,int ai_id,int date,int time)const;

	  QVariant DealCalReport(int dataInfo,int station_id,int device_id,int ai_id,int date,int time)const;
	  //添加年报表处理
	  QVariant DealYSReport(int dataInfo,int station_id,int device_id,int ai_id,int type,int date) const;
private:
      mutable QVariant m_vCachedValued;

      mutable bool m_bCachIsDirty;

public:
      QDate m_Date;
        
      CCellData m_ItemData;
};


#endif // QCELL_H
