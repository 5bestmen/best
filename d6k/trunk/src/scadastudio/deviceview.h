#ifndef DEVICEVIEW_H
#define DEVICEVIEW_H

#include <QMainWindow>
#include "scadastudio/base.h"

namespace Config
{
	class CDevice;
	class CFesData;
}
class IMainModuleInterface;
class CQuoteItem;
class CChannel;
class CDeviceTable;

class CDeviceView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CDeviceView(IMainModuleInterface *pCore, Config::CDevice *pDeviceData, QString &strTag,
		CQuoteItem *pItem, CChannel *pModule, Config::CFesData *pFesData);
	~CDeviceView();

	void Refresh();

	void Save()
	{

	}

	virtual void SetModifyFlag(bool bFlag)
	{
		Q_UNUSED(bFlag);
	}

	virtual bool GetModifyFlag()
	{
		return true;
	}

	virtual void Delete();

private:
	//QTabWidget *m_pTabCenter;
	//QTabWidget *m_pTabDevice;

	CDeviceTable *m_pTable;
};

#endif // DEVICEVIEW_H
