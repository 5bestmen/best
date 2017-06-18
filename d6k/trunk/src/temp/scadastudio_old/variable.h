#ifndef VARIABLE_H
#define VARIABLE_H

#include <QObject>
#include "scadastudio.h"
#include <QModelIndex>

class CVariable : public QObject
{
	Q_OBJECT

public:
	CVariable(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType);
	~CVariable();

	void ShowMenu();

	void AddUserGroup();

private:
	CScadastudio *m_pUi;

	int m_nMouseClickType;

	QModelIndex &m_index;
};

#endif // VARIABLE_H
