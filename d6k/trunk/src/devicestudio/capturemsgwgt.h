#ifndef CAPTUREMSGWGT_H
#define CAPTUREMSGWGT_H
#include <deque>
#include <QWidget>
#include <QMutex>
#include <memory>
#include <QDate>
#include <QTime>
#include <QColor>
#include "./GeneratedFiles/ui_capturemsgwgt.h"
#include <deque>
#include <string>
#include <mutex>
#include "singleton.h"

//设置最大显示行数
#define MAXCAPTURENUMS 10000

struct RECVOUTMSG
{
	QString moduleName;
	QByteArray msgContext;
	int msgLegth;
	unsigned int msgType;
	QString subType;
	QString dataSoruce;
	QString dataDesc;
};

class QStandardItemModel;

class CChooseInterfaceWgt : public QWidget
{
	Q_OBJECT
public:
	CChooseInterfaceWgt(QWidget *parent = 0);
	//增加选项
	void AddPluginSection(const QString &strPluginDeviceName);

	~CChooseInterfaceWgt();
	//
public slots:
	//选中树父节点
	void Slot_SelectTreeTopLevelItem(QTreeWidgetItem *pItem, int iIndex);
	//点击开始
	void Slot_StartCapturing();
	//点击停止
	void Slot_StopCapturing();
signals:
	void Signal_ChoosingInterfaces(QStringList strChoose);
	void Singal_CloseCapturing();

private:
	QTreeWidget *m_pTreeWgtChoose;
	//开始按钮
	QPushButton *m_pBtnStart;
	//结束按钮
	QPushButton *m_pBtnStop;
	//关闭窗口按钮
	QPushButton *m_pBtnClose;
};

struct CCaptureMsg
{
	QString strNumber;
	QString strModuleName;
	QDate tdata;
	QTime tTime;
	QString strSender;
	QString strRecver;
	QString strProtrol;
	QString strType;
	int nLength;
	QString strPartContext;
	QByteArray btDetailInfo;
	QColor colorBackground;
};

class CCaptureModel : public QAbstractTableModel
{
	Q_OBJECT

	enum
	{
		COLUMN_ZERO = 0,
		CONUMN_ONE = 1,
		COLUMN_TOW = 2,
		COLUMN_THREE = 3,
		COLUMN_FOUR = 4,
		COLUMN_FIVE = 5,
		COLUMN_SIX = 6,
		COLUMN_SEVEN = 7,
		COLUMN_EIGHT = 8,
		COLUMN_NINE = 9
	};

public:
	explicit CCaptureModel(QObject *parent = 0, int rowCount = 1000, int colCount = COLUMN_NINE+1);
	~CCaptureModel();

	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QByteArray GetDetailData(const QModelIndex &index);
	QString GetColumnData(int nColumn, const QModelIndex &index);
	void RemoveFirstRow();

	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	QString GetTotalData();
private:
	int m_nRowCount;
	int m_nColCount;

	QStringList m_lstHeader;
private:
    std::deque<std::shared_ptr<CCaptureMsg>> m_arrLogs;
public:
	//virtual void AddItem(CCaptureMsg *pLog);

	virtual void  AddItem(CCaptureMsg *pLog, QColor& color);
	void ClearAll();
};


class CCaptureMsgWgt : public QWidget
{
	Q_OBJECT

public:
	static CCaptureMsgWgt *GetInstance();



public:
	//初始化界面
	void InitCapTureWgt();

	~CCaptureMsgWgt();
	//更新监控数据
	void UpdateCapturingData(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc);
	//插入数据到表格
	void UpdateDataToTable(const RECVOUTMSG &sMsgdata, const QString &strShowData);
	//解析数据
	void AnalyseData(const QByteArray &btBuff);
	//往表格中插入数据
	void InsertDataToTables(const QString &byBuff,const QByteArray &btBuff);
	//增加选项
	void AddPluginSection(const QString &strPluginDeviceName);

	//读取表格中的文件写入到execl表格中
	void ExportDataToFile(const QString &strFileName);
    //提供tree接口
    QTreeWidget *GetDetailTreeWgt();
    //
signals:
    void Signal_CurrentClickBtData(QByteArray &btData);


protected:
	//定时获取发送过来的信息
	virtual void timerEvent(QTimerEvent *event);

public slots:
    //点击选择接口
    void Slot_ClickChooseInterfaces();
	//开始检测
	void Slot_BeginCapturing(const QStringList &strlstCode);
	//停止检测
	void Slot_endCapturing();
	//点击显示详细信息
	void Slot_ClickShowDetailInfo(const QModelIndex &modelIndex);
	//重新检测
	void Slot_RestartCapturing();
	//导出
	void Slot_ClickExport();
	//筛选
	void Slot_ClickFilter();


private:
	CCaptureMsgWgt(QWidget *parent = 0);

	friend DefaultSingletonTraits<CCaptureMsgWgt>;

	Ui::CCaptureMsgWgt ui;
	//实时数据moduel
	CCaptureModel *m_pTableModel;
	//选择接口界面
	CChooseInterfaceWgt *m_pChooseWgt;

	//是否检测
	bool m_bCapturing;
	//需要监测的字符
	QStringList m_strlstCapturing;

	QMutex m_mutexLock;
	//接收到的数据
	QList<RECVOUTMSG> m_lstRecvMsg;
	//记数
	int m_nIndexNum;

};

#endif // CAPTUREMSGWGT_H
