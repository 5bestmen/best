#ifndef CHANNELTABLE_H
#define CHANNELTABLE_H

#include <QTableview>
#include "config_data.h"
#include "scadastudio/imainwindow.h"
#include "scadastudiodefine.h"

class CChannelModel;
class CSortFilterModel;
class CQuoteItem;
class IMainWindow;
class CChannel;
class IMainModuleInterface;

class CChannelTable : public QTableView
{
	Q_OBJECT

public:
	CChannelTable(QWidget *parent, QString strObjectName, Config::CChannelData *pChannel, CQuoteItem *pItem, CChannel *pChannelModel,
		Config::CFesData *pfesData, IMainModuleInterface *pCore);
	~CChannelTable();

	void mouseReleaseEvent(QMouseEvent * event);

	//Í¨µÀÄ£¿é
	CChannelModel *GetModel();

signals:
	void SendSelectedRows(QSet<int32s> &set, int32s nType, CChannelModel *pModel);

private:
	CChannelModel *m_pModel;
	CSortFilterModel *m_pSortModel;

	Config::CChannelData *m_pChannel;

	IMainWindow *m_pUi;
};

#endif // CHANNELTABLE_H
