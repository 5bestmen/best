/*! @file graph_file.cpp
<PRE>
********************************************************************************
模块名       :  
文件名       : graph_file.cpp
文件实现功能 :
作者         :
版本         :
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  任务入口.
*  @author LiJin
*  @version 1.0
*  @date    2014.10.26
*******************************************************************************/

#include "graph_file.h"
#include "graph_folder.h"

#include "base_widget.h"
#include "background.h"
#include "graphics_layer.h"
#include "graphscene.h"

  
CGraphFile::CGraphFile()
{
	m_pScene = nullptr;
	m_pFolder = nullptr;
	m_nMaxWidgetID = 0;
	m_pScene = nullptr;
	m_szGraphName = "图形1";

	m_Size = QSize(1400, 800);
	// 建立图层
  	unsigned int nIdx = 1;
	for (unsigned int i = 0; i < MAX_LAYER_CNT; i++)
	{
		m_arrLayers[i] = std::make_shared<CGraphicsLayer>(nIdx,nullptr);
		m_arrLayers[i]->SetGraphSize(m_Size);
		nIdx++;
	}
//	m_nCurEditingLayer = 1;
#if 0
	m_arrLayers.clear();
	for (unsigned int i = 0; i < MAX_LAYER_CNT;i++)
	{
		std::shared_ptr<CGraphicsLayer> pLayer = std::make_shared<CGraphicsLayer>(nIdx);
		m_arrLayers.push_back(pLayer);
		nIdx++;
	}
#endif
	m_pBackground = std::make_shared<CBackground>();

	// for test
	SetGraphSize(m_Size);	
}

CGraphFile::CGraphFile(CGraphScene *pScene) :m_pScene(pScene)
{
	m_pFolder = nullptr;
	m_nMaxWidgetID = 0;

	m_Size = QSize(1400, 800);
	m_szGraphName = "图形1";

	unsigned int nIdx = 1;
	//for (auto i : m_arrLayers)
//	m_arrLayers = new CGraphicsLayer[MAX_LAYER_CNT];
	for (unsigned int i = 0; i < MAX_LAYER_CNT;i++)
	{	
		m_arrLayers[i] = std::make_shared<CGraphicsLayer>(nIdx, pScene);
	//	m_arrLayers[i] = new CGraphicsLayer(nIdx);
		m_arrLayers[i]->SetGraphSize(m_Size);
		nIdx++; 
	}
//	m_nCurEditingLayer = 1;

#if 0
	m_arrLayers.clear();
	for (unsigned int i = 0; i < MAX_LAYER_CNT; i++)
	{
		std::shared_ptr<CGraphicsLayer> pLayer = std::make_shared<CGraphicsLayer>(nIdx);
		m_arrLayers.push_back(pLayer);
		nIdx++;
	}
#endif
	m_pBackground = std::make_shared<CBackground>();
	// for test
	SetGraphSize(m_Size);
	Q_ASSERT(pScene);
}

CGraphFile::CGraphFile(CGraphFolder *pFolder)
{
	m_Size = QSize(1600, 800);
	// 建立图层
	unsigned int nIdx = 1;
	for (auto i : m_arrLayers)
	{
	 	i = std::make_shared<CGraphicsLayer>(nIdx,nullptr);
		i->SetGraphSize(m_Size);
		nIdx++;
	}
//	m_nCurEditingLayer = 1;

	m_pBackground = std::make_shared<CBackground>();
	
	// for test
	SetGraphSize(m_Size);
}

CGraphFile::~CGraphFile()
{

}

void CGraphFile::SetGraphSize(QSize &sizeGraph)
{
	Q_ASSERT(sizeGraph.height() && sizeGraph.width());

	if (m_pBackground)
	{
		m_pBackground->SetSize(sizeGraph);
	}

	for (auto i : m_arrLayers)
	{
	 	Q_ASSERT(i != nullptr);
	 	if (i != nullptr)
		{
		 //	i->SetGraphSize(sizeGraph);
		}
	}
}


QRectF& CGraphFile::GetBackgroundRect() const
{
	Q_ASSERT(m_pBackground);
	return m_pBackground->GetBackgroundRect();
}

void CGraphFile::DrawBackground(QPainter *painter, const QRectF &rect)
{
	if (m_pBackground)
	{
		m_pBackground->DrawBackground(painter, rect);
	}
}
void CGraphFile::DrawMovingWidgets(QPainter *pPainter, unsigned int nEditingLayerIdx)
{
	if (nEditingLayerIdx == 0 || nEditingLayerIdx > MAX_LAYER_CNT)
	{
		return;
	}
	if (pPainter)
	{
		m_arrLayers[nEditingLayerIdx - 1]->DrawMovingWidgets(pPainter);
	}
}

/*! \fn void CGraphFile::AddWidget(unsigned int nLayerIdx, CBaseWidget *pObj)
********************************************************************************************************* 
** \brief CGraphFile::AddWidget 
** \details 在指定图层增加一个对象
** \param nLayerIdx  从1开始
** \param pObj 
** \return void 
** \author LiJin 
** \date 2016年1月18日 
** \note 
********************************************************************************************************/
void CGraphFile::AddWidget(unsigned int nLayerIdx, CBaseWidget *pObj)
{
	Q_ASSERT(pObj);
	if (pObj == nullptr)
		return;

	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= MAX_LAYER_CNT);
	if (nLayerIdx > MAX_LAYER_CNT)
		return;

	std::string szFolderName;
	if (m_pFolder)
	{
		szFolderName = m_pFolder->GetFolderName();
	}

	pObj->BuildWidgetName(++m_nMaxWidgetID, nLayerIdx, m_szGraphName, szFolderName);

	Q_ASSERT(m_arrLayers[nLayerIdx-1]);
	if (m_arrLayers[nLayerIdx-1] != nullptr)
	{
		m_arrLayers[nLayerIdx-1]->AddWidget(pObj);
		Q_ASSERT(m_pScene);
		if (m_pScene != nullptr)
		{
			m_pScene->addItem(pObj);
		}
	}
}
/*! \fn bool  CGraphFile::IsWidgetsCanMove(const QPointF & , const std::vector <CBaseWidget *>& ) const
********************************************************************************************************* 
** \brief CGraphFile::IsWidgetsCanMove 
** \details 确定图元不会被移动到界外去
** \param delta 
** \param arrSelections 
** \return bool 
** \author LiJin 
** \date 2016年1月14日 
** \note 
********************************************************************************************************/
bool  CGraphFile::IsWidgetsCanMove(const QPointF & delta, const std::vector <CBaseWidget *>& arrSelections) const
{
	bool bCanMove = true;
	// 移动距离小于一个像素
	if ( fabs(double(delta.x())) < 1.0f && fabs(double(delta.y())) < 1.0f) 
	{
		return false;
	}

	if (arrSelections.empty() == true)
	{
		return false;
	}

	QRectF rcTemp;
	//auto iter = arrSelections.begin();	
	for (auto iter : arrSelections)
	{
		if ((iter)->IsLocked() == false)
		{
			rcTemp = iter->GetPosition();
			rcTemp.translate(delta);
			// 判断
			if (rcTemp.x() <0 || rcTemp.y() < 0 || rcTemp.right() > m_Size.width() || rcTemp.bottom() > m_Size.height())
			{
				bCanMove = false;
				break;
			}
		}
	}
	return bCanMove;
}
/*! \fn bool CGraphFile::EraseWidget( unsigned int nLayerIdx, CBaseWidget *pObj )
*********************************************************************************************************
** \brief  在指定图层删除一个指定对象
** \details
** \param nLayerIdx  从1开始
** \param pObj
** \return bool
** \author LiJin
** \date 2010年9月6日
** \note
********************************************************************************************************/
bool CGraphFile::EraseWidget(unsigned int nLayerIdx, CBaseWidget *pObj)
{
 	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= MAX_LAYER_CNT);
 	Q_ASSERT(pObj);
	if (nLayerIdx > 0 && nLayerIdx <= MAX_LAYER_CNT && pObj && &(m_arrLayers[nLayerIdx-1]))
	{
		//pObj->setParentItem(nullptr);
		Q_ASSERT(m_pScene);
		if (m_pScene != nullptr)
		{
			m_pScene->removeItem(pObj);
		}
		return (m_arrLayers[nLayerIdx-1])->EraseWidget(pObj);
	}
	return false;
// 	if (pObj == nullptr)
// 		return false;
// 
// 	auto it = std::find(m_arrWidgets.begin(), m_arrWidgets.end(), pObj);
// 	if (it != m_arrWidgets.end())
// 	{//找到
// 		m_arrWidgets.erase(it);
// 		// 回收内存
// 		delete pObj;
// 		return true;
// 	}
// 	return false;
}


std::shared_ptr<CGraphicsLayer> CGraphFile::GetLayer(unsigned int nIdx)
{
	Q_ASSERT(nIdx > 0 && nIdx <= MAX_LAYER_CNT);
	if (nIdx == 0 || nIdx > MAX_LAYER_CNT)
	{
		return nullptr;
	}

	return m_arrLayers[nIdx - 1];
}

#if 0
CGraphicsLayer* CGraphFile::GetLayer(unsigned int nIdx)
{
	Q_ASSERT(nIdx > 0 && nIdx <= MAX_LAYER_CNT);

	return m_arrLayers[nIdx - 1];
}
#endif

void  CGraphFile::SetLayerVisible(unsigned int nLayerIdx, bool bDisp, unsigned int & nCurEditLayer)
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return;

	auto pLayer =  m_arrLayers[nLayerIdx - 1];
	if (pLayer)
	{
		pLayer->SetVisible(bDisp);
	}
	bool bFind = false;
	// 确定当前正在编辑的图层
	for (auto i : m_arrLayers)
	{
		if (i->IsVisible())
		{
			nCurEditLayer = i->GetLayerIdx();
			bFind = true;
			break;
		}
	}
	// 没有图层，不能编辑
	if (bFind == false)
	{
		nCurEditLayer = 0;
	}
}

bool CGraphFile::IsLayerVisible(unsigned int nLayerIdx)  
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return false;

	auto pLayer = GetLayer(nLayerIdx);
	return pLayer->IsVisible();
}

/*! \fn void CGraphFile::SelectMultiWidgets(unsigned int nLayerIdx, CBaseWidget *pWidget)
********************************************************************************************************* 
** \brief CGraphFile::SelectMultiWidgets 
** \details  SHIFT 按键 多选
** \param nLayerIdx 
** \param pWidget 
** \return void 
** \author LiJin 
** \date 2016年2月6日 
** \note 
********************************************************************************************************/
void CGraphFile::SelectMultiWidgets(unsigned int nLayerIdx, CBaseWidget *pWidget)
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return ;

	auto pLayer = GetLayer(nLayerIdx);	
	pLayer->SelectMultiWidgets(pWidget);
}

CBaseWidget * CGraphFile::GetCurrentWidget(unsigned int nLayerIdx)
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return nullptr;

	return m_arrLayers[nLayerIdx - 1]->GetCurrentWidget();
}
void CGraphFile::SetCurrentWidget(unsigned int nLayerIdx, CBaseWidget*pWidget)
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return ;
	m_arrLayers[nLayerIdx - 1]->SetCurrentWidget(pWidget);
}

bool CGraphFile::SelectAll(unsigned int nLayerIdx)
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return false;

	return  m_arrLayers[nLayerIdx - 1]->SelectAll();
}

void CGraphFile::SelectWidgetsInRect(unsigned int nLayerIdx, const QRectF &rcSel)
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return  ;
	m_arrLayers[nLayerIdx - 1]->SelectWidgetsInRect(rcSel);
}

bool CGraphFile::SelectNone(unsigned int nLayerIdx)
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return false;

	if (m_arrLayers[nLayerIdx - 1]->GetSelectedWidgets().empty() == false)
	{// 如果原来不为空，现在清空
		m_arrLayers[nLayerIdx - 1]->Select(nullptr);
		return true;
	}
	return false;
}

bool  CGraphFile::SelectWidget(unsigned int nLayerIdx, CBaseWidget *pWidget)
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return false;

	m_arrLayers[nLayerIdx - 1]->Select(pWidget);
	 
	return false;
}
void CGraphFile::Deselect(unsigned int nLayerIdx, CBaseWidget* pWidget)
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return  ;

	m_arrLayers[nLayerIdx - 1]->Deselect(pWidget); 
}

bool CGraphFile::ReSelectWidget(unsigned int nLayerIdx, CBaseWidget *pWidget)
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return false;

	return	m_arrLayers[nLayerIdx - 1]->ReSelectWidget(pWidget);	 
}

/*@}*/
