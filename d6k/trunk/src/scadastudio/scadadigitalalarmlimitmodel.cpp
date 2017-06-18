#include "scadadigitalalarmlimitmodel.h"
#include "scadavirable_data.h"
#include "scadastudio/icore.h"
#include "delegate.h"
#include "scadadigitalalarmlimittable.h"
#include "global.h"

#include <QStandardItem>

CScadaDigitalAlarmLimitModel::CScadaDigitalAlarmLimitModel(QObject *parent, IMainModuleInterface *pCore,
	Config::CScadaDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada)
	: QAbstractTableModel(parent), m_pConfig(nullptr), m_pAlarmData(nullptr), m_pItem(nullptr), m_pScada(nullptr)
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

CScadaDigitalAlarmLimitModel::~CScadaDigitalAlarmLimitModel()
{

}

int CScadaDigitalAlarmLimitModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_pAlarmData->m_arrDigtallimit.size();
}

int CScadaDigitalAlarmLimitModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_horizontal_header_list.size() > 0)
		return m_horizontal_header_list.size();

	return 0;
}

QVariant CScadaDigitalAlarmLimitModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

	auto bFlag = nRow < 0 || nRow >= m_pAlarmData->m_arrDigtallimit.size();
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		return QVariant();
	}

	auto pLimit = m_pAlarmData->m_arrDigtallimit[nRow];

	if (role == Qt::BackgroundRole)
	{
		if (index.column() == BackColor || index.column() == TextColor
			|| index.column() == BlinkBackColor || index.column() == BlinkTextColor)
		{
			int color = index.data(Qt::EditRole).toInt();
			int red = color & 255;
			int green = color >> 8 & 255;
			int blue = color >> 16 & 255;
			return QColor(red, green, blue);
		}
	}

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);
		if (index.column() == ID)
		{
			return (uint)pLimit->ID;
		}
		else if (index.column() == TagName)
		{
			return pLimit->TagName;
		}
		else if (index.column() == Description)
		{
			return pLimit->Description;
		}
		else if (index.column() == OccNo)
		{
			return (uint)pLimit->GetOccNo();
		}
		else if (index.column() == BlockNo)
		{
			return (uint)pLimit->BlockNo;
		}
		else if (index.column() == Condition)
		{
			return (uint)pLimit->Condition;
		}
		else if (index.column() == Category)
		{
			return (uint)pLimit->Category;
		}
		else if (index.column() == DelayAlarm)
		{
			return (uint)pLimit->DelayAlarm;
		}
		else if (index.column() == Priority)
		{
			return (uint)pLimit->Priority;
		}
		else if (index.column() == SupportAck)
		{
			return pLimit->SupportAck;
		}
		else if (index.column() == SupportDelete)
		{
			return pLimit->SupportDelete;
		}
		else if (index.column() == AckType)
		{
			return (uint)pLimit->AckType;
		}
		else if (index.column() == SoundFile)
		{
			return pLimit->SoundFile;
		}
		else if (index.column() == PlaySoundTimes)
		{
			return (uint)pLimit->PlaySoundTimes;
		}
		else if (index.column() == SpeechAlarmText)
		{
			return pLimit->SpeechAlarmText;
		}
		else if (index.column() == Beep)
		{
			return pLimit->Beep;
		}
		else if (index.column() == PushGraph)
		{
			return pLimit->PushGraph;
		}
		else if (index.column() == BlinkGraph)
		{
			return pLimit->BlinkGraph;
		}
		else if (index.column() == Log)
		{
			return pLimit->Log;
		}
		else if (index.column() == BackColor)
		{
			//return (uint)pLimit->BackColor;
		}
		else if (index.column() == TextColor)
		{
			//return (uint)pLimit->TextColor;
		}
		else if (index.column() == BlinkBackColor)
		{
			//return (uint)pLimit->BlinkBackColor;
		}
		else if (index.column() == BlinkTextColor)
		{
			//return (uint)pLimit->BlinkTextColor;
		}
		else if (index.column() == DispGuide)
		{
			return pLimit->DispGuide;
		}
		else if (index.column() == Commands)
		{
			return pLimit->Commands;
		}

		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == ID)
		{
			return (uint)pLimit->ID;
		}
		else if (index.column() == TagName)
		{
			return pLimit->TagName;
		}
		else if (index.column() == Description)
		{
			return pLimit->Description;
		}
		else if (index.column() == OccNo)
		{
			return (uint)pLimit->GetOccNo();
		}
		else if (index.column() == BlockNo)
		{
			return (uint)pLimit->BlockNo;
		}
		else if (index.column() == Condition)
		{
			return (uint)pLimit->Condition;
		}
		else if (index.column() == Category)
		{
			return (uint)pLimit->Category;
		}
		else if (index.column() == DelayAlarm)
		{
			return (uint)pLimit->DelayAlarm;
		}
		else if (index.column() == Priority)
		{
			return (uint)pLimit->Priority;
		}
		else if (index.column() == SupportAck)
		{
			return pLimit->SupportAck;
		}
		else if (index.column() == SupportDelete)
		{
			return pLimit->SupportDelete;
		}
		else if (index.column() == AckType)
		{
			return (uint)pLimit->AckType;
		}
		else if (index.column() == SoundFile)
		{
			return pLimit->SoundFile;
		}
		else if (index.column() == PlaySoundTimes)
		{
			return (uint)pLimit->PlaySoundTimes;
		}
		else if (index.column() == SpeechAlarmText)
		{
			return pLimit->SpeechAlarmText;
		}
		else if (index.column() == Beep)
		{
			return pLimit->Beep;
		}
		else if (index.column() == PushGraph)
		{
			return pLimit->PushGraph;
		}
		else if (index.column() == BlinkGraph)
		{
			return pLimit->BlinkGraph;
		}
		else if (index.column() == Log)
		{
			return pLimit->Log;
		}
		else if (index.column() == BackColor)
		{
			return (uint)pLimit->BackColor;
		}
		else if (index.column() == TextColor)
		{
			return (uint)pLimit->TextColor;
		}
		else if (index.column() == BlinkBackColor)
		{
			return (uint)pLimit->BlinkBackColor;
		}
		else if (index.column() == BlinkTextColor)
		{
			return (uint)pLimit->BlinkTextColor;
		}
		else if (index.column() == DispGuide)
		{
			return pLimit->DispGuide;
		}
		else if (index.column() == Commands)
		{
			return pLimit->Commands;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CScadaDigitalAlarmLimitModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool CScadaDigitalAlarmLimitModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
	{
		return false;
	}

	int nRow = index.row();

	auto bFlag = nRow < 0 || nRow >= m_pAlarmData->m_arrDigtallimit.size();
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		return false;
	}

	auto pLimit = m_pAlarmData->m_arrDigtallimit[nRow];

	if (role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			if (index.column() == ID)
			{
				if (pLimit->ID != value.toInt())
				{
					pLimit->ID = value.toInt();
				}
			}
			else if (index.column() == TagName)
			{
				if (pLimit->TagName != value.toString())
				{
					//memset(pLimit->TagName, 0, sizeof(pLimit->TagName));
					//int nSize = value.toString().size();
					//strncpy(pLimit->TagName, value.toString().toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));

					if (!CheckTagNameIsValid(value.toString(), SCADAVARIABLE_DESC))
					{
						return false;
					}

					if (m_pScada->CheckTagNameIsExist(value.toString()))
					{
						auto strError = tr("digital alarm limit tagname %1 exist!!!").arg(value.toString());
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return true;
					}

					if (!m_pScada->ModifyHashMap(SCADA_VARIABLE_HASH_ID::DIGITALALARMLIMITID, value.toString(), pLimit->TagName))
					{
						auto strError = QObject::tr("scada digital limit modify hash fail!!!");
						m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						return false;
					}

					memset(pLimit->TagName, 0, sizeof(pLimit->TagName));
					int nSize = value.toString().size();
					strncpy(pLimit->TagName, value.toString().toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
				}
			}
			else if (index.column() == Description)
			{
				if (pLimit->Description != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), SCADAVARIABLE_DESC))
					{
						return false;
					}

					pLimit->Description = value.toString();

					//m_pItem->setData(pLimit->Description, TREE_TOOL_TIP_ROLE);
				}
			}
			else if (index.column() == Condition)
			{
				if (pLimit->Condition != value.toInt())
				{
					pLimit->Condition = value.toInt();
				}
			}
			else if (index.column() == Category)
			{
				if (pLimit->Category != value.toInt())
				{
					pLimit->Category = value.toInt();
				}
			}
			else if (index.column() == DelayAlarm)
			{
				if (pLimit->DelayAlarm != value.toInt())
				{
					pLimit->DelayAlarm = value.toInt();
				}
			}
			else if (index.column() == Priority)
			{
				if (pLimit->Priority != value.toInt())
				{
					pLimit->Priority = value.toInt();
				}
			}
			else if (index.column() == SupportAck)
			{
				if (pLimit->SupportAck != value.toBool())
				{
					pLimit->SupportAck = value.toBool();
				}
			}
			else if (index.column() == SupportDelete)
			{
				if (pLimit->SupportDelete != value.toBool())
				{
					pLimit->SupportDelete = value.toBool();
				}
			}
			else if (index.column() == AckType)
			{
				if (pLimit->AckType != value.toInt())
				{
					pLimit->AckType = value.toInt();
				}
			}
			else if (index.column() == SoundFile)
			{
				if (pLimit->SoundFile != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), SCADAVARIABLE_DESC))
					{
						return false;
					}
					
					pLimit->SoundFile = value.toString();
				}
			}
			else if (index.column() == PlaySoundTimes)
			{
				if (pLimit->PlaySoundTimes != value.toInt())
				{
					pLimit->PlaySoundTimes = value.toInt();
				}
			}
			else if (index.column() == SpeechAlarmText)
			{
				if (pLimit->SpeechAlarmText != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), SCADAVARIABLE_DESC))
					{
						return false;
					}
					
					pLimit->SpeechAlarmText = value.toString();
				}
			}
			else if (index.column() == Beep)
			{
				if (pLimit->Beep != value.toBool())
				{
					pLimit->Beep = value.toBool();
				}
			}
			else if (index.column() == PushGraph)
			{
				if (pLimit->PushGraph != value.toString())
				{
					pLimit->PushGraph = value.toString();
				}
			}
			else if (index.column() == BlinkGraph)
			{
				if (pLimit->BlinkGraph != value.toBool())
				{
					pLimit->BlinkGraph = value.toBool();
				}
			}
			else if (index.column() == Log)
			{
				if (pLimit->Log != value.toBool())
				{
					pLimit->Log = value.toBool();
				}
			}
			else if (index.column() == BackColor)
			{
				if (pLimit->BackColor != value.toInt())
				{
					pLimit->BackColor = value.toInt();
				}
			}
			else if (index.column() == TextColor)
			{
				if (pLimit->TextColor != value.toInt())
				{
					pLimit->TextColor = value.toInt();
				}
			}
			else if (index.column() == BlinkBackColor)
			{
				if (pLimit->BlinkBackColor != value.toInt())
				{
					pLimit->BlinkBackColor = value.toInt();
				}
			}
			else if (index.column() == BlinkTextColor)
			{
				if (pLimit->BlinkTextColor != value.toInt())
				{
					pLimit->BlinkTextColor = value.toInt();
				}
			}
			else if (index.column() == DispGuide)
			{
				if (pLimit->DispGuide != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), SCADAVARIABLE_DESC))
					{
						return false;
					}

					
					pLimit->DispGuide = value.toString();
				}
			}
			else if (index.column() == Commands)
			{
				if (pLimit->Commands != value.toString())
				{
					if (!CheckTagNameIsValid(value.toString(), SCADAVARIABLE_DESC))
					{
						return false;
					}
					
					pLimit->Commands = value.toString();
				}
			}

			emit dataChanged(index, index);

			return true;
		}

		return false;
	}

	return false;
}

Qt::ItemFlags CScadaDigitalAlarmLimitModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CScadaDigitalAlarmLimitModel::InitPara()
{
	CONFIG_PARA para;

	para.desc = tr("ID");
	para.tagname = "ID";
	//m_arrInfo.insert(ID, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	//m_arrInfo.insert(TagName, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEdit = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("Description");
	para.tagname = "Description";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEditDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(Description, pLineEditDesc);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	//m_arrInfo.insert(OccNo, para);
	m_horizontal_header_list.append(para.desc);
	auto pReadOnly = new CReadOnlyDelegate(this);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnly);

	para.desc = tr("BlockNo");
	para.tagname = "BlockNo";
	//m_arrInfo.insert(OccNo, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(BlockNo, pReadOnly);

	para.desc = tr("Condition");
	para.tagname = "Condition";
	//m_arrInfo.insert(Condition, para);
	m_horizontal_header_list.append(para.desc);
	QVector<IDDESC> vec;
	vec.clear();
	IDDESC tmp;
	tmp.desc = tr("ZEROTOONE");
	tmp.ID = QString("%1").arg(ZEROTOONE);
	vec.append(tmp);
	tmp.desc = tr("ONETOZERO");
	tmp.ID = QString("%1").arg(ONETOZERO);
	vec.append(tmp);
	CComboBoxDelegate *pCombo = new CComboBoxDelegate(this, vec);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(Condition, pCombo);

	para.desc = tr("Category");
	para.tagname = "Category";
	//m_arrInfo.insert(Category, para);
	m_horizontal_header_list.append(para.desc);
	vec.clear();
	tmp.desc = tr("ACCIDENT");
	tmp.ID = QString("%1").arg(ACCIDENT);
	vec.append(tmp);
	tmp.desc = tr("FAULT");
	tmp.ID = QString("%1").arg(FAULT);
	vec.append(tmp);
	tmp.desc = tr("NOMAL");
	tmp.ID = QString("%1").arg(COMMON);
	vec.append(tmp);
	pCombo = new CComboBoxDelegate(this, vec);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(Category, pCombo);

	para.desc = tr("DelayAlarm");
	para.tagname = "DelayAlarm";
	//m_arrInfo.insert(DelayAlarm, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("Priority");
	para.tagname = "Priority";
	//m_arrInfo.insert(Priority, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("SupportAck");
	para.tagname = "SupportAck";
	//m_arrInfo.insert(SupportAck, para);
	m_horizontal_header_list.append(para.desc);
	CCheckBoxDelegate *pCheck = new CCheckBoxDelegate(this);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(SupportAck, pCheck);

	para.desc = tr("SupportDelete");
	para.tagname = "SupportDelete";
	//m_arrInfo.insert(SupportDelete, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(SupportDelete, pCheck);

	para.desc = tr("AckType");
	para.tagname = "AckType";
	//m_arrInfo.insert(AckType, para);
	m_horizontal_header_list.append(para.desc);
	vec.clear();
	tmp.desc = tr("DELETE");
	tmp.ID = QString("%1").arg(DELETE);
	vec.append(tmp);
	tmp.desc = tr("KEEP");
	tmp.ID = QString("%1").arg(KEEP);
	vec.append(tmp);
	tmp.desc = tr("CONFORM");
	tmp.ID = QString("%1").arg(CONFORM);
	vec.append(tmp);
	pCombo = new CComboBoxDelegate(this, vec);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(AckType, pCombo);

	para.desc = tr("SoundFile");
	para.tagname = "SoundFile";
	//m_arrInfo.insert(SoundFile, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(SoundFile, pLineEditDesc);

	para.desc = tr("PlaySoundTimes");
	para.tagname = "PlaySoundTimes";
	//m_arrInfo.insert(PlaySoundTimes, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("SpeechAlarmText");
	para.tagname = "SpeechAlarmText";
	//m_arrInfo.insert(SpeechAlarmText, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(SpeechAlarmText, pLineEditDesc);

	para.desc = tr("Beep");
	para.tagname = "Beep";
	//m_arrInfo.insert(Beep, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(Beep, pCheck);

	para.desc = tr("PushGraph");
	para.tagname = "PushGraph";
	//m_arrInfo.insert(PushGraph, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(PushGraph, pCheck);

	para.desc = tr("BlinkGraph");
	para.tagname = "BlinkGraph";
	//m_arrInfo.insert(BlinkGraph, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(BlinkGraph, pCheck);

	para.desc = tr("Log");
	para.tagname = "Log";
	//m_arrInfo.insert(Log, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(Log, pCheck);

	para.desc = tr("BackColor");
	para.tagname = "BackColor";
	//m_arrInfo.insert(BackColor, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(BackColor, pReadOnly);

	para.desc = tr("TextColor");
	para.tagname = "TextColor";
	//m_arrInfo.insert(TextColor, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(TextColor, pReadOnly);


	para.desc = tr("BlinkBackColor");
	para.tagname = "BlinkBackColor";
	//m_arrInfo.insert(BlinkBackColor, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(BlinkBackColor, pReadOnly);

	para.desc = tr("BlinkTextColor");
	para.tagname = "BlinkTextColor";
	//m_arrInfo.insert(BlinkTextColor, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(BlinkTextColor, pReadOnly);

	para.desc = tr("DispGuide");
	para.tagname = "DispGuide";
	//m_arrInfo.insert(DispGuide, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(DispGuide, pLineEditDesc);


	para.desc = tr("Commands");
	para.tagname = "Commands";
	//m_arrInfo.insert(Commands, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(Commands, pLineEdit);
}

void CScadaDigitalAlarmLimitModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

bool CScadaDigitalAlarmLimitModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	//auto nCount = m_pAlarmData->m_arrDigtallimit.size();

	beginInsertRows(parent, position, position + rows - 1);

	Q_ASSERT(m_pConfig);
	if (!m_pConfig)
	{
		return false;
	}

	for (int i = position; i < position + rows; i++)
	{

		auto pAnalogLimitPoint = m_pScada->AddDigitalAlarmLimit();

		pAnalogLimitPoint->SetOccNo(i + 1);
		pAnalogLimitPoint->ID = i + 1;

		m_pAlarmData->m_arrDigtallimit.push_back(pAnalogLimitPoint);
	}

	endInsertRows();

	return true;
}
bool CScadaDigitalAlarmLimitModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, rows, rows);

	auto nCount = m_pAlarmData->m_arrDigtallimit.size();

	auto bFlag = position < 0 || position >= nCount;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		return false;
	}

	auto pAnalog = m_pAlarmData->m_arrDigtallimit[position];

	auto it = std::find(m_pAlarmData->m_arrDigtallimit.begin(), m_pAlarmData->m_arrDigtallimit.end(), pAnalog);
	if (it != m_pAlarmData->m_arrDigtallimit.end())
	{
		m_pAlarmData->m_arrDigtallimit.erase(it);
	}

	QString strTmp = pAnalog->TagName;
	if (!m_pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::DIGITALALARMLIMITID, strTmp))
	{
		endRemoveRows();

		auto strError = QObject::tr("delete limit tagname %1fail!!!").arg(strTmp);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}


	delete pAnalog;
	pAnalog = nullptr;

	endRemoveRows();

	if (m_pAlarmData->m_arrDigtallimit.size() == 0)
	{
		RefrushModel();
	}

	return true;
}




