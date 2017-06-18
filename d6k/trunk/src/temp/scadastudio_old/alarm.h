#ifndef ALARM_H
#define ALARM_H

#include <QObject>
#include <QModelIndex>

class CScadastudio;

class CAlarm : public QObject
{
	Q_OBJECT

public:
	CAlarm(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType);
	~CAlarm();

	void AddAlarmGroup();

	void ShowMenu();

	void AddAlarm();

private:
	CScadastudio *m_pUi;

	int m_nMouseClickType;

	QModelIndex &m_index;
};

#endif // ALARM_H
