#ifndef HISSAVE_H
#define HISSAVE_H

#include <QObject>

class CScadastudio;
class QModelIndex;

class CHisSave : public QObject
{
	Q_OBJECT

public:
	CHisSave(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType);
	~CHisSave();

private:
	CScadastudio *m_pUi;

	int m_nMouseClickType;

	QModelIndex &m_index;
};

#endif // HISSAVE_H
