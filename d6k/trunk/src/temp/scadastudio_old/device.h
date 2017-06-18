#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

class CScadastudio;
class QModelIndex;

class CDevice : public QObject
{
	Q_OBJECT

public:
	CDevice(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType);
	~CDevice();

	void InitDevice();

	void ShowMenu();


private:
	CScadastudio *m_pUi;

	int m_nMouseClickType;

	QModelIndex &m_index;
};

#endif // DEVICE_H
