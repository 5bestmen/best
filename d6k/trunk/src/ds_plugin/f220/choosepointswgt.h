#ifndef CHOOSEPOINTSWGT_H
#define CHOOSEPOINTSWGT_H

#define BINARYGROUP        4           //遥信
#define TBINARYGROUP       113         //遥信
#define TABINARYGROUP       255         //遥信


#define ANALOGGROUP        5           //遥测
#define TANALOGGROUP       49          //遥测
#define TBANALOGGROUP      96          //遥测
#define TBCANALOGGROUP     136          //遥测
#define TBCBNALOGGROUP     138          //遥测
#define TBCCNALOGGROUP     139          //遥测

#define  KWHGROUP          6            //遥脉

#define  CONTROLGROUP      7            //遥控


#include <QWidget>
#include "remoteponittableanalyse.h"

class CDragTableWgt;
class QTabWidget;
class CRemotePonitTableAnalyse;
class CPointInfo;
class CAnalysePointBin;
class CVirtualTableWgt;
class CAnalogDragTableWgt;

class CChoosePointsWgt : public QWidget
{
	Q_OBJECT

public:
	CChoosePointsWgt(CPointInfo *pPointInfo, QWidget *parent = 0);
	//设置页面布局
	void SetWgtLayout();
	void InitWgt(CRemotePonitTableAnalyse *pPointAnalyse);
	~CChoosePointsWgt();
	//往表格中插入数据
	bool InsertDataToTable();
	//获取遥信点表信息
	bool GetBinaryPoints();
	//获取遥测点表信息
	bool GetAnalogPoints();
	//获取遥脉点表信息
	bool GetKwhPoints();
	//获取遥控点表信息
	bool GetControlPoints();
    //往表里插入输入
    void UpdateToDesTables(RPT rptInfo, CDragTableWgt *pDesTableWgt);
	//往遥信表格中插入数据
	void UpdateAnalogToDesTables(RPT rptInfo, CAnalogDragTableWgt *pDesTableWgt);
    //获取表格对应的表述
    QMap<int, QString> GetRemoteToDesc();
    //设置名称
    void SetProjectPath(const QString &strPath);
    //保存修改后的点表
    void SaveChooseBinFile(const QString &strFilename);
public slots:
    //更新遥信测点
    void Slot_UpdateBinaryPointData();
	//更新遥测测点
	void Slot_UpdateAnalogPointData();
	//更新遥脉测点信息
	void Slot_UpdateKwhPointData();
	//更新遥控测点信息
	void Slot_UpdateControlPointData();
    //
    void Slot_UpdateAllChoosePoints(const QString &strFilename);

private:
	//desc 遥信
	CDragTableWgt *m_pDescTableWgt;
	//source 遥信
	CDragTableWgt *m_pSourceTableWgt;
	// desc 遥测
	CAnalogDragTableWgt *m_pAnalogDescTableWgt;
	//source  遥测
	CAnalogDragTableWgt *m_pAnalogSourceTableWgt;
	//desc 遥脉
	CDragTableWgt *m_pKwhDescTableWgt;
	//source  遥脉
	CDragTableWgt *m_pKwhSourceTableWgt;

	//遥控 desc
	CDragTableWgt *m_pControlDescTableWgt;
	//遥控 source
	CDragTableWgt *m_pControlSourceTableWgt;
    //虚遥信  desc
    CVirtualTableWgt *m_pVirtualDescTableWgt;
    //虚遥信  source
    CDragTableWgt *m_pVirtualSourceTableWgt;
	//tab
	QTabWidget *m_pTabWgt;
	//点表解析
	CRemotePonitTableAnalyse *m_pPointTableAnalyse;
	//点表数据
	CPointInfo *m_pPointInfo;
    //
    CAnalysePointBin *m_pCurrentPoints;
};

#endif // CHOOSEPOINTSWGT_H
