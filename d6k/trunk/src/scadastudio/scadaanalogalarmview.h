#ifndef SCADAANALOGALARMVIEW_H
#define SCADAANALOGALARMVIEW_H

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
class CScadaAnalogAlarmTable;

class CScadaAnalogAlarmView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CScadaAnalogAlarmView(QWidget *parent, IMainModuleInterface *pCore,
		Config::CScadaAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pData);
	~CScadaAnalogAlarmView();

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
	CScadaAnalogAlarmTable *m_pTable;
};

#endif // SCADAANALOGALARMVIEW_H
