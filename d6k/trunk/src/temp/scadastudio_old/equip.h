#ifndef CEQUIP_H
#define CEQUIP_H

#include <QObject>

class CScadastudio;
class QModelIndex;

class CEquip : public QObject
{
	Q_OBJECT

public:
	CEquip(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType);
	~CEquip();

	void AddEquipGroup();

	void ShowMenu();

	void AddEquip();

private:
	CScadastudio *m_pUi;

	int m_nMouseClickType;

	QModelIndex &m_index;
};

#endif // CEQUIP_H
