#ifndef USER_H
#define USER_H

#include <QObject>

class CScadastudio;
class QModelIndex;

class CUser : public QObject
{
	Q_OBJECT

public:
	CUser(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType);
	~CUser();

	void AddUserGroup();

	void ShowMenu();

	void AddUser();

private:
	CScadastudio *m_pUi;

	int m_nMouseClickType;

	QModelIndex &m_index;
};

#endif // USER_H
