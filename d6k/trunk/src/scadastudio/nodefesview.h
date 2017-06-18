#ifndef NODEFESVIEW_H
#define NODEFESVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

class IMainModuleInterface;
namespace Config
{
	class CNode;
	class CNodeConfigData;
};

class CNodeFesTable;
class CQuoteItem;

class CNodeFesView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CNodeFesView(QWidget *parent, IMainModuleInterface *pCore, Config::CNode *pData
		, CQuoteItem *pItem, Config::CNodeConfigData *pNodeConfig);
	~CNodeFesView();

	virtual void Save()
	{

	}

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

private:
	CNodeFesTable *m_pTable;
};

#endif // NODEFESVIEW_H
