#ifndef SCADAANALOGALARMTABLE_H
#define SCADAANALOGALARMTABLE_H

#include <QTableView>

namespace Config
{
	class CScadaAnalogAlarm;
	class CScadaVariableConfigData;
	class CScadaData;
}

class QStandardItem;
class IMainModuleInterface;
class CScadaAnalogAlarmModel;

class CScadaAnalogAlarmTable : public QTableView
{
	Q_OBJECT

public:
	CScadaAnalogAlarmTable(QWidget *parent, IMainModuleInterface *pCore,
		Config::CScadaAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pData);
	~CScadaAnalogAlarmTable();

	CScadaAnalogAlarmModel *GetModel()
	{
		return m_pModel;
	}

private:
	CScadaAnalogAlarmModel *m_pModel;
};

#endif // SCADAANALOGALARMTABLE_H
