#ifndef FESTABLE_H
#define FESTABLE_H

#include <QTableView>
#include "config_data.h"

class CQuoteItem;
class CSortFilterModel;
class CFesDataModel;
class IMainModuleInterface;
namespace Config
{
	class CFesConfigData;
}

class CFesTable : public QTableView
{
	Q_OBJECT

public:
	CFesTable(QWidget *parent, Config::CFesData *pFesData, CQuoteItem *pItem, int nType, IMainModuleInterface *pCore
		, Config::CFesConfigData *pFesConfig);
	~CFesTable();

	void mouseReleaseEvent(QMouseEvent * event)
	{
		QAbstractItemView::mouseReleaseEvent(event);
	}

	CFesDataModel *GetModel()
	{	
		return m_pModel;
	}

private:
	CFesDataModel *m_pModel;
	CSortFilterModel *m_pSortModel;
};

#endif // FESTABLE_H
