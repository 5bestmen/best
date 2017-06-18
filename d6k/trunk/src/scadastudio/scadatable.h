#ifndef SCADATABLE_H
#define SCADATABLE_H

#include <QTableView>
#include "config_data.h"

class CQuoteItem;
class CSortFilterModel;
class CScadaModel;
class IMainModuleInterface;
namespace Config
{
	class CScadaData;
	class CScadaVariableConfigData;
}

class CScadaTable : public QTableView
{
	Q_OBJECT

public:
	CScadaTable(QWidget *parent, Config::CScadaData *pData, CQuoteItem *pItem, int nType, IMainModuleInterface *pCore
		, Config::CScadaVariableConfigData *pConfig);
	~CScadaTable();

	void mouseReleaseEvent(QMouseEvent * event)
	{
		QAbstractItemView::mouseReleaseEvent(event);
	}

	//CScadaModel *GetModel()
	//{	
	//	return m_pModel;
	//}

	void Refresh();

private:
	CScadaModel *m_pModel;
	CSortFilterModel *m_pSortModel;
};

#endif // FESTABLE_H
