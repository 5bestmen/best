#ifndef FORWARIDNG_DITABLE_H
#define FORWARIDNG_DITABLE_H

#include <QTableView>
#include "datatypes.h"

class CSortFilterModel;
class IMainModuleInterface;
class CForwardingDIModel;

namespace Config
{
	class CForwardingChannelData;
	class CFesData;
}

class CForwardingDITable : public QTableView
{
	Q_OBJECT

public:
	CForwardingDITable(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData
		, Config::CFesData *pFes);
	~CForwardingDITable();

	void Refresh();

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
	CForwardingDIModel *m_pModel;
	Config::CFesData *m_pFes;
	IMainModuleInterface *m_pCore;
};

#endif // FORWARIDNG_DITABLE_H
