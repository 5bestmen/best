#ifndef SYSOUTDOCK_H
#define SYSOUTDOCK_H

#include <vector>
#include <memory>

#include <QDockWidget>
#include <QDateTime>
#include <QAbstractTableModel> 
//#include "../../include/devicestudio/iabstracttablemodel.h"

class QTabWidget;
class QTableView;
class CMsgLog;

#if 0
// struct MSG_LOG
// {
// 	unsigned int Level;
// 	TIMEPAK Time;
// 
// 	unsigned long long ProcessID;
// 	unsigned long long ThreadID;
// 
// 	char ModuleName[MAX_LOG_MODULE_NAME_LEN];
// 	char LogText[MAX_LOG_TEXT_LEN];
// 
// };


class IAbstractTableModel : public QAbstractTableModel
{
public:
	IAbstractTableModel(QObject *parent = 0) :QAbstractTableModel(parent) 	{}
	virtual ~IAbstractTableModel()	{}

//	virtual void AddItem(CMsgLog *pLog) = 0;

//	virtual int  AddItem(CMsgLog &log, QColor& color) = 0;

private:
};
#endif

class COutputModel : public QAbstractTableModel
{
	Q_OBJECT
	enum{ Date, Time, Tag, Type };

	enum
	{
		COL_LEVEL = 0,
		COL_INDEX,  //!< 序号
		COL_DATE ,
		COL_TIME ,
		COL_MODULE,
		COL_COMMENT,
		COL_PROCESS,
		COL_THREAD,
		COL_MAX ,
	};
	
public:
	explicit COutputModel(QObject *parent = 0, int rowCount = 1000, int colCount = COL_MAX);
	~COutputModel();

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
	std::vector<std::shared_ptr<CMsgLog>> m_arrLogs;
public:
	virtual void AddItem(CMsgLog *pLog);

//	virtual int  AddItem(CMsgLog &log, QColor& color);
 	void ClearAll();
};


class COutputDock : public QDockWidget
{
	Q_OBJECT
public:
	COutputDock(QWidget *parent);
	~COutputDock();
	//获取tableview的指针
	QTableView *GetTableView();
public slots:
	void ModelRowsInserted(const QModelIndex & parent, int start, int end);
	//右击菜单
	void Slot_ContextMenuRequest(const QPoint &cPoint);
	//copy
	void Slot_CpyAction();
	//clear
	void Slot_ClearItemAction();
	//run
	void Slot_StartRunAction();
	//stop
	void Slot_StopAction();
	//Export
	void Slot_ExportAction();
protected:
	void resizeEvent(QResizeEvent *event);
	void timerEvent(QTimerEvent *);
private:
	void  createCommandView();
	void  createLogCommandView();

private:
	QTabWidget	*m_pTabWnd;

// 	CSystemOutputView *m_commandView;
// 	CSystemOutputModel *m_commandModel;
// 
	QTableView *m_pLogView;
	COutputModel *m_pLogModel;
	int m_nLogTimerId;
	//是否滚动
	bool b_m_srcollFlag;
};

#endif // SYSOUTDOCK_H
