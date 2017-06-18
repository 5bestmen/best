#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <QObject>

class IMainWindow;

class CEquipmentModel : public QObject
{
	Q_OBJECT

public:
	CEquipmentModel(IMainWindow *pUi);
	~CEquipmentModel();

public:
	void Init(IMainWindow *pUi);

	void UnInit();

private:
	IMainWindow *m_pUi;
};

#endif // EQUIPMENT_H
