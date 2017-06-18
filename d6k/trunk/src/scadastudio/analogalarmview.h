#ifndef ANALOGALARMVIEW_H
#define ANALOGALARMVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

class IMainModuleInterface;
class CAnalogAlarmTable;
class CAlarm;

namespace Config
{
	class CAnalogAlarm;
	class CFesData;
}

class QStandardItem;

class CAnalogAlarmView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CAnalogAlarmView(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
		Config::CAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes);
	~CAnalogAlarmView();

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
	CAnalogAlarmTable *m_pTable;
};

#endif // ANALOGALARMVIEW_H
