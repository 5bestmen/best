#ifndef FESVIEW_H
#define FESVIEW_H

#include <QMainWindow>
#include "config_data.h"
#include "scadastudio/base.h"

class CQuoteItem;
class CFesTable;
class IMainModuleInterface;
class QStandardItem;
namespace Config
{
	class CFesConfigData;
}


class CFesView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CFesView(IMainModuleInterface *pCore, Config::CFesData *pFesData, CQuoteItem *pItem
		, Config::CFesConfigData *pFesConfig);
	~CFesView();

	virtual void Save();

	virtual void Refresh();

	virtual void Delete();

	virtual void SetModifyFlag(bool bFlag)
	{
		Q_UNUSED(bFlag);
	}

	virtual bool GetModifyFlag()
	{
		return true;
	}

public slots:
	//void ItemChanged(QStandardItem *item);

private:
	CFesTable *m_pTable;

	Config::CFesData *m_pFesData;

	IMainModuleInterface *m_pCore;
};

#endif // FESVIEW_H
