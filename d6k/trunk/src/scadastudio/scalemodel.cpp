#include "scalemodel.h"
#include "scadastudio/quoteItem.h"
#include "fesmodule.h"
#include "delegate.h"
#include "scaletable.h"
#include "config_data.h"
#include "scadastudio/define.h"
#include "scadastudio/icore.h"

extern IMainModuleInterface *s_pGlobleCore;
/*! \fn CScaleModel::CScaleModel(QObject *parent, Config::CFesData *pFesData, CQuoteItem *pItem)
********************************************************************************************************* 
** \brief CScaleModel::CScaleModel 
** \details 
** \param parent 
** \param pFesData 
** \param pItem 
** \return  
** \author gw
** \date 2016年4月18日 
** \note 
********************************************************************************************************/
CScaleModel::CScaleModel(QObject *parent, Config::CTransform *pTransform, CQuoteItem *pItem
	, Config::CFesData *pFesConfig)
	: QAbstractTableModel(parent), m_pTransform(NULL), m_pScaleLiear(NULL), m_nType(LINEAR), m_pNonScaleLiear(NULL),
	m_pItem(nullptr), m_pFesConfig(nullptr)
{
	Q_ASSERT(pTransform);
	
	if (!pTransform)
	{
		return;
	}

	m_pTransform = pTransform;

	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}
	m_pItem = pItem;

	Q_ASSERT(pFesConfig);
	if (!pFesConfig)
	{
		return;
	}
	m_pFesConfig = pFesConfig;

	int nType = pItem->data(Qt::UserRole).toInt();

	//QString strTagName = pItem->data(TAGNAME).toString();

	//m_pTransform = CFesModule::GetFetTransform(pFesData, strTagName);


	if (nType == FES_TYPE_SCALE_LINEAR_CHILD_ITEM)
	{

		m_nType = LINEAR;

		{
			m_pScaleLiear = m_pTransform->GetLinear();

			m_pTransform->m_nType = LINEAR;

			m_pNonScaleLiear = m_pTransform->GetNonLinear();

			m_pTransform->CopyLinearToNon();
			//m_pTransform->SetLinear(pTmp);
		}

	}
	else
	{
		m_nType = NONLINEAR;

		{
			m_pNonScaleLiear = m_pTransform->GetNonLinear();

			m_pTransform->m_nType = NONLINEAR;

			m_pTransform->CopyNonToLinear();
			
			m_pScaleLiear = m_pTransform->GetLinear();
			//m_pTransform->SetNonLinear(m_pNonScaleLiear);
		}
	}

	InitPara();
}

CScaleModel::~CScaleModel()
{
	//delete m_pTransform;
	//m_pTransform = NULL;
}

int CScaleModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return /*m_pFesData->m_arrNonLinears.size()*/ 1;
}

int CScaleModel::columnCount(const QModelIndex &parent /* = QModelIndex() */) const
{
	Q_UNUSED(parent);
	
	return horizontal_header_list.count();
}

QVariant CScaleModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (m_nType == LINEAR)
	{
		if (role == Qt::DisplayRole)
		{
			if (index.column() == OccNo)
			{
				return (uint)m_pTransform->GetOccNo();
			}
			else if (index.column() == TagName)
			{
				return m_pScaleLiear->m_szTagName;
			}
			else if (index.column() == ID)
			{
				return m_pScaleLiear->m_nID;
			}
			else if (index.column() == ScaleName)
			{
				return m_pScaleLiear->m_strDescription;
			}
			else if (index.column() == Extention)
			{
				return m_pScaleLiear->m_strExtention;
			}
			//else if (index.column() == GroupName)
			//{
			//	return m_pScaleLiear->m_strGroupName;
			//}
			else if (index.column() == ScaleConfig)
			{
				return m_arrVecType[m_nType];
			}
		}
		else if (role == Qt::EditRole)
		{
			if (index.column() == OccNo)
			{
				return (uint)m_pTransform->GetOccNo();
			}
			else if (index.column() == TagName)
			{
				return m_pScaleLiear->m_szTagName;
			}
			else if (index.column() == ID)
			{
				return m_pScaleLiear->m_nID;
			}
			else if (index.column() == ScaleName)
			{
				return m_pScaleLiear->m_strDescription;
			}
			else if (index.column() == Extention)
			{
				return m_pScaleLiear->m_strExtention;
			}
			//else if (index.column() == GroupName)
			//{
			//	return m_pScaleLiear->m_strGroupName;
			//}
			else if (index.column() == ScaleConfig)
			{
				return m_arrVecType[m_nType];
			}
		}
	}
	else
	{
		if (role == Qt::DisplayRole)
		{
			if (index.column() == OccNo)
			{
				return (uint)m_pNonScaleLiear->GetOccNo();
			}
			else if (index.column() == TagName)
			{
				return m_pNonScaleLiear->m_szTagName;
			}
			else if (index.column() == ID)
			{
				return m_pNonScaleLiear->m_nID;
			}
			else if (index.column() == ScaleName)
			{
				return m_pNonScaleLiear->m_strDescription;
			}
			else if (index.column() == Extention)
			{
				return m_pNonScaleLiear->m_strExtention;
			}
			else if (index.column() == ScaleConfig)
			{
				return m_arrVecType[m_nType];
			}
		}
		else if (role == Qt::EditRole)
		{
			if (index.column() == OccNo)
			{
				return (uint)m_pNonScaleLiear->GetOccNo();
			}
			else if (index.column() == TagName)
			{
				return m_pNonScaleLiear->m_szTagName;
			}
			else if (index.column() == ID)
			{
				return m_pNonScaleLiear->m_nID;
			}
			else if (index.column() == ScaleName)
			{
				return m_pNonScaleLiear->m_strDescription;
			}
			else if (index.column() == Extention)
			{
				return m_pNonScaleLiear->m_strExtention;
			}
			else if (index.column() == ScaleConfig)
			{
				return m_arrVecType[m_nType];
			}
		}
	}

	return QVariant();
}

QVariant CScaleModel::headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (horizontal_header_list.size() > section)
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

Qt::ItemFlags CScaleModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

bool CScaleModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
	QFont font = value.value<QFont>();

	//QSet<int32s> set;

	//set.insert(index.row());

	if (m_nType == LINEAR)
	{
		if (index.isValid() && role == Qt::EditRole)
		{
			if (index.column() == TagName)
			{
				if (value.toString().compare(m_pScaleLiear->m_szTagName) != 0)
				{
					if (m_pFesConfig->CheckTagNameIsExist(value.toString()))
					{
						auto strError = tr("scale tag name %1 exists!!!").arg(value.toString());
						s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
						
						return true;
					}
					
					memset(m_pScaleLiear->m_szTagName, 0, sizeof(m_pScaleLiear->m_szTagName));
					int nSize = value.toString().size();
					strncpy(m_pScaleLiear->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));

					//m_pFesData->SetModifyFlag(true);

					//TagName
					m_pItem->setData(value, Qt::EditRole);

					//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
				}
			}
			else if (index.column() == ID)
			{
				if (m_pScaleLiear->m_nID != value.toInt())
				{
					m_pScaleLiear->m_nID = value.toInt();

					//m_pFesData->SetModifyFlag(true);
					//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
				}
			}
			else if (index.column() == ScaleName)
			{
				if (m_pScaleLiear->m_strDescription.compare(value.toString()) != 0)
				{
					m_pScaleLiear->m_strDescription = value.toString();

					//m_pFesData->SetModifyFlag(true);

					//修改关联的描述
					m_pFesConfig->ModifyAIsRelationScaleDescArr(std::string(m_pScaleLiear->m_szTagName), value.toString().toStdString());
					m_pFesConfig->ModifyAOsRelationScaleDescArr(std::string(m_pScaleLiear->m_szTagName), value.toString().toStdString());

					//转换名
					m_pItem->setData(m_pScaleLiear->m_strDescription, TREE_TOOL_TIP_ROLE);

					//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
				}
			}
			else if (index.column() == Extention)
			{
				if (m_pScaleLiear->m_strExtention.compare(value.toString()) != 0)
				{
					m_pScaleLiear->m_strExtention = value.toString();

					//m_pFesData->SetModifyFlag(true);

					//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
				}
			}
			else if (index.column() == ScaleConfig)
			{
				m_pScaleLiear->m_nType = value.toInt();

				//m_pFesData->SetModifyFlag(true);
			}

			return true;
		}
		else if (index.isValid() && role == Qt::UserRole)
		{

		}
		else if (index.isValid() && role == TREE_TOOL_TIP_ROLE)
		{

		}
	}
	else
	{
		if (index.isValid() && role == Qt::EditRole)
		{
			if (index.column() == TagName)
			{
				if (value.toString().compare(m_pNonScaleLiear->m_szTagName) != 0)
				{
					if (m_pFesConfig->CheckTagNameIsExist(value.toString()))
					{
						auto strError = tr("scale tag name %1 exists!!!").arg(value.toString());
						s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return true;
					}
					
					//memset(m_pNonScaleLiear->m_szTagName, 0, sizeof(m_pNonScaleLiear->m_szTagName));
					//int nSize = value.toString().size();
					//strncpy(m_pNonScaleLiear->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));

					//m_pFesData->SetModifyFlag(true);

					//TagName
					m_pItem->setData(value, Qt::EditRole);

					//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
				}
			}
			else if (index.column() == ID)
			{
				if (m_pNonScaleLiear->m_nID != value.toInt())
				{
					m_pNonScaleLiear->m_nID = value.toInt();

					//m_pFesData->SetModifyFlag(true);;
					//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
				}
			}
			else if (index.column() == ScaleName)
			{
				if (m_pNonScaleLiear->m_strDescription.compare(value.toString()) != 0)
				{
					m_pNonScaleLiear->m_strDescription = value.toString();

					m_pFesConfig->ModifyAIsRelationScaleDescArr(std::string(m_pNonScaleLiear->m_szTagName), value.toString().toStdString());
					m_pFesConfig->ModifyAOsRelationScaleDescArr(std::string(m_pNonScaleLiear->m_szTagName), value.toString().toStdString());

					m_pItem->setData(value, TREE_TOOL_TIP_ROLE);

					//m_pFesData->SetModifyFlag(true);

					//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
				}
			}
			else if (index.column() == Extention)
			{
				if (m_pNonScaleLiear->m_strExtention.compare(value.toString()) != 0)
				{
					m_pNonScaleLiear->m_strExtention = value.toString();

					//m_pFesData->SetModifyFlag(true);

					//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
				}
			}
			else if (index.column() == ScaleConfig)
			{
				m_pNonScaleLiear->m_nType = value.toInt();

				//m_pFesData->SetModifyFlag(true);
			}

			return true;
		}
		else if (index.isValid() && role == Qt::UserRole)
		{

		}
		else if (index.isValid() && role == TREE_TOOL_TIP_ROLE)
		{

		}
	}

	return false;
}

void CScaleModel::sort(int column, Qt::SortOrder order /* = Qt::AscendingOrder */)
{
	Q_UNUSED(column);
	Q_UNUSED(order);
	
	//g_column = column;

	//g_sortOrder = order;

	//if (arr_row_list.count() == 0)
	//{
	//	return;
	//}

	//qSort(arr_row_list.begin(), arr_row_list.end(), lessThan);

	//for (int i = 0; i < arr_row_list.count(); i++)
	//{
	//	QString strTmp;

	//	strTmp = arr_row_list[i][0];

	//	strTmp = "test";
	//}

	//RefrushModel();
}


/*! \fn void CScaleModel::InitPara()
********************************************************************************************************* 
** \brief CScaleModel::InitPara 
** \details 
** \return void 
** \author gw
** \date 2016年4月18日 
** \note 
********************************************************************************************************/
void CScaleModel::InitPara()
{
	m_arrInfo.clear();
	
	CONFIG_PARA para;

	para.desc = tr("ID");
	para.tagname = "ID";
	m_arrInfo.insert(ID, para);
	horizontal_header_list.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	m_arrInfo.insert(1, para);
	horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEdit = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CScaleTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("ScaleName");
	para.tagname = "ScaleName";
	m_arrInfo.insert(ScaleName, para);
	horizontal_header_list.append(para.desc);
	pLineEdit = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CScaleTable *)parent())->setItemDelegateForColumn(ScaleName, pLineEdit);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_arrInfo.insert(OccNo, para);
	horizontal_header_list.append(para.desc);
	auto pDelegateRead = new CReadOnlyDelegate(this);
	((CScaleTable *)parent())->setItemDelegateForColumn(OccNo, pDelegateRead);

	para.desc = tr("Extention");
	para.tagname = "Extention";
	m_arrInfo.insert(Extention, para);
	horizontal_header_list.append(para.desc);
	((CScaleTable *)parent())->setItemDelegateForColumn(Extention, pLineEdit);

	m_arrVecType.clear();

	para.desc = tr("ScaleConfig");
	para.tagname = "ScaleConfig";
	m_arrInfo.insert(ScaleConfig, para);
	horizontal_header_list.append(para.desc);
	
	IDDESC tmp;
	tmp.desc = tr("LINEAR CONFIG");
	tmp.ID = QString("%1").arg(LINEAR);
	m_arrVecType.insert(LINEAR, tmp.desc);

	tmp.desc = tr("NONLINEAR CONFIG");
	tmp.ID = QString("%1").arg(NONLINEAR);
	m_arrVecType.insert(NONLINEAR, tmp.desc);
	CReadOnlyDelegate *pReadLine = new CReadOnlyDelegate(this);
	((CScaleTable *)parent())->setItemDelegateForColumn(ScaleConfig, pReadLine);

	//para.desc = tr("GroupName");
	//para.tagname = "GroupName";
	//m_arrInfo.insert(GroupName, para);
	//horizontal_header_list.append(para.desc);
}

/*! \fn void CScaleModel::GetLinear(Config::CLinearTransform *pLinear
********************************************************************************************************* 
** \brief CScaleModel::GetLinear 
** \details 线性转换信息
** \param pLinear 
** \return void 
** \author gw
** \date 2016年4月18日 
** \note 
********************************************************************************************************/
Config::CLinearTransform *CScaleModel::GetLinear()
{
	Config::CLinearTransform *pTmp = NULL;
	
	pTmp = m_pTransform->GetLinear();
	Q_ASSERT(pTmp);

	return pTmp;
}

Config::CNonlinearTransform *CScaleModel::GetNonLinear()
{
	Q_ASSERT(m_pTransform->GetNonLinear());

	return m_pTransform->GetNonLinear();
}

/*! \fn void CScaleModel::SetLinear(Config::CLinearTransform *pLinear)
********************************************************************************************************* 
** \brief CScaleModel::SetLinear 
** \details 设置linear
** \param pLinear 
** \return void 
** \author gw
** \date 2016年4月19日 
** \note 
********************************************************************************************************/
void CScaleModel::SetLinear(Config::CLinearTransform *pLinear)
{
	m_pTransform->SetLinear(pLinear);
}

/*! \fn void CScaleModel::RefrushModel()
********************************************************************************************************* 
** \brief CScaleModel::RefrushModel 
** \details 刷新model
** \return void 
** \author gw
** \date 2016年8月3日 
** \note 
********************************************************************************************************/
void CScaleModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}