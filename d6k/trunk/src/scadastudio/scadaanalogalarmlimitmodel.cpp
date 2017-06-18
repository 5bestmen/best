#include "scadaanalogalarmlimitmodel.h"
#include "scadavirable_data.h"
#include "delegate.h"
#include "scadaanalogalarmtable.h"
#include "scadastudio/icore.h"
#include "global.h"

#include <QStandardItem>

CScadaAnalogAlarmLimitModel::CScadaAnalogAlarmLimitModel(QObject *parent, IMainModuleInterface *pCore,
	Config::CScadaAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada)
	: QAbstractTableModel(parent), m_pConfig(nullptr), m_pAlarmData(nullptr), m_pItem(nullptr), m_pCore(nullptr), m_pScada(nullptr)
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

CScadaAnalogAlarmLimitModel::~CScadaAnalogAlarmLimitModel()
{

}

int CScadaAnalogAlarmLimitModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_pAlarmData->m_arrAlarmOfflimit.size();
}

int CScadaAnalogAlarmLimitModel::columnCount(const QModelIndex &parent) const
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

QVariant CScadaAnalogAlarmLimitModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

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


	int nRow = index.row();
	auto pLimit = m_pAlarmData->m_arrAlarmOfflimit[nRow];

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);
		if (index.column() == ID)
		{
			return (uint)pLimit->m_nID;
		}
		else if (index.column() == TagName)
		{
			return pLimit->m_szTagName;
		}
		else if (index.column() == Description)
		{
			return pLimit->m_strDescription;
		}
		else if (index.column() == OccNo)
		{
			return (uint)pLimit->GetOccNo();
		}
		else if (index.column() == BlockNo)
		{
			return (uint)pLimit->GetBlockNo();
		}
		else if (index.column() == Condition)
		{
			return pLimit->m_nCondition;
		}
		else if (index.column() == Category)
		{
			return pLimit->m_nCategory;
		}
		else if (index.column() == Value)
		{
			return pLimit->m_dbValue;
		}
		else if (index.column() == LowValue)
		{
			return pLimit->m_dbValue;
		}
		else if (index.column() == DelayAlarm)
		{
			return (uint)pLimit->m_nDelayAlarm;
		}
		else if (index.column() == Priority)
		{
			return (uint)pLimit->m_nPriority;
		}
		else if (index.column() == SupportAck)
		{
			return pLimit->m_bSupportAck;
		}
		//else if (index.column() == SupportDelete)
		//{
		//	return pLimit->m_bSupportDelete;
		//}
		else if (index.column() == SupportDelete)
		{
			return pLimit->m_bSupportDelete;
		}
		else if (index.column() == AckType)
		{
			return (uint)pLimit->m_nAckType;
		}
		else if (index.column() == SoundFile)
		{
			return pLimit->m_strSoundFile;
		}
		else if (index.column() == PlaySoundTimes)
		{
			return (uint)pLimit->m_nPlaySoundTimes;
		}
		else if (index.column() == SpeechAlarmText)
		{
			return pLimit->m_strSpeechAlarmText;
		}
		else if (index.column() == Beep)
		{
			return pLimit->m_bBeep;
		}
		else if (index.column() == PushGraph)
		{
			return pLimit->m_strPushGraph;
		}
		else if (index.column() == BlinkGraph)
		{
			return pLimit->m_bBlinkGraph;
		}
		else if (index.column() == Log)
		{
			return pLimit->m_bLog;
		}
		//else if (index.column() == BackColor)
		//{
		//	return (uint)pLimit->m_nBackColor;
		//}
		//else if (index.column() == TextColor)
		//{
		//	return (uint)pLimit->m_nTextColor;
		//}
		//else if (index.column() == BlinkBackColor)
		//{
		//	return (uint)pLimit->m_nBlinkBackColor;
		//}
		//else if (index.column() == BlinkTextColor)
		//{
		//	return (uint)pLimit->m_nBlinkTextColor;
		//}
		else if (index.column() == LightBoard)
		{
			return pLimit->m_bLightBoard;
		}
		else if (index.column() == DispGuide)
		{
			return pLimit->m_strDispGuide;
		}
		else if (index.column() == Commands)
		{
			return pLimit->m_strCommands;
		}


		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		//Q_ASSERT(m_pChannel);
		if (index.column() == ID)
		{
			return (uint)pLimit->m_nID;
		}
		else if (index.column() == TagName)
		{
			return pLimit->m_szTagName;
		}
		else if (index.column() == Description)
		{
			return pLimit->m_strDescription;
		}
		else if (index.column() == OccNo)
		{
			return (uint)pLimit->GetOccNo();
		}
		else if (index.column() == BlockNo)
		{
			return (uint)pLimit->GetBlockNo();
		}
		else if (index.column() == Condition)
		{
			return pLimit->m_nCondition;
		}
		else if (index.column() == Category)
		{
			return pLimit->m_nCategory;
		}
		else if (index.column() == Value)
		{
			return pLimit->m_dbValue;
		}
		else if (index.column() == LowValue)
		{
			return pLimit->m_dbValue;
		}
		else if (index.column() == DelayAlarm)
		{
			return (uint)pLimit->m_nDelayAlarm;
		}
		else if (index.column() == Priority)
		{
			return (uint)pLimit->m_nPriority;
		}
		else if (index.column() == SupportAck)
		{
			return pLimit->m_bSupportAck;
		}
		//else if (index.column() == SupportDelete)
		//{
		//	return pLimit->m_bSupportDelete;
		//}
		else if (index.column() == SupportDelete)
		{
			return pLimit->m_bSupportDelete;
		}
		else if (index.column() == AckType)
		{
			return (uint)pLimit->m_nAckType;
		}
		else if (index.column() == SoundFile)
		{
			return pLimit->m_strSoundFile;
		}
		else if (index.column() == PlaySoundTimes)
		{
			return (uint)pLimit->m_nPlaySoundTimes;
		}
		else if (index.column() == SpeechAlarmText)
		{
			return pLimit->m_strSpeechAlarmText;
		}
		else if (index.column() == Beep)
		{
			return pLimit->m_bBeep;
		}
		else if (index.column() == PushGraph)
		{
			return pLimit->m_strPushGraph;
		}
		else if (index.column() == BlinkGraph)
		{
			return pLimit->m_bBlinkGraph;
		}
		else if (index.column() == Log)
		{
			return pLimit->m_bLog;
		}
		else if (index.column() == BackColor)
		{
			return (uint)pLimit->m_nBackColor;
		}
		else if (index.column() == TextColor)
		{
			return (uint)pLimit->m_nTextColor;
		}
		else if (index.column() == BlinkBackColor)
		{
			return (uint)pLimit->m_nBlinkBackColor;
		}
		else if (index.column() == BlinkTextColor)
		{
			return (uint)pLimit->m_nBlinkTextColor;
		}
		else if (index.column() == LightBoard)
		{
			return pLimit->m_bLightBoard;
		}
		else if (index.column() == DispGuide)
		{
			return pLimit->m_strDispGuide;
		}
		else if (index.column() == Commands)
		{
			return pLimit->m_strCommands;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CScadaAnalogAlarmLimitModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool CScadaAnalogAlarmLimitModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
	{
		return false;
	}
	
	int nRow = index.row();
	//auto bFlag = nRow > 0 || nRow >= m_pAlarmData->m_arrAlarmOfflimit.size();
	auto pLimit = m_pAlarmData->m_arrAlarmOfflimit[nRow];

	if (role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			if (index.column() == ID)
			{
				if (pLimit->m_nID != value.toUInt())
				{
					pLimit->m_nID = value.toUInt();
				}
			}
			else if (index.column() == TagName)
			{
				if (pLimit->m_szTagName != value.toString())
				{
					if (!CheckTagNameIsValid(value.toString(), SCADAVARIABLE_DESC))
					{
						return false;
					}
					
					//check tagname
					if (m_pScada->CheckTagNameIsExist(value.toString()))
					{
						auto strError = QString(tr("analog alarm limit tagname %1 modify fail!!!")).arg(value.toString());
						m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						return true;
					}

					//修改hash
					if (!m_pScada->ModifyHashMap(SCADA_VARIABLE_HASH_ID::ANALOGALARMLIMITID, value.toString(), pLimit->m_szTagName))
					{
						auto strError = QString(tr("analog alarm limit tagname %1 hash modify fail!!!")).arg(value.toString());
						m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						return true;
					}

					//拷贝
					memset(pLimit->m_szTagName, 0, sizeof(pLimit->m_szTagName));
					auto nSize = value.toString().size();
					strncpy(pLimit->m_szTagName, value.toString().toStdString().c_str(), qMin(nSize, MAX_TAGNAME_LEN_SCADASTUDIO));
				}
			}
			else if (index.column() == Description)
			{
				if (pLimit->m_strDescription != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), SCADAVARIABLE_DESC))
					{
						return false;
					}

					pLimit->m_strDescription = value.toString();
				}
			}
			else if (index.column() == Condition)
			{
				if (pLimit->m_nCondition != value.toUInt())
				{
					pLimit->m_nCondition = value.toUInt();
				}
			}
			else if (index.column() == Category)
			{
				if (pLimit->m_nCategory != value.toUInt())
				{
					pLimit->m_nCategory = value.toUInt();
				}
			}
			else if (index.column() == Value)
			{
				if (pLimit->m_dbValue != value.toDouble())
				{
					pLimit->m_dbValue = value.toDouble();
				}
			}
			else if (index.column() == LowValue)
			{
				if (pLimit->m_dbLowValue != value.toDouble())
				{
					pLimit->m_dbLowValue = value.toDouble();
				}
			}
			else if (index.column() == DelayAlarm)
			{
				if (pLimit->m_nDelayAlarm != value.toUInt())
				{
					pLimit->m_nDelayAlarm = value.toUInt();
				}
			}
			else if (index.column() == Priority)
			{
				if (pLimit->m_nPriority != value.toUInt())
				{
					pLimit->m_nPriority = value.toUInt();
				}
			}
			else if (index.column() == SupportAck)
			{
				if (pLimit->m_bSupportAck != value.toBool())
				{
					pLimit->m_bSupportAck = value.toBool();
				}
			}
			else if (index.column() == SupportDelete)
			{
				if (pLimit->m_bSupportDelete != value.toBool())
				{
					pLimit->m_bSupportDelete = value.toBool();
				}
			}
			else if (index.column() == AckType)
			{
				if (pLimit->m_nAckType != value.toUInt())
				{
					pLimit->m_nAckType = value.toUInt();
				}
			}
			else if (index.column() == SoundFile)
			{
				if (pLimit->m_strSoundFile != value.toString())
				{
					pLimit->m_strSoundFile = value.toString();
				}
			}
			else if (index.column() == PlaySoundTimes)
			{
				if (pLimit->m_nPlaySoundTimes != value.toUInt())
				{
					pLimit->m_nPlaySoundTimes = value.toUInt();
				}
			}
			else if (index.column() == SpeechAlarmText)
			{
				if (pLimit->m_strSpeechAlarmText != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), SCADAVARIABLE_DESC))
					{
						return false;
					}
					
					pLimit->m_strSpeechAlarmText = value.toString();
				}
			}
			else if (index.column() == Beep)
			{
				if (pLimit->m_bBeep != value.toBool())
				{
					pLimit->m_bBeep = value.toBool();
				}
			}
			else if (index.column() == PushGraph)
			{
				if (pLimit->m_strPushGraph != value.toString())
				{
					pLimit->m_strPushGraph = value.toString();
				}
			}
			else if (index.column() == BlinkGraph)
			{
				if (pLimit->m_bBlinkGraph != value.toBool())
				{
					pLimit->m_bBlinkGraph = value.toBool();
				}
			}
			else if (index.column() == Log)
			{
				if (pLimit->m_bLog != value.toBool())
				{
					pLimit->m_bLog = value.toBool();
				}
			}
			else if (index.column() == Log)
			{
				if (pLimit->m_bLog != value.toBool())
				{
					pLimit->m_bLog = value.toBool();
				}
			}
			else if (index.column() == BackColor)
			{
				if (pLimit->m_nBackColor != value.toInt())
				{
					pLimit->m_nBackColor = value.toInt();
				}
			}
			else if (index.column() == TextColor)
			{
				if (pLimit->m_nTextColor != value.toInt())
				{
					pLimit->m_nTextColor = value.toInt();
				}
			}
			else if (index.column() == BlinkBackColor)
			{
				if (pLimit->m_nBlinkBackColor != value.toInt())
				{
					pLimit->m_nBlinkBackColor = value.toInt();
				}
			}
			else if (index.column() == BlinkTextColor)
			{
				if (pLimit->m_nBlinkTextColor != value.toInt())
				{
					pLimit->m_nBlinkTextColor = value.toInt();
				}
			}
			else if (index.column() == LightBoard)
			{
				if (pLimit->m_bLightBoard != value.toBool())
				{
					pLimit->m_bLightBoard = value.toBool();
				}
			}
			else if (index.column() == DispGuide)
			{
				if (pLimit->m_strDispGuide != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), SCADAVARIABLE_DESC))
					{
						return false;
					}
					
					pLimit->m_strDispGuide = value.toString();
				}
			}
			else if (index.column() == Commands)
			{
				if (pLimit->m_strCommands != value.toString())
				{
					if (!CheckTagNameIsValid(value.toString(), SCADAVARIABLE_DESC))
					{
						return false;
					}
					
					pLimit->m_strCommands = value.toString();
				}
			}

			emit dataChanged(index, index);

			return true;
		}

		return false;
	}

	return true;
}

Qt::ItemFlags CScadaAnalogAlarmLimitModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CScadaAnalogAlarmLimitModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

void CScadaAnalogAlarmLimitModel::InitPara()
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
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("Description");
	para.tagname = "Description";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEditDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(Description, pLineEditDesc);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	//m_arrInfo.insert(OccNo, para);
	m_horizontal_header_list.append(para.desc);
	CReadOnlyDelegate *pReadDelegate = new CReadOnlyDelegate(this);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(OccNo, pReadDelegate);

	para.desc = tr("BlockNo");
	para.tagname = "BlockNo";
	//m_arrInfo.insert(OccNo, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("Condition");
	para.tagname = "Condition";
	//m_arrInfo.insert(Condition, para);
	m_horizontal_header_list.append(para.desc);
	QVector<IDDESC> vec;
	vec.clear();
	IDDESC tmp;
	tmp.desc = tr("GREATEREQUAL");
	tmp.ID = QString("%1").arg(GREATEREQUAL);
	vec.append(tmp);
	tmp.desc = tr("LESSEQUAL");
	tmp.ID = QString("%1").arg(LESSEQUAL);
	vec.append(tmp);
	tmp.desc = tr("EQUAL");
	tmp.ID = QString("%1").arg(EQUAL);
	vec.append(tmp);
	tmp.desc = tr("NOTEQUAL");
	tmp.ID = QString("%1").arg(NOTEQUAL);
	vec.append(tmp);
	tmp.desc = tr("BWTEEN");
	tmp.ID = QString("%1").arg(BWTEEN);
	vec.append(tmp);
	tmp.desc = tr("RATEREDUCE");
	tmp.ID = QString("%1").arg(RATEREDUCE);
	vec.append(tmp);
	tmp.desc = tr("RATEINCREASE");
	tmp.ID = QString("%1").arg(RATEINCREASE);
	vec.append(tmp);
	CComboBoxDelegate *pCombo = new CComboBoxDelegate(this, vec);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(Condition, pCombo);

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
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(Category, pCombo);

	para.desc = tr("Value");
	para.tagname = "Value";
	//m_arrInfo.insert(Value, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("LowValue");
	para.tagname = "LowValue";
	//m_arrInfo.insert(LowValue, para);
	m_horizontal_header_list.append(para.desc);

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
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(SupportAck, pCheck);

	para.desc = tr("SupportDelete");
	para.tagname = "SupportDelete";
	//m_arrInfo.insert(SupportDelete, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(SupportDelete, pCheck);

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
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(AckType, pCombo);

	para.desc = tr("SoundFile");
	para.tagname = "SoundFile";
	//m_arrInfo.insert(SoundFile, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(SoundFile, pLineEditDesc);

	para.desc = tr("PlaySoundTimes");
	para.tagname = "PlaySoundTimes";
	//m_arrInfo.insert(PlaySoundTimes, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(SoundFile, pLineEditDesc);

	para.desc = tr("SpeechAlarmText");
	para.tagname = "SpeechAlarmText";
	//m_arrInfo.insert(SpeechAlarmText, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(SoundFile, pLineEditDesc);

	para.desc = tr("Beep");
	para.tagname = "Beep";
	//m_arrInfo.insert(Beep, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(Beep, pCheck);

	para.desc = tr("PushGraph");
	para.tagname = "PushGraph";
	//m_arrInfo.insert(PushGraph, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(PushGraph, pCheck);

	para.desc = tr("BlinkGraph");
	para.tagname = "BlinkGraph";
	//m_arrInfo.insert(BlinkGraph, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(BlinkGraph, pCheck);

	para.desc = tr("Log");
	para.tagname = "Log";
	//m_arrInfo.insert(Log, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(Log, pCheck);

	para.desc = tr("BackColor");
	para.tagname = "BackColor";
	//m_arrInfo.insert(BackColor, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(BackColor, pReadDelegate);

	para.desc = tr("TextColor");
	para.tagname = "TextColor";
	//m_arrInfo.insert(TextColor, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(TextColor, pReadDelegate);

	para.desc = tr("BlinkBackColor");
	para.tagname = "BlinkBackColor";
	//m_arrInfo.insert(BlinkBackColor, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(BlinkBackColor, pReadDelegate);

	para.desc = tr("BlinkTextColor");
	para.tagname = "BlinkTextColor";
	//m_arrInfo.insert(BlinkTextColor, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(BlinkTextColor, pReadDelegate);

	para.desc = tr("LightBoard");
	para.tagname = "LightBoard";
	//m_arrInfo.insert(LightBoard, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(LightBoard, pCheck);

	para.desc = tr("DispGuide");
	para.tagname = "DispGuide";
	//m_arrInfo.insert(DispGuide, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("Commands");
	para.tagname = "Commands";
	//m_arrInfo.insert(Commands, para);
	m_horizontal_header_list.append(para.desc);
	((CScadaAnalogAlarmTable *)parent())->setItemDelegateForColumn(Commands, pLineEdit);
}

bool CScadaAnalogAlarmLimitModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	//auto nCount = m_pAlarmData->m_arrAlarmOfflimit.size();

	beginInsertRows(parent, position, position + rows - 1);

	Q_ASSERT(m_pConfig);
	if (!m_pConfig)
	{
		return false;
	}

	for (int i = position; i < position + rows; i++)
	{

		auto pAnalogLimitPoint = m_pScada->AddAnalogAlarmLimit();

		pAnalogLimitPoint->SetOccNo(i + 1);
		pAnalogLimitPoint->m_nID = i + 1;

		m_pAlarmData->m_arrAlarmOfflimit.push_back(pAnalogLimitPoint);
	}

	endInsertRows();

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CScadaAnalogAlarmLimitModel::RemoveRows 
** \details 
** \param position 
** \param rows 
** \param parent 
** \return bool 
** \author gw
** \date 2017年1月12日 
** \note 
********************************************************************************************************/
bool CScadaAnalogAlarmLimitModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, rows, rows);

	auto nCount = m_pAlarmData->m_arrAlarmOfflimit.size();

	auto bFlag = position < 0 || position >= nCount;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		return false;
	}

	auto pAnalog = m_pAlarmData->m_arrAlarmOfflimit[position];

	QString strTmp = pAnalog->m_szTagName;
	if (!m_pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::ANALOGALARMLIMITID, strTmp))
	{
		auto strError = QObject::tr("Scada analog alarm limit hash delete fail!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);
		
		return false;
	}

	auto it = std::find(m_pAlarmData->m_arrAlarmOfflimit.begin(), m_pAlarmData->m_arrAlarmOfflimit.end(), pAnalog);
	if (it != m_pAlarmData->m_arrAlarmOfflimit.end())
	{
		m_pAlarmData->m_arrAlarmOfflimit.erase(it);
	}

	delete pAnalog;
	pAnalog = nullptr;

	endRemoveRows();

	if (m_pAlarmData->m_arrAlarmOfflimit.size() == 0)
	{
		RefrushModel();
	}

	return true;
}