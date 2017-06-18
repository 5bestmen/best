
#include "datamodel.h"
#include "db_svc.h"

#include <QHostAddress>
#include <QColor>



CDataModel::CDataModel(std::shared_ptr<CDbSvc>& pMem, QObject *parent /*= 0*/) :QAbstractTableModel(parent), m_pMem(pMem)
{
	m_nCurrentChannelOccNo = 0;
	m_nCurrentDeviceOccNo = 0;
}

CDataModel::~CDataModel()
{
	m_VecChannel.clear();
	m_VecDevice.clear();
	m_MapAin.clear();
	m_MapDin.clear();
	m_VecAout.clear();
	m_VecDout.clear();
}

int CDataModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return m_nColumnCount;
}

int CDataModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return m_nRowCount;
}

/*! \fn QVariant CDataModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const
********************************************************************************************************* 
** \brief CDataModel::data 
** \details 根据当前数据类型判断使用何种表格
** \param index 
** \param role 
** \return QT_NAMESPACE::QVariant 
** \author xingzhibing
** \date 2016年9月20日 
** \note 
********************************************************************************************************/
QVariant CDataModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
	if (!index.isValid())
	{
		return QVariant();
	}
	if (role == Qt::DisplayRole)
	{
		int nRow = index.row();

		int nColumn = index.column();

		switch (m_nDataType)
		{
		case PROJECT_TYPE:
			return ShowProjInfo(nRow, nColumn);
		case CHANNEL_GERENAL_TYPE:
			return ShowChannelGernelData(nRow, nColumn);
		case CHANNEL_CONTAINERS_TYPE:
			return ShowChannelContainersData(nRow, nColumn);
		case DEVICE_GERENAL_TYPE:
			return ShowDeviceGernelData(nRow, nColumn);
		case  DEVICE_CONTAINERS_TYPE:
			return ShowDeviceContainersData(nRow, nColumn);
		case AIN_TYPE:
			return ShowAINData(nRow, nColumn);
		case DIN_TYPE:
			return ShowDINData(nRow, nColumn);
		case  AOUT_TYPE:
			return ShowAoutData(nRow, nColumn);
		case  DOUT_TYPE:
			return ShowDoutData(nRow, nColumn);				
		default:
			break;
		}
	}
	if (role == Qt::EditRole)
	{
		return QVariant();
	}
	return QVariant();
}

bool CDataModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
	if (!index.isValid())
		return false;

	int nColumn = index.column();

	switch (role)
	{
	case Qt::CheckStateRole:
	{
		if (nColumn == 4)
		{
	//		emit dataChanged(index, index);
			return true;
		}
	}
	default:
		return false;
	}
	return false;
}

QVariant CDataModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	
	switch (m_nDataType)
	{
		case PROJECT_TYPE:
		{
			if (orientation == Qt::Horizontal)
			{
				return  m_szHeaderList.at(section);
			}
			else
				return QString("%1").arg(section);
		}
		case CHANNEL_GERENAL_TYPE:
		case CHANNEL_CONTAINERS_TYPE:
		case DEVICE_GERENAL_TYPE:
		case DEVICE_CONTAINERS_TYPE:
		{
			if (orientation == Qt::Vertical)
			{
				return  m_szHeaderList.at(section);
			}
			else
				return QString("%1").arg(section);
		}			
					
		default:
			//一般数据为横列排放
			{
				{
					if (orientation == Qt::Horizontal)
					{
						return  m_szHeaderList.at(section);
					}
					else
						return QString("%1").arg(section);
				}
			}
	}
	return QVariant();
}

Qt::ItemFlags CDataModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return QAbstractItemModel::flags(index);

	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if (index.column() == 4)
		flags |= Qt::ItemIsUserCheckable;

	return flags;
}

QVariant CDataModel::ShowChannelGernelData(int nRow, int nCol) const
{

	CHANNEL* pChannel = m_pCurrentChannel;

	if (!m_pCurrentChannel)
	{
		return QVariant();
	}

	if (!pChannel)
	{
		return QVariant();
	}

	switch (nRow)
	{
		case Channel_OccNo:
			return QString::number(pChannel->OccNo);
		case Channel_NameOccNo:
			return QString::number(pChannel->NameOccNo);
		case Channel_IsDefined:
			return pChannel->IsDefined;
		case Channel_ScanEnable:
			return pChannel->ScanEnable;
		case Channel_Init:
			return pChannel->Init;
		case Chanel_Quality:
			return pChannel->Quality;
		case Channel_PeerQuality:
			return pChannel->PeerQuality;
		case Channel_Heartbeat:
			return pChannel->Heartbeat;
		case Channel_PrintOn:
			return pChannel->PrintOn;
		case Channel_WorkNet:
			return pChannel->WorkNet;
		case Channel_MasterComPort:
			return QString::number(pChannel->CommPara.S.MasterComPort);
		case Channel_SlaveComPort:
			return QString::number(pChannel->CommPara.S.SlaveComPort);
		case Channel_BaudRate:
			return  QString::number(pChannel->CommPara.S.BaudRate);
		case Channel_Parity:
			return  QString::number(pChannel->CommPara.S.Parity);
		case Channel_DataBit:
			return  QString::number(pChannel->CommPara.S.DataBit);
		case Channel_StopBit:
			return  QString::number(pChannel->CommPara.S.StopBit);
		case Channel_Pad:
			return  QString::number(pChannel->CommPara.S.Pad);
		case Channel_MasterIP:
			return  QHostAddress(pChannel->CommPara.E.MasterIP).toString();
		case Channel_SlaveIP:
			return  QHostAddress(pChannel->CommPara.E.SlaveIP).toString();
		case Channel_MasterPort:
			return QString::number(pChannel->CommPara.E.MasterPort);
		case Channel_SlavePort:
			return QString::number(pChannel->CommPara.E.SlavePort);
		case Channel_State:
			return QString::number(pChannel->State);
		case Channel_Prority:
			return QString::number(pChannel->Prority);
		case Channel_SetTimeAttr:
			return pChannel->SetTimeAttr;
			//统计数据
		case Channel_DeviceCount:
			return QString::number(pChannel->DeviceCount);
		case Channel_NodeOccNo:
			return QString::number(pChannel->NodeOccNo);
		case Channel_ChannelName:
			return pChannel->ChannelName;
		case Channel_DriverName:
			return pChannel->DriverName;
		case Channel_DriverType:
			return pChannel->DriverType;
		case Channel_StateCore:
			return pChannel->StateCore;
		case Channel_StateDriver:
			return pChannel->StateDriver;
		default:
			break;
	}

	return QVariant();
}

QVariant CDataModel::ShowChannelContainersData(int nRow, int Col) const
{
	return QVariant();
}

QVariant CDataModel::ShowDeviceGernelData(int nRow, int nCol) const
{
	DEVICE* pDev = m_pCurrentDevice;

	if (!m_pCurrentDevice)
	{
		return QVariant();
	}

	if (!pDev)
	{
		return QVariant();
	}

	switch (nRow)
	{
	case DEVICE_OccNo:
		return QString::number(pDev->OccNo);
	case DEVICE_NameOccNo:
		return QString::number(pDev->NameOccNo);
	case DEVICE_NodeOccNo:
		return QString::number(pDev->NodeOccNo);
	case DEVICE_ChannelOccNo:
		return QString::number(pDev->ChannelOccNo);
	case DEVICE_ModelOccNo:
		return QString::number(pDev->ModelOccNo);
	case DEVICE_Type:
		return (pDev->Type);
	case DEVICE_Pad1:
		return QString::number(pDev->Pad1);
	case DEVICE_Pad2:
		return QString::number(pDev->Pad2);
	case DEVICE_ADDRESS:
		return (pDev->Address);
	case DEVICE_Extension:
		return (pDev->Extention);
	case DEVICE_IsDefined:
		return QString::number(pDev->IsDefined);
	case DEVICE_ScanEnable:
		return QString::number(pDev->ScanEnable);
	case DEVICE_Init:
		return QString::number(pDev->Init);
	case DEVICE_Quality:
		return QString::number(pDev->Quality);
	default:
		break;
	}
	return QVariant();
}

QVariant CDataModel::ShowDeviceContainersData(int nRow, int Col) const
{
	return QVariant();
}

QVariant CDataModel::ShowAINData(int nRow, int nColumn) const
{

	if (m_MapAin.empty())
	{
		return QVariant();
	}

	if (nRow >= m_MapAin.size())
	{
		return QVariant();
	}

	AIN* pAin;

	auto iter = m_MapAin.find(nRow);
	
	if (iter == m_MapAin.end())
	{
		 return QVariant();
	}
	else
	{
		pAin = iter->second;
	}

	switch (nColumn)
	{
	case AIN_OccNo:
		return QString::number(pAin->OccNo);
	case AIN_BlockNo:
		return QString::number(pAin->BlockNo);
	case AIN_NameOccNo:
		return QString::number(pAin->NameOccNo);
	case AIN_IsSpare:
		return QString::number(pAin->IsSpare);
//	case AIN_Pad1:
//		return QString::number(pAin->BlockNo);
	case AIN_Pad2:
		return QString::number(pAin->Pad2);
	case AIN_Address:
		return pAin->Address;
	case AIN_State:
		return QString::number(pAin->State);
	case AIN_IsDefined:
		return QString::number(pAin->IsDefined);
	case AIN_ScanEnable:
		return QString::number(pAin->ScanEnable);
	case AIN_Init:
		return QString::number(pAin->Init);
	case AIN_Quality:
		return QString::number(pAin->Quality);
	case AIN_ManSet:
		return QString::number(pAin->ManSet);
//	case AIN_AlarmState:
//		return QString::number(pAin->BlockNo);
	case AIN_SignalValue:
		return QString::number(pAin->SignalValue);
	case AIN_RawValue:
		return QString::number(pAin->RawValue);
	case AIN_Value:
		return QString::number(pAin->Value);
	case AIN_OldValue:
		return QString::number(pAin->OldValue);
	case AIN_InitValue:
		return QString::number(pAin->InitValue);
	case AIN_DataSource:
		return QString::number(pAin->DataSource);
	case AIN_RangeL:
		return QString::number(pAin->RangeL);
	case AIN_RangeH:
		return QString::number(pAin->RangeH);
	case AIN_HighQua:
		return QString::number(pAin->HighQua);
	case AIN_LowQua:
		return QString::number(pAin->LowQua);
	case AIN_NodeOccNo:
		return QString::number(pAin->NodeOccNo);
	case AIN_ChannelOccNo:
		return QString::number(pAin->ChannelOccNo);
	case AIN_DeviceOccNo:
		return QString::number(pAin->DeviceOccNo);
	case AIN_AlarmOccNo:
		return QString::number(pAin->AlarmOccNo);
	case AIN_ExpressOccNo:
		return QString::number(pAin->ExpressOccNo);
	case AIN_TransformType:
		return QString::number(pAin->TransformType);
	case AIN_TransformOccNo:
		return QString::number(pAin->TransformOccNo);
	case AIN_PinLabelOccNo:
		return QString::number(pAin->PinLabelOccNo);
	case AIN_UnitNameOccNo:
		return QString::number(pAin->UnitNameOccNo);
	case AIN_LastUpdateTime:
		return QString("%1/%2/%3-%4:%5:%6:%7").
			arg(pAin->LastUpdateTime.Year).
			arg(pAin->LastUpdateTime.Month).
			arg(pAin->LastUpdateTime.Day).
			arg(pAin->LastUpdateTime.Hour).
			arg(pAin->LastUpdateTime.Minute).
			arg(pAin->LastUpdateTime.Second).
			arg(pAin->LastUpdateTime.Milliseconds);
	default:
		break;
	}
	return QVariant();
}

QVariant CDataModel::ShowDINData(int nRow, int nColumn) const
{
	if (m_MapDin.empty())
	{
		return QVariant();
	}

	if (nRow >= m_MapDin.size())
	{
		return QVariant();
	}

	DIN* pDin;

	auto iter = m_MapDin.find(nRow);

	if (iter == m_MapDin.end())
	{
		return QVariant();
	}
	else
	{
		pDin = iter->second;
	}

	switch (nColumn)
	{
	case DIN_OccNo:
		return QString::number(pDin->OccNo);
	case DIN_BlockNo:
		return QString::number(pDin->BlockNo);
	case DIN_NameOccNo:
		return QString::number(pDin->NameOccNo);
	case DIN_NeedTranmitFlag:
		return QString::number(pDin->NeedTranmitFlag);
	case DIN_Pad2:
		return QString::number(pDin->Pad2);
	case DIN_IsSpare:
		return QString::number(pDin->IsSpare);
	case DIN_Address:
		return pDin->Address;
	case DIN_InitValEnable:
		return QString::number(pDin->InitValEnable);
	case DIN_InitValue:
		return QString::number(pDin->InitValue);
	case DIN_InitQua:
		return QString::number(pDin->InitQua);
	case DIN_SlaveOccNo:
		return QString::number(pDin->SlaveOccNo);
	case DIN_DDIFilter:
		return QString::number(pDin->DDIFilter);
	case DIN_DDIFlag:
		return QString::number(pDin->DDIFlag);
	case DIN_DDIInUse:
		return QString::number(pDin->DDIInUse);
	case DIN_State:
		return QString::number(pDin->State);
	case DIN_IsDefined:
		return QString::number(pDin->IsDefined);
	case DIN_ScanEnable:
		return QString::number(pDin->ScanEnable);
	case DIN_Init:
		return QString::number(pDin->Init);
	case DIN_Quality:
		return QString::number(pDin->Quality);
	case DIN_ManSet:
		return QString::number(pDin->ManSet);
	case DIN_Value:
		return QString::number(pDin->Value);
	case DIN_NegValue:
		return QString::number(pDin->NegValue);
	case DIN_RawValue:
		return QString::number(pDin->RawValue);
	case DIN_CtrlByte:
		return QString::number(pDin->CtrlByte);
	case DIN_IsSOE:
		return QString::number(pDin->IsSOE);
	case DIN_StartCtrl:
		return QString::number(pDin->StartCtrl);
	case DIN_SignalType:
		return QString::number(pDin->SignalType);
	case DIN_NodeOccNo:
		return QString::number(pDin->NodeOccNo);
	case DIN_ChannelOccNo:
		return QString::number(pDin->ChannelOccNo);
	case DIN_DeviceOccNo:
		return QString::number(pDin->DeviceOccNo);
	case DIN_AlarmOccNo:
		return QString::number(pDin->AlarmOccNo);
	case DIN_DataSource:
		return QString::number(pDin->DataSource);
	case DIN_ExpressOccNo:
		return QString::number(pDin->ExpressOccNo);
	case DIN_Desc0_OccNo:
		return QString::number(pDin->Desc0_OccNo);
	case DIN_Desc1_OccNo:
		return QString::number(pDin->Desc1_OccNo);
	case DIN_PinLabelOccNo:
		return QString::number(pDin->PinLabelOccNo);
	case DIN_LastUpdateTime:
		return QString("%1/%2/%3-%4:%5:%6:%7").
			arg(pDin->LastUpdateTime.Year).
			arg(pDin->LastUpdateTime.Month).
			arg(pDin->LastUpdateTime.Day).
			arg(pDin->LastUpdateTime.Hour).
			arg(pDin->LastUpdateTime.Minute).
			arg(pDin->LastUpdateTime.Second).
			arg(pDin->LastUpdateTime.Milliseconds);
	default:
		break;
	}
	return QVariant();
}

QVariant CDataModel::ShowAoutData(int nRow, int nColumn) const
{
	if (m_VecAout.empty())
	{
		return QVariant();
	}
	if (nRow >= m_VecAout.size())
	{
		return QVariant();
	}
	AOUT_PTR pAout = m_VecAout[nRow];

	switch (nColumn)
	{
	case AOUT_OccNo:
		return QString::number(pAout->OccNo);
	case AOUT_BlockNo:
		return QString::number(pAout->BlockNo);
	case AOUT_NameOccNo:
		return QString::number(pAout->NameOccNo);
	case AOUT_State:
		return QString::number(pAout->State);
	case AOUT_IsDefined:
		return QString::number(pAout->IsDefined);
	case AOUT_ScanEnable:
		return QString::number(pAout->ScanEnable);
	case AOUT_Init:
		return QString::number(pAout->Init);
	case AOUT_Quality:
		return QString::number(pAout->Quality);
	case AOUT_Output:
		return QString::number(pAout->Output);
	case AOUT_RawData:
		return QString::number(pAout->RawData);
	case AOUT_DataSource:
		return QString::number(pAout->DataSource);
	case AOUT_NodeOccNo:
		return QString::number(pAout->NodeOccNo);
	case AOUT_ChannelOccNo:
		return QString::number(pAout->ChannelOccNo);
	case AOUT_DeviceOccNo:
		return QString::number(pAout->DeviceOccNo);
	case AOUT_AlarmOccNo:
		return QString::number(pAout->AlarmOccNo);
	case AOUT_ExpressOccNo:
		return QString::number(pAout->ExpressOccNo);
	case AOUT_TransformType:
		return QString::number(pAout->TransformType);
	case AOUT_TransformOccNo:
		return QString::number(pAout->TransformOccNo);
	case AOUT_PinLabelOccNo:
		return QString::number(pAout->PinLabelOccNo);
	case AOUT_UnitNameOccNo:
		return QString::number(pAout->UnitNameOccNo);
	case AOUT_LastUpdateTime:
		return QString("%1/%2/%3-%4:%5:%6:%7").
			arg(pAout->LastUpdateTime.Year).
			arg(pAout->LastUpdateTime.Month).
			arg(pAout->LastUpdateTime.Day).
			arg(pAout->LastUpdateTime.Hour).
			arg(pAout->LastUpdateTime.Minute).
			arg(pAout->LastUpdateTime.Second).
			arg(pAout->LastUpdateTime.Milliseconds);
	default:
		break;
	}
	return QVariant();
}

QVariant CDataModel::ShowDoutData(int nRow, int nColumn) const
{
	if (m_VecDout.empty())
	{
		return QVariant();
	}
	if (nRow >= m_VecDout.size())
	{
		return QVariant();
	}
	DOUT_PTR pDout = m_VecDout[nRow];

	switch (nColumn)
	{
	case DOUT_OccNo:
		return QString::number(pDout->OccNo);
	case DOUT_BlockNo:
		return QString::number(pDout->BlockNo);
	case DOUT_NameOccNo:
		return QString::number(pDout->NameOccNo);
	case DOUT_State:
		return QString::number(pDout->State);
	case DOUT_IsDefined:
		return QString::number(pDout->IsDefined);
	case DOUT_ScanEna:
		return QString::number(pDout->ScanEnable);
	case DOUT_Init:
		return QString::number(pDout->Init);
	case DOUT_Quality:
		return QString::number(pDout->Quality);
	case DOUT_Output:
		return QString::number(pDout->Output);
	case DOUT_RelayState:
		return QString::number(pDout->RelayState);
	case DOUT_DataSource:
		return QString::number(pDout->DataSource);
	case DOUT_CtrlType:
		return QString::number(pDout->CtrlType);
	case DOUT_Filter:
		return QString::number(pDout->Filter);
	case DOUT_NodeOccNo:
		return QString::number(pDout->NodeOccNo);
	case DOUT_ChannelOccNo:
		return QString::number(pDout->ChannelOccNo);
	case DOUT_DeviceOccNo:
		return QString::number(pDout->DeviceOccNo);
	case DOUT_AlarmOccNo:
		return QString::number(pDout->AlarmOccNo);
	case DOUT_ExpressionOccNo:
		return QString::number(pDout->ExpressionOccNo);
	case DOUT_PinLabelOccNo:
		return QString::number(pDout->PinLabelOccNo);
	case DOUT_LastUpdateTime:
		return QString("%1/%2/%3-%4:%5:%6:%7").
			arg(pDout->LastUpdateTime.Year).
			arg(pDout->LastUpdateTime.Month).
			arg(pDout->LastUpdateTime.Day).
			arg(pDout->LastUpdateTime.Hour).
			arg(pDout->LastUpdateTime.Minute).
			arg(pDout->LastUpdateTime.Second).
			arg(pDout->LastUpdateTime.Milliseconds);
	default:
		break;
	}


	return QVariant();
}

/*! \fn bool CDataModel::GetAllData()
********************************************************************************************************* 
** \brief CDataModel::GetAllData 
** \details 加载不变量数据
** \return bool 
** \author xingzhibing
** \date 2016年9月19日 
** \note 
********************************************************************************************************/
bool CDataModel::GetTotalInfo()
{
	switch (m_nDataType)
	{
	case PROJECT_TYPE:
	{
		for (int i = 0; i < m_pMem->GetChannelCount(); ++i)
		{
			CHANNEL *pChannel;

			m_pMem->GetChannelByOccNo(i+1,&pChannel);

			if (pChannel)
			{
				m_VecChannel.push_back(pChannel);
			}
		}
		break;
	}
	case CHANNEL_GERENAL_TYPE:
	{
		CHANNEL *pChannel = Q_NULLPTR ;

		m_pMem->GetChannelByOccNo(m_nCurrentChannelOccNo, &pChannel);
		
		if (pChannel)
		{
			m_pCurrentChannel = (pChannel);
		}

		break;
	}
	case CHANNEL_CONTAINERS_TYPE:
	{
		DEVICE* pDev = Q_NULLPTR;

		m_pMem->GetDeviceByOccNo(m_nCurrentDeviceOccNo, &pDev);
		
		if (pDev)
		{
			m_pCurrentDevice =(pDev);
		}
		break;
	}
	case DEVICE_GERENAL_TYPE:
	{
		DEVICE* pDev = Q_NULLPTR;

		m_pMem->GetDeviceByOccNo(m_nCurrentDeviceOccNo, &pDev);

		if (pDev)
		{
			m_pCurrentDevice =(pDev);
		}
		break;
	}
	case DEVICE_CONTAINERS_TYPE:
	{
		break;
	}
	case AIN_TYPE:
	{

		CDevicePtr pDev = m_pMem->GetChannelMgr()->GetDevice(m_nCurrentDeviceOccNo - 1);

		size_t nAinCount = pDev->GetAinCount();

		for (int i = 0; i < nAinCount;++i)
		{
			AIN* pAin;

			m_pMem->GetAinByOccNo(pDev->GetAIN(i)->OccNo,&pAin);
		
			m_MapAin.insert(std::make_pair(i, pAin));
		
		}
		break;
	}
	case DIN_TYPE:
	{
		CDevicePtr pDev = m_pMem->GetChannelMgr()->GetDevice(m_nCurrentDeviceOccNo - 1);

		size_t nDinCount = pDev->GetDinCount();

		for (int i = 0; i < nDinCount; ++i)
		{
			DIN* pDin;

			m_pMem->GetDinByOccNo(pDev->GetAIN(i)->OccNo, &pDin);
			
			m_MapDin.insert(std::make_pair(i, pDin));	
		}
		break;
	}
	case AOUT_TYPE:
	{
		CDevicePtr pDev = m_pMem->GetChannelMgr()->GetDevice(m_nCurrentDeviceOccNo -1);

		size_t nAoutCount = pDev->GetAoutCount();
		for (int i = 0; i < nAoutCount; ++i)
		{
			AOUT_PTR pAout = pDev->GetAout(i);
			m_VecAout.push_back(pAout);
		}
		break;
	}
	case DOUT_TYPE:
	{
		CDevicePtr pDev = m_pMem->GetChannelMgr()->GetDevice(m_nCurrentDeviceOccNo-1);

		size_t nDoutCount = pDev->GetDoutCount();

		for (int i = 0; i < nDoutCount; ++i)
		{
			DOUT_PTR pDout = pDev->GetDout(i);
			m_VecDout.push_back(pDout);
		}
		break;
	}
	default:
		break;
	}

	return true;
}

QVariant CDataModel::ShowProjInfo(int nRow, int nCol) const
{
	if (m_VecChannel.empty())
	{
		return QVariant();
	}

	CHANNEL* pChannel = m_VecChannel[nRow];

	if (!pChannel)
	{
		return QVariant();
	}

	switch (nCol)
	{
	case Channel_OccNo:
		return QString::number(pChannel->OccNo);
	case Channel_NameOccNo:
		return QString::number(pChannel->NameOccNo);
	case Channel_IsDefined:
		return pChannel->IsDefined;
	case Channel_ScanEnable:
		return pChannel->ScanEnable;
	case Channel_Init:
		return pChannel->Init;
	case Chanel_Quality:
		return pChannel->Quality;
	case Channel_PeerQuality:
		return pChannel->PeerQuality;
	case Channel_Heartbeat:
		return pChannel->Heartbeat;
	case Channel_PrintOn:
		return pChannel->PrintOn;
	case Channel_WorkNet:
		return pChannel->WorkNet;
	case Channel_MasterComPort:
		return QString::number(pChannel->CommPara.S.MasterComPort);
	case Channel_SlaveComPort:
		return QString::number(pChannel->CommPara.S.SlaveComPort);
	case Channel_BaudRate:
		return  QString::number(pChannel->CommPara.S.BaudRate);
	case Channel_Parity:
		return  QString::number(pChannel->CommPara.S.Parity);
	case Channel_DataBit:
		return  QString::number(pChannel->CommPara.S.DataBit);
	case Channel_StopBit:
		return  QString::number(pChannel->CommPara.S.StopBit);
	case Channel_Pad:
		return  QString::number(pChannel->CommPara.S.Pad);
	case Channel_MasterIP:
		return  QHostAddress(pChannel->CommPara.E.MasterIP).toString();
	case Channel_SlaveIP:
		return  QHostAddress(pChannel->CommPara.E.SlaveIP).toString();
	case Channel_MasterPort:
		return QString::number(pChannel->CommPara.E.MasterPort);
	case Channel_SlavePort:
		return QString::number(pChannel->CommPara.E.SlavePort);
	case Channel_State:
		return QString::number(pChannel->State);
	case Channel_Prority:
		return QString::number(pChannel->Prority);
	case Channel_SetTimeAttr:
		return pChannel->SetTimeAttr;
		//统计数据
	case Channel_DeviceCount:
		return QString::number(pChannel->DeviceCount);
	case Channel_NodeOccNo:
		return QString::number(pChannel->NodeOccNo);
	case Channel_ChannelName:
		return pChannel->ChannelName;
	case Channel_DriverName:
		return pChannel->DriverName;
	case Channel_DriverType:
		return pChannel->DriverType;
	case Channel_StateCore:
		return pChannel->StateCore;
	case Channel_StateDriver:
		return pChannel->StateDriver;
	default:
		break;
	}

	return QVariant();
}


