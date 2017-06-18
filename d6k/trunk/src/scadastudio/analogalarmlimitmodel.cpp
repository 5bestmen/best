#include "analogalarmlimitmodel.h"
#include "scadastudio/icore.h"
#include "alarm.h"
#include "config_data.h"
#include "delegate.h"
#include "analogalarmlimittable.h"
#include "scadastudiodefine.h"
#include "global.h"

#include <QStandardItem>

CAnalogAlarmLimitModel::CAnalogAlarmLimitModel(QObject *parent, IMainModuleInterface *pCore, CAlarm *pModule,
	Config::CAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes)
	: QAbstractTableModel(parent), m_pAlarmData(nullptr), m_pItem(nullptr), m_pFes(nullptr), m_pCore(nullptr)
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

	if (!pModule)
	{
		return;
	}

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

	m_pCore = pCore;

	InitPara();
}

CAnalogAlarmLimitModel::~CAnalogAlarmLimitModel()
{

}

int CAnalogAlarmLimitModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_pAlarmData->m_arrAlarmOfflimit.size();
}

int CAnalogAlarmLimitModel::columnCount(const QModelIndex &parent) const
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

QVariant CAnalogAlarmLimitModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();

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
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nID;
		}
		else if (index.column() == TagName)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_szTagName;
		}
		else if (index.column() == Description)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strDescription;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->GetOccNo();
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->GetBlockNo();
		}
		else if (index.column() == Condition)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nCondition;
		}
		else if (index.column() == Category)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nCategory;
		}
		else if (index.column() == Value)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_dbValue;
		}
		else if (index.column() == LowValue)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_dbValue;
		}
		else if (index.column() == DelayAlarm)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nDelayAlarm;
		}
		else if (index.column() == Priority)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nPriority;
		}
		else if (index.column() == SupportAck)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bSupportAck;
		}
		//else if (index.column() == SupportDelete)
		//{
		//	return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bSupportDelete;
		//}
		else if (index.column() == SupportDelete)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bSupportDelete;
		}
		else if (index.column() == AckType)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nAckType;
		}
		else if (index.column() == SoundFile)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strSoundFile;
		}
		else if (index.column() == PlaySoundTimes)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nPlaySoundTimes;
		}
		else if (index.column() == SpeechAlarmText)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strSpeechAlarmText;
		}
		else if (index.column() == Beep)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bBeep;
		}
		else if (index.column() == PushGraph)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strPushGraph;
		}
		else if (index.column() == BlinkGraph)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bBlinkGraph;
		}
		else if (index.column() == Log)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bLog;
		}
		else if (index.column() == BackColor)
		{
			//return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nBackColor;
		}
		else if (index.column() == TextColor)
		{
			//return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nTextColor;
		}
		else if (index.column() == BlinkBackColor)
		{
			//return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nBlinkBackColor;
		}
		else if (index.column() == BlinkTextColor)
		{
			//return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nBlinkTextColor;
		}
		else if (index.column() == LightBoard)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bLightBoard;
		}
		else if (index.column() == DispGuide)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strDispGuide;
		}
		else if (index.column() == Commands)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strCommands;
		}


		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		//Q_ASSERT(m_pChannel);
		if (index.column() == ID)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nID;
		}
		else if (index.column() == TagName)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_szTagName;
		}
		else if (index.column() == Description)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strDescription;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->GetOccNo();
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->GetBlockNo();
		}
		else if (index.column() == Condition)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nCondition;
		}
		else if (index.column() == Category)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nCategory;
		}
		else if (index.column() == Value)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_dbValue;
		}
		else if (index.column() == LowValue)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_dbValue;
		}
		else if (index.column() == DelayAlarm)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nDelayAlarm;
		}
		else if (index.column() == Priority)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nPriority;
		}
		else if (index.column() == SupportAck)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bSupportAck;
		}
		//else if (index.column() == SupportDelete)
		//{
		//	return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bSupportDelete;
		//}
		else if (index.column() == SupportDelete)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bSupportDelete;
		}
		else if (index.column() == AckType)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nAckType;
		}
		else if (index.column() == SoundFile)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strSoundFile;
		}
		else if (index.column() == PlaySoundTimes)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nPlaySoundTimes;
		}
		else if (index.column() == SpeechAlarmText)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strSpeechAlarmText;
		}
		else if (index.column() == Beep)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bBeep;
		}
		else if (index.column() == PushGraph)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strPushGraph;
		}
		else if (index.column() == BlinkGraph)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bBlinkGraph;
		}
		else if (index.column() == Log)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bLog;
		}
		else if (index.column() == BackColor)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nBackColor;
		}
		else if (index.column() == TextColor)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nTextColor;
		}
		else if (index.column() == BlinkBackColor)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nBlinkBackColor;
		}
		else if (index.column() == BlinkTextColor)
		{
			return (uint)m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nBlinkTextColor;
		}
		else if (index.column() == LightBoard)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bLightBoard;
		}
		else if (index.column() == DispGuide)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strDispGuide;
		}
		else if (index.column() == Commands)
		{
			return m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strCommands;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CAnalogAlarmLimitModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool CAnalogAlarmLimitModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			if (index.column() == ID)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nID != value.toUInt())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nID = value.toUInt();
				}
			}
			else if (index.column() == TagName)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_szTagName != value.toString())
				{
					if (!CheckTagNameIsValid(value.toString(), FES_DESC))
					{
						return false;
					}
					
					if (m_pFes->CheckTagNameIsExist(value.toString()))
					{
						auto strError = tr("analog alarm limit modify fail!!!");
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
						
						return true;
					}
					
					if (!m_pFes->ModifyFesHashMap(HASHID::ANALOGALARMLIMITHASHID, value.toString(), m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_szTagName))
					{
						return false;
					}

					memset(m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_szTagName, 0, sizeof(m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_szTagName));
					int nSize = value.toString().size();
					strncpy(m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));
				}
			}
			else if (index.column() == Description)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strDescription != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC))
					{
						return false;
					}
					
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strDescription = value.toString();

					//m_pItem->setData(value, TREE_TOOL_TIP_ROLE);
				}
			}
			else if (index.column() == Condition)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nCondition != value.toUInt())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nCondition = value.toUInt();
				}
			}
			else if (index.column() == Category)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nCategory != value.toUInt())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nCategory = value.toUInt();
				}
			}
			else if (index.column() == Value)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_dbValue != value.toDouble())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_dbValue = value.toDouble();
				}
			}
			else if (index.column() == LowValue)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_dbLowValue != value.toDouble())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_dbLowValue = value.toDouble();
				}
			}
			else if (index.column() == DelayAlarm)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nDelayAlarm != value.toUInt())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nDelayAlarm = value.toUInt();
				}
			}
			else if (index.column() == Priority)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nPriority != value.toUInt())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nPriority = value.toUInt();
				}
			}
			else if (index.column() == SupportAck)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bSupportAck != value.toBool())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bSupportAck = value.toBool();
				}
			}
			else if (index.column() == SupportDelete)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bSupportDelete != value.toBool())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bSupportDelete = value.toBool();
				}
			}
			else if (index.column() == AckType)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nAckType != value.toUInt())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nAckType = value.toUInt();
				}
			}
			else if (index.column() == SoundFile)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strSoundFile != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC))
					{
						return false;
					}
					
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strSoundFile = value.toString();
				}
			}
			else if (index.column() == PlaySoundTimes)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nPlaySoundTimes != value.toUInt())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nPlaySoundTimes = value.toUInt();
				}
			}
			else if (index.column() == SpeechAlarmText)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strSpeechAlarmText != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), FES_DESC))
					{
						return false;
					}
					
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strSpeechAlarmText = value.toString();
				}
			}
			else if (index.column() == Beep)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bBeep != value.toBool())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bBeep = value.toBool();
				}
			}
			else if (index.column() == PushGraph)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strPushGraph != value.toString())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strPushGraph = value.toString();
				}
			}
			else if (index.column() == BlinkGraph)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bBlinkGraph != value.toBool())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bBlinkGraph = value.toBool();
				}
			}
			else if (index.column() == Log)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bLog != value.toBool())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bLog = value.toBool();
				}
			}
			else if (index.column() == Log)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bLog != value.toBool())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bLog = value.toBool();
				}
			}
			else if (index.column() == BackColor)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nBackColor != value.toInt())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nBackColor = value.toInt();
				}
			}
			else if (index.column() == TextColor)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nTextColor != value.toInt())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nTextColor = value.toInt();
				}
			}
			else if (index.column() == BlinkBackColor)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nBlinkBackColor != value.toInt())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nBlinkBackColor = value.toInt();
				}
			}
			else if (index.column() == BlinkTextColor)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nBlinkTextColor != value.toInt())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_nBlinkTextColor = value.toInt();
				}
			}
			else if (index.column() == LightBoard)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bLightBoard != value.toBool())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_bLightBoard = value.toBool();
				}
			}
			else if (index.column() == DispGuide)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strDispGuide != value.toString())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strDispGuide = value.toString();
				}
			}
			else if (index.column() == Commands)
			{
				if (m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strCommands != value.toString())
				{
					m_pAlarmData->m_arrAlarmOfflimit[nRow]->m_strCommands = value.toString();
				}
			}

			emit dataChanged(index, index);

			return true;
		}

		return false;
	}

	return true;
}

void CAnalogAlarmLimitModel::InitPara()
{
	//m_arrInfo.clear();

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
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("Description");
	para.tagname = "Description";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEditDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(Description, pLineEditDesc);


	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	//m_arrInfo.insert(OccNo, para);
	m_horizontal_header_list.append(para.desc);
	auto pDelegateRead = new CReadOnlyDelegate(this);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(OccNo, pDelegateRead);


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
	tmp.desc = tr("ZEROTOONE");
	tmp.ID = QString("%1").arg(ZEROTOONE);
	vec.append(tmp);
	tmp.desc = tr("ONETOZERO");
	tmp.ID = QString("%1").arg(ONETOZERO);
	vec.append(tmp);
	CComboBoxDelegate *pCombo = new CComboBoxDelegate(this, vec);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(Condition, pCombo);

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
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(Category, pCombo);

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
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(SupportAck, pCheck);

	para.desc = tr("SupportDelete");
	para.tagname = "SupportDelete";
	//m_arrInfo.insert(SupportDelete, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(SupportDelete, pCheck);

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
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(AckType, pCombo);

	para.desc = tr("SoundFile");
	para.tagname = "SoundFile";
	//m_arrInfo.insert(SoundFile, para);
	m_horizontal_header_list.append(para.desc);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(SoundFile, pLineEditDesc);

	para.desc = tr("PlaySoundTimes");
	para.tagname = "PlaySoundTimes";
	//m_arrInfo.insert(PlaySoundTimes, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("SpeechAlarmText");
	para.tagname = "SpeechAlarmText";
	//m_arrInfo.insert(SpeechAlarmText, para);
	m_horizontal_header_list.append(para.desc);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(SpeechAlarmText, pLineEditDesc);

	para.desc = tr("Beep");
	para.tagname = "Beep";
	//m_arrInfo.insert(Beep, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(Beep, pCheck);

	//是否推图
	para.desc = tr("PushGraph");
	para.tagname = "PushGraph";
	//m_arrInfo.insert(PushGraph, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(PushGraph, pCheck);

	para.desc = tr("BlinkGraph");
	para.tagname = "BlinkGraph";
	//m_arrInfo.insert(BlinkGraph, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(BlinkGraph, pCheck);

	para.desc = tr("Log");
	para.tagname = "Log";
	//m_arrInfo.insert(Log, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(Log, pCheck);

	para.desc = tr("BackColor");
	para.tagname = "BackColor";
	//m_arrInfo.insert(BackColor, para);
	m_horizontal_header_list.append(para.desc);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(BackColor, pDelegateRead);

	para.desc = tr("TextColor");
	para.tagname = "TextColor";
	//m_arrInfo.insert(TextColor, para);
	m_horizontal_header_list.append(para.desc);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(TextColor, pDelegateRead);

	para.desc = tr("BlinkBackColor");
	para.tagname = "BlinkBackColor";
	//m_arrInfo.insert(BlinkBackColor, para);
	m_horizontal_header_list.append(para.desc);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(BlinkBackColor, pDelegateRead);

	para.desc = tr("BlinkTextColor");
	para.tagname = "BlinkTextColor";
	//m_arrInfo.insert(BlinkTextColor, para);
	m_horizontal_header_list.append(para.desc);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(BlinkTextColor, pDelegateRead);

	para.desc = tr("LightBoard");
	para.tagname = "LightBoard";
	//m_arrInfo.insert(LightBoard, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(LightBoard, pCheck);

	para.desc = tr("DispGuide");
	para.tagname = "DispGuide";
	//m_arrInfo.insert(DispGuide, para);
	m_horizontal_header_list.append(para.desc);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(DispGuide, pLineEditDesc);

	para.desc = tr("Commands");
	para.tagname = "Commands";
	//m_arrInfo.insert(Commands, para);
	m_horizontal_header_list.append(para.desc);
	((CAnalogAlarmLimitTable *)parent())->setItemDelegateForColumn(Commands, pLineEditDesc);
}

Qt::ItemFlags CAnalogAlarmLimitModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CAnalogAlarmLimitModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

bool CAnalogAlarmLimitModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	//auto nCount = m_pAlarmData->m_arrAlarmOfflimit.size();

	beginInsertRows(parent, position, position + rows - 1);

	Q_ASSERT(m_pFes);
	if (!m_pFes)
	{
		return false;
	}

	for (int i = position; i < position + rows; i++)
	{

		auto pAnalogLimitPoint = m_pFes->AddNewAinAlarmLimit();

		pAnalogLimitPoint->SetOccNo(i + 1);
		pAnalogLimitPoint->m_nID = i + 1;

		m_pAlarmData->m_arrAlarmOfflimit.push_back(pAnalogLimitPoint);
	}

	endInsertRows();

	return true;
}

bool CAnalogAlarmLimitModel::RemoveRows(int position, int rows, const QModelIndex &parent)
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

	auto it = std::find(m_pAlarmData->m_arrAlarmOfflimit.begin(), m_pAlarmData->m_arrAlarmOfflimit.end(), pAnalog);
	if (it != m_pAlarmData->m_arrAlarmOfflimit.end())
	{
		m_pAlarmData->m_arrAlarmOfflimit.erase(it);
	}

	QString strTmp = pAnalog->m_szTagName;
	if (!m_pFes->DeleteFesHashItem(HASHID::ANALOGALARMLIMITHASHID, strTmp))
	{
		return false;
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