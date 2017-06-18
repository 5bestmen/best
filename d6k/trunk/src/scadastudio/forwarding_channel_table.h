#ifndef CHANNELTABLE_H
#define CHANNELTABLE_H

#include "scadastudio/imainwindow.h"
#include "scadastudiodefine.h"

#include <QTableview>

namespace Config
{
	class CForwardingChannelData;
	class CFesData;
}

class CForwardingChannelModel;
class CSortFilterModel;
class CQuoteItem;
class IMainWindow;
class CChannel;
class IMainModuleInterface;

class CForwardingChannelTable : public QTableView
{
	Q_OBJECT

public:
	CForwardingChannelTable(QWidget *parent, Config::CForwardingChannelData *pChannel, CQuoteItem *pItem, Config::CFesData *pFes, IMainModuleInterface *pCore);
	~CForwardingChannelTable();

	void mouseReleaseEvent(QMouseEvent * event);

	//Í¨µÀÄ£¿é
	CForwardingChannelModel *GetModel();

signals:
	//void SendSelectedRows(QSet<int32s> &set, int32s nType, CChannelModel *pModel);

private:
	CForwardingChannelModel *m_pModel;
	CSortFilterModel *m_pSortModel;

	Config::CForwardingChannelData *m_pChannel;

	IMainWindow *m_pUi;
};

#endif // CHANNELTABLE_H
