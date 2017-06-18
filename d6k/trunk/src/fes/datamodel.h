#ifndef CDATAMODEL_H
#define CDATAMODEL_H

#include <QAbstractTableModel>
#include "fesconfig.h"

enum DATAMODEL_TYPE
{
	PROJECT_TYPE,                 //!<������Ϣ
	CHANNEL_GERENAL_TYPE,         //!<ͨ�������Ϣ
	CHANNEL_CONTAINERS_TYPE,      //!<ͨ�������豸����Ϣ
	DEVICE_GERENAL_TYPE,          //!<�豸�����Ϣ
	DEVICE_CONTAINERS_TYPE,       //!<�豸��������Ϣ
	AIN_TYPE,                     //!<ҳ��ΪAIN 
	DIN_TYPE,                     //!<ҳ��ΪDIN
	AOUT_TYPE,                    //!<ҳ��ΪAout
	DOUT_TYPE                     //!<ҳ��ΪDout
};


enum CHANNEL_HEADER
{
	Channel_OccNo,
	Channel_NameOccNo,
	Channel_IsDefined,
	Channel_ScanEnable,
	Channel_Init,
	Chanel_Quality,
	Channel_PeerQuality,
	Channel_Heartbeat,
	Channel_PrintOn,
	Channel_WorkNet,
	//COM
	Channel_MasterComPort,
	Channel_SlaveComPort,
	Channel_BaudRate,
	Channel_Parity,
	Channel_DataBit,
	Channel_StopBit,
	Channel_Pad,
	//ETHE
	Channel_MasterIP,
	Channel_SlaveIP,
	Channel_MasterPort,
	Channel_SlavePort,

	Channel_State,
	Channel_Prority,
	Channel_SetTimeAttr,
	Channel_DeviceCount,
	Channel_NodeOccNo,
	Channel_ChannelName,
	Channel_DriverName,
	Channel_DriverType,
	Channel_StateCore,
	Channel_StateDriver,
	
};


enum DEVICE_HEADER
{
	DEVICE_OccNo,
	DEVICE_NameOccNo,
	DEVICE_NodeOccNo,
	DEVICE_ChannelOccNo,
	DEVICE_ModelOccNo,
	DEVICE_Type,
	DEVICE_Pad1,
	DEVICE_Pad2,
	DEVICE_ADDRESS,
	DEVICE_Extension,
	DEVICE_IsDefined,
	DEVICE_ScanEnable,
	DEVICE_Init,
	DEVICE_Quality,
};


enum AIN_HEADER
{
	AIN_OccNo,
	AIN_BlockNo,
	AIN_NameOccNo,
	AIN_IsSpare,
//  AIN_Pad1,
	AIN_Pad2,
	AIN_Address,
	AIN_State,
	AIN_IsDefined,
	AIN_ScanEnable,
	AIN_Init,
	AIN_Quality,
	AIN_ManSet,

//	AIN_AlarmState,

	AIN_SignalValue,
	AIN_RawValue,
	AIN_Value,
	AIN_OldValue,

	AIN_InitValue,
	AIN_DataSource,
	AIN_RangeL,
	AIN_RangeH,
	AIN_HighQua,
	AIN_LowQua,

	AIN_NodeOccNo,
	AIN_ChannelOccNo,
	AIN_DeviceOccNo,

	AIN_AlarmOccNo,
	AIN_ExpressOccNo,
	AIN_TransformType,
	AIN_TransformOccNo,
	AIN_PinLabelOccNo,
	AIN_UnitNameOccNo,
	AIN_LastUpdateTime,
}; 


enum DIN_HEADER
{
	DIN_OccNo,
	DIN_BlockNo,
	DIN_NameOccNo,

	DIN_NeedTranmitFlag,	
	DIN_Pad2,
	DIN_IsSpare,

	DIN_Address,

	DIN_InitValEnable,
	DIN_InitValue,
	DIN_InitQua,

	DIN_SlaveOccNo,
	DIN_DDIFilter,
	DIN_DDIFlag,
	DIN_DDIInUse,

	DIN_State,

	DIN_IsDefined,
	DIN_ScanEnable,
	DIN_Init,
	DIN_Quality,
	DIN_ManSet,

	DIN_Value,
	DIN_NegValue,
	DIN_RawValue,

	DIN_CtrlByte,
	DIN_IsSOE,

	DIN_StartCtrl,
	DIN_SignalType,

//	DIN_SlaveOccNo,
//	DIN_PeerOccNo,

	DIN_NodeOccNo,
	DIN_ChannelOccNo,
	DIN_DeviceOccNo,

	DIN_AlarmOccNo,
	DIN_DataSource,

	DIN_ExpressOccNo,

	DIN_Desc0_OccNo,
	DIN_Desc1_OccNo,
	DIN_PinLabelOccNo,
	DIN_LastUpdateTime,
};


enum AOUT_HEADER
{
	AOUT_OccNo,
	AOUT_BlockNo,
	AOUT_NameOccNo,

	AOUT_State,
	AOUT_IsDefined,
	AOUT_ScanEnable,
	AOUT_Init,
	AOUT_Quality,

	AOUT_Output,
	AOUT_RawData,
	AOUT_DataSource,

	AOUT_NodeOccNo,
	AOUT_ChannelOccNo,
	AOUT_DeviceOccNo,

	AOUT_AlarmOccNo,
	AOUT_ExpressOccNo,

	AOUT_TransformType,
	AOUT_TransformOccNo,

	AOUT_PinLabelOccNo,
	AOUT_UnitNameOccNo,
	AOUT_LastUpdateTime,
};

enum DOUT_HEADER
{
	DOUT_OccNo,
	DOUT_BlockNo,
	DOUT_NameOccNo,

	DOUT_State,
	DOUT_IsDefined,
	DOUT_ScanEna,
	DOUT_Init,
	DOUT_Quality,

	DOUT_Output,
	DOUT_RelayState,
	DOUT_DataSource,
	DOUT_CtrlType,
	DOUT_Filter,

	DOUT_NodeOccNo,
	DOUT_ChannelOccNo,
	DOUT_DeviceOccNo,

	DOUT_AlarmOccNo,
	DOUT_ExpressionOccNo,
	DOUT_PinLabelOccNo,
	DOUT_LastUpdateTime,
};

class CDbSvc;

class CDataModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	CDataModel(std::shared_ptr<CDbSvc>& pMem, QObject *parent=0 );

	~CDataModel();
public:
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole)const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */);
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
public:
	void SetColumnCount(int nCount)
	{ 
		m_nColumnCount = nCount; 
	}
	void SetRowCount(int nCount)
	{ 
		m_nRowCount = nCount; 
	}
	void SetHeaderList(QStringList headerList)
	{
		m_szHeaderList = headerList;
	}
	void SetDataType(const int nType)
	{
		m_nDataType = nType;
		GetTotalInfo();
	}
	void SetChannelOccNo(const int32u& OccNo)
	{
		m_nCurrentChannelOccNo = OccNo;
	}
	void SetDeviceOccNo(const int32u& OccNo)
	{
		m_nCurrentDeviceOccNo = OccNo;
	}
private:
	QVariant ShowProjInfo(int nRow, int Col) const;
	QVariant ShowChannelGernelData(int nRow,int Col) const;
	QVariant ShowChannelContainersData(int nRow, int Col) const;
	QVariant ShowDeviceGernelData(int nRow, int Col) const;
	QVariant ShowDeviceContainersData(int nRow, int Col) const;
	QVariant ShowAINData(int nRow, int nColumn) const;
	QVariant ShowDINData(int nRow, int nColumn) const;
	QVariant ShowAoutData(int nRow, int nColumn) const;
	QVariant ShowDoutData(int nRow, int nColumn) const;
	//���ڴ��������ֱ�ӳ�ʼ��
	bool GetTotalInfo();

private:
	int m_nRowCount;
	int m_nColumnCount;
	QStringList m_szHeaderList;
private:
	//ֱ�Ӵ���ʹ��
	std::shared_ptr<CChannelMgr> m_pChannelMgr;

	std::shared_ptr<CDbSvc>& m_pMem;

	//��ǰ��������
	int m_nDataType;
	//����ͨ����Ϣ
	std::vector<CHANNEL* > m_VecChannel;	
	//�����豸��Ϣ
	std::vector<DEVICE* > m_VecDevice;
	//���е���Ϣ
	std::map < int, AIN* >  m_MapAin;
	std::map < int, DIN* >  m_MapDin;
	std::vector<AOUT_PTR>   m_VecAout;
	std::vector<DOUT_PTR>   m_VecDout;
	//��ǰͨ����
	int m_nCurrentChannelOccNo;
	//��ǰ�豸��
	int m_nCurrentDeviceOccNo;
	//��ȡ��ǰͨ��
	CHANNEL* m_pCurrentChannel;
	//��ȡ��ǰ�豸
	DEVICE*  m_pCurrentDevice;
 };

#endif // CDATAMODEL_H
