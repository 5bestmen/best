#ifndef ANALOGALARMLIMITVIEW_H
#define ANALOGALARMLIMITVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

class IMainModuleInterface;
class CAnalogAlarmLimitTable;
class CAlarm;

namespace Config
{
	class CAinAlarmLimit;
	class CFesData;
	class CAnalogAlarm;
}

class QStandardItem;

class CAnalogAlarmLimitView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CAnalogAlarmLimitView(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
		Config::CAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes);
	~CAnalogAlarmLimitView();

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
	CAnalogAlarmLimitTable *m_pTable;
};

#endif // ANALOGALARMLIMITVIEW_H
