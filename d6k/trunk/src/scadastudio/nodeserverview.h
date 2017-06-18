#ifndef NODESERVERVIEW_H
#define NODESERVERVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

class IMainModuleInterface;
namespace Config
{
	class CNodeServer;
	class CNodeConfigData;
};

class CNodeServerTable;

class CNodeServerView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CNodeServerView(QWidget *parent, IMainModuleInterface *pCore, Config::CNodeServer *pServerData
		, Config::CNodeConfigData *pNodeConfig);
	~CNodeServerView();

	virtual void Save()
	{

	}

	virtual void Refresh()
	{

	}

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
	CNodeServerTable *m_pTable;
};

#endif // NODESERVERVIEW_H
