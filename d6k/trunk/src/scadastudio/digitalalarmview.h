#ifndef DIGITALALARMVIEW_H
#define DIGITALALARMVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

class IMainModuleInterface;
class CDigitalAlarmTable;
class CAlarm;

namespace Config
{
	class CDigitalAlarm;
	class CFesData;
}

class QStandardItem;

class CDigitalAlarmView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CDigitalAlarmView(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
		Config::CDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes);
	~CDigitalAlarmView();

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
	CDigitalAlarmTable *m_pTable;
};

#endif // DIGITALALARMVIEW_H
