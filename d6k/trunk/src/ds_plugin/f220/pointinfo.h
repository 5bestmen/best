#ifndef POINTINFO_H
#define POINTINFO_H

#include <QMap>
#include <QDebug>
#include "remoteponittableanalyse.h"

class CPointInfo
{
public:
	CPointInfo();
	~CPointInfo();
	//设置值
	void SetBinaryMap(const QMap<unsigned int, RPT> &mapBinary);
    //
	//设置遥测值
	void SetAnalogMap(const QMap<unsigned int, RPT> &mapAnalog);
	//设置遥脉值
	void SetKwhMap(const QMap<unsigned int, RPT> &mapKwh);
	//设置遥控
	void SetControlMap(const QMap<unsigned int, RPT> &mapControl);
	//获取值
	//遥信
	const QMap<unsigned int, RPT>& GetBinaryMap();
	//遥测
	const QMap<unsigned int, RPT>& GetAnalogMap();
	//遥脉
	const QMap<unsigned int, RPT>& GetKwhMap();
	//遥控
	const QMap<unsigned int, RPT>& GetControlMap();
    //
    void SetBinaryFlag(bool bFlag){ m_bBinaryFlag = bFlag; };
    //
    void SetAnalogFlag(bool bFlag){ m_bAnalogFlag = bFlag; };
    //
    void SetKwhFlag(bool bFlag){ m_bKwhFlag = bFlag; };
    //
    bool GetBinaryFlag(){return m_bBinaryFlag;}
    //
    bool GetAnalogFlag(){ return m_bAnalogFlag; }
    //
    bool GetKwhFlag(){ return m_bKwhFlag; }


private:
	
	//遥信map   装置序列号   说明
	QMap<unsigned int, RPT> m_mapBinary;
	//遥测
	QMap<unsigned int, RPT> m_mapAnalog;
	//遥脉
	QMap<unsigned int, RPT> m_mapKwh;
	//遥控
	QMap<unsigned int, RPT> m_mapControl;
    //
    bool m_bBinaryFlag;
    //
    bool m_bAnalogFlag;
    //
    bool m_bKwhFlag;
	
};

#endif // POINTINFO_H
