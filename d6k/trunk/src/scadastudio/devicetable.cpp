#include "devicetable.h"
#include "devicemodel.h"

CDeviceTable::CDeviceTable(QWidget *parent, IMainModuleInterface *pCore, Config::CDevice *pDeviceData, CQuoteItem *pItem, CChannel *pModule
	, Config::CFesData *pFesData)
	: QTableView(parent)
{
	CDeviceModel *pModel = new CDeviceModel(this, pCore, pDeviceData, pModule, pItem, pFesData);
	m_pDeviceModel = pModel;

	setModel(m_pDeviceModel);
}

CDeviceTable::~CDeviceTable()
{

}

CDeviceModel *CDeviceTable::GetModel()
{
	return m_pDeviceModel;
}