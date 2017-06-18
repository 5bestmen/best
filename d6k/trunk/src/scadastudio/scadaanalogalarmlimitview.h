#ifndef SCADAANALOGALARMLIMITVIEW_H
#define SCADAANALOGALARMLIMITVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

namespace Config
{
	class CScadaAnalogAlarm;
	class CScadaVariableConfigData;
	class CScadaData;
}

class QStandardItem;
class IMainModuleInterface;
class CScadaAnalogAlarmLimitTable;

class CScadaAnalogAlarmLimitView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CScadaAnalogAlarmLimitView(QWidget *parent, IMainModuleInterface *pCore,
		Config::CScadaAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada);
	~CScadaAnalogAlarmLimitView();

	virtual void Save()
	{

	}

	virtual void Refresh();

	virtual void Delete();

	virtual void SetModifyFlag(bool bFlag)
	{
		Q_UNUSED(bFlag);
	}

	virtual bool GetModifyFlag()
	{
		return true;
	}

private:
	CScadaAnalogAlarmLimitTable *m_pTable;
};

#endif // SCADAANALOGALARMLIMITVIEW_H
