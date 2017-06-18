#ifndef USERMODEL_H
#define USERMODEL_H

#include <QObject>

class CTreeWidget;
class QStandardItemModel;
class CQuoteItem;
class QMenu;
class QAction;
class QModelIndex;
class CScadastudio;
class CTabWidget;

class CUserModel : public QObject
{
	Q_OBJECT

public:
	CUserModel(CScadastudio *pUi, CQuoteItem *item);
	~CUserModel();

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

#endif // USERMODEL_H
