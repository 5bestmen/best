#ifndef SCALETABLE_H
#define SCALETABLE_H

#include <QTableView>
//#include "config_data.h"

class CQuoteItem;
class CSortFilterModel;
class CScaleModel;
class CScale;

namespace Config
{
	class CTransform;
	class CFesData;
}

class CScaleTable : public QTableView
{
	Q_OBJECT

public:
	CScaleTable(QWidget *parent, Config::CTransform *pTransform, CQuoteItem *pItem, CScale *pModel
		, Config::CFesData *pFesConfig);
	~CScaleTable();

	void mouseReleaseEvent(QMouseEvent *event);

	CScaleModel *GetModel();


public slots:
	void DoubleClicked(const QModelIndex & index);

signals:
	void SendSelectedRows(QSet<int> &set, int nType, void *pModel);

private:
	//CFesDataModel *m_pModel;
	CSortFilterModel *m_pSortModel;

	CScaleModel *m_pModel;
};

#endif // SCALETABLE_H
