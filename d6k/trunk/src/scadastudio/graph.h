#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>

class IMainWindow;
class QModelIndex;

class CGraph : public QObject
{
	Q_OBJECT

public:
	CGraph(IMainWindow *pUi);
	~CGraph();

	void AddGraphGroup();

	void ShowMenu(QModelIndex &index);

	void AddGraph();
protected:
	void doubleClicked(const QModelIndex &index);
public slots:
	void showMouseRightButton(const QPoint& point);

private:
	IMainWindow *m_pUi;
};

#endif // GRAPH_H
