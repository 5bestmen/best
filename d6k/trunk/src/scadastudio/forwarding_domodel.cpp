#include "forwarding_domodel.h"
#include "forwarding_dotable.h"
#include "delegate.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "variant.h"
#include "global.h"

CForwardingDOModel::CForwardingDOModel(QObject *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes)
	: QAbstractTableModel(parent), m_pFes(nullptr)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return;
	}
	m_pFes = pFes;

	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}
	m_pCore = pCore;

	Q_ASSERT(pChannelData);
	if (!pChannelData)
	{
		return;
	}
	m_pChannel = pChannelData;

	InitPara();
}

CForwardingDOModel::~CForwardingDOModel()
{

}

int CForwardingDOModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_pChannel->m_arrDOs.size();
}

int CForwardingDOModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (horizontal_header_list.size() > 0)
		return horizontal_header_list.size();

	return 0;
}

QVariant CForwardingDOModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);

		if (index.column() == TagName)
		{
			return m_pChannel->m_arrDOs[nRow]->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return (int)m_pChannel->m_arrDOs[nRow]->m_nID;
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pChannel->m_arrDOs[nRow]->m_nBlockNo;
		}
		else if (index.column() == Desc)
		{
			return m_pChannel->m_arrDOs[nRow]->m_strDescription;
		}
		else if (index.column() == SourceTagName)
		{
			return m_pChannel->m_arrDOs[nRow]->m_strSourceTagName;
		}
		else if (index.column() == OccNo)
		{
			return uint(m_pChannel->m_arrDOs[nRow]->GetOccNo());
		}
		else if (index.column() == GroupNo)
		{
			return (uint)m_pChannel->m_arrDOs[nRow]->m_nGroupNo;
		}
		else if (index.column() == EntryNo)
		{
			return (uint)m_pChannel->m_arrDOs[nRow]->m_nEntryNo;
		}
		else if (index.column() == Address)
		{
			return m_pChannel->m_arrDOs[nRow]->m_strAddress;
		}
		else if (index.column() == FUN)
		{
			return m_pChannel->m_arrDOs[nRow]->m_strFUN;
		}
		else if (index.column() == INF)
		{
			return m_pChannel->m_arrDOs[nRow]->m_strInfo;
		}
		//else if (index.column() == AssociatedDI)
		//{
		//	return m_pChannel->m_arrDOs[nRow]->m_szAssociatedDI;
		//}
		//else if (index.column() == BlockingSignalTag)
		//{
		//	return m_pChannel->m_arrDOs[nRow]->m_szBlockingSignalTag;
		//}
		//else if (index.column() == BlockingSignalCondition)
		//{
		//	return m_pChannel->m_arrDOs[nRow]->m_bBlockingSignalCondition;
		//}
		
		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == TagName)
		{
			return m_pChannel->m_arrDOs[nRow]->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return (int)m_pChannel->m_arrDOs[nRow]->m_nID;
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pChannel->m_arrDOs[nRow]->m_nBlockNo;
		}
		else if (index.column() == Desc)
		{
			return m_pChannel->m_arrDOs[nRow]->m_strDescription;
		}
		else if (index.column() == SourceTagName)
		{
			return m_pChannel->m_arrDOs[nRow]->m_strSourceTagName;
		}
		else if (index.column() == OccNo)
		{
			return uint(m_pChannel->m_arrDOs[nRow]->GetOccNo());
		}
		else if (index.column() == GroupNo)
		{
			return (uint)m_pChannel->m_arrDOs[nRow]->m_nGroupNo;
		}
		else if (index.column() == EntryNo)
		{
			return (uint)m_pChannel->m_arrDOs[nRow]->m_nEntryNo;
		}
		else if (index.column() == Address)
		{
			return m_pChannel->m_arrDOs[nRow]->m_strAddress;
		}
		else if (index.column() == FUN)
		{
			return m_pChannel->m_arrDOs[nRow]->m_strFUN;
		}
		else if (index.column() == INF)
		{
			return m_pChannel->m_arrDOs[nRow]->m_strInfo;
		}
		//else if (index.column() == AssociatedDI)
		//{
		//	return m_pChannel->m_arrDOs[nRow]->m_szAssociatedDI;
		//}
		//else if (index.column() == BlockingSignalTag)
		//{
		//	return m_pChannel->m_arrDOs[nRow]->m_szBlockingSignalTag;
		//}
		//else if (index.column() == BlockingSignalCondition)
		//{
		//	return m_pChannel->m_arrDOs[nRow]->m_bBlockingSignalCondition;
		//}

		return QVariant();
	}

	return QVariant();
}

QVariant CForwardingDOModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (horizontal_header_list.size() > section && section >= 0)
				return horizontal_header_list[section];
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

Qt::ItemFlags CForwardingDOModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

bool CForwardingDOModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
	int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			//Q_ASSERT(m_pChannel);

			//if (index.column() == OccNo)
			//{
			//	if (m_arrDOs[nRow]->m_nOccNo != value.toInt())
			//	{
			//		m_arrDOs[nRow]->m_nOccNo = value.toInt();
			//	}
			//}
			if (index.column() == TagName)
			{
				if (m_pChannel->m_arrDOs[nRow]->m_szTagName != value.toString())
				{
					if (CheckTagNameIsValid(value.toString(), FES_DESC))
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
						auto strError = tr("Forwarding do tagname %1 is exist!!!").arg(value.toString());
						Q_ASSERT(m_pCore);
						if (!m_pCore)
						{
							return false;
						}

						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}

					if (!m_pFes->ModifyFesHashMap(FORWARDINGDOHASHID, value.toString(), m_pChannel->m_arrDOs[nRow]->m_szTagName))
					{
						auto strError = tr("Forwarding do tagname %1 modify fail!!!").arg(m_pChannel->m_arrDOs[nRow]->m_szTagName);
						Q_ASSERT(m_pCore);
						if (!m_pCore)
						{
							return false;
						}

						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}


					memset(m_pChannel->m_arrDOs[nRow]->m_szTagName, 0, sizeof(m_pChannel->m_arrDOs[nRow]->m_szTagName));
					int nSize = value.toString().size();
					strncpy(m_pChannel->m_arrDOs[nRow]->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					emit dataChanged(index, index);
				}
			}
			else if (index.column() == ID)
			{
				if (m_pChannel->m_arrDOs[nRow]->m_nID != value.toInt())
				{
					m_pChannel->m_arrDOs[nRow]->m_nID = value.toInt();
				}
			}
			else if (index.column() == BlockNo)
			{
				if (m_pChannel->m_arrDOs[nRow]->m_nBlockNo != value.toInt())
				{
					m_pChannel->m_arrDOs[nRow]->m_nBlockNo = value.toInt();
				}
			}
			else if (index.column() == Desc)
			{
				if (m_pChannel->m_arrDOs[nRow]->m_strDescription.compare(value.toString()) != 0)
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC))
					{
						return false;
					}
					
					m_pChannel->m_arrDOs[nRow]->m_strDescription = value.toString();
				}
			}
			else if (index.column() == SourceTagName)
			{
				if (m_pChannel->m_arrDOs[nRow]->m_strSourceTagName.compare(value.toString()) != 0)
				{
					m_pChannel->m_arrDOs[nRow]->m_strSourceTagName = value.toString();
				}
			}
			else if (index.column() == GroupNo)
			{
				if (m_pChannel->m_arrDOs[nRow]->m_nGroupNo != value.toInt())
				{
					m_pChannel->m_arrDOs[nRow]->m_nGroupNo = value.toInt();
				}
			}
			else if (index.column() == EntryNo)
			{
				if (m_pChannel->m_arrDOs[nRow]->m_nEntryNo != value.toInt())
				{
					m_pChannel->m_arrDOs[nRow]->m_nEntryNo = value.toInt();
				}
			}
			else if (index.column() == Address)
			{
				if (m_pChannel->m_arrDOs[nRow]->m_strAddress != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC, MAX_ADDRESS_LENGTH_SCADASTUDIO))
					{
						return false;
					}

					m_pChannel->m_arrDOs[nRow]->m_strAddress = value.toString();
				}
			}
			else if (index.column() == FUN)
			{
				if (m_pChannel->m_arrDOs[nRow]->m_strFUN != value.toString())
				{
					m_pChannel->m_arrDOs[nRow]->m_strFUN = value.toString();
				}
			}
			else if (index.column() == INF)
			{
				if (m_pChannel->m_arrDOs[nRow]->m_strInfo.compare(value.toString()) != 0)
				{
					m_pChannel->m_arrDOs[nRow]->m_strInfo = value.toString();
				}
			}
			//else if (index.column() == BlockingSignalCondition)
			//{
			//	if (m_pChannel->m_arrDOs[nRow]->m_bBlockingSignalCondition != value.toBool())
			//	{
			//		m_pChannel->m_arrDOs[nRow]->m_bBlockingSignalCondition = value.toBool();
			//	}
			//}

			emit dataChanged(index, index);

			return true;
		}
	}
		
	return false;
}

void CForwardingDOModel::InitPara()
{
	horizontal_header_list.clear();

	CONFIG_PARA para;

	para.desc = tr("ID");
	para.tagname = "ID";
	horizontal_header_list.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegate = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CForwardingDOTable *)parent())->setItemDelegateForColumn(TagName, delegate);

	para.desc = tr("Desc");
	para.tagname = "Desc";
	horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegateDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CForwardingDOTable *)parent())->setItemDelegateForColumn(Desc, delegateDesc);

	para.desc = tr("SourceTagName");
	para.tagname = "SourceTagName";
	horizontal_header_list.append(para.desc);
	CReadOnlyDelegate *pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CForwardingDOTable *)parent())->setItemDelegateForColumn(SourceTagName, pReadOnlyDelegate);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	horizontal_header_list.append(para.desc);
	CReadOnlyDelegate *pReadDelegate = new CReadOnlyDelegate(this);
	((CForwardingDOTable *)parent())->setItemDelegateForColumn(OccNo, pReadDelegate);

	para.desc = tr("BlockNo");
	para.tagname = "BlockNo";
	horizontal_header_list.append(para.desc);

	para.desc = tr("GroupNo");
	para.tagname = "GroupNo";
	horizontal_header_list.append(para.desc);

	para.desc = tr("EntryNo");
	para.tagname = "EntryNo";
	horizontal_header_list.append(para.desc);

	para.desc = tr("Address");
	para.tagname = "Address";
	horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegateAddress = new CLineEditDelegate(this, TAGNAMELINE, MAX_ADDRESS_LENGTH_SCADASTUDIO);
	((CForwardingDOTable *)parent())->setItemDelegateForColumn(Address, delegateAddress);

	para.desc = tr("FUN");
	para.tagname = "FUN";
	horizontal_header_list.append(para.desc);
	((CForwardingDOTable *)parent())->setItemDelegateForColumn(FUN, delegateDesc);

	para.desc = tr("INF");
	para.tagname = "INF";
	horizontal_header_list.append(para.desc);
	((CForwardingDOTable *)parent())->setItemDelegateForColumn(INF, delegateDesc);

	//para.desc = tr("AssociatedDI");
	//para.tagname = "AssociatedDI";
	//horizontal_header_list.append(para.desc);
	//((CForwardingDOTable *)parent())->setItemDelegateForColumn(AssociatedDI, pReadDelegate);

	//para.desc = tr("BlockingSignalTag");
	//para.tagname = "BlockingSignalTag";
	//horizontal_header_list.append(para.desc);
	//((CForwardingDOTable *)parent())->setItemDelegateForColumn(BlockingSignalTag, pReadDelegate);

	//para.desc = tr("BlockingSignalCondition");
	//para.tagname = "BlockingSignalCondition";
	//horizontal_header_list.append(para.desc);
	//auto pCheckDelegate = new CCheckBoxDelegate(this);
	//((CForwardingDOTable *)parent())->setItemDelegateForColumn(BlockingSignalCondition, pCheckDelegate);
}

std::vector <Config::CForwardingDigitalOutputData *> & CForwardingDOModel::GetDOs()
{
	return m_pChannel->m_arrDOs;
}

bool CForwardingDOModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	//auto nCount = m_arrDOs.size();

	beginInsertRows(parent, position, position + rows - 1);

	Q_ASSERT(m_pFes);
	if (!m_pFes)
	{
		return false;
	}

	for (int i = position; i < position + rows; i++)
	{
		auto pDigtalPoint = m_pFes->AddNewForwardingDOItem();

		pDigtalPoint->SetOccNo(i + 1);
		pDigtalPoint->m_nID = i + 1;

		pDigtalPoint->m_nID = pDigtalPoint->m_nID;

		m_pChannel->m_arrDOs.push_back(pDigtalPoint);
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
bool CForwardingDOModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	Q_ASSERT(m_pFes);
	if (!m_pFes)
	{
		return false;
	}

	beginRemoveRows(parent, rows, rows);

	//auto nCount = m_arrDOs.size();

	auto pDigital = m_pChannel->m_arrDOs[position];

	auto it = std::find(m_pChannel->m_arrDOs.begin(), m_pChannel->m_arrDOs.end(), pDigital);
	if (it != m_pChannel->m_arrDOs.end())
	{
		m_pChannel->m_arrDOs.erase(it);
	}

	//note 关联关系
	QString strTmp = pDigital->m_szTagName;
	if (!m_pFes->DeleteFesHashItem(FORWARDINGDOHASHID, strTmp))
	{
		endRemoveRows();
		return false;
	}

	//
	if (!m_pFes->DeleteFesForwardingDOCombineRelaton(strTmp.toStdString(), pDigital))
	{
		auto strError = QObject::tr("Delete Fes Forwarding DO %1 Combine Relaton failed!!!").arg(strTmp);
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	////转发通道DO删除，如果转发DO有关联sourcetagname hash，那么关联hash也删除绑定关系
	//if (!pDigital->m_strSourceTagName.isEmpty())
	//{
	//	if (!m_pFes->DeleteForwardingDORelationSourceTagNameArr(pDigital->m_strSourceTagName.toStdString(), pDigital))
	//	{
	//		auto strError = QObject::tr("Delete Forwarding DO Relation SourceTagName Arr %1 delete failed!!!").arg(pDigital->m_strSourceTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	delete pDigital;
	pDigital = nullptr;

	endRemoveRows();

	if (m_pChannel->m_arrDOs.size() == 0)
	{
		RefrushModel();
	}
	
	return true;
}