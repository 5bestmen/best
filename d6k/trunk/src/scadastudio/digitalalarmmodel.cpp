#include "digitalalarmmodel.h"
#include "scadastudio/icore.h"
#include "alarm.h"
#include "config_data.h"
#include "delegate.h"
#include "digitalalarmtable.h"
#include "scadastudiodefine.h"

#include <QStandardItem>

CDigitalAlarmModel::CDigitalAlarmModel(QObject *parent, IMainModuleInterface *pCore, CAlarm *pModule,
	Config::CDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes)
	: QAbstractTableModel(parent), m_pAlarmData(nullptr), m_pItem(nullptr), m_pFes(nullptr)
{
	Q_ASSERT(pCore);
	Q_ASSERT(pModule);
	Q_ASSERT(pAlarmData);
	Q_ASSERT(pItem);
	Q_ASSERT(pFes);

	if (!pAlarmData)
	{
		return;
	}

	if (!pAlarmData)
	{
		return;
	}

	if (!pCore)
	{
		return;
	}

	if (!pItem)
	{
		return;
	}

	m_pAlarmData = pAlarmData;

	m_pItem = pItem;

	m_pFes = pFes;

	m_pCore = pCore;

	InitPara();
}

CDigitalAlarmModel::~CDigitalAlarmModel()
{

}

int CDigitalAlarmModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return 1;
}

int CDigitalAlarmModel::columnCount(const QModelIndex &parent) const
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

QVariant CDigitalAlarmModel::data(const QModelIndex &index, int role) const
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

QVariant CDigitalAlarmModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool CDigitalAlarmModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
					if (m_pFes->CheckTagNameIsExist(value.toString()))
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
					m_pAlarmData->m_strDescription = value.toString();

					m_pFes->ModifyDIsRelationAlarmDescArr(std::string(m_pAlarmData->m_szTagName), value.toString().toStdString());

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

Qt::ItemFlags CDigitalAlarmModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CDigitalAlarmModel::InitPara()
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
	((CDigitalAlarmTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("Description");
	para.tagname = "Description";
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEditDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CDigitalAlarmTable *)parent())->setItemDelegateForColumn(Description, pLineEditDesc);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_horizontal_header_list.append(para.desc);
	auto pReadOnly = new CReadOnlyDelegate(this);
	((CDigitalAlarmTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnly);

	para.desc = tr("Enable");
	para.tagname = "Enable";
	m_horizontal_header_list.append(para.desc);
	auto pCheck = new CCheckBoxDelegate(this);
	((CDigitalAlarmTable *)parent())->setItemDelegateForColumn(Enable, pCheck);

	para.desc = tr("OnQualityGood");
	para.tagname = "OnQualityGood";
	m_horizontal_header_list.append(para.desc);
	((CDigitalAlarmTable *)parent())->setItemDelegateForColumn(OnQualityGood, pCheck);
}

void CDigitalAlarmModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}