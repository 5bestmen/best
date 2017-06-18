#include "forwarding_aomodel.h"
#include "scadastudiodefine.h"
#include "delegate.h"
#include "forwarding_aotable.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "variant.h"
#include "global.h"

CForwardingAOModel::CForwardingAOModel(QObject *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes)
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

CForwardingAOModel::~CForwardingAOModel()
{

}

int CForwardingAOModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_pChannelData->m_arrAOs.size();
}

int CForwardingAOModel::columnCount(const QModelIndex &parent) const
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

QVariant CForwardingAOModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);

		if (index.column() == TagName)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return (uint)m_pChannelData->m_arrAOs[nRow]->m_nID;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pChannelData->m_arrAOs[nRow]->GetOccNo();
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pChannelData->m_arrAOs[nRow]->m_nBlockNo;
		}
		else if (index.column() == Desc)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_strDescription;
		}
		else if (index.column() == SourceTagName)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_strSourceTagName;
		}
		else if (index.column() == GroupNo)
		{
			return (uint)m_pChannelData->m_arrAOs[nRow]->m_nGroupNo;
		}
		else if (index.column() == EntryNo)
		{
			return (uint)m_pChannelData->m_arrAOs[nRow]->m_nEntryNo;
		}
		else if (index.column() == Address)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_strAddress;
		}
		else if (index.column() == FUN)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_strFUN;
		}
		else if (index.column() == Info)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_strInfo;
		}
		else if (index.column() == MaxRaw)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_dbMaxRaw;
		}
		else if (index.column() == MinRaw)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_dbMinRaw;
		}
		else if (index.column() == MaxConvert)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_dbMaxConvert;
		}
		else if (index.column() == MinConvert)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_dbMinConvert;
		}
		else if (index.column() == ScaleTagName)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_strScaleTagName;
		}
		else if (index.column() == ScaleType)
		{
			if (m_pChannelData->m_arrAOs[nRow]->m_nScaleType == ScaleType::LINEAR)
			{
				return m_arrVecScaleType[ScaleType::LINEAR];
			}
			else
			{
				return m_arrVecScaleType[ScaleType::NONLINEAR];
			}
		}

		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == TagName)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return (uint)m_pChannelData->m_arrAOs[nRow]->m_nID;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pChannelData->m_arrAOs[nRow]->GetOccNo();
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pChannelData->m_arrAOs[nRow]->m_nBlockNo;
		}
		else if (index.column() == GroupNo)
		{
			return (uint)m_pChannelData->m_arrAOs[nRow]->m_nGroupNo;
		}
		else if (index.column() == EntryNo)
		{
			return (uint)m_pChannelData->m_arrAOs[nRow]->m_nEntryNo;
		}
		else if (index.column() == Address)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_strAddress;
		}
		else if (index.column() == SourceTagName)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_strSourceTagName;
		}
		else if (index.column() == Desc)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_strDescription;
		}
		else if (index.column() == FUN)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_strFUN;
		}
		else if (index.column() == Info)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_strInfo;
		}
		else if (index.column() == MaxRaw)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_dbMaxRaw;
		}
		else if (index.column() == MinRaw)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_dbMinRaw;
		}
		else if (index.column() == MaxConvert)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_dbMaxConvert;
		}
		else if (index.column() == MinConvert)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_dbMinConvert;
		}
		else if (index.column() == ScaleTagName)
		{
			return m_pChannelData->m_arrAOs[nRow]->m_strScaleTagName;
		}
		else if (index.column() == ScaleType)
		{
			if (m_pChannelData->m_arrAOs[nRow]->m_nScaleType == ScaleType::LINEAR)
			{
				return m_arrVecScaleType[ScaleType::LINEAR];
			}
			else
			{
				return m_arrVecScaleType[ScaleType::NONLINEAR];
			}
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CForwardingAOModel::headerData(int section, Qt::Orientation orientation, int role) const
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

void CForwardingAOModel::InitPara()
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
	((CForwardingAOTable *)parent())->setItemDelegateForColumn(TagName, delegate);

	para.desc = tr("Desc");
	para.tagname = "Desc";
	m_arrInfo.insert(Desc, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegateDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CForwardingAOTable *)parent())->setItemDelegateForColumn(TagName, delegateDesc);

	para.desc = tr("SourceTagName");
	para.tagname = "SourceTagName";
	m_arrInfo.insert(SourceTagName, para);
	m_horizontal_header_list.append(para.desc);
	CReadOnlyDelegate *pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CForwardingAOTable *)parent())->setItemDelegateForColumn(SourceTagName, pReadOnlyDelegate);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_arrInfo.insert(OccNo, para);
	m_horizontal_header_list.append(para.desc);
	((CForwardingAOTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnlyDelegate);

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
	((CForwardingAOTable *)parent())->setItemDelegateForColumn(Address, delegateAddress);

	para.desc = tr("FUN");
	para.tagname = "FUN";
	m_arrInfo.insert(FUN, para);
	m_horizontal_header_list.append(para.desc);
	((CForwardingAOTable *)parent())->setItemDelegateForColumn(FUN, delegateDesc);

	para.desc = tr("Info");
	para.tagname = "Info";
	m_arrInfo.insert(Info, para);
	m_horizontal_header_list.append(para.desc);
	((CForwardingAOTable *)parent())->setItemDelegateForColumn(Info, delegateDesc);

	para.desc = tr("MaxRaw");
	para.tagname = "MaxRaw";
	m_arrInfo.insert(MaxRaw, para);
	m_horizontal_header_list.append(para.desc);
	auto delegateDouble = new CLineEditDelegate(this, DOUBLETYPE, THREE_COUNT);
	((CForwardingAOTable *)parent())->setItemDelegateForColumn(MaxRaw, delegateDouble);

	para.desc = tr("MinRaw");
	para.tagname = "MinRaw";
	m_arrInfo.insert(MinRaw, para);
	m_horizontal_header_list.append(para.desc);
	((CForwardingAOTable *)parent())->setItemDelegateForColumn(MinRaw, delegateDouble);

	para.desc = tr("MaxConvert");
	para.tagname = "MaxConvert";
	m_arrInfo.insert(MaxConvert, para);
	m_horizontal_header_list.append(para.desc);
	((CForwardingAOTable *)parent())->setItemDelegateForColumn(MaxConvert, delegateDouble);

	para.desc = tr("MinConvert");
	para.tagname = "MinConvert";
	m_arrInfo.insert(MinConvert, para);
	m_horizontal_header_list.append(para.desc);
	((CForwardingAOTable *)parent())->setItemDelegateForColumn(MinConvert, delegateDouble);

	para.desc = tr("ScaleTagName");
	para.tagname = "ScaleTagName";
	m_arrInfo.insert(ScaleTagName, para);
	m_horizontal_header_list.append(para.desc);
	CReadOnlyDelegate *pReadDelegate = new CReadOnlyDelegate(this);
	((CForwardingAOTable *)parent())->setItemDelegateForColumn(ScaleTagName, pReadDelegate);

	para.desc = tr("ScaleType");
	para.tagname = "ScaleType";
	m_arrInfo.insert(ScaleType, para);
	m_horizontal_header_list.append(para.desc);
	m_arrVecScaleType.clear();
	IDDESC tmp;
	tmp.desc = tr("linear");
	//tmp.ID = LINEAR;
	m_arrVecScaleType.insert(LINEAR, tmp.desc);
	tmp.desc = tr("nonlinear");
	//tmp.ID = NONLINEAR;
	m_arrVecScaleType.insert(NONLINEAR, tmp.desc);
	((CForwardingAOTable *)parent())->setItemDelegateForColumn(ScaleType, pReadDelegate);
}

bool CForwardingAOModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			//Q_ASSERT(m_pChannel);

			if (index.column() == TagName)
			{
				if (m_pChannelData->m_arrAOs[nRow]->m_szTagName != value.toString())
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
						auto strError = tr("Forwarding ao tagname is exist!!!");
						Q_ASSERT(m_pCore);
						if (!m_pCore)
						{
							return false;
						}
						
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}
					
					if (!m_pFes->ModifyFesHashMap(FORWARDINGAOHASHID, value.toString(), m_pChannelData->m_arrAOs[nRow]->m_szTagName))
					{
						auto strError = tr("Forwarding ao tagname modify fail!!!").arg(value.toString());
						Q_ASSERT(m_pCore);
						if (!m_pCore)
						{
							return false;
						}

						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
						
						return false;
					}

					//m_arrAOs[nRow]->m_szTagName = value.toString();
					memset(m_pChannelData->m_arrAOs[nRow]->m_szTagName, 0, sizeof(m_pChannelData->m_arrAOs[nRow]->m_szTagName));
					strncpy(m_pChannelData->m_arrAOs[nRow]->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, value.toString().size()));
				}
			}
			else if (index.column() == ID)
			{
				if (m_pChannelData->m_arrAOs[nRow]->m_nID != value.toInt())
				{
					m_pChannelData->m_arrAOs[nRow]->m_nID = value.toInt();
				}
			}
			else if (index.column() == BlockNo)
			{
				if (m_pChannelData->m_arrAOs[nRow]->m_nBlockNo != value.toInt())
				{
					m_pChannelData->m_arrAOs[nRow]->m_nBlockNo = value.toInt();
				}
			}
			else if (index.column() == GroupNo)
			{
				if (m_pChannelData->m_arrAOs[nRow]->m_nGroupNo != value.toInt())
				{
					m_pChannelData->m_arrAOs[nRow]->m_nGroupNo = value.toInt();
				}
			}
			else if (index.column() == EntryNo)
			{
				if (m_pChannelData->m_arrAOs[nRow]->m_nEntryNo != value.toInt())
				{
					m_pChannelData->m_arrAOs[nRow]->m_nEntryNo = value.toInt();
				}
			}
			else if (index.column() == Address)
			{
				if (m_pChannelData->m_arrAOs[nRow]->m_strAddress != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC, MAX_ADDRESS_LENGTH_SCADASTUDIO))
					{
						return false;
					}

					m_pChannelData->m_arrAOs[nRow]->m_strAddress = value.toString();
				}
			}
			else if (index.column() == Desc)
			{
				if (m_pChannelData->m_arrAOs[nRow]->m_strDescription.compare(value.toString()) != 0)
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC))
					{
						return false;
					}
					
					m_pChannelData->m_arrAOs[nRow]->m_strDescription = value.toString();

					//m_pFes->ModifyAOsRelationScaleDescArr(m_arrAOs[nRow]->m_strScaleTagName.toStdString(), value.toString().toStdString());
				}
			}
			else if (index.column() == FUN)
			{
				if (m_pChannelData->m_arrAOs[nRow]->m_strFUN != value.toString())
				{
					m_pChannelData->m_arrAOs[nRow]->m_strFUN = value.toString();
				}
			}
			else if (index.column() == Info)
			{
				if (m_pChannelData->m_arrAOs[nRow]->m_strInfo.compare(value.toString()) != 0)
				{
					m_pChannelData->m_arrAOs[nRow]->m_strInfo = value.toString();
				}
			}
			else if (index.column() == MaxRaw)
			{
				if (m_pChannelData->m_arrAOs[nRow]->m_dbMaxRaw != value.toDouble())
				{
					m_pChannelData->m_arrAOs[nRow]->m_dbMaxRaw = value.toDouble();
				}
			}
			else if (index.column() == MinRaw)
			{
				if (m_pChannelData->m_arrAOs[nRow]->m_dbMinRaw != value.toDouble())
				{
					m_pChannelData->m_arrAOs[nRow]->m_dbMinRaw = value.toDouble();
				}
			}
			else if (index.column() == MaxConvert)
			{
				if (m_pChannelData->m_arrAOs[nRow]->m_dbMaxConvert != value.toDouble())
				{
					m_pChannelData->m_arrAOs[nRow]->m_dbMaxConvert = value.toDouble();
				}
			}
			else if (index.column() == MinConvert)
			{
				if (m_pChannelData->m_arrAOs[nRow]->m_dbMinConvert != value.toDouble())
				{
					m_pChannelData->m_arrAOs[nRow]->m_dbMinConvert = value.toDouble();
				}
			}
			else if (index.column() == ScaleTagName)
			{
				if (m_pChannelData->m_arrAOs[nRow]->m_strScaleTagName != value.toString())
				{
					m_pChannelData->m_arrAOs[nRow]->m_strScaleTagName = value.toString();
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
Qt::ItemFlags CForwardingAOModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

std::vector <Config::CForwardingAnalogOutputData *> & CForwardingAOModel::GetAOs()
{
	return m_pChannelData->m_arrAOs;
}

bool CForwardingAOModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	//auto nCount = m_arrAOs.size();

	beginInsertRows(parent, position, position + rows - 1);

	for (int i = position; i < position + rows; i++)
	{
		auto pAnalogPoint = m_pFes->AddNewForwardingAOItem();

		pAnalogPoint->SetOccNo(i + 1);
		pAnalogPoint->m_nID = i + 1;

		//auto strNewTagName = QString("AnalogOutput%1").arg(pAnalogPoint->m_nID);
		//auto strNewDesc = QString("AnalogOutput%1").arg(pAnalogPoint->m_nID);

		////是否校验

		////pAnalogPoint->m_szTagName = strNewTagName;
		//memset(pAnalogPoint->m_szTagName, 0, sizeof(pAnalogPoint->m_szTagName));
		//strncpy(pAnalogPoint->m_szTagName, strNewTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH, strNewTagName.size()));

		//pAnalogPoint->m_strDescription = strNewDesc;
		//pAnalogPoint->m_bNewPointAlarm = true;

		m_pChannelData->m_arrAOs.push_back(pAnalogPoint);
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
bool CForwardingAOModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, rows, rows);

	//auto nCount = m_arrAOs.size();

	auto pAnalog = m_pChannelData->m_arrAOs[position];

	auto it = std::find(m_pChannelData->m_arrAOs.begin(), m_pChannelData->m_arrAOs.end(), pAnalog);
	if (it != m_pChannelData->m_arrAOs.end())
	{
		m_pChannelData->m_arrAOs.erase(it);
	}
	
	Q_ASSERT(m_pFes);
	if (!m_pFes)
	{
		return false;
	}

	//note 关联关系
	QString strTmp = pAnalog->m_szTagName;
	if (!m_pFes->DeleteFesHashItem(FORWARDINGAOHASHID, strTmp))
	{
		endRemoveRows();

		return false;
	}

	if (!m_pFes->DeleteFesForwardingAOCombineRelaton(strTmp.toStdString(), pAnalog))
	{
		auto strError = QObject::tr("Delete Fes Forwarding DI %1 Combine Relaton failed!!!").arg(strTmp);
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	////转发通道AO删除，如果转发AO有关联sourcetagname hash，那么关联hash也删除绑定关系
	//if (!pAnalog->m_strSourceTagName.isEmpty())
	//{
	//	if (!m_pFes->DeleteForwardingAORelationSourceTagNameArr(pAnalog->m_strSourceTagName.toStdString(), pAnalog))
	//	{
	//		auto strError = QObject::tr("Delete Forwarding AO Relation SourceTagName Arr %1 delete failed!!!").arg(pAnalog->m_strSourceTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	////转发通道AO删除，如果转发AO有关联Scale hash，那么关联hash也删除绑定关系
	//if (!pAnalog->m_strScaleTagName.isEmpty())
	//{
	//	if (!m_pFes->DeleteForwardingAORelationScaleArr(pAnalog->m_strScaleTagName.toStdString(), pAnalog))
	//	{
	//		auto strError = QObject::tr("Delete Forwarding AO Relation Scale Arr %1 delete failed!!!").arg(pAnalog->m_strScaleTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	delete pAnalog;
	pAnalog = nullptr;

	endRemoveRows();

	if (m_pChannelData->m_arrAOs.size() == 0)
	{
		RefrushModel();
	}

	return true;
}

void CForwardingAOModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}