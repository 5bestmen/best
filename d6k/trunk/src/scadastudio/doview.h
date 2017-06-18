#ifndef DOVIEW_H
#define DOVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>


class IMainModuleInterface;
namespace Config
{
	class CDevice;
	class CFesData;
}
class CQuoteItem;
class CChannel;
class CDOTable;

class CDOView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CDOView(QWidget *parent, IMainModuleInterface *pCore, Config::CDevice *pDeviceData, QString &strTag,
		CQuoteItem *pItem, CChannel *pModule, Config::CFesData *pFes);
	~CDOView();

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
	CDOTable *m_pTable;
};

#endif // DOVIEW_H
