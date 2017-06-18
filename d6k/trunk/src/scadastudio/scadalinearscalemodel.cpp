#include "scadalinearscalemodel.h"
#include "scadavirable_data.h"
#include "delegate.h"
#include "scadalinearscaletable.h"
#include "scadastudio/quoteItem.h"
#include "global.h"

CScadaLinearScaleModel::CScadaLinearScaleModel(QObject *parent, Config::CScadaTransform *pTransform, CQuoteItem *pItem, CScadaVariableModule *pModule
	, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada)
	: QAbstractTableModel(parent), m_pTransform(nullptr), m_pItem(nullptr), m_pConfig(nullptr), m_pLinear(nullptr), m_pScada(nullptr)
{
	Q_UNUSED(pModule);
	
	m_horizontal_header_list.clear();

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

	Q_ASSERT(pConfig);
	if (!pConfig)
	{
		return;
	}
	m_pConfig = pConfig;

	m_pLinear = m_pTransform->GetLinear();
	Q_ASSERT(m_pLinear);
	if (!m_pLinear)
	{
		return;
	}

	Q_ASSERT(pScada);
	if (!pScada)
	{
		return;
	}
	m_pScada = pScada;

	InitPara();
}

CScadaLinearScaleModel::~CScadaLinearScaleModel()
{

}

int CScadaLinearScaleModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return /*m_pFesData->m_arrNonLinears.size()*/ 1;
}

int CScadaLinearScaleModel::columnCount(const QModelIndex &parent /* = QModelIndex() */) const
{
	Q_UNUSED(parent);
	
	return m_horizontal_header_list.count();
}

QVariant CScadaLinearScaleModel::data(const QModelIndex &index, int role) const
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
			return m_pLinear->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return m_pLinear->m_nID;
		}
		else if (index.column() == ScaleName)
		{
			return m_pLinear->m_strDescription;
		}
		else if (index.column() == Extention)
		{
			return m_pLinear->m_strExtention;
		}
		//else if (index.column() == GroupName)
		//{
		//	return m_pLinear->m_strGroupName;
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
			return m_pLinear->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return m_pLinear->m_nID;
		}
		else if (index.column() == ScaleName)
		{
			return m_pLinear->m_strDescription;
		}
		else if (index.column() == Extention)
		{
			return m_pLinear->m_strExtention;
		}
		//else if (index.column() == GroupName)
		//{
		//	return m_pLinear->m_strGroupName;
		//}
		else if (index.column() == ScaleType)
		{
			return m_strTypeDesc;
		}
	}

	return QVariant();
}

void CScadaLinearScaleModel::InitPara()
{
	CONFIG_PARA para;

	para.desc = tr("ID");
	para.tagname = "ID";

	m_horizontal_header_list.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEdit = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CScadaLinearScaleTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("ScaleName");
	para.tagname = "ScaleName";
	m_horizontal_header_list.append(para.desc);
	auto pLineEditDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CScadaLinearScaleTable *)parent())->setItemDelegateForColumn(ScaleName, pLineEdit);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_horizontal_header_list.append(para.desc);
	CReadOnlyDelegate *pReadLine = new CReadOnlyDelegate(this);
	((CScadaLinearScaleTable *)parent())->setItemDelegateForColumn(OccNo, pReadLine);

	para.desc = tr("Extention");
	para.tagname = "Extention";
	m_horizontal_header_list.append(para.desc);
	((CScadaLinearScaleTable *)parent())->setItemDelegateForColumn(Extention, pLineEditDesc);

	para.desc = tr("ScaleType");
	para.tagname = "ScaleType";
	m_horizontal_header_list.append(para.desc);
	//CReadOnlyDelegate *pReadLine = new CReadOnlyDelegate(this);
	((CScadaLinearScaleTable *)parent())->setItemDelegateForColumn(ScaleType, pReadLine);
	m_strTypeDesc = QObject::tr("LINEARCONFIG");

	//para.desc = tr("GroupName");
	//para.tagname = "GroupName";
	//m_horizontal_header_list.append(para.desc);
}

void CScadaLinearScaleModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

bool CScadaLinearScaleModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
	if (index.isValid() && role == Qt::EditRole)
	{
		if (index.column() == TagName)
		{
			if (value.toString().compare(m_pLinear->m_szTagName) != 0)
			{
				if (m_pScada->CheckTagNameIsExist(value.toString()))
				{
					return true;
				}

				if (!CheckTagNameIsValid(value.toString(), SCADAVARIABLE_DESC))
				{
					return false;
				}

				//memset(m_pLinear->m_szTagName, 0, sizeof(m_pLinear->m_szTagName));
				//int nSize = value.toString().size();
				//strncpy(m_pLinear->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));

				//m_pFesData->SetModifyFlag(true);

				//TagName
				m_pItem->setData(value, Qt::EditRole);

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == ID)
		{
			if (m_pLinear->m_nID != value.toInt())
			{
				m_pLinear->m_nID = value.toInt();

				//m_pFesData->SetModifyFlag(true);
				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == ScaleName)
		{
			if (m_pLinear->m_strDescription.compare(value.toString()) != 0)
			{
				if (!CheckDescIsValid(value.toString(), SCADAVARIABLE_DESC))
				{
					return false;
				}
				
				m_pLinear->m_strDescription = value.toString();

				//m_pFesData->SetModifyFlag(true);

				//m_pFesConfig->ModifyAIsRelationScaleDescArr(std::string(m_pScaleLiear->m_szTagName), value.toString().toStdString());

				//转换名
				m_pItem->setData(m_pLinear->m_strDescription, TREE_TOOL_TIP_ROLE);

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == Extention)
		{
			if (m_pLinear->m_strExtention.compare(value.toString()) != 0)
			{
				if (!CheckDescIsValid(value.toString(), SCADAVARIABLE_DESC))
				{
					return false;
				}
				
				m_pLinear->m_strExtention = value.toString();

				//m_pFesData->SetModifyFlag(true);

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		//else if (index.column() == GroupName)
		//{
		//	m_pLinear->m_strGroupName = value.toString();

		//	//m_pFesData->SetModifyFlag(true);
		//}
		else if (index.column() == ScaleType)
		{
			m_pLinear->m_nType = value.toInt();

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

Qt::ItemFlags CScadaLinearScaleModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

QVariant CScadaLinearScaleModel::headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const
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