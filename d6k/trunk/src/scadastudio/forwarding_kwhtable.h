#ifndef FOREARDING_KWHTABLE_H
#define FOREARDING_KWHTABLE_H

#include <QTableView>
#include "datatypes.h"

class CSortFilterModel;
class IMainModuleInterface;
class CForwardingKWHModel;

namespace Config
{
	class CForwardingChannelData;
	class CFesData;
}

class CForwardingKWHTable : public QTableView
{
	Q_OBJECT

public:
	CForwardingKWHTable(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes);
	~CForwardingKWHTable();

	void refresh();

public slots:
	void ShowMouseRightButton(const QPoint& point);
	void DoubleClicked(const QModelIndex & index);
protected:
	void mouseReleaseEvent(QMouseEvent * event);
private:
	void AddKWHPoint(QModelIndex &index);
	void DeleteKWHPoint(QModelIndex &index);
signals:
	void SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel);

private:
	CSortFilterModel *m_pSortModel;
	CForwardingKWHModel *m_pModel;
	Config::CFesData *m_pFes;
};

#endif // FOREARDING_KWHTABLE_H
