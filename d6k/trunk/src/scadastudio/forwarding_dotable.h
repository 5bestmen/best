#ifndef FOREARDING_DOTABLE_H
#define FOREARDING_DOTABLE_H

#include <QTableView>
#include "datatypes.h"

class CSortFilterModel;
class IMainModuleInterface;
class CForwardingDOModel;
class CAlarm;

namespace Config
{
	class CForwardingChannelData;
	class CFesData;
}

class CForwardingDOTable : public QTableView
{
	Q_OBJECT

public:
	CForwardingDOTable(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes);
	~CForwardingDOTable();

	void refresh();

	void mouseReleaseEvent(QMouseEvent * event);
public slots:
	void ShowMouseRightButton(const QPoint& point);
	void DoubleClicked(const QModelIndex & index);
private:
	void AddPoint(QModelIndex &index);
	void DeletePoint(QModelIndex &index);

signals:
	void SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel);

private:
	CSortFilterModel *m_pSortModel;
	CForwardingDOModel *m_pModel;
	Config::CFesData *m_pFes;
	IMainModuleInterface *m_pCore;
};

#endif // FOREARDING_DOTABLE_H
