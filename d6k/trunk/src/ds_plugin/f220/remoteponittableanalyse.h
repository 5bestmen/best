#ifndef REMOTEPONITTABLEANALYSE_H
#define REMOTEPONITTABLEANALYSE_H

#include <QObject>
#include <QMap>
//#pragma execution_character_set("utf-8")

//点表数据结构
struct  RPT
{
	//条目号
	unsigned int NUM;
    //组号
	unsigned int GroupNum;
	//条目号2
	unsigned int NUM2;
	//名称
	char Destriber[255];
	//单位
	char Unit[255];
    //遥测缩放比例
    float zoom;

	//b
	float fBanalog;
	//k
	float fKanalog;
	//上限
	float fUpperLimit;
	//下限
	float fDownLimit;
	//绝对门槛
	float fThreshold;

};
//点表名称
#define  POINTTABLENAME "ini/DA-D200.txt"
//缩放比例
#define  MAXZOOMDATA 4095

class QIODevice;
class CRemotePonitTableAnalyse : public QObject
{
	Q_OBJECT

public:
	CRemotePonitTableAnalyse(QObject *parent);
	~CRemotePonitTableAnalyse();
	//打开文件
	bool OpenPointTableFile(const QString &strFileName);
	//解析文件
	bool AnalyseXmlData(QIODevice *pDevice);
	//获取原始值
	//遥信
	QList<RPT> GetBinaryData();
    // 遥信序号 组
    QMap<int, RPT> GetBinaryIdGroup();
    //遥测序号组
    QMap<int, RPT> GetAnalogIdGroup();
    //遥脉序号  组
    QMap<int, RPT> GetKwhIdGroup();
    //遥控序号  组  
    QMap<int, RPT> GetControlIdGroup();

	//遥测
	QList<RPT> GetAnaloyData();
	//遥脉
	QList<RPT> GetKwhData();
	//遥控
	QList<RPT> GetControlData();
    //获取全部数据
    QMap<int, QMap<int, RPT> > GetAllPointInfo();
    //获取缩放比例
    QMap<int, float> GetAnalogZoom();
    //
    const QStringList & GetBinaryCodes()
    {
        return m_lstBinaryCodes;
    }
    //
    const QStringList & GetAnalogCodes()
    {
        return m_lstAnalogCodes;
    }
    //
    const QStringList & GetKwhCodes()
    {
        return m_lstKwhCodes;
    }
    //
    const QStringList & GetControlCodes()
    {
        return m_lstControlCodes;
    }

private:
	//遥信 
	QList<RPT> m_lstBinary;
	//遥测
	QList<RPT> m_lstAnalog;
	//遥脉
	QList<RPT> m_lstKWH;
    //遥控
	QList<RPT> m_lstControl;
	//所有数据  类型   编号
    QMap<int, QMap<int, RPT> > m_mapAllPoints;
    //遥测    装置序号  缩放比例
    QMap<int, float> m_mapAnalogZoom;
    //
    QStringList m_lstBinaryCodes;
    //
    QStringList m_lstAnalogCodes;
    //
    QStringList m_lstKwhCodes;
    //
    QStringList m_lstControlCodes;
    //遥信  设备号  组
    QMap<int, RPT> m_mapBinaryDeviceIdGroup;
    //遥测  设备号  组
    QMap<int, RPT> m_mapAnalogDeviceIdGroup;
    //遥脉   设备号  组
    QMap<int, RPT> m_mapKwhDeviceIdGroup;
    //遥控  设备号 组
    QMap<int, RPT> m_mapControlDeviceIdGroup;

};

#endif // REMOTEPONITTABLEANALYSE_H
