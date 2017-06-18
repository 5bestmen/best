#ifndef DITABLE_H
#define DITABLE_H

#include <QTableView>
#include "datatypes.h"

class CSortFilterModel;
class IMainModuleInterface;
class CDIModel;

namespace Config
{
	class CDigitalInput;
	class CFesData;
}

class CDITable : public QTableView
{
	Q_OBJECT

public:
	CDITable(QWidget *parent, std::vector <Config::CDigitalInput *> &arrDIs, IMainModuleInterface *pCore, void *pModule, Config::CFesData *pFes);
	~CDITable();

	void mouseReleaseEvent(QMouseEvent * event);
private:
	void AddAnalogPoint(QModelIndex &index);
	void DeleteAnalogPoint(QModelIndex &index);
public slots:
	void ShowMouseRightButton(const QPoint& point);
	void DoubleClicked(const QModelIndex & index);

signals:
	void SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel);

private:
	CSortFilterModel *m_pSortModel;
	CDIModel *m_pModel;
	Config::CFesData *m_pFes;
	IMainModuleInterface *m_pCore;
};

#endif // DITABLE_H
