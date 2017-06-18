#ifndef CNODEWORKSTATIONGROUPVIEW_H
#define CNODEWORKSTATIONGROUPVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

class IMainModuleInterface;

namespace Config
{
	class CNodeGroup;
	class CNodeConfigData;
}

class CNodeWorkstationGroupTable;

class CNodeWorkstationGroupView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CNodeWorkstationGroupView(QWidget *parent, IMainModuleInterface *pCore, Config::CNodeGroup *pWorkstationData
		, Config::CNodeConfigData *pNodeConfig);
	~CNodeWorkstationGroupView();

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
	CNodeWorkstationGroupTable *m_pTable;
};

#endif // CNODEWORKSTATIONGROUPVIEW_H
