/*! @file qxmlfunction.h
<PRE>
********************************************************************************
模块名       :
文件名       :  qxmlfunction.h
文件实现功能 :  文件以xml格式读写
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   文件以xml格式读写
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/

#ifndef QXMLFUNCTION_H
#define QXMLFUNCTION_H
#include <QDomElement>
#include <QObject>
#include <QVariant>
#include <QList>
#include "qcell.h"


enum{ DAY = 0, MONTH = 1, YEAR = 2, SPECIAL = 3 };

class QXmlWriter
{
public:
	QXmlWriter();
	~QXmlWriter();
public:
	void OnWriteXmlFile(const QString& nFileName);
public:
	void SetReportInfo(CTableInfo *pReportInfo)
	{
		Q_ASSERT( pReportInfo != Q_NULLPTR );
		m_pReportInfo = pReportInfo;
	}
	void SetReportCell(QList<CCellData*>& nList)
	{
		m_CellList = nList;
	}
	void SetRptCtrls(QList<CCtrlData*>& nList)
	{
		m_CtrlList = nList;
	}
protected:
	void AddReportInfo(QDomElement elemInfo);
	void AddReportSpan(QDomElement elemInfo);
	void AddReportCell(QDomElement elemInfo);
	void AddReportH(QDomElement elemInfo);
	void AddReportW(QDomElement elemInfo);
	//控件整体信息存储
	void AddRptCtrl(QDomElement elemInfo);
	
private:
	//报表基本信息
	CTableInfo *m_pReportInfo;
	//报表单元格集合
	QList< CCellData* > m_CellList;
	//报表控件集合
	QList< CCtrlData* > m_CtrlList;
	//文档存储
	QDomDocument m_doc;
};


class QXmlReader
{
public:
	QXmlReader();
	~QXmlReader();
public:
	bool OnReadXmlFile(const QString& nFileName);
	CTableInfo* GetReportInfo(){ return m_pReportInfo; }
	QList< CCellData* >& GetCellList()
	{ 
		return m_CellList; 
	}
	QList< CCtrlData* >& GetCtrlList()
	{
		return m_CtrlList;
	}
protected:
	void SetFileName(const QString& szName)
	{
		m_szFileName = szName;
	}
	QString& GetFIleName()
	{
		return m_szFileName;
	}
	bool OnParaseBaseInfo(QDomElement eleInfo);
	bool OnParaseSpanInfo(QDomElement eleInfo);
	bool OnParaseWidthInfo(QDomElement eleInfo);
	bool OnParaseHeigthInfo(QDomElement eleInfo);
	bool OnParaseCellInfo(QDomElement eleInfo);
	bool OnParaseCtrlInfo(QDomElement eleInfo);
private:
	//报表基本信息
	CTableInfo *m_pReportInfo;
	//文件信息，文件传入信息，包含，名称和路径
	QString m_szFileName;
	//报表单元格集合
	QList< CCellData* >m_CellList;
	//报表控件集合
	QList< CCtrlData* >m_CtrlList;
private:
	QDomElement m_BaseInfoElement;
	QDomElement m_SpanElement;
	QDomElement m_HeightElement;
	QDomElement m_WidthElement;
	QDomElement m_CellElement;
	QDomElement m_CtrlInfoElement;
};



#endif // QXMLFUNCTION_H
