#ifndef SYSTEMVARIABLEVIEW_H
#define SYSTEMVARIABLEVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

namespace Config
{
	class CFesData;
}

class IMainModuleInterface;
class CSystemVariableTable;

class CSystemVariableView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CSystemVariableView(QWidget *parent, IMainModuleInterface *pCore, Config::CFesData *pFesData);
	~CSystemVariableView();

	virtual void Save()
	{

	}

	virtual void Refresh()
	{

	}

	virtual void SetModifyFlag(bool bFlag)
	{
		Q_UNUSED(bFlag);
	}

	virtual bool GetModifyFlag()
	{
		return true;
	}

	virtual void Delete();

private:
	CSystemVariableTable *m_pTable;
};

#endif // SYSTEMVARIABLEVIEW_H
