#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H

#include <QObject>

class CTreeWidget;
class QStandardItemModel;
class CQuoteItem;
class QMenu;
class QAction;
class QModelIndex;
class IMainWindow;
class CTabWidget;
class CXmlReader;
class QStandardItem;

class CGraphModel : public QObject
{
	Q_OBJECT

public:
	CGraphModel(IMainWindow *pUi);
	~CGraphModel();

	void Init(IMainWindow *pUi, CXmlReader *pXmlReader);

	void UnInit();

private:
#if 0
	void ShowMenu(QModelIndex &index);

	void AddGraph(QModelIndex &index);

	void AddGraphGroup(QModelIndex &index);
	void OpenView(QWidget *pParent);
private slots:
	void showMouseRightButton(const QPoint &point);

	void itemChanged(QStandardItem *pItem);
	void doubleClicked(const QModelIndex &index);
#endif

private:
	QStandardItemModel *m_pModel;

	CTreeWidget *m_pTree;

	CQuoteItem *m_pTopFesItem;

	CQuoteItem *m_pTopProjectItem;

	CTabWidget *m_pTabWidget;

	IMainWindow *m_pUi;	
// private:
// 	CGraphView *m_pGraphView;
// 	CGraphScene *m_pGraphScene;

};

#endif // GRAPHMODEL_H
