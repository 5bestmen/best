/*! @file configinfo.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  configinfo.h
�ļ�ʵ�ֹ��� :  XML�������ݶ���
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   XML�������ݶ���
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
	//���ݳ�Ա
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
	//���ݳ�Ա
	//���ݳ�Ա
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
