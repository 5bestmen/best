#include "forwarding_aimodel.h"
#include "delegate.h"
#include "forwading_ai_table.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "global.h"

CForwardingAIModel::CForwardingAIModel(QObject *parent, Config::CForwardingChannelData *pChannel, Config::CFesData *pFes, IMainModuleInterface *pCore)
	: QAbstractTableModel(parent), m_pChannel(nullptr), m_pFes(nullptr)
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

	Q_ASSERT(pChannel);
	if (!pChannel)
	{
		return;
	}
	m_pChannel = pChannel;

	InitPara();
}

CForwardingAIModel::~CForwardingAIModel()
{

}

void CForwardingAIModel::SetHorizontalHeaderList(QStringList list)
{
	if (!horizontal_header_list.isEmpty())
	{
		horizontal_header_list.clear();
	}

	horizontal_header_list = list;
}

void CForwardingAIModel::SetVerticalHeaderList(QStringList verticalHeaderList)
{
	//vertical_header_list = verticalHeaderList;
}

int CForwardingAIModel::rowCount(const QModelIndex &parent) const
{
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	Q_UNUSED(parent);

	return (int)m_pChannel->m_arrAIs.size();
}

int CForwardingAIModel::columnCount(const QModelIndex &parent) const
{	
	Q_UNUSED(parent);
	
	if (horizontal_header_list.size() > 0)
		return horizontal_header_list.size();

	//if(0 == arr_row_list.count())
	//	return 0;
	//else if(arr_row_list.count() < 1)
	//	return 0;
	//else
	//	return arr_row_list.at(0).size();

	return true;
}

QVariant CForwardingAIModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	auto &arr = m_pChannel->m_arrAIs;

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);

		if (index.column() == ID)
		{
			return static_cast<uint>(arr[nRow]->m_nID);
		}
		else if (index.column() == TagName)
		{
			return arr[nRow]->m_szTagName;
		}
		else if (index.column() == Desc)
		{
			return arr[nRow]->m_strDescription;
		}
		else if (index.column() == SourceTagName)
		{
			return arr[nRow]->m_strSourceTagName;
		}
		else if (index.column() == OccNo)
		{
			return (int)arr[nRow]->GetOccNo();
		}
		else if (index.column() == BlockOccNo)
		{
			return (uint)arr[nRow]->m_nBlockNo;
		}
		else if (index.column() == GroupNo)
		{
			return (uint)arr[nRow]->m_nGroupNo;
		}
		else if (index.column() == EntryNo)
		{
			return (uint)arr[nRow]->m_nEntryNo;
		}
		else if (index.column() == Address)
		{
			return arr[nRow]->m_strAddress;
		}
		//else if (index.column() == DataType)
		//{
		//	return m_arrAIs[nRow]->m_nDataType;
		//}
		else if (index.column() == FUN)
		{
			return arr[nRow]->m_strFUN;
		}
		else if (index.column() == INF)
		{
			return arr[nRow]->m_strInfo;
		}
		else if (index.column() == MaxRaw)
		{
			return arr[nRow]->m_dblMaxRaw;
		}
		else if (index.column() == MinRaw)
		{
			return arr[nRow]->m_dblMinRaw;
		}
		else if (index.column() == MaxConvert)
		{
			return arr[nRow]->m_dblMaxConvert;
		}
		else if (index.column() == MinConvert)
		{
			return arr[nRow]->m_dblMinConvert;
		}
		else if (index.column() == ScaleTagName)
		{
			return arr[nRow]->m_strScaleTagName;
		}
		else if (index.column() == ScaleType)
		{
			if (arr[nRow]->m_nScaleType == ScaleType::LINEAR)
			{
				return m_arrVecScaleType[ScaleType::LINEAR];
			}
			else
			{
				return m_arrVecScaleType[ScaleType::NONLINEAR];
			}
		}
		else if (index.column() == HLimit)
		{
			return arr[nRow]->m_dblHLimit;
		}
		else if (index.column() == LLimit)
		{
			return arr[nRow]->m_dblLLimit;
		}
		else if (index.column() == Threshold)
		{
			return arr[nRow]->m_dblThreshold;
		}
	
		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == ID)
		{
			return static_cast<uint>(arr[nRow]->m_nID);
		}
		else if (index.column() == TagName)
		{
			return arr[nRow]->m_szTagName;
		}
		else if (index.column() == SourceTagName)
		{
			return arr[nRow]->m_strSourceTagName;
		}
		else if (index.column() == Desc)
		{
			return arr[nRow]->m_strDescription;
		}
		else if (index.column() == OccNo)
		{
			return (int)arr[nRow]->GetOccNo();
		}
		else if (index.column() == BlockOccNo)
		{
			return (uint)arr[nRow]->m_nBlockNo;
		}
		else if (index.column() == GroupNo)
		{
			return (uint)arr[nRow]->m_nGroupNo;
		}
		else if (index.column() == EntryNo)
		{
			return (uint)arr[nRow]->m_nEntryNo;
		}
		else if (index.column() == Address)
		{
			return arr[nRow]->m_strAddress;
		}
		//else if (index.column() == DataType)
		//{
		//	return m_arrAIs[nRow]->m_nDataType;
		//}
		else if (index.column() == FUN)
		{
			return arr[nRow]->m_strFUN;
		}
		else if (index.column() == INF)
		{
			return arr[nRow]->m_strInfo;
		}
		else if (index.column() == MaxRaw)
		{
			return arr[nRow]->m_dblMaxRaw;
		}
		else if (index.column() == MinRaw)
		{
			return arr[nRow]->m_dblMinRaw;
		}
		else if (index.column() == ScaleTagName)
		{
			return arr[nRow]->m_strScaleTagName;
		}
		else if (index.column() == ScaleType)
		{
			if (arr[nRow]->m_nScaleType == ScaleType::LINEAR)
			{
				return m_arrVecScaleType[ScaleType::LINEAR];
			}
			else
			{
				return m_arrVecScaleType[ScaleType::NONLINEAR];
			}
		}
		else if (index.column() == HLimit)
		{
			return arr[nRow]->m_dblHLimit;
		}
		else if (index.column() == LLimit)
		{
			return arr[nRow]->m_dblLLimit;
		}
		else if (index.column() == Threshold)
		{
			return arr[nRow]->m_dblThreshold;
		}



		return QVariant();
	}

	return QVariant();
}

QVariant CForwardingAIModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags CForwardingAIModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

bool CForwardingAIModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{	
	if (!index.isValid())
	{
		return false;
	}
	
	int nRow = index.row();
	
	auto &arr = m_pChannel->m_arrAIs;

	if (role == Qt::EditRole)
	{
		if (index.column() == CForwardingAIModel::TagName)
		{
			if (arr[nRow]->m_szTagName != value.toString())
			{
				if (!CheckTagNameIsValid(value.toString(), FES_DESC))
				{
					auto strError = tr("Forwarding ai tagname %1 is invalid!!!").arg(value.toString());
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
				}

				if (m_pFes->CheckTagNameIsExist(value.toString()))
				{
					Q_ASSERT(m_pCore);
					if (!m_pCore)
					{
						return false;
					}

					auto strError = tr("Forwarding ai tagname %1 is exist!!!").arg(value.toString());
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return false;
				}
				auto strLastValue = QString("%1").arg(arr[nRow]->m_szTagName);
				auto bFlag = m_pFes->ModifyFesHashMap(FORWARDINGAIHASHID, value.toString(), strLastValue);
				if (!bFlag)
				{
					return false;
				}
				memset(arr[nRow]->m_szTagName, 0, sizeof(arr[nRow]->m_szTagName));
				strncpy(arr[nRow]->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, value.toString().size()));
			}
		}
		else if (index.column() == CForwardingAIModel::ID)
		{
			if (arr[nRow]->m_nID != value.toInt())
			{
				arr[nRow]->m_nID = value.toInt();
			}
		}
		else if (index.column() == Desc)
		{
			if (arr[nRow]->m_strDescription != value.toString())
			{
				if (!CheckDescIsValid(value.toString(), FES_DESC))
				{
					return false;
				}
				
				arr[nRow]->m_strDescription = value.toString();
			}
		}
		else if (index.column() == GroupNo)
		{
			if (arr[nRow]->m_nGroupNo != value.toInt())
			{
				arr[nRow]->m_nGroupNo = value.toInt();
			}
		}
		else if (index.column() == EntryNo)
		{
			if (arr[nRow]->m_nEntryNo != value.toInt())
			{
				arr[nRow]->m_nEntryNo = value.toInt();
			}
		}
		else if (index.column() == Address)
		{
			if (arr[nRow]->m_strFUN != value.toString())
			{
				if (!CheckDescIsValid(value.toString(), FES_DESC, MAX_ADDRESS_LENGTH_SCADASTUDIO))
				{
					return false;
				}
				
				arr[nRow]->m_strAddress = value.toString();
			}
		}
		else if (index.column() == FUN)
		{
			if (arr[nRow]->m_strFUN != value.toString())
			{
				if (!CheckDescIsValid(value.toString(), FES_DESC))
				{
					return false;
				}
				
				arr[nRow]->m_strFUN = value.toString();
			}
		}
		else if (index.column() == INF)
		{
			if (arr[nRow]->m_strInfo != value.toString())
			{
				if (!CheckDescIsValid(value.toString(), FES_DESC))
				{
					return false;
				}
				
				arr[nRow]->m_strInfo = value.toString();
			}
		}
		else if (index.column() == MaxRaw)
		{
			if (arr[nRow]->m_dblMaxRaw != value.toDouble())
			{
				arr[nRow]->m_dblMaxRaw = value.toDouble();
			}
		}
		else if (index.column() == MinRaw)
		{
			if (arr[nRow]->m_dblMinRaw != value.toDouble())
			{
				arr[nRow]->m_dblMinRaw = value.toDouble();
			}
		}
		else if (index.column() == MaxConvert)
		{
			if (arr[nRow]->m_dblMaxConvert != value.toDouble())
			{
				arr[nRow]->m_dblMaxConvert = value.toDouble();
			}
		}
		else if (index.column() == MinConvert)
		{
			if (arr[nRow]->m_dblMinConvert != value.toDouble())
			{
				arr[nRow]->m_dblMinConvert = value.toDouble();
			}
		}
		else if (index.column() == ScaleTagName)
		{
			if (arr[nRow]->m_strScaleTagName != value.toString())
			{
				arr[nRow]->m_strScaleTagName = value.toString();
			}
		}
		else if (index.column() == HLimit)
		{
			if (arr[nRow]->m_dblHLimit != value.toDouble())
			{
				arr[nRow]->m_strScaleTagName = value.toDouble();
			}
		}
		else if (index.column() == LLimit)
		{
			if (arr[nRow]->m_dblLLimit != value.toDouble())
			{
				arr[nRow]->m_dblLLimit = value.toDouble();
			}
		}
		else if (index.column() == Threshold)
		{
			if (arr[nRow]->m_dblThreshold != value.toDouble())
			{
				arr[nRow]->m_dblThreshold = value.toDouble();
			}
		}
	}
	
	return false;
}

//void CAIModel::sort(int column, Qt::SortOrder order/* = Qt::AscendingOrder */)
//{
//
//}

/*! \fn bool CAIModel::InsertRows(int position, int rows, const QModelIndex &parent)
********************************************************************************************************* 
** \brief CAIModel::InsertRows 
** \details 插入数据
** \param position 
** \param rows 
** \param parent 
** \return bool 
** \author gw
** \date 2016年8月11日 
** \note 
********************************************************************************************************/
bool CForwardingAIModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	//auto nCount = m_arrAIs.size();

	beginInsertRows(parent, position, position + rows - 1);

	Q_ASSERT(m_pFes);
	if(!m_pFes)
	{
		return false;
	}

	for (int i = position; i < position + rows; i++)
	{
		
		auto pAnalogPoint = m_pFes->AddNewForwardingAIItem();

		pAnalogPoint->SetOccNo(i + 1);
		pAnalogPoint->m_nID = i + 1;

		m_pChannel->m_arrAIs.push_back(pAnalogPoint);
	}

	endInsertRows();

	return true;
}

bool CForwardingAIModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, rows, rows);

//	auto nCount = m_pChannel->m_arrAIs.size();

	auto pAnalog = m_pChannel->m_arrAIs[position];

	auto it = std::find(m_pChannel->m_arrAIs.begin(), m_pChannel->m_arrAIs.end(), pAnalog);
	if (it != m_pChannel->m_arrAIs.end())
	{
		m_pChannel->m_arrAIs.erase(it);
	}

	//note 删除关联关系
	QString strTmp = pAnalog->m_szTagName;
	if (!m_pFes->DeleteFesHashItem(FORWARDINGAIHASHID, strTmp))
	{
		return false;
	}


	if (!m_pFes->DeleteFesForwardingAICombineRelaton(strTmp.toStdString(), pAnalog))
	{
		auto strError = QObject::tr("Delete Fes Forwarding AI %1 Combine Relaton failed!!!").arg(strTmp);
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	////转发通道AI删除，如果转发AI有关联sourcetagname hash，那么关联hash也删除绑定关系
	//if (!pAnalog->m_strSourceTagName.isEmpty())
	//{
	//	if (!m_pFes->DeleteForwardingAIRelationSourceTagNameArr(pAnalog->m_strSourceTagName.toStdString(), pAnalog))
	//	{
	//		auto strError = QObject::tr("Delete Forwarding AI Relation SourceTagName Arr %1 delete failed!!!").arg(pAnalog->m_strSourceTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	////转发通道AI删除，如果转发AI有关联Scale hash，那么关联hash也删除绑定关系
	//if (!pAnalog->m_strScaleTagName.isEmpty())
	//{
	//	if (!m_pFes->DeleteForwardingAIRelationScaleArr(pAnalog->m_strScaleTagName.toStdString(), pAnalog))
	//	{
	//		auto strError = QObject::tr("Delete Forwarding AI Relation Scale Arr %1 delete failed!!!").arg(pAnalog->m_strScaleTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return false;
	//	}
	//}

	delete pAnalog;
	pAnalog = nullptr;

	endRemoveRows();

	if (m_pChannel->m_arrAIs.size() == 0)
	{
		RefrushModel();
	}
	
	return true;
}

void CForwardingAIModel::emitDataChanged(const QModelIndex &index)
{
	emit dataChanged(index, index);
}

/*! \fn void CAIModel::InitPara()
********************************************************************************************************* 
** \brief CAIModel::InitPara 
** \details 初始化参数
** \return void 
** \author gw
** \date 2016年5月17日 
** \note 
********************************************************************************************************/
void CForwardingAIModel::InitPara()
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
	((CForwadingAITable *)parent())->setItemDelegateForColumn(TagName, delegate);


	para.desc = tr("Desc");
	para.tagname = "Desc";
	m_arrInfo.insert(Desc, para);
	horizontal_header_list.append(para.desc);
	CLineEditDelegate *delegateDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(Desc, delegateDesc);

	para.desc = tr("SourceTagName");
	para.tagname = "SourceTagName";
	m_arrInfo.insert(SourceTagName, para);
	horizontal_header_list.append(para.desc);
	CReadOnlyDelegate *pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(SourceTagName, pReadOnlyDelegate);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_arrInfo.insert(OccNo, para);
	horizontal_header_list.append(para.desc);
	pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(OccNo, pReadOnlyDelegate);


	para.desc = tr("BlockOccNo");
	para.tagname = "BlockOccNo";
	m_arrInfo.insert(BlockOccNo, para);
	horizontal_header_list.append(para.desc);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(BlockOccNo, pReadOnlyDelegate);

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
	((CForwadingAITable *)parent())->setItemDelegateForColumn(Address, delegateAddress);


	para.desc = tr("FUN");
	para.tagname = "FUN";
	m_arrInfo.insert(FUN, para);
	horizontal_header_list.append(para.desc);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(FUN, delegateDesc);

	para.desc = tr("INF");
	para.tagname = "INF";
	m_arrInfo.insert(INF, para);
	horizontal_header_list.append(para.desc);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(INF, delegateDesc);

	para.desc = tr("MaxRaw");
	para.tagname = "MaxRaw";
	m_arrInfo.insert(MaxRaw, para);
	horizontal_header_list.append(para.desc);
	auto delegateDouble = new CLineEditDelegate(this, DOUBLETYPE, THREE_COUNT);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(MaxRaw, delegateDouble);

	para.desc = tr("MinRaw");
	para.tagname = "MinRaw";
	m_arrInfo.insert(MinRaw, para);
	horizontal_header_list.append(para.desc);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(MinRaw, delegateDouble);

	para.desc = tr("MaxConvert");
	para.tagname = "MaxConvert";
	m_arrInfo.insert(MaxConvert, para);
	horizontal_header_list.append(para.desc);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(MaxConvert, delegateDouble);

	para.desc = tr("MinConvert");
	para.tagname = "MinConvert";
	m_arrInfo.insert(MinConvert, para);
	horizontal_header_list.append(para.desc);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(MinConvert, delegateDouble);

	para.desc = tr("ScaleTagName");
	para.tagname = "ScaleTagName";
	m_arrInfo.insert(ScaleTagName, para);
	horizontal_header_list.append(para.desc);
	CReadOnlyDelegate *pReadDelegate = new CReadOnlyDelegate(this);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(ScaleTagName, pReadDelegate);

	para.desc = tr("ScaleType");
	para.tagname = "ScaleType";
	m_arrInfo.insert(ScaleType, para);
	horizontal_header_list.append(para.desc);
	m_arrVecScaleType.clear();
	IDDESC tmp;
	tmp.desc = tr("linear");
	//tmp.ID = LINEAR;
	m_arrVecScaleType.insert(LINEAR, tmp.desc);
	tmp.desc = tr("nonlinear");
	//tmp.ID = NONLINEAR;
	m_arrVecScaleType.insert(NONLINEAR, tmp.desc);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(ScaleType, pReadDelegate);

	para.desc = tr("HLimit");
	para.tagname = "HLimit";
	m_arrInfo.insert(HLimit, para);
	horizontal_header_list.append(para.desc);
	delegate = new CLineEditDelegate(this, DOUBLETYPE, THREE_COUNT);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(HLimit, delegate);

	para.desc = tr("LLimit");
	para.tagname = "LLimit";
	m_arrInfo.insert(LLimit, para);
	horizontal_header_list.append(para.desc);
	delegate = new CLineEditDelegate(this, DOUBLETYPE, THREE_COUNT);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(LLimit, delegate);

	para.desc = tr("Threshold");
	para.tagname = "Threshold";
	m_arrInfo.insert(Threshold, para);
	horizontal_header_list.append(para.desc);
	((CForwadingAITable *)parent())->setItemDelegateForColumn(Threshold, delegate);


	SetHorizontalHeaderList(horizontal_header_list);
}

QMap<int32s, CONFIG_PARA> & CForwardingAIModel::GetColumnInfo()
{
	return m_arrInfo;
}

std::vector <Config::CForwardingAnalogInputData *> & CForwardingAIModel::GetAIs()
{
	return m_pChannel->m_arrAIs;
}

void CForwardingAIModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}