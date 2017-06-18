#ifndef DIVIEW_H
#define DIVIEW_H

#include <QMainWindow>
#include "scadastudio/base.h"

class IMainModuleInterface;
namespace Config
{
	class CDevice;
	class CFesData;
}
class CQuoteItem;
class CChannel;
class CDITable;

class CDIView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CDIView(QWidget *parent, IMainModuleInterface *pCore, Config::CDevice *pDeviceData, QString &strTag,
		CQuoteItem *pItem, CChannel *pModule, Config::CFesData *pFes);
	~CDIView();

	virtual void Save()
	{

	}
	virtual void Refresh()
	{

	}

	//module file
	virtual void SetModifyFlag(bool bFlag)
	{
		Q_UNUSED(bFlag);
	}

	virtual bool GetModifyFlag()
	{
		return m_bModify;
	}

	virtual void Delete();

private:
	CDITable *m_pTable;
};

#endif // DIVIEW_H
