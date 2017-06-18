#ifndef REPORTMODEL_H
#define REPORTMODEL_H

#include <QObject>

class CTreeWidget;
class QStandardItemModel;
class CQuoteItem;
class QMenu;
class QAction;
class QModelIndex;
class CScadastudio;
class CTabWidget;

class CReportModel : public QObject
{
	Q_OBJECT

public:
	CReportModel(CScadastudio *pUi, CQuoteItem *item);
	~CReportModel();

private slots:
	void GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType);

private:
	QStandardItemModel *m_pModel;

	CTreeWidget *m_pTree;

	CQuoteItem *m_pTopFesItem;

	CQuoteItem *m_pTopProjectItem;

	CScadastudio *m_pUI;

	CTabWidget *m_pTabWidget;

	CScadastudio *m_pUi;
};

#endif // REPORTMODEL_H
