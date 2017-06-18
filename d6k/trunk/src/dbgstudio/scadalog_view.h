

#ifndef SCADA_LOG_VIEW_H
#define SCADA_LOG_VIEW_H

#include <vector>
#include <QWidget>
#include <QMutex>
#include <memory>
#include <QDate>
#include <QTime>
#include <QColor>

#include "./GeneratedFiles/ui_scadalog_view.h"
#include <deque>
#include <string>
#include <mutex>
#include "singleton.h"

class CScadaLogMQTask;

class QStandardItemModel;
class CScadaLog;

class CScadaLogModel : public QAbstractTableModel
{
	Q_OBJECT
	enum{ Date, Time, Tag, Type };
public:
	enum
	{
		COL_INDEX = 0,  //!< 序号
		COL_DATE,
		COL_TIME,
		COL_MODULE_NAME,
		COL_LOG_TEXT,
		COL_PROCESS,
		COL_THREAD,

		COL_FILE_NAME,
		COL_LINE,
		COL_FUNC_NAME,
		COL_LEVEL,
		COL_PAD,
		COL_MAX,
	};

public:
	explicit CScadaLogModel(QObject *parent = 0, int rowCount = 1000, int colCount = COL_MAX);
	~CScadaLogModel();

	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	//获取一行数据
	QString CpyRowData(const QModelIndex &index);
private:
	int m_nRowCount;
	int m_nColCount;

	QStringList m_lstHeader;
private:
	//std::vector<CMsgLog *>  m_arrLogs;
	std::deque<std::shared_ptr<CScadaLog>> m_arrLogs;
public:
	virtual void AddItem(CScadaLog *pLog);

//	virtual int  AddItem(CMsgLog &log, QColor& color);
	void ClearAll();

	void DispLogString(std::deque<std::shared_ptr<CScadaLog>> &arrLogs);

	enum
	{
		MAX_LOG_NUM = 1000,
	};
};


class CScadaLogView : public QWidget
{
	Q_OBJECT
public:
	static CScadaLogView *GetInstance();
public:
	~CScadaLogView();

	//初始化界面
	void InitView();

	void InitTask();

	//读取表格中的文件写入到execl表格中
	void ExportDataToFile(const QString &strFileName);
protected:
	//定时获取发送过来的信息
	virtual void timerEvent(QTimerEvent *event);
	void DispLogString();

public slots:
 	//导出
	void Slot_ClickExport();
	//筛选
	void Slot_ClickFilter();
private:
	CScadaLogView(QWidget *parent = 0);

	friend DefaultSingletonTraits<CScadaLogView>;

	Ui::CScadaLogView ui;
	//实时数据moduel
 	CScadaLogModel *m_pTableModel;

	std::shared_ptr<CScadaLogMQTask> m_pPullLogTask;

	int m_nLogTimerId;

};

#endif // SCADA_LOG_VIEW_H
