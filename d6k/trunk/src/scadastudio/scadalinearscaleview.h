#ifndef SCADALINEARSCALEVIEW_H
#define SCADALINEARSCALEVIEW_H

#include "scadastudio/base.h"

#include <QMainWindow>

namespace Config
{
	class CScadaTransform;
	class CScadaVariableConfigData;
	class CScadaData;
}
class CQuoteItem;
class CScadaVariableModule;
class CScadaLinearScaleTable;

class CScadaLinearScaleView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CScadaLinearScaleView(QWidget *parent, Config::CScadaTransform *pTransform, CQuoteItem *pItem, CScadaVariableModule *pModule
		, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada);
	~CScadaLinearScaleView();

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
	CScadaLinearScaleTable *m_pTable;
};

#endif // SCADALINEARSCALEVIEW_H
