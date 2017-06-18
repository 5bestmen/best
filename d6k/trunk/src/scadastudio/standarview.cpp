/*! @file
<PRE>
********************************************************************************
ģ����       :  ��׼�Ի���
�ļ���       :  channelview.cpp
�ļ�ʵ�ֹ��� :  ��׼�Ի���
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��> ͨ�� װ��
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
** \param strDesc ����
** \param strTableName ��
** \param nType �ڵ�����
** \return  
** \author gw
** \date 2015��11��20�� 
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

	//m_pOkButton = new QPushButton(QStringLiteral("ȷ��"), this);
	//m_pCancelButton = new QPushButton(QStringLiteral("ȡ��"), this);
	//m_pDeleteButton = new QPushButton(QStringLiteral("ɾ��"), this);

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


	//pLayout->setContentsMargins(0,0,0,0);//���ò����ڲ��ıߵĿ�϶
	//pLayout->setAlignment(Qt::AlignHCenter);

	//QWidget *pWidget = new QWidget(this);

	//pWidget->setLayout(pLayout);

	//setCentralWidget(pWidget);

	//m_pUi = parent;

	m_pTalbeView = new CTableView(pUi, strTagName, strDescName, nType, strTableName);

	m_pTalbeView->GetFieldInfo(strTableName);

	QAction *pSaveAction = new QAction(QIcon(SAVE_PNG), "����", this);
	QAction *pDeleteAction = new QAction(QIcon(DELETE_PNG), "ɾ��", this);

	QToolBar *pToolBar = addToolBar("��׼toolbar");

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
** \date 2015��11��18�� 
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
** \date 2015��11��20�� 
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
** \details ɾ��
** \return void 
** \author gw
** \date 2015��12��14�� 
** \note 
********************************************************************************************************/
void CStandardView::OnDelete()
{
	m_pTalbeView->SetDeleFlag();

	//m_pTalbeView->horizontalHeader()->reset();
}