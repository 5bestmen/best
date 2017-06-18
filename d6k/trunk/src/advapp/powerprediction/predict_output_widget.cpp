#include "predict_output_widget.h"
#include "predict_custom_delegate.h"


/*! \fn CPredictOutputView::CPredictOutputView(QWidget* parent, CPredictItem* pPrdtItem, IMainModuleInterface* pCore)
***********************************************************************************************************************
** \brief   CPredictOutputView::CPredictOutputView
** \details 构造函数
** \return
** \author  GuoHaijun
** \date    2017年3月30日
** \note
***********************************************************************************************************************/
CPredictOutputView::CPredictOutputView(QWidget* parent, CWeatherData* pWeaDatas, IMainModuleInterface* pCore)
	: QMainWindow(parent)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	Q_ASSERT(pWeaDatas);
	if (!pWeaDatas)
	{
		return;
	}

	m_pTable = new CPredictOutputTable(this, pWeaDatas->m_vecTableInfo);

	m_pTable->setModel(m_pTable->m_pPrdtOutputModel);

	setCentralWidget(m_pTable);
}



/*! \fn CPredictOutputView::~CPredictOutputView()
****************************************************************
** \brief   CPredictOutputView::~CPredictOutputView
** \details 析构函数
** \return
** \author  GuoHaijun
** \date    2017年3月30日
** \note
***************************************************************/
CPredictOutputView::~CPredictOutputView()
{


}



/*! \fn void CPredictOutputView::Delete()
*******************************************
** \brief   CPredictOutputView::Delete
** \details 删除
** \return  void
** \author  GuoHaijun
** \date    2017年3月30日
** \note
*******************************************/
void CPredictOutputView::Delete()
{
	m_pTable->setModel(nullptr);
	deleteLater();
}



/*
void CPredictOutputView::ShowWidget(CStationData* pStnDatas)
{
m_pPrdtStnModel = new CPredictStationModel(*pStnDatas);

QStringList headList;
headList << "ID" << "Name" << "SelectPoint";
m_pPrdtStnModel->SetHorizontalHeaderList(headList);

m_pPrdtStnModel->SetColumnCount(3);
m_pPrdtStnModel->setHeaderData(0, Qt::Horizontal, "ID");
m_pPrdtStnModel->setHeaderData(1, Qt::Horizontal, "Name");
m_pPrdtStnModel->setHeaderData(2, Qt::Horizontal, "SelectPoint");

m_pPrdtStnModel->SetRowCount(2);

int nRow = m_pPrdtStnModel->rowCount();

for (int i = 0; i < nRow; i++)
{
QModelIndex index = m_pPrdtStnModel->index(i, 0, QModelIndex());

m_pPrdtStnModel->setData(index, i);
}

QModelIndex index_0 = m_pPrdtStnModel->index(0, 1, QModelIndex());

m_pPrdtStnModel->setData(index_0, QString("Active Power"));

QModelIndex index_1 = m_pPrdtStnModel->index(1, 1, QModelIndex());

m_pPrdtStnModel->setData(index_1, QString("InActive Power"));




m_pTable = new CPredictStationTable(this, pStnDatas);

CPredictSelctPointDelegate *pPrdtSelPoint = new CPredictSelctPointDelegate();

m_pTable->setItemDelegateForColumn(2, pPrdtSelPoint);

m_pTable->setModel(m_pPrdtStnModel);

m_pTable->setContextMenuPolicy(Qt::CustomContextMenu);

m_pTable->setSortingEnabled(true);

}*/


