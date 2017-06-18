#ifndef SCADADIGITALALARMLIMITMODEL_H
#define SCADADIGITALALARMLIMITMODEL_H

#include <QAbstractTableModel>

namespace Config
{
	class CScadaDigitalAlarm;
	class CScadaVariableConfigData;
	class CScadaData;
}

class QStandardItem;
class IMainModuleInterface;
class CScadaDigitalAlarmTable;

class CScadaDigitalAlarmLimitModel : public QAbstractTableModel
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
	CScadaDigitalAlarmLimitModel(QObject *parent, IMainModuleInterface *pCore,
		Config::CScadaDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada);
	~CScadaDigitalAlarmLimitModel();

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
	Config::CScadaVariableConfigData *m_pConfig;
	QStringList m_horizontal_header_list;
	Config::CScadaDigitalAlarm *m_pAlarmData;
	QStandardItem *m_pItem;
	IMainModuleInterface *m_pCore;
	Config::CScadaData *m_pScada;
};

#endif // SCADADIGITALALARMLIMITMODEL_H
