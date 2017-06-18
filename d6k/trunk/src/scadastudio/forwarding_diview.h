#ifndef FORWARDING_DIVIEW_H
#define FORWARDING_DIVIEW_H

#include <QMainWindow>
#include "scadastudio/base.h"

class IMainModuleInterface;
namespace Config
{
	class CForwardingChannelData;
	class CFesData;
}
class CQuoteItem;
class CChannel;
class CForwardingDITable;

class CForwardingDIView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CForwardingDIView(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData
		, Config::CFesData *pFes);
	~CForwardingDIView();

	virtual void Save()
	{

	}
	virtual void Refresh();

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
	CForwardingDITable *m_pTable;
};

#endif // FORWARDING_DIVIEW_H
