#ifndef DIGITALALARMMODEL_H
#define DIGITALALARMMODEL_H

#include <QAbstractTableModel>

class IMainModuleInterface;
class CAlarm;

namespace Config
{
	class CDigitalAlarm;
	class CFesData;
}

class QStandardItem;
class IMainModuleInterface;

class CDigitalAlarmModel : public QAbstractTableModel
{
	Q_OBJECT

	enum COLUMN
	{
		ID, TagName, Description, OccNo, Enable, OnQualityGood
	};

public:
	CDigitalAlarmModel(QObject *parent, IMainModuleInterface *pCore, CAlarm *pModule,
		Config::CDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes);
	~CDigitalAlarmModel();

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
	Config::CDigitalAlarm *m_pAlarmData;
	QStandardItem *m_pItem;
	Config::CFesData *m_pFes;
	IMainModuleInterface *m_pCore;
};

#endif // DIGITALALARMMODEL_H
