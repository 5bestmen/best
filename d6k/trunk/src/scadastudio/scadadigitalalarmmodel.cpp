#include "scadadigitalalarmmodel.h"
#include "scadavirable_data.h"
#include "scadastudio/icore.h"
#include "delegate.h"
#include "scadadigitalalarmtable.h"
#include "global.h"

#include <QStandardItem>

CScadaDigitalAlarmModel::CScadaDigitalAlarmModel(QObject *parent, IMainModuleInterface *pCore,
	Config::CScadaDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada)
	: QAbstractTableModel(parent), m_pAlarmData(nullptr), m_pConfig(nullptr), m_pCore(nullptr), m_pScada(nullptr)
{
	Q_ASSERT(pConfig);
	if (!pConfig)
	{
		return;
	}
	m_pConfig = pConfig;

	Q_ASSERT(pAlarmData);
	if (!pAlarmData)
	{
		return;
	}
	m_pAlarmData = pAlarmData;

	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}
	m_pItem = pItem;

	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}
	m_pCore = pCore;

	Q_ASSERT(pScada);
	if (!pScada)
	{
		return;
	}
	m_pScada = pScada;

	InitPara();
}

CScadaDigitalAlarmModel::~CScadaDigitalAlarmModel()
{

}

int CScadaDigitalAlarmModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return 1;
}

int CScadaDigitalAlarmModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_horizontal_header_list.size() > 0)
		return m_horizontal_header_list.size();

	return 0;
}

QVariant CScadaDigitalAlarmModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	//int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);
		if (index.column() == ID)
		{
			return (uint)m_pAlarmData->m_nID;
		}
		else if (index.column() == TagName)
		{
			return m_pAlarmData->m_szTagName;
		}
		else if (index.column() == Description)
		{
			return m_pAlarmData->m_strDescription;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pAlarmData->GetOccNo();
		}
		else if (index.column() == Enable)
		{
			return m_pAlarmData->m_bEnable;
		}
		else if (index.column() == OnQualityGood)
		{
			return m_pAlarmData->m_bOnQualityGood;
		}

		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		//Q_ASSERT(m_pChannel);
		if (index.column() == ID)
		{
			return (uint)m_pAlarmData->m_nID;
		}
		else if (index.column() == TagName)
		{
			return m_pAlarmData->m_szTagName;
		}
		else if (index.column() == Description)
		{
			return m_pAlarmData->m_strDescription;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pAlarmData->GetOccNo();
		}
		else if (index.column() == Enable)
		{
			return m_pAlarmData->m_bEnable;
		}
		else if (index.column() == OnQualityGood)
		{
			return m_pAlarmData->m_bOnQualityGood;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CScadaDigitalAlarmModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (m_horizontal_header_list.size() > section && m_horizontal_header_list.size() > -1)
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

bool CScadaDigitalAlarmModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	//int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			if (index.column() == ID)
			{
				if (m_pAlarmData->m_nID != value.toInt())
				{
					m_pAlarmData->m_nID = value.toInt();
				}
			}
			else if (index.column() == TagName)
			{
				if (m_pAlarmData->m_szTagName != value.toString())
				{
					if (!CheckTagNameIsValid(value.toString(), SCADAVARIABLE_DESC))
					{
						return false;
					}
					
					if (m_pScada->CheckTagNameIsExist(value.toString()))
					{
						auto strError = tr("digital alarm tagname %1 exists!!!").arg(value.toString());
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return true;
					}

					//memset(m_pAlarmData->m_szTagName, 0, sizeof(m_pAlarmData->m_szTagName));
					//int nSize = value.toString().size();
					//strncpy(m_pAlarmData->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));

					m_pItem->setData(value, Qt::EditRole);
				}
			}
			else if (index.column() == Description)
			{
				if (m_pAlarmData->m_strDescription != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), SCADAVARIABLE_DESC))
					{
						return false;
					}
					
					m_pAlarmData->m_strDescription = value.toString();

					//m_pConfig->ModifyDIsRelationAlarmDescArr(std::string(m_pAlarmData->m_szTagName), value.toString().toStdString());

					m_pItem->setData(value, TREE_TOOL_TIP_ROLE);
				}
			}
			else if (index.column() == Enable)
			{
				if (m_pAlarmData->m_bEnable != value.toBool())
				{
					m_pAlarmData->m_bEnable = value.toBool();
				}
			}
			else if (index.column() == OnQualityGood)
			{
				if (m_pAlarmData->m_bOnQualityGood != value.toBool())
				{
					m_pAlarmData->m_bOnQualityGood = value.toBool();
				}
			}


			emit dataChanged(index, index);

			return true;
		}

		return false;
	}

	return false;
}

Qt::ItemFlags CScadaDigitalAlarmModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CScadaDigitalAlarmModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

void CScadaDigitalAlarmModel::InitPara()
{
	CONFIG_PARA para;

	para.desc = tr("ID");
	para.tagname = "ID";
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	//m_arrInfo.insert(TagName, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEdit = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CScadaDigitalAlarmTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("Description");
	para.tagname = "Description";
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEditDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CScadaDigitalAlarmTable *)parent())->setItemDelegateForColumn(Description, pLineEditDesc);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_horizontal_header_list.append(para.desc);
	auto pReadOnly = new CReadOnlyDelegate(this);
	((CScadaDigitalAlarmTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnly);

	para.desc = tr("Enable");
	para.tagname = "Enable";
	m_horizontal_header_list.append(para.desc);
	auto pCheck = new CCheckBoxDelegate(this);
	((CScadaDigitalAlarmTable *)parent())->setItemDelegateForColumn(Enable, pCheck);

	para.desc = tr("OnQualityGood");
	para.tagname = "OnQualityGood";
	m_horizontal_header_list.append(para.desc);
	((CScadaDigitalAlarmTable *)parent())->setItemDelegateForColumn(OnQualityGood, pCheck);
}