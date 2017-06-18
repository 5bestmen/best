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

#include <QGridLayout>
#include <qaction.h>
#include <qtoolbar.h>
#include "standarview.h"
#include "singleton.h"
#include "sortfiltermodel.h"
#include "tablemodel.h"

/*! \fn CStandardView::CStandardView(QWidget *parent, QString strDesc, QString strTableName, int nType)
********************************************************************************************************* 
** \brief CStandardView::CStandardView 
** \details 
** \param parent 
** \param strDesc 描述
** \param strTableName 表
** \param nType 节点类型
** \return  
** \author gw
** \date 2015年11月20日 
** \note 
********************************************************************************************************/
CStandardView::CStandardView(IMainWindow *pUi, QString strTagName, QString strDescName, QString strTableName, int nType)
	: QMainWindow(NULL), m_pUi(NULL)
{
	//QGridLayout *pLayout = new QGridLayout(this);
	//
	//m_pTalbeView = new CTableView(this, strTagName, strDescName, nType, strTableName);

	//m_pTalbeView->GetFieldInfo(strTableName);

	//pLayout->addWidget(m_pTalbeView, 0, 0);
	//

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

	m_pTalbeView = new CTableView(pUi, strTagName, strDescName, nType, strTableName);

	m_pTalbeView->GetFieldInfo(strTableName);

	QAction *pSaveAction = new QAction(QIcon(SAVE_PNG), "保存", this);
	QAction *pDeleteAction = new QAction(QIcon(DELETE_PNG), "删除", this);

	QToolBar *pToolBar = addToolBar("标准toolbar");

	pToolBar->addAction(pSaveAction);
	pToolBar->addAction(pDeleteAction);

	connect(pSaveAction, SIGNAL(triggered()), this, SLOT(OnOk()));
	connect(pDeleteAction, SIGNAL(triggered()), this, SLOT(OnDelete()));

	setCentralWidget(m_pTalbeView);
}

/*! \fn CStandardView::~CStandardView()
********************************************************************************************************* 
** \brief CStandardView::~CStandardView
** \details 
** \return  
** \author gw
** \date 2015年11月18日 
** \note 
********************************************************************************************************/
CStandardView::~CStandardView()
{
	m_pTalbeView->deleteLater();
	
	this->deleteLater();
}

void CStandardView::OnOk()
{	
	//m_pTalbeView->setSortingEnabled(false);
	
	//m_pTalbeView->m_pSortFilterModel->setDynamicSortFilter(false);

	m_pTalbeView->Save();

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

/*! \fn void CStandardView::closeEvent( QCloseEvent * event )
********************************************************************************************************* 
** \brief CStandardView::closeEvent 
** \details 
** \param event 
** \return void 
** \author gw
** \date 2015年11月20日 
** \note 
********************************************************************************************************/
void CStandardView::closeEvent(QCloseEvent * event)
{	
	Q_UNUSED(event);
	
	this->deleteLater();
}

#include <qheaderview.h>

/*! \fn void CStandardView::OnDelete()
********************************************************************************************************* 
** \brief CStandardView::OnDelete 
** \details 删除
** \return void 
** \author gw
** \date 2015年12月14日 
** \note 
********************************************************************************************************/
void CStandardView::OnDelete()
{
	m_pTalbeView->SetDeleFlag();

	//m_pTalbeView->horizontalHeader()->reset();
}