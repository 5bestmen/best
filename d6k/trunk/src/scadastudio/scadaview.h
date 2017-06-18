#ifndef SCADAVIEW_H
#define SCADAVIEW_H

#include <QMainWindow>
#include "config_data.h"
#include "scadastudio/base.h"

class CQuoteItem;
class CScadaTable;
class IMainModuleInterface;
class QStandardItem;
namespace Config
{
	class CScadaData;
	class CScadaVariableConfigData;
}


class CScadaView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CScadaView(IMainModuleInterface *pCore, Config::CScadaData *pData, CQuoteItem *pItem
		, Config::CScadaVariableConfigData *pConfig);
	~CScadaView();

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
	CScadaTable *m_pTable;

	Config::CScadaData *m_pScadaData;

	IMainModuleInterface *m_pCore;
};

#endif // FESVIEW_H
