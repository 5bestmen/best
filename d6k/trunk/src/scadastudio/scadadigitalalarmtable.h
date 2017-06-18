#ifndef SCADADIGITALALARMTABLE_H
#define SCADADIGITALALARMTABLE_H

#include <QTableView>

namespace Config
{
	class CScadaDigitalAlarm;
	class CScadaVariableConfigData;
	class CScadaData;
}

class QStandardItem;
class IMainModuleInterface;
class CScadaDigitalAlarmModel;

class CScadaDigitalAlarmTable : public QTableView
{
	Q_OBJECT

public:
	CScadaDigitalAlarmTable(QWidget *parent, IMainModuleInterface *pCore,
		Config::CScadaDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pData);
	~CScadaDigitalAlarmTable();

	CScadaDigitalAlarmModel *GetModel()
	{
		return m_pModel;
	}

	void Refresh();

private:
	CScadaDigitalAlarmModel *m_pModel;
};

#endif // SCADADIGITALALARMTABLE_H
