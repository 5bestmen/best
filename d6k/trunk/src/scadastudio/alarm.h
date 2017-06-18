#ifndef ALARM_H
#define ALARM_H

#include <QObject>
#include <QModelIndex>
#include "config_data.h"

class IMainWindow;
class QStandardItem;
class IMainModuleInterface;
class CAttriwindow;
class QtProperty;

class CAlarm : public QObject
{
	Q_OBJECT

	enum ALARM_CHILD
	{
		LIMIT
	};
public:
	CAlarm(IMainModuleInterface *pCore);
	~CAlarm();

	void AddAnalogAlarmGroup(QModelIndex &index);
	void AddDigtalAlarmGroup(QModelIndex &index);

	void ShowMenu(QModelIndex &index);

	void AddAnalogAlarm(QModelIndex &index);

	void AddDigtalAlarm(QModelIndex &index);

	void Disconnect();

public slots:
	void showMouseRightButton(const QPoint& point);

	void doubleClicked(const QModelIndex &index);

	void ItemChanged(QStandardItem *item);

	void Clicked(const QModelIndex &index);
	void Expanded(const QModelIndex &index);
	void Collapsed(const QModelIndex &index);

private:
	void CreateAttriConnect();

	void CreateAttri(Config::CAnalogAlarm *pAnalogAlarm);
	void CreateAttri(Config::CAinAlarmLimit *pAnalogAlarmLimit);

	void CreateAttri(Config::CDigitalAlarm *pDigitalAlarm);
	void CreateAttri(Config::CDinAlarmLimit *pDigitalAlarmLimit);

	bool CreateDigtalAlarm(Config::CFesData *pFes, Config::CDigitalAlarm *&pDigtal);
	bool FindAnalogGroup(QModelIndex &index, QString &strGroup);
	void FindAnalogAlarm(const QAbstractItemModel *pModel, const QModelIndex & top, QVector<QModelIndex> &vec);

	void AddAnalogOfflimit(QModelIndex &index);
	void AddDigtalOfflimit(QModelIndex &index);
	bool CreateDigtalAlarmOfflimit(Config::CDigitalAlarm *pDigtalAlarm, Config::CDinAlarmLimit *&pDigtalAlarmOfflimit);

	void DeleteAnalogAlarmGroup(QModelIndex &index);
	bool DeleteAnalogAlarmModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes);
	bool DeleteAnalogAlarmLimitModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes);
	void DeleteAnalogAlarmItem(QModelIndex &index);
	void DeleteAnalogAlarmLimitItem(QModelIndex &index);

	void DeleteDigitalAlarmGroup(QModelIndex &index);
	bool DeleteDigitalAlarmModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes);
	bool DeleteDigitalAlarmLimitModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes);
	void DeleteDigitalAlarmItem(QModelIndex &index);
	void DeleteDigitalAlarmLimitItem(QModelIndex &index);
private:
	IMainWindow *m_pUi;

	IMainModuleInterface *m_pCore;

	//std::vector<Config::CFesData *> &m_arrFes;

	bool m_bConnectAttr;

	CAttriwindow *m_pAttr;

	QMap<QtProperty *, QString> m_propertyToId;
	QMap<QString, QtProperty *> m_idToProperty;
};

#endif // ALARM_H
