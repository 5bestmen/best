/*! @file graph_file.cpp
<PRE>
********************************************************************************
ģ����       :  
�ļ���       : graph_file.cpp
�ļ�ʵ�ֹ��� :
����         :
�汾         :
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  �������.
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
	m_szGraphName = "ͼ��1";

	m_Size = QSize(1400, 800);
	// ����ͼ��
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
	m_szGraphName = "ͼ��1";

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
	// ����ͼ��
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
** \details ��ָ��ͼ������һ������
** \param nLayerIdx  ��1��ʼ
** \param pObj 
** \return void 
** \author LiJin 
** \date 2016��1��18�� 
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
** \details ȷ��ͼԪ���ᱻ�ƶ�������ȥ
** \param delta 
** \param arrSelections 
** \return bool 
** \author LiJin 
** \date 2016��1��14�� 
** \note 
********************************************************************************************************/
bool  CGraphFile::IsWidgetsCanMove(const QPointF & delta, const std::vector <CBaseWidget *>& arrSelections) const
{
	bool bCanMove = true;
	// �ƶ�����С��һ������
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
			// �ж�
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
** \brief  ��ָ��ͼ��ɾ��һ��ָ������
** \details
** \param nLayerIdx  ��1��ʼ
** \param pObj
** \return bool
** \author LiJin
** \date 2010��9��6��
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
// 	{//�ҵ�
// 		m_arrWidgets.erase(it);
// 		// �����ڴ�
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
	// ȷ����ǰ���ڱ༭��ͼ��
	for (auto i : m_arrLayers)
	{
		if (i->IsVisible())
		{
			nCurEditLayer = i->GetLayerIdx();
			bFind = true;
			break;
		}
	}
	// û��ͼ�㣬���ܱ༭
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
** \details  SHIFT ���� ��ѡ
** \param nLayerIdx 
** \param pWidget 
** \return void 
** \author LiJin 
** \date 2016��2��6�� 
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
	{// ���ԭ����Ϊ�գ��������
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
