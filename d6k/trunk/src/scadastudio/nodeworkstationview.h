#ifndef CNODEWORKSTATIONVIEW_H
#define CNODEWORKSTATIONVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

class IMainModuleInterface;

namespace Config
{
	class CNode;
	class CNodeConfigData;
}

class CNodeWorkstationTable;
class CQuoteItem;

class CNodeWorkstationView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CNodeWorkstationView(QWidget *parent, IMainModuleInterface *pCore, Config::CNode *pWorkstationData
		, CQuoteItem *pItem, Config::CNodeConfigData *pNodeConfig);
	~CNodeWorkstationView();

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
	CNodeWorkstationTable *m_pTable;
};

#endif // CNODEWORKSTATIONVIEW_H
