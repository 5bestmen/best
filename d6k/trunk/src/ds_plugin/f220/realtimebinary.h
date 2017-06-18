#ifndef REALTIMEBINARY_H
#define REALTIMEBINARY_H

#include <QWidget>
#include <QMap>

class CCommThread;
class QTableView;
class QStandardItemModel;
class QStandardItem;
class CPointInfo;
class CInfoConfigWgt;

class CRealTimeBinary : public QWidget
{
	Q_OBJECT

public:
    CRealTimeBinary(QWidget *parent, CCommThread *pCommunicate, CPointInfo *pPointInfo, CInfoConfigWgt *pConfgWgt);
	~CRealTimeBinary();
	void AnalyseRealBinaryData(int iDeviceID, int nPointID, int nValue, const QString &strType);
	void InitWidget();
	//初始化数据
	void InitData();

	enum
	{
		eSignal = 1,
		eDouble = 2,
		eSignalGroup = 3,
	};
public slots:
    void Slot_AnalyseoneBinaryData(int iDeviceID, int nPointID, int nValue);
    void Slot_AnalyseDoubleBinaryData(int iDeviceID, int nPointID, int nValue);

	void Slot_AnalyseGroupBinaryData(int iDeviceID, int nPointID, int nValue);

private:
	//通信插件
	CCommThread *m_pCommunicate;
	//id item
	QMap<int, QStandardItem*> m_mapIdItemObj;
	//tableview
	QTableView *m_pTableView;
	//model
	QStandardItemModel *m_pTableModel;
	//当前个数
	int m_iCurrentPointsNum;
	//点表数据
	CPointInfo *m_pPointInfo;
    //
    CInfoConfigWgt *m_pConfgWgt;

};

#endif // REALTIMEBINARY_H
