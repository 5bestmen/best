#ifndef NODEFESGROUPVIEW_H
#define NODEFESGROUPVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

class IMainModuleInterface;
namespace Config
{
	class CNodeGroup;
	class CNodeConfigData;
};

class CNodeFesGroupTable;

class CNodeFesGroupView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CNodeFesGroupView(QWidget *parent, IMainModuleInterface *pCore, Config::CNodeGroup *pFesGroupData
		, Config::CNodeConfigData *pNodeConfig);
	~CNodeFesGroupView();

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
	CNodeFesGroupTable *m_pTable;
};

#endif // NODEFESGROUPVIEW_H
