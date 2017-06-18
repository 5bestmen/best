#ifndef FORWARDING_AOVIEW_H
#define FORWARDING_AOVIEW_H

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
class CForwardingAOTable;

class CForwardingAOView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CForwardingAOView(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes);
	~CForwardingAOView();

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
	CForwardingAOTable *m_pTable;
};

#endif // FORWARDING_AOVIEW_H
