#include "pointinfo.h"


CPointInfo::CPointInfo()
{
    m_bBinaryFlag = false;
    m_bAnalogFlag = false;
	m_bKwhFlag = false;
}

//设置值
void CPointInfo::SetBinaryMap(const QMap<unsigned int, RPT> & mapBinary)
{
	m_mapBinary.clear();
	m_mapBinary = mapBinary;
    m_bBinaryFlag = true;
}

//设置遥测值
void CPointInfo::SetAnalogMap(const QMap<unsigned int, RPT> & mapAnalog)
{
	m_mapAnalog.clear();
	m_mapAnalog = mapAnalog;
    m_bAnalogFlag = true;
}

//设置遥脉值
void CPointInfo::SetKwhMap(const QMap<unsigned int, RPT> & mapKwh)
{
	m_mapKwh.clear();
	m_mapKwh = mapKwh;
    m_bKwhFlag = true;
}

//设置遥控
void CPointInfo::SetControlMap(const QMap<unsigned int, RPT> & mapControl)
{
	m_mapControl.clear();
	m_mapControl = mapControl;
}

//遥信
const QMap<unsigned int, RPT>& CPointInfo::GetBinaryMap()
{
	return m_mapBinary;
}

//遥测
const QMap<unsigned int, RPT>& CPointInfo::GetAnalogMap()
{
	return m_mapAnalog;
}

//遥脉
const QMap<unsigned int, RPT>& CPointInfo::GetKwhMap()
{
	return m_mapKwh;
}

//遥控
const QMap<unsigned int, RPT>& CPointInfo::GetControlMap()
{

	return m_mapControl;
}

CPointInfo::~CPointInfo()
{
}
