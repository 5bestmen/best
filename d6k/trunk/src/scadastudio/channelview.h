#ifndef CCHANNELVIEW_H
#define CCHANNELVIEW_H

#include <QMainWindow>
#include "config_data.h"
#include "scadastudio/base.h"
#include "scadastudio/quoteItem.h"

class CChannelTable;
class CQuoteItem;
class CChannel;
class QStandardItem;
class IMainModuleInterface;

class CChannelView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CChannelView(QWidget *parent, Config::CChannelData *pChannel, CQuoteItem *pItem, CChannel *pChannelModel, Config::CFesData *m_pFes, IMainModuleInterface *pCore);
	~CChannelView();

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
	CChannelTable *m_pTable;

	Config::CChannelData *m_pChannel;

	CQuoteItem *m_pItem;

	Config::CFesData *m_pFes;

public:
	int GetNumber() {return m_nNumber;}
private:
	int m_nNumber;
};

#endif // CCHANNELVIEW_H
