#ifndef ANALOGALARMMODEL_H
#define ANALOGALARMMODEL_H

#include <QAbstractTableModel>
#include "scadastudiodefine.h"

class IMainModuleInterface;
class CAlarm;

namespace Config
{
	class CAnalogAlarm;
	class CFesData;
}

class QStandardItem;
class CAnalogAlarmModel : public QAbstractTableModel
{
	Q_OBJECT

	enum COLUMN
	{
		ID, TagName, Description, OccNo, Enable, OnQualityGood, DeadArea, DeadType, GroupName
	};
public:
	CAnalogAlarmModel(QObject *parent, IMainModuleInterface *pCore, CAlarm *pModule,
		Config::CAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes);
	~CAnalogAlarmModel();

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
	QStringList m_horizontal_header_list;
	Config::CAnalogAlarm *m_pAlarmData;
	QMap<int, CONFIG_PARA> m_arrInfo;
	QVector<IDDESC> m_arrDeadType;
	QStandardItem *m_pItem;
	Config::CFesData *m_pFes;
};

#endif // ANALOGALARMMODEL_H
