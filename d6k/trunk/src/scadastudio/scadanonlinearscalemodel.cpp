#include "scadanonlinearscalemodel.h"
#include "scadavirable_data.h"
#include "delegate.h"
#include "scadanonlinearscaletalbe.h"
#include "scadastudio/quoteItem.h"
#include "global.h"

CScadaNonlinearScaleModel::CScadaNonlinearScaleModel(QObject *parent, Config::CScadaTransform *pTransform, CQuoteItem *pItem, CScadaVariableModule *pModule
	, Config::CScadaVariableConfigData *pConfig)
	: QAbstractTableModel(parent), m_pTransform(nullptr), m_pNonlinear(nullptr), m_strTypeDesc(""), m_pConfig(nullptr)
{
	Q_UNUSED(pModule);
	
	Q_ASSERT(pTransform);
	if (!pTransform)
	{
		return;
	}
	m_pTransform = pTransform;
	auto pNonlinear = m_pTransform->GetNonLinear();
	m_pNonlinear = pNonlinear;

	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}
	m_pItem = pItem;

	Q_ASSERT(pConfig);
	if (!pConfig)
	{
		return;
	}
	m_pConfig = pConfig;

	InitPara();
}

CScadaNonlinearScaleModel::~CScadaNonlinearScaleModel()
{

}

/*! \fn int CScadaNonlinearScaleModel::rowCount(const QModelIndex &parent = QModelIndex()) const
********************************************************************************************************* 
** \brief CScadaNonlinearScaleModel::rowCount 
** \details 
** \param parent 
** \return int 
** \author gw
** \date 2017年1月21日 
** \note 
********************************************************************************************************/
int CScadaNonlinearScaleModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	return /*m_pFesData->m_arrNonLinears.size()*/ 1;
}

/*! \fn 
********************************************************************************************************* 
** \brief CScadaNonlinearScaleModel::columnCount 
** \details 
** \param parent 
** \return int 
** \author gw
** \date 2017年1月21日 
** \note 
********************************************************************************************************/
int CScadaNonlinearScaleModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	return m_horizontal_header_list.count();
}

QVariant CScadaNonlinearScaleModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role == Qt::DisplayRole)
	{
		if (index.column() == OccNo)
		{
			return (uint)m_pTransform->GetOccNo();
		}
		else if (index.column() == TagName)
		{
			return m_pNonlinear->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return m_pNonlinear->m_nID;
		}
		else if (index.column() == ScaleName)
		{
			return m_pNonlinear->m_strDescription;
		}
		else if (index.column() == Extention)
		{
			return m_pNonlinear->m_strExtention;
		}
		//else if (index.column() == GroupName)
		//{
		//	return m_pNonlinear->m_strGroupName;
		//}
		else if (index.column() == ScaleType)
		{
			return m_strTypeDesc;
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
			return m_pNonlinear->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return m_pNonlinear->m_nID;
		}
		else if (index.column() == ScaleName)
		{
			return m_pNonlinear->m_strDescription;
		}
		else if (index.column() == Extention)
		{
			return m_pNonlinear->m_strExtention;
		}
		//else if (index.column() == GroupName)
		//{
		//	return m_pNonlinear->m_strGroupName;
		//}
		else if (index.column() == ScaleType)
		{
			return m_strTypeDesc;
		}
	}

	return QVariant();
}

void CScadaNonlinearScaleModel::InitPara()
{
	CONFIG_PARA para;

	para.desc = tr("ID");
	para.tagname = "ID";

	m_horizontal_header_list.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEdit = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CScadaNonlinearScaleTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("ScaleName");
	para.tagname = "ScaleName";
	m_horizontal_header_list.append(para.desc);
	auto pLineEditDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CScadaNonlinearScaleTable *)parent())->setItemDelegateForColumn(ScaleName, pLineEditDesc);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_horizontal_header_list.append(para.desc);
	CReadOnlyDelegate *pReadLine = new CReadOnlyDelegate(this);
	((CScadaNonlinearScaleTable *)parent())->setItemDelegateForColumn(OccNo, pReadLine);

	para.desc = tr("Extention");
	para.tagname = "Extention";
	m_horizontal_header_list.append(para.desc);
	((CScadaNonlinearScaleTable *)parent())->setItemDelegateForColumn(Extention, pLineEditDesc);

	para.desc = tr("ScaleType");
	para.tagname = "ScaleType";
	m_horizontal_header_list.append(para.desc);
	//CReadOnlyDelegate *pReadLine = new CReadOnlyDelegate(this);
	((CScadaNonlinearScaleTable *)parent())->setItemDelegateForColumn(ScaleType, pReadLine);
	m_strTypeDesc = QObject::tr("NONLINEARCONFIG");

	//para.desc = tr("GroupName");
	//para.tagname = "GroupName";
	//m_horizontal_header_list.append(para.desc);
}

void CScadaNonlinearScaleModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

bool CScadaNonlinearScaleModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
	if (index.isValid() && role == Qt::EditRole)
	{
		if (index.column() == TagName)
		{
			if (value.toString().compare(m_pNonlinear->m_szTagName) != 0)
			{
				//if (m_pConfig->CheckTagNameIsExist(value.toString()))
				//{
				//	auto strError = tr("scale tag name %1 exists!!!").arg(value.toString());
				//	s_pGlobleCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				//	return true;
				//}

				if (!CheckTagNameIsValid(value.toString(), SCADAVARIABLE_DESC))
				{
					return false;
				}

				//m_pFesData->SetModifyFlag(true);

				//TagName
				m_pItem->setData(value, Qt::EditRole);

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == ID)
		{
			if (m_pNonlinear->m_nID != value.toInt())
			{
				m_pNonlinear->m_nID = value.toInt();

				//m_pFesData->SetModifyFlag(true);
				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == ScaleName)
		{
			if (m_pNonlinear->m_strDescription.compare(value.toString()) != 0)
			{
				if (!CheckTagNameIsValid(value.toString(), SCADAVARIABLE_DESC))
				{
					return false;
				}
				
				m_pNonlinear->m_strDescription = value.toString();

				//m_pFesData->SetModifyFlag(true);

				//m_pFesConfig->ModifyAIsRelationScaleDescArr(std::string(m_pScaleLiear->m_szTagName), value.toString().toStdString());

				//转换名
				m_pItem->setData(m_pNonlinear->m_strDescription, TREE_TOOL_TIP_ROLE);

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == Extention)
		{
			if (m_pNonlinear->m_strExtention.compare(value.toString()) != 0)
			{
				if (!CheckTagNameIsValid(value.toString(), SCADAVARIABLE_DESC))
				{
					return false;
				}
				
				m_pNonlinear->m_strExtention = value.toString();

				//m_pFesData->SetModifyFlag(true);

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		//else if (index.column() == GroupName)
		//{
		//	m_pNonlinear->m_strGroupName = value.toString();

		//	//m_pFesData->SetModifyFlag(true);
		//}
		else if (index.column() == ScaleType)
		{
			m_pNonlinear->m_nType = value.toInt();

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

	return false;
}

Qt::ItemFlags CScadaNonlinearScaleModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

QVariant CScadaNonlinearScaleModel::headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (m_horizontal_header_list.size() > section)
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

			return QVariant();
		}
	}

	return QVariant();
}