#ifndef SCADANONLINEARSCALEVIEW_H
#define SCADANONLINEARSCALEVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

namespace Config
{
	class CScadaTransform;
	class CScadaVariableConfigData;
}
class CQuoteItem;
class CScadaVariableModule;
class CScadaNonlinearScaleTable;

class CScadaNonlinearScaleView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CScadaNonlinearScaleView(QWidget *parent, Config::CScadaTransform *pTransform, CQuoteItem *pItem, CScadaVariableModule *pModule
		, Config::CScadaVariableConfigData *pConfig);
	~CScadaNonlinearScaleView();

	virtual void Save();

	virtual void Refresh();

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
	CScadaNonlinearScaleTable *m_pTable;
};

#endif // SCADANONLINEARSCALEVIEW_H
