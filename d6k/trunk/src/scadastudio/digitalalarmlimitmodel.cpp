#include "digitalalarmlimitmodel.h"
#include "config_data.h"
#include "scadastudiodefine.h"
#include "delegate.h"
#include "digitalalarmlimittable.h"
#include "scadastudio/icore.h"

#include <QStandardItem>

CDigitalAlarmLimitModel::CDigitalAlarmLimitModel(QObject *parent, IMainModuleInterface *pCore, CAlarm *pModule,
	Config::CDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes)
	: QAbstractTableModel(parent), m_pDigitalData(nullptr), m_pItem(nullptr), m_pFes(nullptr)
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

	if (!pFes)
	{
		return;
	}

	m_pDigitalData = pAlarmData;

	m_pItem = pItem;

	m_pFes = pFes;

	m_pCore = pCore;

	InitPara();
}

CDigitalAlarmLimitModel::~CDigitalAlarmLimitModel()
{

}

int CDigitalAlarmLimitModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_pDigitalData->m_arrDigtalOfflimit.size();
}

int CDigitalAlarmLimitModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_horizontal_header_list.size() > 0)
		return m_horizontal_header_list.size();

	return 0;
}

QVariant CDigitalAlarmLimitModel::data(const QModelIndex &index, int role) const
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
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->ID;
		}
		else if (index.column() == TagName)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->TagName;
		}
		else if (index.column() == Description)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->Description;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->OccNo;
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->BlockNo;
		}
		else if (index.column() == Condition)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->Condition;
		}
		else if (index.column() == Category)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->Category;
		}
		else if (index.column() == DelayAlarm)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->DelayAlarm;
		}
		else if (index.column() == Priority)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->Priority;
		}
		else if (index.column() == SupportAck)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->SupportAck;
		}
		else if (index.column() == SupportDelete)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->SupportDelete;
		}
		else if (index.column() == AckType)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->AckType;
		}
		else if (index.column() == SoundFile)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->SoundFile;
		}
		else if (index.column() == PlaySoundTimes)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->PlaySoundTimes;
		}
		else if (index.column() == SpeechAlarmText)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->SpeechAlarmText;
		}
		else if (index.column() == Beep)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->Beep;
		}
		else if (index.column() == PushGraph)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->PushGraph;
		}
		else if (index.column() == BlinkGraph)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->BlinkGraph;
		}
		else if (index.column() == Log)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->Log;
		}
		else if (index.column() == BackColor)
		{
			//return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->BackColor;
		}
		else if (index.column() == TextColor)
		{
			//return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->TextColor;
		}
		else if (index.column() == BlinkBackColor)
		{
			//return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->BlinkBackColor;
		}
		else if (index.column() == BlinkTextColor)
		{
			//return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->BlinkTextColor;
		}
		else if (index.column() == DispGuide)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->DispGuide;
		}
		else if (index.column() == Commands)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->Commands;
		}

		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == ID)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->ID;
		}
		else if (index.column() == TagName)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->TagName;
		}
		else if (index.column() == Description)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->Description;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->OccNo;
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->BlockNo;
		}
		else if (index.column() == Condition)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->Condition;
		}
		else if (index.column() == Category)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->Category;
		}
		else if (index.column() == DelayAlarm)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->DelayAlarm;
		}
		else if (index.column() == Priority)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->Priority;
		}
		else if (index.column() == SupportAck)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->SupportAck;
		}
		else if (index.column() == SupportDelete)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->SupportDelete;
		}
		else if (index.column() == AckType)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->AckType;
		}
		else if (index.column() == SoundFile)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->SoundFile;
		}
		else if (index.column() == PlaySoundTimes)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->PlaySoundTimes;
		}
		else if (index.column() == SpeechAlarmText)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->SpeechAlarmText;
		}
		else if (index.column() == Beep)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->Beep;
		}
		else if (index.column() == PushGraph)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->PushGraph;
		}
		else if (index.column() == BlinkGraph)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->BlinkGraph;
		}
		else if (index.column() == Log)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->Log;
		}
		else if (index.column() == BackColor)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->BackColor;
		}
		else if (index.column() == TextColor)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->TextColor;
		}
		else if (index.column() == BlinkBackColor)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->BlinkBackColor;
		}
		else if (index.column() == BlinkTextColor)
		{
			return (uint)m_pDigitalData->m_arrDigtalOfflimit[nRow]->BlinkTextColor;
		}
		else if (index.column() == DispGuide)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->DispGuide;
		}
		else if (index.column() == Commands)
		{
			return m_pDigitalData->m_arrDigtalOfflimit[nRow]->Commands;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CDigitalAlarmLimitModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool CDigitalAlarmLimitModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			if (index.column() == ID)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->ID != value.toInt())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->ID = value.toInt();
				}
			}
			else if (index.column() == TagName)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->TagName != value.toString())
				{
					//memset(m_pAlarmData->m_arrDigtalOfflimit[nRow]->TagName, 0, sizeof(m_pAlarmData->m_arrDigtalOfflimit[nRow]->TagName));
					//int nSize = value.toString().size();
					//strncpy(m_pAlarmData->m_arrDigtalOfflimit[nRow]->TagName, value.toString().toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));

					if (m_pFes->CheckTagNameIsExist(value.toString()))
					{
						auto strError = tr("digital alarm limit tagname %1 exist!!!").arg(value.toString());
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
						
						return true;
					}

					m_pItem->setData(value, Qt::EditRole);
				}
			}
			else if (index.column() == Description)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->Description != value.toString())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->Description = value.toString();

					m_pItem->setData(value, TREE_TOOL_TIP_ROLE);
				}
			}
			else if (index.column() == Condition)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->Condition != value.toInt())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->Condition = value.toInt();
				}
			}
			else if (index.column() == Category)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->Category != value.toInt())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->Category = value.toInt();
				}
			}
			else if (index.column() == DelayAlarm)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->DelayAlarm != value.toInt())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->DelayAlarm = value.toInt();
				}
			}
			else if (index.column() == Priority)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->Priority != value.toInt())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->Priority = value.toInt();
				}
			}
			else if (index.column() == SupportAck)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->SupportAck != value.toBool())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->SupportAck = value.toBool();
				}
			}
			else if (index.column() == SupportDelete)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->SupportDelete != value.toBool())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->SupportDelete = value.toBool();
				}
			}
			else if (index.column() == AckType)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->AckType != value.toInt())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->AckType = value.toInt();
				}
			}
			else if (index.column() == SoundFile)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->SoundFile != value.toString())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->SoundFile = value.toString();
				}
			}
			else if (index.column() == PlaySoundTimes)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->PlaySoundTimes != value.toInt())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->PlaySoundTimes = value.toInt();
				}
			}
			else if (index.column() == SpeechAlarmText)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->SpeechAlarmText != value.toString())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->SpeechAlarmText = value.toString();
				}
			}
			else if (index.column() == Beep)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->Beep != value.toBool())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->Beep = value.toBool();
				}
			}
			else if (index.column() == PushGraph)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->PushGraph != value.toString())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->PushGraph = value.toString();
				}
			}
			else if (index.column() == BlinkGraph)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->BlinkGraph != value.toBool())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->BlinkGraph = value.toBool();
				}
			}
			else if (index.column() == Log)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->Log != value.toBool())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->Log = value.toBool();
				}
			}
			else if (index.column() == BackColor)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->BackColor != value.toInt())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->BackColor = value.toInt();
				}
			}
			else if (index.column() == TextColor)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->TextColor != value.toInt())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->TextColor = value.toInt();
				}
			}
			else if (index.column() == BlinkBackColor)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->BlinkBackColor != value.toInt())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->BlinkBackColor = value.toInt();
				}
			}
			else if (index.column() == BlinkTextColor)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->BlinkTextColor != value.toInt())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->BlinkTextColor = value.toInt();
				}
			}
			else if (index.column() == DispGuide)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->DispGuide != value.toString())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->DispGuide = value.toString();
				}
			}
			else if (index.column() == Commands)
			{
				if (m_pDigitalData->m_arrDigtalOfflimit[nRow]->Commands != value.toString())
				{
					m_pDigitalData->m_arrDigtalOfflimit[nRow]->Commands = value.toString();
				}
			}

			emit dataChanged(index, index);

			return true;
		}

		return false;
	}

	return false;
}

Qt::ItemFlags CDigitalAlarmLimitModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CDigitalAlarmLimitModel::InitPara()
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
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("Description");
	para.tagname = "Description";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEditDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(Description, pLineEditDesc);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	//m_arrInfo.insert(OccNo, para);
	m_horizontal_header_list.append(para.desc);
	auto pReadOnly = new CReadOnlyDelegate(this);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnly);

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
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(Condition, pCombo);

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
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(Category, pCombo);

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
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(SupportAck, pCheck);

	para.desc = tr("SupportDelete");
	para.tagname = "SupportDelete";
	//m_arrInfo.insert(SupportDelete, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(SupportDelete, pCheck);

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
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(AckType, pCombo);

	para.desc = tr("SoundFile");
	para.tagname = "SoundFile";
	//m_arrInfo.insert(SoundFile, para);
	m_horizontal_header_list.append(para.desc);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(SoundFile, pLineEditDesc);

	para.desc = tr("PlaySoundTimes");
	para.tagname = "PlaySoundTimes";
	//m_arrInfo.insert(PlaySoundTimes, para);
	m_horizontal_header_list.append(para.desc);

	para.desc = tr("SpeechAlarmText");
	para.tagname = "SpeechAlarmText";
	//m_arrInfo.insert(SpeechAlarmText, para);
	m_horizontal_header_list.append(para.desc);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(SpeechAlarmText, pLineEditDesc);

	para.desc = tr("Beep");
	para.tagname = "Beep";
	//m_arrInfo.insert(Beep, para);
	m_horizontal_header_list.append(para.desc);
	pCheck = new CCheckBoxDelegate(this);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(Beep, pCheck);

	para.desc = tr("PushGraph");
	para.tagname = "PushGraph";
	//m_arrInfo.insert(PushGraph, para);
	m_horizontal_header_list.append(para.desc);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(PushGraph, pCheck);

	para.desc = tr("BlinkGraph");
	para.tagname = "BlinkGraph";
	//m_arrInfo.insert(BlinkGraph, para);
	m_horizontal_header_list.append(para.desc);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(BlinkGraph, pCheck);

	para.desc = tr("Log");
	para.tagname = "Log";
	//m_arrInfo.insert(Log, para);
	m_horizontal_header_list.append(para.desc);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(Log, pCheck);

	para.desc = tr("BackColor");
	para.tagname = "BackColor";
	//m_arrInfo.insert(BackColor, para);
	m_horizontal_header_list.append(para.desc);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(BackColor, pReadOnly);

	para.desc = tr("TextColor");
	para.tagname = "TextColor";
	//m_arrInfo.insert(TextColor, para);
	m_horizontal_header_list.append(para.desc);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(TextColor, pReadOnly);

	para.desc = tr("BlinkBackColor");
	para.tagname = "BlinkBackColor";
	//m_arrInfo.insert(BlinkBackColor, para);
	m_horizontal_header_list.append(para.desc);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(BlinkBackColor, pReadOnly);

	para.desc = tr("BlinkTextColor");
	para.tagname = "BlinkTextColor";
	//m_arrInfo.insert(BlinkTextColor, para);
	m_horizontal_header_list.append(para.desc);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(BlinkTextColor, pReadOnly);

	para.desc = tr("DispGuide");
	para.tagname = "DispGuide";
	//m_arrInfo.insert(DispGuide, para);
	m_horizontal_header_list.append(para.desc);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(DispGuide, pLineEditDesc);

	para.desc = tr("Commands");
	para.tagname = "Commands";
	//m_arrInfo.insert(Commands, para);
	m_horizontal_header_list.append(para.desc);
	((CDigitalAlarmLimitTable *)parent())->setItemDelegateForColumn(Commands, pLineEditDesc);
}

void CDigitalAlarmLimitModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

bool CDigitalAlarmLimitModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	//auto nCount = m_pDigitalData->m_arrDigtalOfflimit.size();

	beginInsertRows(parent, position, position + rows - 1);

	Q_ASSERT(m_pFes);
	if (!m_pFes)
	{
		return false;
	}

	for (int i = position; i < position + rows; i++)
	{

		auto pDigitalLimitPoint = m_pFes->AddNewDigitalAlarmLimit();

		pDigitalLimitPoint->OccNo = (i + 1);
		pDigitalLimitPoint->ID = i + 1;

		m_pDigitalData->m_arrDigtalOfflimit.push_back(pDigitalLimitPoint);
	}

	endInsertRows();

	return true;
}

bool CDigitalAlarmLimitModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, rows, rows);

	auto nCount = m_pDigitalData->m_arrDigtalOfflimit.size();

	auto bFlag = position < 0 || position >= nCount;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		return false;
	}

	auto pAnalog = m_pDigitalData->m_arrDigtalOfflimit[position];

	auto it = std::find(m_pDigitalData->m_arrDigtalOfflimit.begin(), m_pDigitalData->m_arrDigtalOfflimit.end(), pAnalog);
	if (it != m_pDigitalData->m_arrDigtalOfflimit.end())
	{
		m_pDigitalData->m_arrDigtalOfflimit.erase(it);
	}

	QString strTmp = pAnalog->TagName;
	if (!m_pFes->DeleteFesHashItem(HASHID::DIGITALALARMLIMITHASHID, strTmp))
	{
		endRemoveRows();

		RefrushModel();

		return false;
	}


	delete pAnalog;
	pAnalog = nullptr;

	endRemoveRows();

	if (m_pDigitalData->m_arrDigtalOfflimit.size() == 0)
	{
		RefrushModel();
	}

	return true;
}