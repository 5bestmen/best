#ifndef SCADASYSTEMVARIABLEVIEW_H
#define SCADASYSTEMVARIABLEVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

namespace Config
{
	class CScadaVariableConfigData;
	class CScadaData;
}

class IMainModuleInterface;
class CScadaSystemVariableTable;

class CScadaSystemVariableView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CScadaSystemVariableView(QWidget *parent, IMainModuleInterface *pCore, Config::CScadaVariableConfigData *pConfigData
		, Config::CScadaData *pData);
	~CScadaSystemVariableView();

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
	CScadaSystemVariableTable *m_pTable;
};

#endif // SCADASYSTEMVARIABLEVIEW_H
