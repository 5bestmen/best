/*! @file qspreadsheet.h
<PRE>
********************************************************************************
模块名       :
文件名       :  qspreadsheet.h
文件实现功能 :  报表基类控件
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   报表基类控件
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/

#ifndef QSPREADSHEET_H
#define QSPREADSHEET_H
#include <QTextDocumentWriter>
#include <QTableWidget>
#include <QColorDialog>
#include <QFileDialog>
#include <QTextEdit>
#include "qcell.h"
#include "qxmlfunction.h"
#include "scadastudio/icore.h"
#include "scadastudio/base.h"
class QCell;
class SpreadsheetCompare;


class CSpreadSheet : public QTableWidget
{
	Q_OBJECT

public:
	CSpreadSheet(QWidget *parent = 0);
	~CSpreadSheet();
public:
	void InitSpreadSheet(CRptFile *pReport);
	bool AutoRecalculate() const
	{
		return m_bAutoRecalc;
	}
	QString CurrentSelection() const;
	QString CurrentLocation() const;
	QString CurrentFormula() const;
	QTableWidgetSelectionRange selectedRange() const;
	void clear(int newRow = RowCount, int newColcum = ColumnCount);
	//获取文件具体信息
//	bool readFile(const QString &fileName);
	//获取文件大概信息
	bool ReadGeneralInfo(const QString &fileName);
	bool writeFile(const QString &fileName);
	void sort(const SpreadsheetCompare &compare);
//	void sleep(int msec);
	void ProduceDailyReport();
	void ProduceHtmlPackage();
	int GetTableTypeInfo();
	int GetEndRow();
	int GetEndColumn();
	int GetTime(int, int, int);
	int GetRowCount();
	int GetColumnCount();
	QList<CMergeInfo> &GetSpanData();
	QCell *cell(int row, int column) const;
	void RecoderMergeInfo(CMergeInfo&);
	void SavePageOrExcel(const QString &str);
	void SetCellFormat(int row, int column, CCellData& cellContent);
	void SetTableType(int index)
	{
		m_TableData.m_nTableType = index; 
	};

	void SetTableType(int dataType, QString tableName)
	{
		m_TableData.m_nTableType = dataType;

		char fileName[128] = { 0 };
		char ptr[128] = { 0 };
		sprintf(ptr, "%s", "../project/report");
		sprintf(fileName, "%s", ptr);
		char path[256];
#ifdef WIN32
		switch (dataType)
		{
		case 0://日报表
			sprintf(path, "%s\\日报表\\%s", fileName, tableName.toStdString().c_str());
			break;
		case 1://月报表
			sprintf(path, "%s\\月报表\\%s", fileName, tableName.toStdString().c_str());
			break;
		case 2://年报表
			sprintf(path, "%s\\年报表\\%s", fileName, tableName.toStdString().c_str());
			break;
		case 3://特殊报表
			sprintf(path, "%s\\特殊报表\\%s", fileName, tableName.toStdString().c_str());
			break;
		default:
			break;
		}
#else
		switch(dataType)
		{
		case 0://日报表
			sprintf(path,"%s/日报表/%s",fileName,tableName.toStdString().c_str());
			break;
		case 1://月报表
			sprintf(path,"%s/月报表/%s",fileName,tableName.toStdString().c_str());
			break;
		case 2://年报表
			sprintf(path,"%s/年报表/%s",fileName,tableName.toStdString().c_str());
			break;
		case 3://特殊报表
			sprintf(path,"%s/特殊报表/%s",fileName,tableName.toStdString().c_str());
			break;
		default:
			break;
		}
#endif
		writeFile(path);
	};
	int GetTableType()const 
	{
		return m_TableData.m_nTableType; 
	}

	int IfInRange(int row, int column);
	QString &ProduceHtml();
	void SetDate(const QDate& date)
	{ 
		m_Date = date; 
	};

	QDate GetDate()const
	{ 
		return m_Date;
	}

signals:
	void modified();
	void cellSelected(const QString&);

public:
	CTableInfo &GetGernelTableInfo();
	void SetTableInfo(const CTableInfo&);
	void SetCellSpan(const CTableInfo&);

	void mouseReleaseEvent(QMouseEvent *event);
	void moveEvent(QMoveEvent *event);
public:
	enum
	{
		MagicNumber = 0x7F51C883,
		RowCount = 200,
		ColumnCount = 26
	};
	enum
	{
		point_normal,
		point_in_range,
		point_start
	};
	QString Text(int row, int column) const;
	QString Formula(int row, int column) const;
	void SetFormula(int row, int column, const QString &formula);
	bool m_bAutoRecalc;
	QList<CMergeInfo>m_lstMergeCells;
	QString    m_szSheetAsHtml;
	CMergeInfo  m_SpanData;
	CTableInfo  m_TableData;
	QDate m_Date;
public slots:
	void cut();
	void copy();
	void paste();
	void del();
	void selectCurrentRow();
	void selectCurrentColumn();
	void recalculate();
	void SetAutoRecalculate(bool recalc);
	void FindNext(const QString &str, Qt::CaseSensitivity cs);
	void FindPrevious(const QString &str, Qt::CaseSensitivity cs);
	void SetMerge();
	void SetUnMerge();
	void SetMyFont();
	void SetFontLeft();
	void SetFontMiddle();
	void SetFontRight();
	void SetFontColor();
	void CheckMergeInfo();
	bool SetChartView(CCtrlData* pData);
	bool SetBarView(CCtrlData* pData);
	bool SetPieView(CCtrlData* pData);
private slots:
	void somethingChanged();

};

class SpreadsheetCompare
{
public:
	bool operator()(const QStringList &row1, const QStringList &row2) const;
	enum { KeyCount = 3 };
	int keys[KeyCount];
	bool ascending[KeyCount];
};

#endif // QSPREADSHEET_H
