#ifndef CSACLE_H
#define CSACLE_H

#include <QObject>

class CScadastudio;
class QModelIndex;

class CSacle : public QObject
{
	Q_OBJECT

public:
	CSacle(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType);
	~CSacle();

	void AddScaleGroup();

	void ShowMenu();

	void AddScale();

private:
	CScadastudio *m_pUi;

	int m_nMouseClickType;

	QModelIndex &m_index;
};

#endif // CSACLE_H
