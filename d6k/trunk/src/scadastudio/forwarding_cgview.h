#ifndef FORWARDING_CGVIEW_H
#define FORWARDING_CGVIEW_H

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
class CForwardingCGTable;

class CForwardingCGView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CForwardingCGView(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes);
	~CForwardingCGView();

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
	CForwardingCGTable *m_pTable;
};

#endif // FORWARDING_CGVIEW_H
