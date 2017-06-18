#include "scadadigitalalarmlimittable.h"
#include "sortfiltermodel.h"
#include "scadadigitalalarmlimitmodel.h"
#include "scadastudiodefine.h"

#include <QMenu>
#include <QAction>
#include <QColorDialog>

CScadaDigitalAlarmLimitTable::CScadaDigitalAlarmLimitTable(QWidget *parent, IMainModuleInterface *pCore,
	Config::CScadaDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada)
	: QTableView(parent), m_pSort(nullptr), m_pModel(nullptr)
{
	m_pSort = new CSortFilterModel(this);
	m_pModel = new CScadaDigitalAlarmLimitModel(this, pCore, pAlarmData, pItem, pConfig, pScada);

	m_pSort->setSourceModel(m_pModel);

	setModel(m_pSort);

	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
}

CScadaDigitalAlarmLimitTable::~CScadaDigitalAlarmLimitTable()
{

}

void CScadaDigitalAlarmLimitTable::Refresh()
{
	m_pModel->RefrushModel();
}

/*! \fn void CScadaDigitalAlarmLimitTable::ShowMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CScadaDigitalAlarmLimitTable::ShowMouseRightButton 
** \details 
** \param point 
** \return void 
** \author gw
** \date 2017��1��13�� 
** \note 
********************************************************************************************************/
void CScadaDigitalAlarmLimitTable::ShowMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = m_pSort->mapToSource(this->indexAt(point));

	QMenu *pMenu = new QMenu(nullptr);

	QAction *pAddPoint = new QAction(tr("add point"), pMenu);
	pAddPoint->setIcon(QIcon(CHANNEL_PNG));
	pMenu->addAction(pAddPoint);

	QAction *pDeletePoint = new QAction(tr("delete point"), pMenu);
	pMenu->addAction(pDeletePoint);
	pDeletePoint->setIcon(QIcon(CLOSE_GROUP_PNG));

	QAction* selectedItem = pMenu->exec(QCursor::pos());

	if (selectedItem == pAddPoint)
	{
		//���ģ������
		AddPoint(indexSelect);
	}
	else if (selectedItem == pDeletePoint)
	{
		//ɾ��ģ������
		DeleteAnalogPoint(indexSelect);
	}

	pMenu->deleteLater();
}

void CScadaDigitalAlarmLimitTable::DoubleClicked(const QModelIndex &index)
{
	if (index.column() == CScadaDigitalAlarmLimitModel::BackColor || index.column() == CScadaDigitalAlarmLimitModel::TextColor
		|| index.column() == CScadaDigitalAlarmLimitModel::BlinkBackColor || index.column() == CScadaDigitalAlarmLimitModel::BlinkTextColor)
	{
		//	//��Color ��int ת���� QColor  
		int nColor = index.data(Qt::EditRole).toInt();
		int red = nColor & 255;
		int green = nColor >> 8 & 255;
		int blue = nColor >> 16 & 255;
		QColor color = QColorDialog::getColor(IntToQColor(nColor), this);
		if (!color.isValid())
		{
			return;
		}

		//	//��Color ��QColor ת���� int  
		nColor = QColorToInt(color);

		m_pModel->setData(index, nColor);
	}
}

/*! \fn
********************************************************************************************************* 
** \brief CScadaDigitalAlarmLimitTable::AddAnalogPoint 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017��1��14�� 
** \note 
********************************************************************************************************/
void CScadaDigitalAlarmLimitTable::AddPoint(QModelIndex &index)
{
	Q_UNUSED(index);
	
	int nRow = m_pModel->rowCount();

	m_pModel->InsertRows(nRow, 1);
}

void CScadaDigitalAlarmLimitTable::DeleteAnalogPoint(QModelIndex &index)
{
	Q_UNUSED(index);

	auto nRow = index.row();
	m_pModel->RemoveRows(nRow, 1);
}
