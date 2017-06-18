#ifndef SCADADIGITALALARMLIMITVIEW_H
#define SCADADIGITALALARMLIMITVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

namespace Config
{
	class CScadaDigitalAlarm;
	class CScadaVariableConfigData;
	class CScadaData;
}

class QStandardItem;
class IMainModuleInterface;
class CScadaDigitalAlarmLimitTable;

class CScadaDigitalAlarmLimitView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CScadaDigitalAlarmLimitView(QWidget *parent, IMainModuleInterface *pCore,
		Config::CScadaDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada);
	~CScadaDigitalAlarmLimitView();

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
	CScadaDigitalAlarmLimitTable *m_pTable;
};

#endif // SCADADIGITALALARMLIMITVIEW_H
