#ifndef SCADAANALOGALARMMODEL_H
#define SCADAANALOGALARMMODEL_H

#include <QAbstractTableModel>

namespace Config
{
	class CScadaAnalogAlarm;
	class CScadaVariableConfigData;
	class CScadaData;
}

class QStandardItem;
class IMainModuleInterface;

class CScadaAnalogAlarmModel : public QAbstractTableModel
{
	Q_OBJECT

	enum COLUMN
	{
		ID, TagName, Description, OccNo, Enable, OnQualityGood, DeadArea, DeadType, GroupName
	};

public:
	CScadaAnalogAlarmModel(QObject *parent, IMainModuleInterface *pCore,
		Config::CScadaAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pData);
	~CScadaAnalogAlarmModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	Qt::ItemFlags flags(const QModelIndex &index) const;

	void RefrushModel();

private:
	void InitPara();
private:
	Config::CScadaVariableConfigData *m_pConfig;
	QStringList m_horizontal_header_list;
	Config::CScadaAnalogAlarm *m_pAlarmData;
	QStandardItem *m_pItem;
	Config::CScadaData *m_pData;
	IMainModuleInterface *m_pCore;
};

#endif // SCADAANALOGALARMMODEL_H
