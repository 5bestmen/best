#include "forwarding_dimodel.h"
#include "forwarding_ditable.h"
#include "delegate.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/icore.h"
#include "global.h"

CForwardingDIModel::CForwardingDIModel(QObject *parent, Config::CForwardingChannelData *pChannel, Config::CFesData *pFes, IMainModuleInterface *pCore)
	: QAbstractTableModel(parent), m_pChannel(nullptr)
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

	Q_ASSERT(pChannel);
	if (!pChannel)
	{
		return;
	}
	m_pChannel = pChannel;

	m_pCore = pCore;

	m_pFes = pFes;

	InitPara();
}

CForwardingDIModel::~CForwardingDIModel()
{

}

int CForwardingDIModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_pChannel->m_arrDIs.size();
}

int CForwardingDIModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (horizontal_header_list.size() > 0)
		return horizontal_header_list.size();

	return 0;
}

QVariant CForwardingDIModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);

		if (index.column() == OccNo)
		{
			return uint(m_pChannel->m_arrDIs[nRow]->GetOccNo());
		}
		else if (index.column() == TagName)
		{
			return m_pChannel->m_arrDIs[nRow]->m_szTagName;
		}
		else if (index.column() == SourceTagName)
		{
			return m_pChannel->m_arrDIs[nRow]->m_strSourceTagName;
		}
		else if (index.column() == ID)
		{
			return (uint)m_pChannel->m_arrDIs[nRow]->m_nID;
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pChannel->m_arrDIs[nRow]->m_nBlockNo;
		}
		//else if (index.column() == DataType)
		//{
		//	return (uint)m_pChannel->m_arrDIs[nRow]->m_nDataType;
		//}
		else if (index.column() == GroupNo)
		{
			return (uint)m_pChannel->m_arrDIs[nRow]->m_nGroupNo;
		}
		else if (index.column() == EntryNo)
		{
			return (uint)m_pChannel->m_arrDIs[nRow]->m_nEntryNo;
		}
		else if (index.column() == Address)
		{
			return m_pChannel->m_arrDIs[nRow]->m_strAddress;
		}
		else if (index.column() == Desc)
		{
			return m_pChannel->m_arrDIs[nRow]->m_strDescription;
		}
		else if (index.column() == FUN)
		{
			return m_pChannel->m_arrDIs[nRow]->m_strFUN;
		}
		else if (index.column() == INF)
		{
			return m_pChannel->m_arrDIs[nRow]->m_strInfo;
		}
		else if (index.column() == Inverse)
		{
			return m_pChannel->m_arrDIs[nRow]->m_bInverse;
		}

		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == OccNo)
		{
			return uint(m_pChannel->m_arrDIs[nRow]->GetOccNo());
		}
		else if (index.column() == TagName)
		{
			return m_pChannel->m_arrDIs[nRow]->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return (uint)m_pChannel->m_arrDIs[nRow]->m_nID;
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pChannel->m_arrDIs[nRow]->m_nBlockNo;
		}
		//else if (index.column() == DataType)
		//{
		//	return (uint)m_pChannel->m_arrDIs[nRow]->m_nDataType;
		//}
		else if (index.column() == GroupNo)
		{
			return (uint)m_pChannel->m_arrDIs[nRow]->m_nGroupNo;
		}
		else if (index.column() == EntryNo)
		{
			return (uint)m_pChannel->m_arrDIs[nRow]->m_nEntryNo;
		}
		else if (index.column() == Address)
		{
			return m_pChannel->m_arrDIs[nRow]->m_strAddress;
		}
		else if (index.column() == SourceTagName)
		{
			return m_pChannel->m_arrDIs[nRow]->m_strSourceTagName;
		}
		else if (index.column() == Desc)
		{
			return m_pChannel->m_arrDIs[nRow]->m_strDescription;
		}
		else if (index.column() == FUN)
		{
			return m_pChannel->m_arrDIs[nRow]->m_strFUN;
		}
		else if (index.column() == INF)
		{
			return m_pChannel->m_arrDIs[nRow]->m_strInfo;
		}
		else if (index.column() == Inverse)
		{
			return m_pChannel->m_arrDIs[nRow]->m_bInverse;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CForwardingDIModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (horizontal_header_list.size() > section && section > -1)
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

Qt::ItemFlags CForwardingDIModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

bool CForwardingDIModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
	int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			//Q_ASSERT(m_pChannel);

			/*if (index.column() == OccNo)
			{
				if (m_pChannel->m_arrDIs[nRow]->m_nOccNo != value.toInt())
				{
					m_pChannel->m_arrDIs[nRow]->m_nOccNo = value.toInt();
				}
			}*/
			if (index.column() == TagName)
			{
				if (m_pChannel->m_arrDIs[nRow]->m_szTagName != value.toString())
				{
					Q_ASSERT(m_pFes);
					if (!m_pFes)
					{
						return false;
					}
					
					if (!CheckTagNameIsValid(value.toString(), FES_DESC))
					{
						auto strError = tr("Forwarding di tagname %1 is invalid!!!").arg(value.toString());
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}


					if (m_pFes->CheckTagNameIsExist(value.toString()))
					{
						auto strError = tr("Forwarding di tagname %1 is exist!!!").arg(value.toString());
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}

					auto bFlag = m_pFes->ModifyFesHashMap(FORWARDINGDIHASHID, value.toString(), m_pChannel->m_arrDIs[nRow]->m_szTagName);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						return false;
					}


					memset(m_pChannel->m_arrDIs[nRow]->m_szTagName, 0, sizeof(m_pChannel->m_arrDIs[nRow]->m_szTagName));
					int nSize = value.toString().size();
					strncpy(m_pChannel->m_arrDIs[nRow]->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					//刷新index
					emit dataChanged(index, index);
				}
			}
			else if (index.column() == ID)
			{
				if (m_pChannel->m_arrDIs[nRow]->m_nID != value.toInt())
				{
					m_pChannel->m_arrDIs[nRow]->m_nID = value.toInt();
				}
			}
			else if (index.column() == BlockNo)
			{
				if (m_pChannel->m_arrDIs[nRow]->m_nBlockNo != value.toInt())
				{
					m_pChannel->m_arrDIs[nRow]->m_nBlockNo = value.toInt();
				}
			}
			else if (index.column() == SourceTagName)
			{
				if (m_pChannel->m_arrDIs[nRow]->m_strSourceTagName != value.toString())
				{
					m_pChannel->m_arrDIs[nRow]->m_strSourceTagName = value.toString();
				}
			}
			else if (index.column() == Desc)
			{
				if (m_pChannel->m_arrDIs[nRow]->m_strDescription.compare(value.toString()) != 0)
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC))
					{
						return false;
					}
					
					m_pChannel->m_arrDIs[nRow]->m_strDescription = value.toString();
				}
			}
			else if (index.column() == GroupNo)
			{
				if (m_pChannel->m_arrDIs[nRow]->m_nGroupNo != value.toInt())
				{
					m_pChannel->m_arrDIs[nRow]->m_nGroupNo = value.toInt();
				}
			}
			else if (index.column() == EntryNo)
			{
				if (m_pChannel->m_arrDIs[nRow]->m_nEntryNo != value.toInt())
				{
					m_pChannel->m_arrDIs[nRow]->m_nEntryNo = value.toInt();
				}
			}
			else if (index.column() == Address)
			{
				if (m_pChannel->m_arrDIs[nRow]->m_strAddress != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC, MAX_ADDRESS_LENGTH_SCADASTUDIO))
					{
						return false;
					}
					
					m_pChannel->m_arrDIs[nRow]->m_strAddress = value.toString();
				}
			}
			else if (index.column() == FUN)
			{
				if (m_pChannel->m_arrDIs[nRow]->m_strFUN != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC))
					{
						return false;
					}
					
					m_pChannel->m_arrDIs[nRow]->m_strFUN = value.toString();
				}
			}
			else if (index.column() == INF)
			{
				if (m_pChannel->m_arrDIs[nRow]->m_strInfo.compare(value.toString()) != 0)
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC))
					{
						return false;
					}
					
					m_pChannel->m_arrDIs[nRow]->m_strInfo = value.toString();
				}
			}
			else if (index.column() == Inverse)
			{
				if (m_pChannel->m_arrDIs[nRow]->m_bInverse != value.toBool())
				{
					m_pChannel->m_arrDIs[nRow]->m_bInverse = value.toBool();
				}
			}

			emit dataChanged(index, index);

			return true;
		}
	}
		
	return false;
}

void CForwardingDIModel::InitPara()
{
	horizontal_header_list.clear();

	m_arrInfo.clear();

	CONFIG_PARA para;

	para.desc = tr("ID");
	para.tagname = "ID";
	m_arrInfo.insert(ID, para);
	horizontal_header_list.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	m_arrInfo.insert(TagName, para);
	horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegate = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CForwardingDITable *)parent())->setItemDelegateForColumn(TagName, delegate);

	para.desc = tr("Desc");
	para.tagname = "Desc";
	m_arrInfo.insert(Desc, para);
	horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegateDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CForwardingDITable *)parent())->setItemDelegateForColumn(Desc, delegateDesc);

	para.desc = tr("SourceTagName");
	para.tagname = "SourceTagName";
	m_arrInfo.insert(SourceTagName, para);
	horizontal_header_list.append(para.desc);
	CReadOnlyDelegate *pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CForwardingDITable *)parent())->setItemDelegateForColumn(SourceTagName, pReadOnlyDelegate);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_arrInfo.insert(OccNo, para);
	horizontal_header_list.append(para.desc);
	((CForwardingDITable *)parent())->setItemDelegateForColumn(OccNo, pReadOnlyDelegate);

	para.desc = tr("BlockNo");
	para.tagname = "BlockNo";
	m_arrInfo.insert(BlockNo, para);
	horizontal_header_list.append(para.desc);

	para.desc = tr("GroupNo");
	para.tagname = "GroupNo";
	m_arrInfo.insert(GroupNo, para);
	horizontal_header_list.append(para.desc);

	para.desc = tr("EntryNo");
	para.tagname = "EntryNo";
	m_arrInfo.insert(EntryNo, para);
	horizontal_header_list.append(para.desc);

	para.desc = tr("Address");
	para.tagname = "Address";
	m_arrInfo.insert(Address, para);
	horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegateAddress = new CLineEditDelegate(this, TAGNAMELINE, MAX_ADDRESS_LENGTH_SCADASTUDIO);
	((CForwardingDITable *)parent())->setItemDelegateForColumn(Address, delegateAddress);

	para.desc = tr("FUN");
	para.tagname = "FUN";
	m_arrInfo.insert(FUN, para);
	horizontal_header_list.append(para.desc);
	((CForwardingDITable *)parent())->setItemDelegateForColumn(FUN, delegateDesc);

	para.desc = tr("INF");
	para.tagname = "INF";
	m_arrInfo.insert(INF, para);
	horizontal_header_list.append(para.desc);
	((CForwardingDITable *)parent())->setItemDelegateForColumn(INF, delegateDesc);

	para.desc = tr("Inverse");
	para.tagname = "Inverse";
	m_arrInfo.insert(Inverse, para);
	horizontal_header_list.append(para.desc);
	CCheckBoxDelegate *pCheckDelegate = new CCheckBoxDelegate(this);
	((CForwardingDITable *)parent())->setItemDelegateForColumn(Inverse, pCheckDelegate);
}

/*! \fn QMap<int32s, CONFIG_PARA> CDIModel::GetColumInfo()
********************************************************************************************************* 
** \brief CDIModel::GetColumInfo 
** \details 
** \return QMap<int32s, CONFIG_PARA> 
** \author gw
** \date 2016年5月26日 
** \note 
********************************************************************************************************/
QMap<int32s, CONFIG_PARA> &CForwardingDIModel::GetColumnInfo()
{
	return m_arrInfo;
}

int CForwardingDIModel::GetDIsCount()
{
	return (int)m_pChannel->m_arrDIs.size();
}

std::vector <Config::CForwardingDigitalInputData *> & CForwardingDIModel::GetDIs()
{
	return m_pChannel->m_arrDIs;
}

/*! \fn bool CDIModel::InsertRows(int position, int rows, const QModelIndex &parent)
********************************************************************************************************* 
** \brief CDIModel::InsertRows 
** \details 添加数据
** \param position 
** \param rows 
** \param parent 
** \return bool 
** \author gw
** \date 2016年8月11日 
** \note 
********************************************************************************************************/
bool CForwardingDIModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	//auto nCount = m_pChannel->m_arrDIs.size();

	beginInsertRows(parent, position, position + rows - 1);

	Q_ASSERT(m_pFes);
	if (!m_pFes)
	{
		return false;
	}

	for (int i = position; i < position + rows; i++)
	{
		auto pDigtalPoint = m_pFes->AddNewForwardingDIItem();

		pDigtalPoint->SetOccNo(i + 1);
		pDigtalPoint->m_nID = i + 1;

		pDigtalPoint->m_nID = pDigtalPoint->m_nID;

		//auto strNewTagName = QString("AnalogInput%1").arg(pDigtalPoint->m_nID);
		//auto strNewDesc = QString("AnalogInput%1").arg(pDigtalPoint->m_nID);

		////是否校验
		//memset(pDigtalPoint->m_szTagName, 0, sizeof(pDigtalPoint->m_szTagName));
		//int nSize = strNewTagName.size();
		//strncpy(pDigtalPoint->m_szTagName, strNewTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH, nSize));
		//pDigtalPoint->m_strDescription = strNewDesc;
		//pDigtalPoint->m_bNewPointAlarm = true;

		m_pChannel->m_arrDIs.push_back(pDigtalPoint);
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
bool CForwardingDIModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, rows, rows);

	//auto nCount = m_pChannel->m_arrDIs.size();

	auto pDigital = m_pChannel->m_arrDIs[position];

	//note 关联删除
	QString strTmp = pDigital->m_szTagName;
	if (!m_pFes->DeleteFesHashItem(FORWARDINGDIHASHID, strTmp))
	{
		endRemoveRows();

		return false;
	}

	if (!m_pFes->DeleteFesForwardingDICombineRelaton(strTmp.toStdString(), pDigital))
	{
		auto strError = QObject::tr("Delete Fes Forwarding DI %1 Combine Relaton failed!!!").arg(strTmp);
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	////转发通道DI删除，如果转发DI有关联sourcetagname hash，那么关联hash也删除绑定关系
	//if (!pDigital->m_strSourceTagName.isEmpty())
	//{
	//	if (!m_pFes->DeleteForwardingDIRelationSourceTagNameArr(pDigital->m_strSourceTagName.toStdString(), pDigital))
	//	{
	//		auto strError = QObject::tr("Delete Forwarding DI Relation SourceTagName Arr %1 delete failed!!!").arg(pDigital->m_strSourceTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	auto it = std::find(m_pChannel->m_arrDIs.begin(), m_pChannel->m_arrDIs.end(), pDigital);
	if (it != m_pChannel->m_arrDIs.end())
	{
		m_pChannel->m_arrDIs.erase(it);
	}

	Q_ASSERT(m_pFes);
	if (!m_pFes)
	{
		return false;
	}

	delete pDigital;
	pDigital = nullptr;

	endRemoveRows();

	if (m_pChannel->m_arrDIs.size() == 0)
	{
		RefrushModel();
	}

	return true;
}

