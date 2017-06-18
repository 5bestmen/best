/*! @file configinfo.h
<PRE>
********************************************************************************
模块名       :
文件名       :  configinfo.h
文件实现功能 :  XML基本数据定义
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   XML基本数据定义
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#ifndef CONFIGINFO_H
#define CONFIGINFO_H

#include <QObject>
#include <QVector>


class AIN
{
public:
	AIN()
	{

	}
	~AIN()
	{

	}
public:
	//数据成员
	int m_OccNo;
	int m_Id;
	int m_ChannelOccNo;
	int m_DeviceOccNo;
	int m_BlockOccNo;
	int m_Address;
	int m_DataSource;

	float m_InitVal;
	float m_InitQua;
	QString m_szTagName;
	QString m_szDescription;

	int m_TransFromType;
	QString m_szTransFormName;
	QString m_szTransFormDesc;

	int m_AlarmType;
	QString m_szAlarmTagName;
	QString m_szAlarmDesc;

	float m_RangeL;
	float m_RangeH;

	float m_HighQua;
	float m_LowQua;

	QString m_szUnitName;
	QString m_szExpress;

	int m_PinLanel;
	int m_DataType;
	int m_SaveDisk;
	int m_SaveDiskPeriod;
	int m_SaveDb;
	int m_SaveDbPeriod;
	int m_Sensitivity;
	int m_ReferenceCount;

};

class DIN
{
public:
	DIN()
	{

	}
	~DIN()
	{

	}
public:
	//数据成员
	//数据成员
	int m_OccNo;
	int m_Id;
	int m_ChannelOccNo;
	int m_DeviceOccNo;
	int m_BlockOccNo;
	int m_Address;
	int m_DataSource;

	float m_InitVal;
	float m_InitQua;
	QString m_szTagName;
	QString m_szDescription;

	int m_TransFromType;
	QString m_szTransFormName;
	QString m_szTransFormDesc;

	int m_AlarmType;
	QString m_szAlarmTagName;
	QString m_szAlarmDesc;

	QString m_szExpress;

	int m_PinLabel;
	int m_bIsSBO;
	int m_TelCtrlWaitTime;
	int m_TelCtrlSendTime;

	int m_ReferenceCount;

};


class CDevice
{
public:
	CDevice(){};
	~CDevice()
	{
		qDeleteAll(m_VecAinData);
		m_VecAinData.clear();

		qDeleteAll(m_VecDinData);
		m_VecDinData.clear();
	}
public:	
	int m_OccNo;

	QString m_szDevName;

	int GetAINCount() const
	{
		return m_VecAinData.size();
	}
	int GetDINCount() const
	{
		return m_VecDinData.size();
	}
	AIN* GetAin(int i)
	{
		if (i<m_VecAinData.size())
		{
			return m_VecAinData.at(i);
		}
		else{
			return Q_NULLPTR;
		}
	}
	DIN* GetDin(int i)
	{
		if (i< m_VecDinData.size())
		{
			return m_VecDinData.at(i);
		}
		else
		{
			return Q_NULLPTR;
		}
	}

	void AddAInInfo(AIN* pAin);
	void AddDINInfo(DIN* pDin);
private:
	QVector<AIN*> m_VecAinData;
	QVector<DIN*> m_VecDinData;

private:
	int m_AinCount;
	int m_DInCount;
};


class CDeviceMgr
{
public:
	CDeviceMgr(){};
	~CDeviceMgr()
	{
		qDeleteAll(m_VecDevice);
		m_VecDevice.clear();
	}
public:
	int GetDevNum() 
	{
		return m_VecDevice.size();
	}
	CDevice* GetDev(int i)
	{
		if (i <m_VecDevice.size())
		{
			return m_VecDevice.at(i);
		}
		else
		{
			return Q_NULLPTR;
		}
	}
	void AddDevice(CDevice* pDevice);
private:
	QVector<CDevice*> m_VecDevice;
};


#endif // CONFIGINFO_H
