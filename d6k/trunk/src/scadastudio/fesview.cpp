#include "fesview.h"
#include "festable.h"
#include "scadastudiodefine.h"
#include "scadastudio/icore.h"
#include "scadastudio/quoteItem.h"
#include "fesdatamodel.h"
#include "scadastudio/imainwindow.h"
#include "fesmodule.h"
#include "scadastudio/define.h"

/*! \fn CFesView::CFesView(IMainModuleInterface *pCore, Config::CFesData *pFesData, CQuoteItem *pItem)
********************************************************************************************************* 
** \brief CFesView::CFesView 
** \details 
** \param parent 
** \param pFesData 
** \return  
** \author gw
** \date 2016��2��14�� 
** \note 
********************************************************************************************************/
CFesView::CFesView(IMainModuleInterface *pCore, Config::CFesData *pFesData, CQuoteItem *pItem
	, Config::CFesConfigData *pFesConfig)
	: QMainWindow((QWidget *)pCore->GetUIMgr()), m_pCore(nullptr)
{
	Q_ASSERT(pCore);
	Q_ASSERT(pFesData);
	Q_ASSERT(pItem);

	m_pCore = pCore;
	
	m_pTable = new CFesTable(this, pFesData, pItem, FES, pCore, pFesConfig);

	setCentralWidget(m_pTable);

	m_pFesData = pFesData;
}

/*! \fn CFesView::~CFesView()
********************************************************************************************************* 
** \brief CFesView::~CFesView 
** \details 
** \return  
** \author gw
** \date 2016��2��14�� 
** \note 
********************************************************************************************************/
CFesView::~CFesView()
{	
	delete m_pTable;
	m_pTable = nullptr;
}

/*! \fn 
********************************************************************************************************* 
** \brief CFesView::Save 
** \details 
** \return void 
** \author gw
** \date 2016��2��18�� 
** \note 
********************************************************************************************************/
void CFesView::Save()
{
	//m_pFesData->Save();
}

/*! \fn 
********************************************************************************************************* 
** \brief CFesView::Refresh 
** \details 
** \return void 
** \author gw
** \date 2016��2��18�� 
** \note 
********************************************************************************************************/
void CFesView::Refresh()
{
	m_pTable->GetModel()->RefrushModel();
}

void CFesView::Delete()
{
	m_pTable->setModel(nullptr);
}

///*! \fn void CFesView::ItemChanged(QStandardItem *item)
//********************************************************************************************************* 
//** \brief CFesView::ItemChanged 
//** \details ǰ�������޸�,��Ӧviewˢ��
//** \param item 
//** \return void 
//** \author gw
//** \date 2016��4��25�� 
//** \note 
//********************************************************************************************************/
//void CFesView::ItemChanged(QStandardItem *item)
//{
//	int nType = item->data(Qt::UserRole).toInt();
//	
//	if(nType == FES_ITEM)
//	{
//		//if (m_pFesData->m_strDescription.compare(item->data(Qt::EditRole).toString()) != 0)
//		//{
//		//	m_pFesData->m_strDescription = item->data(Qt::EditRole).toString();
//
//		//	if (m_pTable->GetModel())
//		//	{
//		//		m_pTable->GetModel()->RefrushModel();
//		//	}
//
//		//	m_pFesData->SetModifyFlag(true);
//		//}
//	}
//	else if(nType == FES_ITEM_GROUP)
//	{
//		QStandardItemModel *pModel = m_pCore->GetUIMgr()->GetLeftTreeModel();
//		
//		QVector<QModelIndex> vec;
//		vec.clear();
//
//		//ǰ�������޸�,��������ǰ����
//		CFesModel::GetItemMap(pModel, pModel->indexFromItem(item), FES_ITEM, vec);
//		for (auto &tmp : vec)
//		{
//			m_pFesData;
//			QString strGroupName = CFesModel::GetGroupName(tmp, FES_CONFIG);
//			m_pFesData->m_strGroupName = strGroupName;
//		}
//		
//
//	}
//
//	//if (m_pFesData->GetModifyFlag())
//	//{
//	//	m_pTable->GetModel()->RefrushModel();
//	//}
//}