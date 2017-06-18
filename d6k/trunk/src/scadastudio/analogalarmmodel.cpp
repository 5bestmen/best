#include "analogalarmmodel.h"
#include "scadastudio/icore.h"
#include "alarm.h"
#include "config_data.h"
#include "delegate.h"
#include "analogalarmtable.h"
#include "global.h"

#include <QStandardItem>

CAnalogAlarmModel::CAnalogAlarmModel(QObject *parent, IMainModuleInterface *pCore, CAlarm *pModule,
	Config::CAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes)
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

	if (!pItem)
	{
		return;
	}

	if (!pFes)
	{
		return;
	}

	m_pAlarmData = pAlarmData;

	m_pItem = pItem;

	m_pFes = pFes;

	InitPara();
}

CAnalogAlarmModel::~CAnalogAlarmModel()
{

}

int CAnalogAlarmModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return 1;
}

int CAnalogAlarmModel::columnCount(const QModelIndex &parent) const
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

QVariant CAnalogAlarmModel::data(const QModelIndex &index, int role) const
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
		else if (index.column() == Enable)
		{
			return m_pAlarmData->m_bEnable;
		}
		else if (index.column() == OnQualityGood)
		{
			return m_pAlarmData->m_bOnQualityGood;
		}
		else if (index.column() == DeadArea)
		{
			return QString::number(m_pAlarmData->m_dbDeadArea, 10, 6);
		}
		else if (index.column() == DeadType)
		{
			return m_pAlarmData->m_nDeadType;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pAlarmData->GetOccNo();
		}
		else if (index.column() == GroupName)
		{
			return m_pAlarmData->m_strGroupName;
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
		else if (index.column() == Enable)
		{
			return m_pAlarmData->m_bEnable;
		}
		else if (index.column() == OnQualityGood)
		{
			return m_pAlarmData->m_bOnQualityGood;
		}
		else if (index.column() == DeadArea)
		{
			return m_pAlarmData->m_dbDeadArea;
		}
		else if (index.column() == DeadType)
		{
			return m_pAlarmData->m_nDeadType;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pAlarmData->GetOccNo();
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CAnalogAlarmModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool CAnalogAlarmModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
					/*memset(m_pAlarmData->m_szTagName, 0, sizeof(m_pAlarmData->m_szTagName));
					int nSize = value.toString().size();
					strncpy(m_pAlarmData->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));*/
					auto strTagName = value.toString();
					if (!CheckTagNameIsValid(strTagName, FES_DESC))
					{
						return false;
					}

					if (m_pFes->CheckTagNameIsExist(value.toString()))
					{
						return true;
					}

					m_pItem->setData(value, Qt::EditRole);
				}
			}
			else if (index.column() == Description)
			{
				if (m_pAlarmData->m_strDescription.compare(value.toString()) != 0)
				{
					auto strDesc = value.toString();
					if (!CheckDescIsValid(strDesc, FES_DESC))
					{
						return false;
					}

					m_pAlarmData->m_strDescription = strDesc;

					////告警描述修改 模拟量关联的告警描述也修改
					//m_pFes->ModifyAIsRelationAlarmDescArr(std::string(m_pAlarmData->m_szTagName), m_pAlarmData->m_strDescription.toStdString());

					m_pItem->setData(m_pAlarmData->m_strDescription, TREE_TOOL_TIP_ROLE);

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
			else if (index.column() == DeadArea)
			{
				if (m_pAlarmData->m_dbDeadArea != value.toDouble())
				{
					m_pAlarmData->m_dbDeadArea = value.toDouble();
				}
			}
			else if (index.column() == DeadType)
			{
				if (m_pAlarmData->m_nDeadType != value.toInt())
				{
					m_pAlarmData->m_nDeadType = value.toInt();
				}
			}

			emit dataChanged(index, index);

			return true;
		}

		return false;
	}

	return false;
}

void CAnalogAlarmModel::InitPara()
{
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
	CLineEditDelegate *pLineEdit = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CAnalogAlarmTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("Description");
	para.tagname = "Description";
	m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineDescEdit = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CAnalogAlarmTable *)parent())->setItemDelegateForColumn(Description, pLineDescEdit);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_arrInfo.insert(OccNo, para);
	m_horizontal_header_list.append(para.desc);
	auto pReadOnly = new CReadOnlyDelegate(this);
	((CAnalogAlarmTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnly);


	para.desc = tr("Enable");
	para.tagname = "Enable";
	m_arrInfo.insert(Enable, para);
	m_horizontal_header_list.append(para.desc);
	CCheckBoxDelegate *pCheck = new CCheckBoxDelegate(this);
	((CAnalogAlarmTable *)parent())->setItemDelegateForColumn(Enable, pCheck);

	para.desc = tr("OnQualityGood");
	para.tagname = "OnQualityGood";
	m_arrInfo.insert(OnQualityGood, para);
	m_horizontal_header_list.append(para.desc);
	((CAnalogAlarmTable *)parent())->setItemDelegateForColumn(OnQualityGood, pCheck);

	para.desc = tr("DeadArea");
	para.tagname = "DeadArea";
	m_arrInfo.insert(DeadArea, para);
	m_horizontal_header_list.append(para.desc);
	auto delegate = new CLineEditDelegate(this, DOUBLETYPE, SIX_COUNT);
	((CAnalogAlarmTable *)parent())->setItemDelegateForColumn(DeadArea, delegate);

	para.desc = tr("DeadType");
	para.tagname = "DeadType";
	m_arrInfo.insert(DeadType, para);
	m_horizontal_header_list.append(para.desc);
	QVector<IDDESC> arrVec;

	m_arrDeadType.clear();

	IDDESC tmp;
	tmp.desc = tr("value");
	tmp.ID = QString("%1").arg(AnalogAlarmDeadArea::VALUE);
	arrVec.append(tmp);
	m_arrDeadType.append(tmp);
	
	tmp.desc = tr("percent");
	tmp.ID = QString("%1").arg(AnalogAlarmDeadArea::PERCENT);
	arrVec.append(tmp);
	m_arrDeadType.append(tmp);
	CComboBoxDelegate *pCombo = new CComboBoxDelegate(this, m_arrDeadType);
	((CAnalogAlarmTable *)parent())->setItemDelegateForColumn(DeadType, pCombo);
}

Qt::ItemFlags CAnalogAlarmModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}


/*! \fn void CAnalogAlarmModel::RefrushModel()
********************************************************************************************************* 
** \brief CAnalogAlarmModel::RefrushModel 
** \details 刷新
** \return void 
** \author gw
** \date 2016年11月26日 
** \note 
********************************************************************************************************/
void CAnalogAlarmModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}