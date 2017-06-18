/*! @file
<PRE>
********************************************************************************
模块名       :  标准对话框
文件名       :  channelview.cpp
文件实现功能 :  标准对话框
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明> 通道 装置
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  
*  @author gw
*  @version 1.0
*  @date    2015.11.17
*/ 

#include "colour_define.h"
#include "graphtabview.h"
#include "graphview.h"
#include "graphscene.h"
#include "graph_file.h"
#include "graph_module.h"

#include <QGridLayout>
#include <QAction>
#include <QToolbar>
#include <QTableWidget>
#include <QHeaderView>

/*! \fn CGraphTabView::CGraphTabView(ICoreui *pUi,QWidget *pParent)
********************************************************************************************************* 
** \brief CGraphTabView::CGraphTabView 
** \details 
** \param pUi 
** \param pParent   
** \return  
** \author gw
** \date 2015年11月20日 
** \note 
********************************************************************************************************/
CGraphTabView::CGraphTabView(ICoreui *pUi,QWidget *pParent)
	: QMainWindow(pParent), m_pUi(pUi), m_bFlag(false)
{
	//QGridLayout *pLayout = new QGridLayout(this);
	//
	//m_pTalbeView = new CTableView(this, strTagName, strDescName, nType, strTableName);

	//m_pTalbeView->GetFieldInfo(strTableName);

	//pLayout->addWidget(m_pTalbeView, 0, 0);
	
	//m_pOkButton = new QPushButton(QStringLiteral("确定"), this);
	//m_pCancelButton = new QPushButton(QStringLiteral("取消"), this);
	//m_pDeleteButton = new QPushButton(QStringLiteral("删除"), this);

	//connect(m_pOkButton, SIGNAL(clicked()), this, SLOT(OnOk()));
	//connect(m_pDeleteButton, SIGNAL(clicked()), this, SLOT(OnDelete()));

	//QWidget *topWidget = new QWidget(this);

	//QVBoxLayout *pVerLayout = new QVBoxLayout();

	//pVerLayout->setSpacing(2);

	//pVerLayout->addWidget(m_pOkButton);
	//pVerLayout->addWidget(m_pCancelButton);
	//pVerLayout->addWidget(m_pDeleteButton);
	////pVerLayout->addStretch(2);

	//topWidget->setLayout(pVerLayout);

	//topWidget->setFixedWidth(200);

	//pVerLayout->addItem((new QSpacerItem(20, 178, QSizePolicy::Minimum, QSizePolicy::Expanding)));

	//pVerLayout->setSpacing(2);

	//pLayout->addWidget(topWidget, 0, 1);


	//pLayout->setContentsMargins(0,0,0,0);//设置布局内部四边的空隙
	//pLayout->setAlignment(Qt::AlignHCenter);

	//QWidget *pWidget = new QWidget(this);

	//pWidget->setLayout(pLayout);

	//setCentralWidget(pWidget);

	//m_pUi = parent;

// 	m_pTalbeView = new CTableView(pUi, strTagName, strDescName, nType, strTableName);
// 
// 	m_pTalbeView->GetFieldInfo(strTableName);

// 	QAction *pSaveAction = new QAction(QIcon(SAVE_PNG), "保存", this);
// 	QAction *pDeleteAction = new QAction(QIcon(DELETE_PNG), "删除", this);
// 
// 	QToolBar *pToolBar = addToolBar("标准toolbar");
// 
// 	pToolBar->addAction(pSaveAction);
// 	pToolBar->addAction(pDeleteAction);
// 
// 	connect(pSaveAction, SIGNAL(triggered()), this, SLOT(OnOk()));
// 	connect(pDeleteAction, SIGNAL(triggered()), this, SLOT(OnDelete()));
// 
// 	setCentralWidget(m_pTalbeView);
}

CGraphTabView::CGraphTabView(QWidget *pParent, CGraphFile *pFile) :
m_pGraphFile(pFile), m_bFlag(false),QMainWindow(pParent)
{
	if (pFile->GetScene() == nullptr)
	{
		m_pGraphScene = new CGraphScene(pFile, this);

		m_pGraphView = new CGraphView(m_pGraphScene, this);

		this->setCentralWidget(m_pGraphView);

		Q_ASSERT(pFile);
		if (pFile)
		{
			pFile->SetScene(m_pGraphScene);
			m_pGraphScene->setSceneRect(pFile->GetBackgroundRect());
		}
	}
	else
	{
		m_pGraphScene = pFile->GetScene();
		m_pGraphScene->setParent(this);

		m_pGraphView = new CGraphView(m_pGraphScene, this);

		this->setCentralWidget(m_pGraphView);
		/*
		Q_ASSERT(pFile);
		if (pFile)
		{
			pFile->SetScene(m_pGraphScene);
			m_pGraphScene->setSceneRect(pFile->GetBackgroundRect());
		}
		*/
	}

}

/*! \fn CGraphTabView::~CGraphTabView()
********************************************************************************************************* 
** \brief CGraphTabView::~CGraphTabView
** \details 
** \return  
** \author gw
** \date 2015年11月18日 
** \note 
********************************************************************************************************/
CGraphTabView::~CGraphTabView()
{
//	m_pTalbeView->deleteLater();
	this->deleteLater();
}

void CGraphTabView::OnOk()
{	
	//m_pTalbeView->setSortingEnabled(false);
	
	//m_pTalbeView->m_pSortFilterModel->setDynamicSortFilter(false);

//	m_pTalbeView->Save();

	//QVector<QString> tmp;
	//
	//for(int i = 0; i < 14; i++)
	//{
	//	QString strTmp = "1";

	//	tmp.append(strTmp);
	//}
	//
	//m_pTalbeView->AddRow(tmp);


	//m_pTalbeView->m_pSortFilterModel->setDynamicSortFilter(true);

	//m_pTalbeView->setSortingEnabled(true);
	//m_pTalbeView->m_pSortFilterModel->insertRow(m_pTalbeView->m_pChannelModelData->rowCount());
}

/*! \fn void CGraphTabView::closeEvent( QCloseEvent * event )
********************************************************************************************************* 
** \brief CGraphTabView::closeEvent 
** \details 
** \param event 
** \return void 
** \author gw
** \date 2015年11月20日 
** \note 
********************************************************************************************************/
void CGraphTabView::closeEvent(QCloseEvent * event)
{	
	Q_UNUSED(event);
	this->deleteLater();
}
/*! \fn void CGraphTabView::OnDelete()
********************************************************************************************************* 
** \brief CGraphTabView::OnDelete 
** \details 删除
** \return void 
** \author gw
** \date 2015年12月14日 
** \note 
********************************************************************************************************/
void CGraphTabView::OnDelete()
{
	//m_pTalbeView->SetDeleFlag();

	//m_pTalbeView->horizontalHeader()->reset();
}

void CGraphTabView::UpdateUI(CGraphFile *pGraphFile, QTableWidget*pGraphLayerList)
{
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	Q_ASSERT(pGraphLayerList);
	if (pGraphLayerList == nullptr)
		return;

	unsigned int nLayerCount = (unsigned int)pGraphFile->GetLayerCount();
	QTableWidgetItem *pItem = nullptr;
	std::shared_ptr<CGraphicsLayer> pLayer = nullptr;

	unsigned int i = 0;
	for (i = 1; i <= CGraphFile::MAX_LAYER_CNT && i <= nLayerCount; i++)
	{
		pLayer = pGraphFile->GetLayer(i);
		pItem = new QTableWidgetItem;
		pItem->setIcon(::GetModuleIcon(ICON_EYE_ON));
		pGraphLayerList->setItem(i - 1, 0, pItem);
		pItem->setData(Qt::UserRole, 1);
		pItem->setData(Qt::UserRole + 1, i + 1);

		QString szTxt = QString(QStringLiteral("图层 %1").arg(i));

		pItem = new QTableWidgetItem(szTxt);
		pItem->setIcon(::GetModuleIcon(ICON_LAYER));
		pGraphLayerList->setItem(i - 1, 1, pItem);
		pItem->setData(Qt::UserRole, i + 1); // 图层号+1

		pItem = new QTableWidgetItem;
		pItem->setData(Qt::UserRole, i + 1); // 图层号+1
		pGraphLayerList->setItem(i - 1, 2, pItem);
	}

	unsigned int nCurEditingLayerIdx = m_pGraphView->GetCurEditingLayerIdx();
	if (nCurEditingLayerIdx)
	{
		SetLayerRowSelState(nCurEditingLayerIdx - 1, true);
		SetOtherItemsUnSelected(nCurEditingLayerIdx - 1);
	}
	else
	{
		SetOtherItemsUnSelected(-1);
	}

	connect(pGraphLayerList, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slotItemClicked(QTableWidgetItem*)));
}

void CGraphTabView::UpdateGraphLayerList(QTableWidget*pGraphLayerList)
{
	Q_ASSERT(pGraphLayerList);
	if (pGraphLayerList == nullptr)
		return;

	Q_ASSERT(m_pGraphFile);
	if (m_pGraphFile == nullptr)
		return;

	CGraphFile *pGraphFile = m_pGraphFile;

	unsigned int nLayerCount = (unsigned int)pGraphFile->GetLayerCount();
	QTableWidgetItem *pItem = nullptr;
	std::shared_ptr<CGraphicsLayer> pLayer = nullptr;

	unsigned int i = 0;
	for (i = 1; i <= CGraphFile::MAX_LAYER_CNT && i <= nLayerCount; i++)
	{
		pLayer = pGraphFile->GetLayer(i);
		pItem = new QTableWidgetItem;
		pItem->setIcon(::GetModuleIcon(ICON_EYE_OFF));
		pGraphLayerList->setItem(i - 1, 0, pItem);
		pItem->setData(Qt::UserRole, 1);
		pItem->setData(Qt::UserRole + 1, i + 1);

		QString szTxt = QString(QStringLiteral("图层 %1").arg(i));

		pItem = new QTableWidgetItem(szTxt);
		pItem->setIcon(::GetModuleIcon(ICON_LAYER));
		pGraphLayerList->setItem(i - 1, 1, pItem);
		pItem->setData(Qt::UserRole, i + 1); // 图层号+1

		pItem = new QTableWidgetItem;
		pItem->setData(Qt::UserRole, i + 1); // 图层号+1
		pGraphLayerList->setItem(i - 1, 2, pItem);
	}

	unsigned int nCurEditingLayerIdx = m_pGraphView->GetCurEditingLayerIdx();
	if (nCurEditingLayerIdx)
	{
		SetLayerRowSelState(nCurEditingLayerIdx - 1, true);
		SetOtherItemsUnSelected(nCurEditingLayerIdx - 1);
	}
	else
	{
		SetOtherItemsUnSelected(-1);
	}
}

void  CGraphTabView::slotItemClicked(QTableWidgetItem* pItem)
{
	Q_ASSERT(pItem);
	if (pItem == nullptr)
		return;
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView == nullptr)
		return;

	QTableWidget *m_pLayerList = ::GetModuleApi()->GetGraphLayerList();
	Q_ASSERT(m_pLayerList);
	if (m_pLayerList == nullptr)
		return;

	int nCol = pItem->column();
	int nRow = pItem->row();

 	if (nCol == 0)
	{
		QVariant varIdx = pItem->data(Qt::UserRole + 1);
		int nIdx = varIdx.toInt();

		QVariant var = pItem->data(Qt::UserRole);
		bool bDsp = false;
		if (var.toInt() != 0)
		{
			pItem->setIcon(::GetModuleIcon(ICON_EYE_OFF));
			pItem->setData(Qt::UserRole, 0);
			bDsp = false;
		}
		else
		{
			pItem->setIcon(::GetModuleIcon(ICON_EYE_ON));
			pItem->setData(Qt::UserRole, 1);
			bDsp = true;
		}
		// 
		SetLayerVisible(nIdx - 1, bDsp);
	}
	else if (nCol == 1)
	{// 如果选择当前编辑的图层
		// 互斥，不能有多个同时在编辑
		QTableWidgetItem* pEditbleItem = m_pLayerList->item(nRow, 0);
		QVariant var = pEditbleItem->data(Qt::UserRole);
		if (var.toInt() == 0)
		{// 不可编辑, 

		}
		else
		{// 可以编辑
			SetLayerRowSelState(nRow, true);
			m_pGraphView->SetCurEditingLayer(nRow + 1);
			// 如果可以编辑 设置当前的图层号为当前的编辑 设置其他的图层取消状态
			SetOtherItemsUnSelected(nRow);
		}
	}
	else if (nCol == 2)
	{
		// 互斥，不能有多个同时在编辑
		QTableWidgetItem* pEditbleItem = m_pLayerList->item(nRow, 0);
		QVariant var = pEditbleItem->data(Qt::UserRole);
		if (var.toInt() == 0)
		{// 不可编辑, 

		}
		else
		{// 可以编辑		 
			SetLayerRowSelState(nRow, true);
			m_pGraphView->SetCurEditingLayer(nRow + 1);
			// 如果可以编辑 设置当前的图层号为当前的编辑 设置其他的图层取消状态
			SetOtherItemsUnSelected(nRow);
		}
	}
}

void CGraphTabView::SetOtherItemsUnSelected(int nCurRow)
{
	QTableWidget *m_pLayerList = ::GetModuleApi()->GetGraphLayerList();
	Q_ASSERT(m_pLayerList);
	if (m_pLayerList == nullptr)
		return;

	int nRowCount = m_pLayerList->rowCount();
	int i = 0;

	for (i = 0; i < nRowCount; i++)
	{
		if (i != nCurRow)
		{
			SetLayerRowSelState(i, false);
		}
	}
 
}
void CGraphTabView::SetLayerRowSelState(int nRow, bool bSelected)
{
	QTableWidget *m_pLayerList = ::GetModuleApi()->GetGraphLayerList();
	Q_ASSERT(m_pLayerList);
	if (m_pLayerList == nullptr)
		return;

	int nRowCount = m_pLayerList->rowCount();

	Q_ASSERT(nRow >= 0 && nRow < nRowCount);

	QTableWidgetItem *pSecondItem = nullptr;
	QTableWidgetItem *pThirdItem = nullptr;

	pSecondItem = m_pLayerList->item(nRow, 1);
	pThirdItem = m_pLayerList->item(nRow, 2);
	Q_ASSERT(pSecondItem);
	Q_ASSERT(pThirdItem);

	if (bSelected)
	{
		if (pSecondItem)
		{
			pSecondItem->setBackgroundColor(XGraphics::LightBlue);
		}
		if (pThirdItem)
		{
			pThirdItem->setIcon(::GetModuleIcon(ICON_LEFT));
		}
	}
	else
	{
		if (pSecondItem)
		{
			pSecondItem->setBackgroundColor(Qt::white);
		}
		if (pThirdItem)
		{
			pThirdItem->setIcon(::GetModuleIcon(ICON_NULL));
		}
	} 
}
void CGraphTabView::SetLayerVisible(unsigned int nLayIdx, bool bDisp)
{
	m_pGraphView->SetLayerVisible(nLayIdx, bDisp);

	unsigned int nCurEditingIdx = m_pGraphView->GetCurEditingLayerIdx();
	// 如果当前开关的图层正好是在编辑的图层，则切换状态
	if (nCurEditingIdx > 0)
	{
		SetOtherItemsUnSelected(nCurEditingIdx - 1);
		SetLayerRowSelState(nCurEditingIdx - 1, true);
	}
	else
	{// 所有的都不可见了，则取消
		SetOtherItemsUnSelected(-1);
	}
}

void CGraphTabView::resizeEvent(QResizeEvent *event)
{
	Q_UNUSED(event);
    m_pGraphScene->setSceneRect(0, 0, this->width(), this->height());
}
