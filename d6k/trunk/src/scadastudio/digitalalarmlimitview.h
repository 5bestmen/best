#ifndef DIGITALALARMLIMITVIEW_H
#define DIGITALALARMLIMITVIEW_H

#include "scadastudio/base.h"
#include <QMainWindow>

class IMainModuleInterface;
class CDigitalAlarmLimitTable;
class CAlarm;

namespace Config
{
	class CDigitalAlarm;
	class CFesData;
}

class QStandardItem;

class CDigitalAlarmLimitView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CDigitalAlarmLimitView(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
		Config::CDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes);
	~CDigitalAlarmLimitView();

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
	CDigitalAlarmLimitTable *m_pTable;
};

#endif // DIGITALALARMLIMITVIEW_H
