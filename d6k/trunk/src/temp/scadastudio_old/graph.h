#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>

class CScadastudio;
class QModelIndex;

class CGraph : public QObject
{
	Q_OBJECT

public:
	CGraph(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType);
	~CGraph();

	void AddGraphGroup();

	void ShowMenu();

	void AddGraph();

private:
	CScadastudio *m_pUi;

	int m_nMouseClickType;

	QModelIndex &m_index;
};

#endif // GRAPH_H
