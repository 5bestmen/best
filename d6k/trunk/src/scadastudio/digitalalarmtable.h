#ifndef DIGITALALARMTABLE_H
#define DIGITALALARMTABLE_H

#include <QTableView>

class IMainModuleInterface;
class CAlarm;

namespace Config
{
	class CDigitalAlarm;
	class CFesData;
}

class QStandardItem;
class CDigitalAlarmModel;

class CDigitalAlarmTable : public QTableView
{
	Q_OBJECT

public:
	CDigitalAlarmTable(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
		Config::CDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes);
	~CDigitalAlarmTable();

	CDigitalAlarmModel *GetModel()
	{
		return m_pModel;
	}

private:
	CDigitalAlarmModel *m_pModel;
};

#endif // DIGITALALARMTABLE_H
