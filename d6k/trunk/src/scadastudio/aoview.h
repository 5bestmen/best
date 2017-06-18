#ifndef AOVIEW_H
#define AOVIEW_H

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
class CAOTable;

class CAOView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CAOView(QWidget *parent, IMainModuleInterface *pCore, Config::CDevice *pDeviceData, QString &strTag,
		CQuoteItem *pItem, CChannel *pModule, Config::CFesData *pFes);
	~CAOView();

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
	CAOTable *m_pTable;
};

#endif // AOVIEW_H
