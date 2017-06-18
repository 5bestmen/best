#ifndef FORWARDING_KWHVIEW_H
#define FORWARDING_KWHVIEW_H

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
class CForwardingKWHTable;

class CForwardingKWHView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CForwardingKWHView(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes);
	~CForwardingKWHView();

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
	CForwardingKWHTable *m_pTable;
};

#endif // FORWARDING_KWHVIEW_H
