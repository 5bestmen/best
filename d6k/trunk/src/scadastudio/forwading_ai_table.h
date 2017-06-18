#ifndef FORWADING_AITABLE_H
#define FORWADING_AITABLE_H

#include "datatypes.h"
#include <QTableView>


class CSortFilterModel;
class IMainModuleInterface;
class CForwardingAIModel;
class CAlarm;
class CChannel;

namespace Config
{
	class CForwardingChannelData;
	class CFesData;
}

class CForwadingAITable : public QTableView
{
	Q_OBJECT

public:
	CForwadingAITable(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes);
	~CForwadingAITable();

	void mouseReleaseEvent(QMouseEvent * event);

	void Refresh();

public slots:
	void DoubleClicked(const QModelIndex & index);
	void ShowMouseRightButton(const QPoint& point);


protected:
	//void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
	//void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
	//void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
	//void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
	//void startDrag(Qt::DropActions supportedActions) Q_DECL_OVERRIDE;

signals:
	void SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel);

private:
	void AddAnalogPoint(QModelIndex &index);
	void DeleteAnalogPoint(QModelIndex &index);
	void BatchAddPoint();

private:
	CSortFilterModel *m_pSortModel;
	CForwardingAIModel *m_pModel;
	Config::CFesData *m_pFes;
	IMainModuleInterface *m_pCore;
};

#endif // FORWADING_AITABLE_H
