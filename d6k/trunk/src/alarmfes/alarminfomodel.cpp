

#include "alarminfomodel.h"
#include <QTimer>
#include <QIcon>

CAlarmInfoModel::CAlarmInfoModel(QObject *parent, std::shared_ptr<CAlarmInfoQueue>& AlarmQueue)
	: QAbstractTableModel(parent), m_AlarmQueue(AlarmQueue)
{
	m_RedIcon = QIcon(":/Resources/red_light.png");
	m_GreenIcon=QIcon(":/Resources/green_light.png");
	m_nCount = 1;
}

CAlarmInfoModel::~CAlarmInfoModel()
{

}


int CAlarmInfoModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return m_nColCount;
}

int CAlarmInfoModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return m_nRowCount;
}

QVariant CAlarmInfoModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	return ShowData(index.row(),index.column(),role);
}

QVariant CAlarmInfoModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (role==Qt::TextAlignmentRole)
	{
		return int(Qt::AlignHCenter | Qt::AlignVCenter);
	}
	if (role != Qt::DisplayRole)
	{
		return QVariant();
	}
	if (m_szHeaderList.isEmpty())
	{
		return QVariant();
	}
	if (orientation==Qt::Horizontal)
	{
		return m_szHeaderList.at(section);
	}
	else
	{
		return QString("%1").arg(section);
	}
	return QVariant();
}

/*! \fn QVariant CAlarmInfoModel::ShowData(int row, int col) const
********************************************************************************************************* 
** \brief CAlarmInfoModel::ShowData 
** \details 
** \param row 
** \param col 
** \return QT_NAMESPACE::QVariant 
** \author xingzhibing
** \date 2016年10月10日 
** \note 
********************************************************************************************************/
QVariant CAlarmInfoModel::ShowData(int row, int col, int role) const
{
	if ( m_AlarmQueue->empty() == true)
	{
		return QVariant();
	}
	m_nCount++;

	std::shared_ptr < CAlarmDataConfig > pMsg;

	m_AlarmQueue->at(row,pMsg);

	Q_ASSERT(pMsg);

	if (!pMsg)
	{
		return QVariant();
	}	
	if (role==Qt::DisplayRole)
	{
		switch (col)
		{
		case ALARM_STATE:
			return QVariant();
		case ALARM_TIME:
			return pMsg->m_szTimeTag;
		case ALARM_TYPE:
			return GetEventType(pMsg->m_Msg.Type);
		case ALARM_VALUE:
			return pMsg->m_fVal;
		case ALARM_DESC:
			return QString::fromLocal8Bit(pMsg->m_szContent);
		default:
			break;
		}
	}
	if (role==Qt::DecorationRole)
	{
		if (col== ALARM_STATE)
		{
			//已经确认
			if (pMsg->m_nAlarmState==1)
			{
				return m_GreenIcon;
			}
			else
			{
				if ( m_nCount%2 == 1 )
				{
					return m_RedIcon;
				} 
				else
				{
					return m_GreenIcon;
				}				
			}		
		}
	}
	if (role == Qt::TextAlignmentRole)
	{
		switch (col)
		{
		case ALARM_STATE:
			return Qt::AlignCenter;
		case ALARM_TIME:
			break;
		case ALARM_TYPE:
			break;
		case ALARM_VALUE:
			return Qt::AlignCenter;
		case ALARM_DESC:
			return Qt::AlignLeft;
		default:
			break;
		}
	}
	return QVariant();
}

QString CAlarmInfoModel::GetEventType(int nType) const 
{
	if (nType== MSG_EVT0)
	{
		return QObject::tr("MSG_EVT0");
	}
	else if (nType== MSG_EVT_ANA)
	{
		return QObject::tr("MSG_EVT_ANA");
	}
	else if (nType== MSG_EVT_DIGI)
	{
		return QObject::tr("MSG_EVT_DIGI");
	}
	else if (nType== MSG_EVT_DIAG)
	{
		return QObject::tr("MSG_EVT_DIAG");
	}
	else if (nType == MSG_EVT_OPER)
	{
		return QObject::tr("MSG_EVT_OPER");
	}
	else if (nType == MSG_EVT_SETVAL)
	{
		return QObject::tr("MSG_EVT_SETVAL");
	}
	else if (nType==MSG_EVT6)
	{
		return QObject::tr("MSG_SOE");
	}
	else if (nType==MSG_EVT7)
	{
		return QObject::tr("MSG_TELE_CTRL");
	}
	return QObject::tr("MSG_NULL");
}
