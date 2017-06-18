#ifndef FORWARDING_DOVIEW_H
#define FORWARDING_DOVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>


class IMainModuleInterface;
namespace Config
{
	class CForwardingChannelData;
	class CFesData;
}
class CQuoteItem;
class CChannel;
class CForwardingDOTable;

class CForwardingDOView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CForwardingDOView(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes);
	~CForwardingDOView();

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
	CForwardingDOTable *m_pTable;
};

#endif // FORWARDING_DOVIEW_H
