#include "scadaanalogalarmlimittable.h"
#include "scadaanalogalarmlimitmodel.h"
#include "sortfiltermodel.h"
#include "scadastudiodefine.h"

#include <QMenu>
#include <QColorDialog>

CScadaAnalogAlarmLimitTable::CScadaAnalogAlarmLimitTable(QWidget *parent, IMainModuleInterface *pCore,
	Config::CScadaAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada)
	: QTableView(parent), m_pModel(nullptr)
{
	m_pSortModel = new CSortFilterModel(this);
	
	m_pModel = new CScadaAnalogAlarmLimitModel(this, pCore, pAlarmData, pItem, pConfig, pScada);
	m_pSortModel->setSourceModel(m_pModel);
	setModel(m_pSortModel);

	setContextMenuPolicy(Qt::CustomContextMenu);

	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
}

CScadaAnalogAlarmLimitTable::~CScadaAnalogAlarmLimitTable()
{

}

void CScadaAnalogAlarmLimitTable::Refresh()
{
	m_pModel->RefrushModel();
}

/*! \fn void CScadaAnalogAlarmTable::ShowMouseRightButton(const QPoint& point)
*********************************************************************************************************
** \brief CScadaAnalogAlarmTable::ShowMouseRightButton
** \details
** \param point
** \return void
** \author gw
** \date 2017年1月12日
** \note
********************************************************************************************************/
void CScadaAnalogAlarmLimitTable::ShowMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = m_pSortModel->mapToSource(this->indexAt(point));

	QMenu *pMenu = new QMenu(nullptr);

	QAction *pAddPoint = new QAction(tr("add point"), pMenu);

	pAddPoint->setIcon(QIcon(CHANNEL_PNG));

	pMenu->addAction(pAddPoint);

	QAction *pDeletePoint = new QAction(tr("delete point"), pMenu);

	pMenu->addAction(pDeletePoint);

	pDeletePoint->setIcon(QIcon(CLOSE_GROUP_PNG));

	pMenu->addAction(pDeletePoint);

	QAction* selectedItem = pMenu->exec(QCursor::pos());

	if (selectedItem == pAddPoint)
	{
		//添加模拟量点
		AddAnalogPoint(indexSelect);
	}
	else if (selectedItem == pDeletePoint)
	{
		//删除模拟量点
		DeleteAnalogPoint(indexSelect);
	}

	pMenu->deleteLater();
}

void CScadaAnalogAlarmLimitTable::DoubleClicked(const QModelIndex &index)
{
	if (index.column() == CScadaAnalogAlarmLimitModel::BackColor || index.column() == CScadaAnalogAlarmLimitModel::TextColor
		|| index.column() == CScadaAnalogAlarmLimitModel::BlinkBackColor || index.column() == CScadaAnalogAlarmLimitModel::BlinkTextColor)
	{
		//	//将Color 从int 转换成 QColor  
		int nColor = index.data(Qt::EditRole).toInt();
		int red = nColor & 255;
		int green = nColor >> 8 & 255;
		int blue = nColor >> 16 & 255;
		QColor color = QColorDialog::getColor(IntToQColor(nColor), this);
		if (!color.isValid())
		{
			return;
		}

		//	//将Color 从QColor 转换成 int  
		nColor = QColorToInt(color);

		m_pModel->setData(index, nColor);
	}
}

/*! \fn
********************************************************************************************************* 
** \brief CScadaAnalogAlarmLimitTable::AddAnalogPoint 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月12日 
** \note 
********************************************************************************************************/
void CScadaAnalogAlarmLimitTable::AddAnalogPoint(QModelIndex &index)
{
	Q_UNUSED(index);
	
	int nRow = m_pModel->rowCount();

	m_pModel->InsertRows(nRow, 1);
}

void CScadaAnalogAlarmLimitTable::DeleteAnalogPoint(QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}
	
	int nRow = index.row();

	m_pModel->RemoveRows(nRow, 1);
}