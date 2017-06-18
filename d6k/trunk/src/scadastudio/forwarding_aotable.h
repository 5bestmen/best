#ifndef FOREARDING_AOTABLE_H
#define FOREARDING_AOTABLE_H

#include <QTableView>
#include "datatypes.h"

class CSortFilterModel;
class IMainModuleInterface;
class CForwardingAOModel;

namespace Config
{
	class CForwardingChannelData;
	class CFesData;
}

class CForwardingAOTable : public QTableView
{
	Q_OBJECT

public:
	CForwardingAOTable(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes);
	~CForwardingAOTable();

	void Refresh();

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
	CForwardingAOModel *m_pModel;
	Config::CFesData *m_pFes;
	IMainModuleInterface *m_pCore;
};

#endif // FOREARDING_
