#include "devicemodel.h"
#include "scadastudio/quoteItem.h"
#include "scadastudio/icore.h"
#include "config_data.h"
#include "delegate.h"
#include "devicetable.h"
#include "global.h"

#include <QObject>

CDeviceModel::CDeviceModel(QObject *parent, IMainModuleInterface *pCore, Config::CDevice *pDeviceData, CChannel *pModule, CQuoteItem *pItem
	, Config::CFesData *pFesData)
	: QAbstractTableModel(parent), m_pDeviceData(nullptr), m_pItem(nullptr), m_pCore(nullptr), m_pFesData(nullptr)
{
	Q_UNUSED(pModule);
	
	Q_ASSERT(pDeviceData);
	if (!pDeviceData)
	{
		return;
	}
	
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	Q_ASSERT(pFesData);
	if (!pFesData)
	{
		return;
	}

	m_pItem = pItem;

	m_pDeviceData = pDeviceData;

	m_pCore = pCore;

	m_pFesData = pFesData;

	InitPara();
}

CDeviceModel::~CDeviceModel()
{
	int a = 5;
	a = 6;
}

QVariant CDeviceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (m_lstHeader.size() > section)
				return m_lstHeader[section];
			else
				return QVariant();
		}
		else
		{
			//if(vertical_header_list.size() > section)
			//	return vertical_header_list[section]; // 垂直表头  
			//else
			//return QVariant();

			return QString(" ");
		}
	}

	return QVariant();
}

int CDeviceModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	return 1;
}

int CDeviceModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_lstHeader.size() > 0)
	{
		return m_lstHeader.size();
	}

	return 0;
}

QVariant CDeviceModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role == Qt::TextAlignmentRole)
	{
		return int(Qt::AlignLeft | Qt::AlignVCenter);
	}
	else if (role == Qt::DisplayRole)
	{
		Q_ASSERT(m_pDeviceData);

		if (index.column() == OccNo)
		{
			return uint(m_pDeviceData->GetOccNo());
		}
		else if (index.column() == TagName)
		{
			return m_pDeviceData->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return (uint)m_pDeviceData->m_nID;
		}
		else if (index.column() == Description)
		{
			return m_pDeviceData->m_strDescription;
		}
		else if (index.column() == Priority)
		{
			return m_pDeviceData->m_nPriority;
		}
		else if (index.column() == DeviceAddr)
		{
			return m_pDeviceData->m_strDeviceAddr;
		}
		else if (index.column() == ExtentionParam)
		{
			return m_pDeviceData->m_strExtentionParam;
		}
		else if (index.column() == DeviceModelID)
		{
			return (uint)m_pDeviceData->m_nDeviceModelID;
		}
		else if (index.column() == DeviceType)
		{
			return m_pDeviceData->m_nType;
		}
		else if (index.column() == OccNo)
		{
			return 0;
		}

		return 1;
	}
	else if (role == Qt::EditRole)
	{
		Q_ASSERT(m_pDeviceData);

		if (index.column() == OccNo)
		{
			return uint(m_pDeviceData->GetOccNo());
		}
		else if (index.column() == TagName)
		{
			return m_pDeviceData->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return (uint)m_pDeviceData->m_nID;
		}
		else if (index.column() == Description)
		{
			return m_pDeviceData->m_strDescription;
		}
		else if (index.column() == Priority)
		{
			return m_pDeviceData->m_nPriority;
		}
		else if (index.column() == DeviceAddr)
		{
			return m_pDeviceData->m_strDeviceAddr;
		}
		else if (index.column() == ExtentionParam)
		{
			return m_pDeviceData->m_strExtentionParam;
		}
		else if (index.column() == DeviceModelID)
		{
			return (uint)m_pDeviceData->m_nDeviceModelID;
		}
		else if (index.column() == DeviceType)
		{
			return m_pDeviceData->m_nType;
		}
		else if (index.column() == OccNo)
		{
			return 0;
		}
	}

	return QVariant();
}

bool CDeviceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid() && role == Qt::EditRole)
	{
		Q_ASSERT(m_pDeviceData);
		if (!m_pDeviceData)
		{
			return false;
		}
		
		if (index.column() == TagName)
		{
			if (m_pDeviceData->m_szTagName != value.toString())
			{
				auto bFlag = m_pFesData->CheckTagNameIsExist(value.toString());
				if (bFlag)
				{
					auto strError = QString("%1 %2 is exist").arg(tr(" fes tag name:")).arg(value.toString());
					m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

					return true;
				}

				//TagName
				m_pItem->setData(value, Qt::EditRole);

				memset(m_pDeviceData->m_szTagName, 0, sizeof(m_pDeviceData->m_szTagName));
				int nSize = value.toString().size();
				strncpy(m_pDeviceData->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));


				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}


		}
		else if (index.column() == ID)
		{
			if (m_pDeviceData->m_nID = value.toInt())
			{
				m_pDeviceData->m_nID = value.toInt();

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == Description)
		{
			if (m_pDeviceData->m_strDescription.compare(value.toString()) != 0)
			{
				auto strDesc = value.toString();

				if (!CheckDescIsValid(strDesc, FES_DESC))
				{
					return false;
				}
				else
				{
					m_pDeviceData->m_strDescription = strDesc;
				}

				//修改树形分支通道名
				m_pItem->setData(value, TREE_TOOL_TIP_ROLE);

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == DeviceAddr)
		{
			if (m_pDeviceData->m_strDeviceAddr.compare(value.toString()) != 0)
			{
				auto strDesc = value.toString();

				if (!CheckDescIsValid(strDesc, FES_DESC))
				{
					return false;
				}
				else
				{
					m_pDeviceData->m_strDeviceAddr = strDesc;
				}

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == ExtentionParam)
		{
			if (m_pDeviceData->m_strExtentionParam != value.toString())
			{
				auto strDesc = value.toString();

				if (!CheckDescIsValid(strDesc, FES_DESC))
				{
					return false;
				}
				else
				{
					m_pDeviceData->m_strExtentionParam = strDesc;
				}

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == DeviceModelID)
		{
			if (m_pDeviceData->m_nDeviceModelID != value.toInt())
			{
				m_pDeviceData->m_nDeviceModelID = value.toInt();

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == DeviceType)
		{
			if (m_pDeviceData->m_nType != value.toInt())
			{
				m_pDeviceData->m_nType = value.toInt();

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}

		return true;
	}
	/*else if (index.isValid() && role == Qt::UserRole)
	{

	}
	else if (index.isValid() && role == TREE_TOOL_TIP_ROLE)
	{

	}*/

	return false;
}

Qt::ItemFlags CDeviceModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}


/*! \fn 
********************************************************************************************************* 
** \brief CDeviceModel::RefrushModel 
** \details 
** \return void 
** \author gw
** \date 2016年11月23日 
** \note 
********************************************************************************************************/
void CDeviceModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

/*! \fn void CDeviceModel::InitPara()
********************************************************************************************************* 
** \brief CDeviceModel::InitPara 
** \details 
** \return void 
** \author gw
** \date 2016年11月23日 
** \note 
********************************************************************************************************/
void CDeviceModel::InitPara()
{
	m_lstHeader.clear();
	m_lstHeader << tr("ID") << tr("TagName") << tr("Description") << tr("OccNo") << tr("Priority") << tr("DeviceAddr")
		<< tr("ExtentionParam") << tr("DeviceModelID") << tr("DeviceType");
	
	CONFIG_PARA para;

	para.desc = tr("TagName");
	para.tagname = "TagName";
	CLineEditDelegate *delegate = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CDeviceTable *)parent())->setItemDelegateForColumn(TagName, delegate);
	
	para.desc = tr("Description");
	para.tagname = "Description";
	CLineEditDelegate *pDelegateDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CDeviceTable *)parent())->setItemDelegateForColumn(Description, pDelegateDesc);


	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	CReadOnlyDelegate *pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CDeviceTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnlyDelegate);



	para.desc = tr("InitQua");
	para.tagname = "InitQua";

	QVector<IDDESC> arrDeviceType;
	arrDeviceType.clear();
	IDDESC tmp;
	tmp.desc = tr("PROTECTIONANDCONTROL");
	tmp.ID = QString("%1").arg(PROTECTIONANDCONTROL);
	arrDeviceType.append(tmp);
	tmp.desc = tr("PROTECTION");
	tmp.ID = QString("%1").arg(PROTECTION);
	arrDeviceType.append(tmp);
	tmp.desc = tr("MEASUREMENTANDCONTROL");
	tmp.ID = QString("%1").arg(MEASUREMENTANDCONTROL);
	arrDeviceType.append(tmp);
	tmp.desc = tr("COMMUNICATIONMANAGEMENTMACHINE");
	tmp.ID = QString("%1").arg(COMMUNICATIONMANAGEMENTMACHINE);
	arrDeviceType.append(tmp);
	tmp.desc = tr("OTHERS");
	tmp.ID = QString("%1").arg(OTHERS);
	arrDeviceType.append(tmp);
	CComboBoxDelegate *pComboInitQua = new CComboBoxDelegate(this, arrDeviceType);
	((CDeviceTable *)parent())->setItemDelegateForColumn(DeviceType, pComboInitQua);

	CLineEditDelegate *pDelegateDevice = new CLineEditDelegate(this, DEVICEADDRESS);
	((CDeviceTable *)parent())->setItemDelegateForColumn(DeviceAddr, pDelegateDevice);

	((CDeviceTable *)parent())->setItemDelegateForColumn(ExtentionParam, pDelegateDesc);
}