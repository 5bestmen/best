#ifndef CAPTUREMSGWGT_H
#define CAPTUREMSGWGT_H

#include "singleton.h"

#include <vector>
#include <QWidget>
#include <QMutex>
#include <memory>
#include <QDate>
#include <QTime>
#include <QColor>
#include "ui_capturemsgwgt.h"

#include <deque>
#include <string>
#include <mutex>

//���������ʾ����
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
	//����ѡ��
	void AddPluginSection(const QString &strPluginDeviceName);

	~CChooseInterfaceWgt();
	//
public slots:
	//ѡ�������ڵ�
	void Slot_SelectTreeTopLevelItem(QTreeWidgetItem *pItem, int iIndex);
	//�����ʼ
	void Slot_StartCapturing();
	//���ֹͣ
	void Slot_StopCapturing();
signals:
	void Signal_ChoosingInterfaces(QStringList strChoose);
	void Singal_CloseCapturing();

private:
	QTreeWidget *m_pTreeWgtChoose;
	//��ʼ��ť
	QPushButton *m_pBtnStart;
	//������ť
	QPushButton *m_pBtnStop;
	//�رմ��ڰ�ť
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
	QByteArray getDetailData(const QModelIndex &index);
	QString GetColumnData(int nColumn, const QModelIndex &index);
	void removeFirstRow();

	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	QString getTotalData();
private:
	int m_nRowCount;
	int m_nColCount;

	QStringList m_lstHeader;
private:
	std::vector<std::shared_ptr<CCaptureMsg>> m_arrLogs;
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
	//��ʼ������
	void InitCapTureWgt();

	~CCaptureMsgWgt();
	//���¼������
	void UpdateCapturingData(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc);
	//�������ݵ����
	void UpdateDataToTable(const RECVOUTMSG &sMsgdata, const QString &strShowData);
	//��������
	void AnalyseData(const QByteArray &btBuff);
	//������в�������
	void InsertDataToTables(const QString &byBuff,const QByteArray &btBuff);
	//����ѡ��
	void AddPluginSection(const QString &strPluginDeviceName);

	//��ȡ����е��ļ�д�뵽execl�����
	void ExportDataToFile(const QString &strFileName);

protected:
	//��ʱ��ȡ���͹�������Ϣ
	virtual void timerEvent(QTimerEvent *event);

public slots:
    //���ѡ��ӿ�
    void Slot_ClickChooseInterfaces();
	//��ʼ���
	void Slot_BeginCapturing(const QStringList &strlstCode);
	//ֹͣ���
	void Slot_endCapturing();
	//�����ʾ��ϸ��Ϣ
	void Slot_ClickShowDetailInfo(const QModelIndex &modelIndex);
	//���¼��
	void Slot_RestartCapturing();
	//����
	void Slot_ClickExport();
	//ɸѡ
	void Slot_ClickFilter();


private:
	CCaptureMsgWgt(QWidget *parent = 0);

	friend DefaultSingletonTraits<CCaptureMsgWgt>;

	Ui::CCaptureMsgWgt ui;
	//ʵʱ����moduel
	CCaptureModel *m_pTableModel;
	//ѡ��ӿڽ���
	CChooseInterfaceWgt *m_pChooseWgt;

	//�Ƿ���
	bool m_bCapturing;
	//��Ҫ�����ַ�
	QStringList m_strlstCapturing;

	QMutex m_mutexLock;
	//���յ�������
	QList<RECVOUTMSG> m_lstRecvMsg;
	//����
	int m_nIndexNum;

};

#endif // CAPTUREMSGWGT_H
