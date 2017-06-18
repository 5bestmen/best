#include "digitalalarmlimittable.h"
#include "digitalalarmlimitmodel.h"
#include "sortfiltermodel.h"
#include "scadastudiodefine.h"

#include <QMenu>
#include <QColorDialog>

CDigitalAlarmLimitTable::CDigitalAlarmLimitTable(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
	Config::CDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes)
	: QTableView(parent), m_pSortModel(nullptr)
{
	m_pSortModel = new CSortFilterModel(this);
	m_pModel = new CDigitalAlarmLimitModel(this, pCore, pModule, pAlarmData, pItem, pFes);

	m_pSortModel->setSourceModel(m_pModel);
	setModel(m_pSortModel);

	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
}

CDigitalAlarmLimitTable::~CDigitalAlarmLimitTable()
{

}

void CDigitalAlarmLimitTable::ShowMouseRightButton(const QPoint& point)
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

void CDigitalAlarmLimitTable::DoubleClicked(const QModelIndex & index)
{
	if (index.column() == CDigitalAlarmLimitModel::BackColor || index.column() == CDigitalAlarmLimitModel::TextColor
		|| index.column() == CDigitalAlarmLimitModel::BlinkBackColor || index.column() == CDigitalAlarmLimitModel::BlinkTextColor)
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

void CDigitalAlarmLimitTable::AddAnalogPoint(QModelIndex &index)
{
	Q_UNUSED(index);
	
	int nRow = m_pModel->rowCount();

	m_pModel->InsertRows(nRow, 1);
}

void CDigitalAlarmLimitTable::DeleteAnalogPoint(QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}

	int nRow = index.row();

	m_pModel->RemoveRows(nRow, 1);
}
