#ifndef ANALOGALARMTABLE_H
#define ANALOGALARMTABLE_H

#include <QTableView>

class IMainModuleInterface;
class CAlarm;
class CAnalogAlarmModel;

namespace Config
{
	class CAnalogAlarm;
	class CFesData;
}

class QStandardItem;

class CAnalogAlarmTable : public QTableView
{
	Q_OBJECT

public:
	CAnalogAlarmTable(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
		Config::CAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes);
	~CAnalogAlarmTable();

	CAnalogAlarmModel *GetModel()
	{
		return m_pModel;
	}

	void Refresh();

private:
	CAnalogAlarmModel *m_pModel;
};

#endif // ANALOGALARMTABLE_H
