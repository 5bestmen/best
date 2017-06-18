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

#include "standarview.h"
#include "singleton.h"
#include <QGridLayout>

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
CStandardView::CStandardView(QWidget *parent, QString strDesc, QString strTableName, int nType)
	: QMainWindow(parent)
{
	QGridLayout *pLayout = new QGridLayout(this);
	
	m_pTalbeView = new CTableView(this, strDesc, nType, strTableName);

	m_pTalbeView->GetFieldInfo(strTableName);

	pLayout->addWidget(m_pTalbeView, 0, 0);
	

	m_pOkButton = new QPushButton(QStringLiteral("确定"), this);
	m_pCancelButton = new QPushButton(QStringLiteral("取消"), this);
	m_pDeleteButton = new QPushButton(QStringLiteral("删除"), this);

	connect(m_pOkButton, SIGNAL(clicked()), this, SLOT(test()));

	QWidget *topWidget = new QWidget(this);

	QVBoxLayout *pVerLayout = new QVBoxLayout();

	pVerLayout->setSpacing(2);

	pVerLayout->addWidget(m_pOkButton);
	pVerLayout->addWidget(m_pCancelButton);
	pVerLayout->addWidget(m_pDeleteButton);
	//pVerLayout->addStretch(2);

	topWidget->setLayout(pVerLayout);

	topWidget->setFixedWidth(200);

	pVerLayout->addItem((new QSpacerItem(20, 178, QSizePolicy::Minimum, QSizePolicy::Expanding)));

	pVerLayout->setSpacing(2);

	pLayout->addWidget(topWidget, 0, 1);


	pLayout->setContentsMargins(0,0,0,0);//设置布局内部四边的空隙
	pLayout->setAlignment(Qt::AlignHCenter);

	QWidget *pWidget = new QWidget(this);

	pWidget->setLayout(pLayout);

	setCentralWidget(pWidget);
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
	deleteLater();
}

#include "sortfiltermodel.h"

#include "tablemodel.h"

void CStandardView::test()
{	
	//m_pTalbeView->setSortingEnabled(false);
	
	//m_pTalbeView->m_pSortFilterModel->setDynamicSortFilter(false);

	QVector<QString> tmp;
	
	for(int i = 0; i < 14; i++)
	{
		QString strTmp = "1";

		tmp.append(strTmp);
	}
	
	m_pTalbeView->AddRow(tmp);


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
	this->deleteLater();
}