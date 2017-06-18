#ifndef FOREARDING_CGTABLE_H
#define FOREARDING_CGTABLE_H

#include <QTableView>
#include "datatypes.h"

class CSortFilterModel;
class IMainModuleInterface;
class CForwardingCGModel;

namespace Config
{
	class CForwardingChannelData;
	class CFesData;
}

class CForwardingCGTable : public QTableView
{
	Q_OBJECT

public:
	CForwardingCGTable(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes);
	~CForwardingCGTable();

	void refresh();

public slots:
	void ShowMouseRightButton(const QPoint& point);
	void DoubleClicked(const QModelIndex & index);
protected:
	void mouseReleaseEvent(QMouseEvent * event);
private:
	void AddAnalogPoint(QModelIndex &index);
	void DeleteAnalogPoint(QModelIndex &index);
signals:
	void SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel);

private:
	CSortFilterModel *m_pSortModel;
	CForwardingCGModel *m_pModel;
	Config::CFesData *m_pFes;
};

#endif // FOREARDING_
