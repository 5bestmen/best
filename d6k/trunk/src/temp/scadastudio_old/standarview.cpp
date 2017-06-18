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

#include "standarview.h"
#include "singleton.h"
#include <QGridLayout>

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
CStandardView::CStandardView(QWidget *parent, QString strDesc, QString strTableName, int nType)
	: QMainWindow(parent)
{
	QGridLayout *pLayout = new QGridLayout(this);
	
	m_pTalbeView = new CTableView(this, strDesc, nType, strTableName);

	m_pTalbeView->GetFieldInfo(strTableName);

	pLayout->addWidget(m_pTalbeView, 0, 0);
	

	m_pOkButton = new QPushButton(QStringLiteral("ȷ��"), this);
	m_pCancelButton = new QPushButton(QStringLiteral("ȡ��"), this);
	m_pDeleteButton = new QPushButton(QStringLiteral("ɾ��"), this);

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


	pLayout->setContentsMargins(0,0,0,0);//���ò����ڲ��ıߵĿ�϶
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
** \date 2015��11��18�� 
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
** \date 2015��11��20�� 
** \note 
********************************************************************************************************/
void CStandardView::closeEvent(QCloseEvent * event)
{
	this->deleteLater();
}