#ifndef CFORWARIDNGCHANNELVIEW_H
#define CFORWARIDNGCHANNELVIEW_H

#include <QMainWindow>
#include "scadastudio/base.h"
#include "scadastudio/quoteItem.h"

namespace Config
{
	class CForwardingChannelData;
	class CFesData;
}

class CForwardingChannelTable;
class CQuoteItem;
class IMainModuleInterface;

class CForwardingChannelView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CForwardingChannelView(QWidget *parent, Config::CForwardingChannelData *pChannel, CQuoteItem *pItem, Config::CFesData *pFes, IMainModuleInterface *pCore);
	~CForwardingChannelView();

	void Refresh();

	void Save();

	virtual void SetModifyFlag(bool bFlag)
	{
		Q_UNUSED(bFlag);
	}

	virtual bool GetModifyFlag()
	{
		return true;
	}

	virtual void Delete();

public slots:
	//void ItemChanged(QStandardItem *item);
private:
	CForwardingChannelTable *m_pTable;

	Config::CForwardingChannelData *m_pChannel;

	CQuoteItem *m_pItem;

	Config::CFesData *m_pFes;

public:
	int GetNumber() {return m_nNumber;}
private:
	int m_nNumber;
};

#endif // CFORWARIDNGCHANNELVIEW_H
