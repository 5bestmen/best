#ifndef SCADADIGITALALARMVIEW_H
#define SCADADIGITALALARMVIEW_H

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
class CScadaDigitalAlarmTable;

class CScadaDigitalAlarmView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CScadaDigitalAlarmView(QWidget *parent, IMainModuleInterface *pCore,
		Config::CScadaDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pData);
	~CScadaDigitalAlarmView();

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
	CScadaDigitalAlarmTable *m_pTable;
};

#endif // SCADADIGITALALARMVIEW_H
