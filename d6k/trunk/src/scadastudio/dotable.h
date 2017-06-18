#ifndef DOTABLE_H
#define DOTABLE_H

#include <QTableView>
#include "datatypes.h"

class CSortFilterModel;
class IMainModuleInterface;
class CDOModel;
class CAlarm;

namespace Config
{
	class CDigitalOutput;
	class CFesData;
}

class CDOTable : public QTableView
{
	Q_OBJECT

public:
	CDOTable(QWidget *parent, std::vector <Config::CDigitalOutput *> &arrDOs, IMainModuleInterface *pCore, void *pModule, Config::CFesData *pFes);
	~CDOTable();

	void mouseReleaseEvent(QMouseEvent * event);
public slots:
	void ShowMouseRightButton(const QPoint& point);
	void DoubleClicked(const QModelIndex & index);
private:
	void AddAnalogPoint(QModelIndex &index);
	void DeleteAnalogPoint(QModelIndex &index);

signals:
	void SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel);

private:
	CSortFilterModel *m_pSortModel;
	CDOModel *m_pModel;
	Config::CFesData *m_pFes;
	IMainModuleInterface *m_pCore;
};

#endif // DOTABLE_H
