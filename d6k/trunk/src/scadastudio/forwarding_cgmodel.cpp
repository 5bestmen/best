#include "forwarding_cgmodel.h"
#include "scadastudiodefine.h"
#include "delegate.h"
#include "forwarding_cgtable.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "variant.h"
#include "global.h"

CForwardingCGModel::CForwardingCGModel(QObject *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes)
	: QAbstractTableModel(parent), m_pFes(nullptr), m_pChannelData(nullptr)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return;
	}

	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	Q_ASSERT(pChannelData);
	if (!pChannelData)
	{
		return;
	}

	m_pCore = pCore;

	m_pFes = pFes;

	m_pChannelData = pChannelData;

	InitPara();
}

CForwardingCGModel::~CForwardingCGModel()
{

}

int CForwardingCGModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_pChannelData->m_arrCGs.size();
}

int CForwardingCGModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	if (m_horizontal_header_list.size() > 0)
		return m_horizontal_header_list.size();

	//if(0 == arr_row_list.count())
	//	return 0;
	//else if(arr_row_list.count() < 1)
	//	return 0;
	//else
	//	return arr_row_list.at(0).size();

	return 0;
}

QVariant CForwardingCGModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);

		if (index.column() == TagName)
		{
			return m_pChannelData->m_arrCGs[nRow]->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return (uint)m_pChannelData->m_arrCGs[nRow]->m_nID;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pChannelData->m_arrCGs[nRow]->GetOccNo();
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pChannelData->m_arrCGs[nRow]->m_nBlockNo;
		}
		else if (index.column() == GroupNo)
		{
			return (uint)m_pChannelData->m_arrCGs[nRow]->m_nGroupNo;
		}
		else if (index.column() == EntryNo)
		{
			return (uint)m_pChannelData->m_arrCGs[nRow]->m_nEntryNo;
		}
		else if (index.column() == Address)
		{
			return m_pChannelData->m_arrCGs[nRow]->m_strAddress;
		}
		else if (index.column() == Desc)
		{
			return m_pChannelData->m_arrCGs[nRow]->m_strDescription;
		}
		else if (index.column() == FUN)
		{
			return m_pChannelData->m_arrCGs[nRow]->m_strFUN;
		}
		else if (index.column() == Info)
		{
			return m_pChannelData->m_arrCGs[nRow]->m_strInfo;
		}

		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == TagName)
		{
			return m_pChannelData->m_arrCGs[nRow]->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return (uint)m_pChannelData->m_arrCGs[nRow]->m_nID;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pChannelData->m_arrCGs[nRow]->GetOccNo();
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pChannelData->m_arrCGs[nRow]->m_nBlockNo;
		}
		else if (index.column() == GroupNo)
		{
			return (uint)m_pChannelData->m_arrCGs[nRow]->m_nGroupNo;
		}
		else if (index.column() == EntryNo)
		{
			return (uint)m_pChannelData->m_arrCGs[nRow]->m_nEntryNo;
		}
		else if (index.column() == Address)
		{
			return m_pChannelData->m_arrCGs[nRow]->m_strAddress;
		}
		else if (index.column() == Desc)
		{
			return m_pChannelData->m_arrCGs[nRow]->m_strDescription;
		}
		else if (index.column() == FUN)
		{
			return m_pChannelData->m_arrCGs[nRow]->m_strFUN;
		}
		else if (index.column() == Info)
		{
			return m_pChannelData->m_arrCGs[nRow]->m_strInfo;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CForwardingCGModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (m_horizontal_header_list.size() > section && section > -1)
				return m_horizontal_header_list[section];
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

void CForwardingCGModel::InitPara()
{
	m_horizontal_header_list.clear();

	m_arrInfo.clear();

	CONFIG_PARA para;

	para.desc = tr("ID");
	para.tagname = "ID";
	m_arrInfo.insert(ID, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	m_arrInfo.insert(TagName, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegate = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CForwardingCGTable *)parent())->setItemDelegateForColumn(TagName, delegate);

	para.desc = tr("Desc");
	para.tagname = "Desc";
	m_arrInfo.insert(Desc, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegateDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CForwardingCGTable *)parent())->setItemDelegateForColumn(Desc, delegate);

	para.desc = tr("SourceTagName");
	para.tagname = "SourceTagName";
	m_arrInfo.insert(SourceTagName, para);
	m_horizontal_header_list.append(para.desc);
	CReadOnlyDelegate *pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CForwardingCGTable *)parent())->setItemDelegateForColumn(SourceTagName, pReadOnlyDelegate);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_arrInfo.insert(OccNo, para);
	m_horizontal_header_list.append(para.desc);
	((CForwardingCGTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnlyDelegate);

	para.desc = tr("BlockNo");
	para.tagname = "BlockNo";
	m_arrInfo.insert(BlockNo, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("GroupNo");
	para.tagname = "GroupNo";
	m_arrInfo.insert(GroupNo, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("EntryNo");
	para.tagname = "EntryNo";
	m_arrInfo.insert(EntryNo, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("Address");
	para.tagname = "Address";
	m_arrInfo.insert(Address, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegateAddress = new CLineEditDelegate(this, TAGNAMELINE, MAX_ADDRESS_LENGTH_SCADASTUDIO);
	((CForwardingCGTable *)parent())->setItemDelegateForColumn(Address, delegateAddress);

	para.desc = tr("FUN");
	para.tagname = "FUN";
	m_arrInfo.insert(FUN, para);
	m_horizontal_header_list.append(para.desc);
	((CForwardingCGTable *)parent())->setItemDelegateForColumn(FUN, delegateDesc);

	para.desc = tr("Info");
	para.tagname = "Info";
	m_arrInfo.insert(Info, para);
	m_horizontal_header_list.append(para.desc);
	((CForwardingCGTable *)parent())->setItemDelegateForColumn(Info, delegateDesc);
}

bool CForwardingCGModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			//Q_ASSERT(m_pChannel);

			if (index.column() == TagName)
			{
				if (m_pChannelData->m_arrCGs[nRow]->m_szTagName != value.toString())
				{
					if (!CheckTagNameIsValid(value.toString(), FES_DESC))
					{
						return false;
					}

					Q_ASSERT(m_pFes);
					if (!m_pFes)
					{
						return false;
					}

					if (m_pFes->CheckTagNameIsExist(value.toString()))
					{
						auto strError = tr("Forwarding cg tagname is exist!!!");
						Q_ASSERT(m_pCore);
						if (!m_pCore)
						{
							return false;
						}

						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}

					if (!m_pFes->ModifyFesHashMap(FORWARDINGCGHASHID, value.toString(), m_pChannelData->m_arrCGs[nRow]->m_szTagName))
					{
						auto strError = tr("Forwarding cg tagname modify fail!!!");
						Q_ASSERT(m_pCore);
						if (!m_pCore)
						{
							return false;
						}

						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}

					//m_arrCGs[nRow]->m_szTagName = value.toString();
					memset(m_pChannelData->m_arrCGs[nRow]->m_szTagName, 0, sizeof(m_pChannelData->m_arrCGs[nRow]->m_szTagName));
					strncpy(m_pChannelData->m_arrCGs[nRow]->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, value.toString().size()));
				}
			}
			else if (index.column() == ID)
			{
				if (m_pChannelData->m_arrCGs[nRow]->m_nID != value.toInt())
				{
					m_pChannelData->m_arrCGs[nRow]->m_nID = value.toInt();
				}
			}
			else if (index.column() == BlockNo)
			{
				if (m_pChannelData->m_arrCGs[nRow]->m_nBlockNo != value.toInt())
				{
					m_pChannelData->m_arrCGs[nRow]->m_nBlockNo = value.toInt();
				}
			}
			else if (index.column() == GroupNo)
			{
				if (m_pChannelData->m_arrCGs[nRow]->m_nGroupNo != value.toInt())
				{
					m_pChannelData->m_arrCGs[nRow]->m_nGroupNo = value.toInt();
				}
			}
			else if (index.column() == EntryNo)
			{
				if (m_pChannelData->m_arrCGs[nRow]->m_nEntryNo != value.toInt())
				{
					m_pChannelData->m_arrCGs[nRow]->m_nEntryNo = value.toInt();
				}
			}
			else if (index.column() == Address)
			{
				if (m_pChannelData->m_arrCGs[nRow]->m_strAddress != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC, MAX_ADDRESS_LENGTH_SCADASTUDIO))
					{
						return false;
					}

					m_pChannelData->m_arrCGs[nRow]->m_strAddress = value.toString();
				}
			}
			else if (index.column() == Desc)
			{
				if (m_pChannelData->m_arrCGs[nRow]->m_strDescription.compare(value.toString()) != 0)
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC))
					{
						return false;
					}
					else
					{
						m_pChannelData->m_arrCGs[nRow]->m_strDescription = value.toString();
					}

					//m_pFes->ModifyAOsRelationScaleDescArr(m_arrCGs[nRow]->m_strScaleTagName.toStdString(), value.toString().toStdString());
				}
			}
			else if (index.column() == FUN)
			{
				if (m_pChannelData->m_arrCGs[nRow]->m_strFUN != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC))
					{
						return false;
					}
					
					m_pChannelData->m_arrCGs[nRow]->m_strFUN = value.toString();
				}
			}
			else if (index.column() == Info)
			{
				if (m_pChannelData->m_arrCGs[nRow]->m_strInfo.compare(value.toString()) != 0)
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC))
					{
						return false;
					}
					
					m_pChannelData->m_arrCGs[nRow]->m_strInfo = value.toString();
				}
			}

			emit dataChanged(index, index);

			return true;
		}

		return false;
	}

	return false;
}

/*! \fn Qt::ItemFlags CAOModel::flags(const QModelIndex &index) const
*********************************************************************************************************
** \brief CAOModel::flags
** \details
** \param index
** \return Qt::ItemFlags
** \author gw
** \date 2016年8月12日
** \note
********************************************************************************************************/
Qt::ItemFlags CForwardingCGModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

std::vector <Config::CForwardingControlGearData *> & CForwardingCGModel::GetCGs()
{
	return m_pChannelData->m_arrCGs;
}

bool CForwardingCGModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	//auto nCount = m_arrCGs.size();

	beginInsertRows(parent, position, position + rows - 1);

	for (int i = position; i < position + rows; i++)
	{
		auto pControlGearPoint = m_pFes->AddNewForwardingCGItem();

		pControlGearPoint->SetOccNo(i + 1);
		pControlGearPoint->m_nID = i + 1;

		//auto strNewTagName = QString("AnalogOutput%1").arg(pAnalogPoint->m_nID);
		//auto strNewDesc = QString("AnalogOutput%1").arg(pAnalogPoint->m_nID);

		////是否校验

		////pAnalogPoint->m_szTagName = strNewTagName;
		//memset(pAnalogPoint->m_szTagName, 0, sizeof(pAnalogPoint->m_szTagName));
		//strncpy(pAnalogPoint->m_szTagName, strNewTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH, strNewTagName.size()));

		//pAnalogPoint->m_strDescription = strNewDesc;
		//pAnalogPoint->m_bNewPointAlarm = true;

		m_pChannelData->m_arrCGs.push_back(pControlGearPoint);
	}

	endInsertRows();

	return true;
}

/*! \fn bool CDIModel::RemoveRows(int position, int rows, const QModelIndex &parent)
*********************************************************************************************************
** \brief CDIModel::RemoveRows
** \details 删除数据
** \param position
** \param rows
** \param parent
** \return bool
** \author gw
** \date 2016年8月11日
** \note
********************************************************************************************************/
bool CForwardingCGModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, rows, rows);

	//auto nCount = m_arrCGs.size();

	auto pAnalog = m_pChannelData->m_arrCGs[position];

	auto it = std::find(m_pChannelData->m_arrCGs.begin(), m_pChannelData->m_arrCGs.end(), pAnalog);
	if (it != m_pChannelData->m_arrCGs.end())
	{
		m_pChannelData->m_arrCGs.erase(it);
	}

	Q_ASSERT(m_pFes);
	if (!m_pFes)
	{
		return false;
	}

	QString strTmp = pAnalog->m_szTagName;
	if (!m_pFes->DeleteFesHashItem(FORWARDINGCGHASHID, strTmp))
	{
		endRemoveRows();

		return false;
	}

	delete pAnalog;
	pAnalog = nullptr;

	endRemoveRows();

	if (m_pChannelData->m_arrCGs.size() == 0)
	{
		RefrushModel();
	}

	return true;
}

void CForwardingCGModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}