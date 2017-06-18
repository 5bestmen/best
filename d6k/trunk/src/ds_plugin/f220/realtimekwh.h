#ifndef REALTIMEKWH_H
#define REALTIMEKWH_H

#include <QWidget>
#include <QMap>

class CCommThread;
class QTableView;
class QStandardItemModel;
class QStandardItem;
class CPointInfo;
class CInfoConfigWgt;

class CRealTimeKwh : public QWidget
{
	Q_OBJECT

public:
    CRealTimeKwh(QWidget *parent, CCommThread *pCommunicate, CPointInfo *pPointInfo, CInfoConfigWgt *pConfgWgt);
	~CRealTimeKwh();
	void AnalyseRealKwhData(int iDeviceID, int nPointID, float nValue, const QString &strType);
	void InitWidget();
	//
	void InitData();
public slots:
	void Slot_AnalyseKwhData(int iDeviceID, int nPointID, int nValue);

	void Slot_AnalyseShortTimeKwhData(int iDeviceID, int nPointID, float nValue);

	void Slot_AnalyseLongTimeKwhData(int iDeviceID, int nPointID, float nValue);

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

#endif // REALTIMEKWH_H
