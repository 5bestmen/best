#ifndef DEVIETABLE_H
#define DEVIETABLE_H

#include <QTableView>

class IMainModuleInterface;
class CDeviceModel;

namespace Config
{
	class CDevice;
	class CFesData;
}
class CQuoteItem;
class CChannel;
class CSortFilterModel;

class CDeviceTable : public QTableView
{
	Q_OBJECT

public:
	CDeviceTable(QWidget *parent, IMainModuleInterface *pCore, Config::CDevice *pDeviceData, CQuoteItem *pItem, CChannel *pModule
		, Config::CFesData *pFesData);
	~CDeviceTable();

	CDeviceModel *GetModel();

private:
	//CChannelModel *m_pModel;
	CSortFilterModel *m_pSortModel;
	CDeviceModel *m_pDeviceModel;
};

#endif // DEVIETABLE_H
