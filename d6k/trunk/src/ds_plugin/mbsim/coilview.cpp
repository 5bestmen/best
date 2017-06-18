

#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QHeaderView>

#include "memdata.h"
#include "coilview.h"
#include "coildlg.h"

CGridModel::CGridModel(CMemData & MemData, QObject *parent) : QAbstractTableModel(parent), m_MemData(MemData)
{
//	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

//	m_pMemData = pMemData;
// 
// 	m_rowCount = 0;
// 	m_colCount = 0;
// 
// 	m_nDataType = DATATYPE_LAST;
// 	m_nNodeType = NODE_TYPE_ROOT;
}

int	CGridModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return m_nColCount;
}

int CGridModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return m_nRowCount;
}

void CGridModel::SetHeadTitles(QStringList& headlist)
{
	m_lstHead = headlist;
}

QVariant CGridModel::headerData(int section, Qt::Orientation /* orientation */, int role) const
{
	if (role == Qt::DisplayRole)
	{
		int nSize = m_lstHead.size();

		if (section >= nSize)
		{
			int asdasdas = 0;
		}
		if (m_lstHead.size() > 0)
			return m_lstHead[section];
	}
	return QVariant();
}

QVariant CGridModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
	{
		return QVariant(); 
	}
	int nRow = index.row();
	int nCol = index.column();
	/*
	if(Qt::FontRole == role)
	{
	return QVariant(CConfig::Instance().GetUIFont());
	}*/
	char szBuff[512] = { 0 };
	if (Qt::DisplayRole == role)
	{
		if (nCol == 0 )
		{
			if (nRow<CMemData::MAX_ITEM_NUM)
			{
				return QVariant(nRow+1);
			}
		}
		else if (nCol == 1)
		{
			if (nRow < CMemData::MAX_ITEM_NUM)
			{
				unsigned short nVal = m_MemData.GetCoilAddr(nRow + 1);
				return QVariant(nVal);
			}
		}
		else if (nCol == 2)
		{
			if (nRow < CMemData::MAX_ITEM_NUM)
			{
				unsigned char nVal = m_MemData.GetCoilValue(nRow+1);
				return QVariant(nVal);
			}
		}
	

// 		if (nRow >= m_vecDataID.size())
// 			return QVariant();
	}

	return QVariant();
}


CCoilView::CCoilView(CMemData & MemData, QWidget *parent) : QWidget(parent),
m_pTableView(nullptr),
m_pTableModel(nullptr)
{
	InitView(MemData);
}

void CCoilView::InitView(CMemData  &mem)
{
	m_pTableView = new QTableView(this);
	//m_pTableModel = new QStandardItemModel(this);
	m_pTableModel = new CGridModel(mem,this);
	m_pTableView->setModel(m_pTableModel);

	m_pTableView->verticalHeader()->setHidden(true);
	m_pTableView->setSortingEnabled(true);
	m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	QStringList lstHorizonLables;
	lstHorizonLables <<tr("Index") << tr("Address") << tr("Value") << ("  ");
//	m_pTableModel->setHorizontalHeaderLabels(lstHorizonLables);
	m_pTableModel->SetHeadTitles(lstHorizonLables);

	QVBoxLayout *pWgtlayout = new QVBoxLayout(this);
	pWgtlayout->addWidget(m_pTableView);

	m_pTableModel->SetRowCount(CMemData::MAX_ITEM_NUM);
	m_pTableModel->SetColCount(lstHorizonLables.size());

	m_pTableView->setColumnWidth(0, 50);
	m_pTableView->setColumnWidth(1, 50);
	m_pTableView->setColumnWidth(2, 50);
	m_pTableView->setColumnWidth(3, 500);


	connect(m_pTableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(OnCoilDoubleClicked(const QModelIndex &)));
}
void CCoilView::OnCoilDoubleClicked(const QModelIndex & index)
{
	std::ignore = index;

	CCoilDlg dlg( this);
	int nRet = dlg.exec();

#if 0
	CUltraGrid* pData = (CUltraGrid*)(m_pCurrentView->model());
	int nDoutID = pData->GetDoutID(index2.row());

	/*
	char buf[128];
	sprintf(buf,"DOUTID:%d",nDoutID);
	QMessageBox::about(NULL,"Eror",buf);
	*/

	CKwh* pKwh = m_pKernel->GetKwh(m_nSelGroupID, m_nSelChannelID, m_nSelDeviceID, nDoutID);
	if (pKwh == NULL)
		return;

	m_nDoutID = nDoutID;
	m_strDoutName = pKwh->m_strKwhName;

	CTeleCtrlDlg telectrldlg(m_pKernel, this);
	int nRet = telectrldlg.exec();
#endif

}

