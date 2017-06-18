#ifndef DIGITALALARMLIMITMODEL_H
#define DIGITALALARMLIMITMODEL_H

#include <QAbstractTableModel>

class IMainModuleInterface;
class CDigitalAlarmLimitTable;
class CAlarm;

namespace Config
{
	class CDigitalAlarm;
	class CFesData;
}

class QStandardItem;

class CDigitalAlarmLimitModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN
	{
		ID, TagName, Description, OccNo, BlockNo, Condition, Category, DelayAlarm, Priority, SupportAck, 
		SupportDelete, AckType, SoundFile, PlaySoundTimes, SpeechAlarmText, Beep, PushGraph, 
		BlinkGraph, Log, BackColor, TextColor, BlinkBackColor, BlinkTextColor, DispGuide, Commands
	};
public:
	CDigitalAlarmLimitModel(QObject *parent, IMainModuleInterface *pCore, CAlarm *pModule,
		Config::CDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes);
	~CDigitalAlarmLimitModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	Qt::ItemFlags flags(const QModelIndex &index) const;

	void RefrushModel();

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

private:
	void InitPara();
private:
	QStringList m_horizontal_header_list;
	Config::CDigitalAlarm *m_pDigitalData;
	QStandardItem *m_pItem;
	Config::CFesData *m_pFes;
	IMainModuleInterface *m_pCore;
};

#endif // DIGITALALARMLIMITMODEL_H
