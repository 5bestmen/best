#ifndef USER_H
#define USER_H

#include <QObject>

class IMainWindow;

class CUser : public QObject
{
	Q_OBJECT

public:
	CUser(IMainWindow *pUi);
	~CUser();

	void AddUserGroup(QModelIndex &index);

	void ShowMenu(QModelIndex &index);

	void AddUser(QModelIndex &index);

public slots:
	void showMouseRightButton(const QPoint &point);

private:
	IMainWindow *m_pUi;
};

#endif // USER_H
