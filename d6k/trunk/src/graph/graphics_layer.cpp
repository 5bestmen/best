/*! @file graphics_layer.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  graphics_layer.cpp
�ļ�ʵ�ֹ��� :  ͼ��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ͼ��
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/
#include "graphics_layer.h"
#include "graph_file.h"
#include "graphscene.h"
#include "graphview.h"
#include "group_widget.h"

#include "colour_define.h"
#include "shape_widget.h"
#include "line_widget.h"

#include <QDebug>
#include <deque>
 
CGraphicsLayer::CGraphicsLayer()
{
	m_nIdx = 0;
	m_bIsVisible = true;
	m_pInsertObj = nullptr;
	m_dblMaxZValue = 1.0f;

	m_pSelectedTempGrpWidget = std::make_shared<CGroupWidget>();
//	setFlag(QGraphicsItem::ItemIsMovable, false);
//	setVisible(false);
}

CGraphicsLayer::CGraphicsLayer(unsigned int nLayerIndex, CGraphScene *pScene) :m_nIdx(nLayerIndex), m_pScene(pScene)
{
	m_bIsVisible = true;
	m_pInsertObj = nullptr;
	m_dblMaxZValue = 1.0f;

	m_pSelectedTempGrpWidget = std::make_shared<CGroupWidget>();
//	setFlag(QGraphicsItem::ItemIsMovable, false);
//	setVisible(false);
}

CGraphicsLayer::~CGraphicsLayer()
{
	m_arrSelection.clear();
	// �����Դ֮�󣬾Ͳ����ظ�������Դ
	m_pSelectedTempGrpWidget->Clear();

	if (m_pScene == nullptr)
	{
		//��û��sceneʱ  ��Ҫ�ֶ�ɾ��
		for (auto it : m_arrWidgets)
		{
			Q_ASSERT(it);
			if (it)
			{
				delete it;
				it = nullptr;
			}
		}
	}


	m_arrWidgets.clear();

}

const CGraphicsLayer& CGraphicsLayer::operator=(const CGraphicsLayer &src)
{
	//���ԭ�е�����
	for (auto it : m_arrWidgets)
	{
		Q_ASSERT(it);
		if (it)
		{
			delete it;
			it = nullptr;
		}
	}

	m_arrWidgets.clear();
	m_arrSelection.clear();
	m_pSelectedTempGrpWidget->Clear();
	m_pInsertObj = nullptr;
	m_arrSelection.clear();


	for each (CBaseWidget * pBaseWgt in src.m_arrWidgets)
	{
		CBaseWidget *pWidget = nullptr;

		if (pBaseWgt->GetWidgetType() == CShapeWidget::WIDGET_GROUP)
		{
			//���
			CGroupWidget *pGroupWgt = (CGroupWidget *)pBaseWgt;
			Q_ASSERT(pGroupWgt);

			if (pGroupWgt == nullptr)
			{
				return *this;
			}
			std::vector <CBaseWidget *> arrBaseWgt;

			for (int i = 0; i < pGroupWgt->GetWidgetCount(); i++)
			{
				pWidget = CWidgetFactory::CreateWidget(pGroupWgt->GetWidget(i)->GetPosition(), pGroupWgt->GetWidget(i)->GetWidgetType());

				Q_ASSERT(pWidget);
				if (pWidget)
				{
					*pWidget = *pGroupWgt->GetWidget(i);
					//pWidget->SetRotateAngle(pGroupWgt->GetWidget(i)->GetRotateAngle());
					arrBaseWgt.push_back(pWidget);
				}


			}

			if (m_pScene != nullptr)
			{
				GroupInputWidgets(arrBaseWgt, pGroupWgt->GetRotateAngle(), CGraphicsLayer::FATHER_GROUP);
			}
			else
			{
				if (arrBaseWgt.size() <= 1)
				{
					return *this;
				}

				std::vector<CBaseWidget *> arrSel;

				bool bFlag = false;
				QRectF rcUnion, rcPos;

				// ��������group��ȫ���ռ�
				for (auto it : arrBaseWgt)
				{
					if ((it)->IsLocked())
						continue;

					if (bFlag == false)
					{
						bFlag = true;
						rcUnion = (it)->GetPosition();
					}
					else
					{
						rcUnion = rcUnion.united((it)->GetPosition());
					}
					arrSel.push_back(it);
				}
				Q_ASSERT(rcUnion.width() != 0 && rcUnion.height() != 0);
				if (rcUnion.width() == 0 || rcUnion.height() == 0)
				{
					*this;
				}

				// ���һ��group
				QRectF rcRelation;
				CGroupWidget *pGroup = new CGroupWidget;

				for (auto it2 : arrSel)
				{
					if (it2->IsLocked() == true)
					{
						continue;
					}

					// ÿ��ͼԪ������group�����λ��
					rcPos = (it2)->GetPosition();

					rcRelation = BuildRelativeLocation(rcUnion, rcPos);

					pGroup->AddWidget(it2, rcRelation);
				}

				AddWidget(pGroup);
			}

		}
		else
		{
			pWidget = CWidgetFactory::CreateWidget(pBaseWgt->GetPosition(), pBaseWgt->GetWidgetType());

			Q_ASSERT(pWidget);
			if (pWidget)
			{
				//pWidget->SetRotateAngle(pBaseWgt->GetRotateAngle());

				*pWidget = *pBaseWgt;

				if (pBaseWgt->GetWidgetType() == DRAW_TOOLS_POLYLINE || pBaseWgt->GetWidgetType() == DRAW_TOOLS_POLYGON || pBaseWgt->GetWidgetType() == DRAW_TOOLS_PIPE)
				{
					CLineWidget *PloyLine = dynamic_cast<CLineWidget *>(pWidget);

					CLineWidget *PloyBaseLine = dynamic_cast<CLineWidget *>(pBaseWgt);

					if (PloyLine != nullptr && PloyBaseLine != nullptr)
					{
						for each (QPointF pfPoint in PloyBaseLine->GetAllPoints())
						{
							PloyLine->InsertNewPoint(pfPoint);
						}


						PloyLine->UpdateAllPoints();
					}
				}
			}

			if (pWidget)
			{
				if (m_pScene != nullptr)
				{
					m_pScene->addItem(pWidget);
				}

				

				AddWidget(pWidget);
			}

		}
	}
	return *this;
}

void CGraphicsLayer::SetCurrentWidget(CBaseWidget *pWidget)
{
	m_pInsertObj = pWidget;
}

#if 0
void CGraphicsLayer::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//	__super::paint(pPainter, option, widget);
	QList<QGraphicsItem *> items = this->childItems();

// 	for (auto i: items)
// 	{
// 		i->paint(pPainter, option, widget);
// 	}
	
}

QRectF CGraphicsLayer::boundingRect() const
{
	QRectF rcClient;
// 	rcClient.setX(0);
// 	rcClient.setY(0);
// 	rcClient.setRight(m_size.width());
// 	rcClient.setBottom(m_size.height());
	return rcClient;
}
#endif

/*! \fn void CGraphicsLayer::AddWidget(CBaseWidget *pObj)
********************************************************************************************************* 
** \brief CGraphicsLayer::AddWidget 
** \details ���һ��ͼԪ��ͼ����
** \param pObj 
** \return void 
** \author LiJin 
** \date 2016��1��18�� 
** \note 
********************************************************************************************************/
void CGraphicsLayer::AddWidget(CBaseWidget *pObj)
{
	Q_ASSERT(pObj);
	if (pObj)
	{		

		std::vector <CBaseWidget*>::iterator it_find = std::find(m_arrWidgets.begin(), m_arrWidgets.end(), pObj);
		if (it_find == m_arrWidgets.end())
		{
			pObj->SetLayer(this);
		//	pObj->setParentItem(this);
			m_arrWidgets.push_back(pObj);
		
			if (pObj->zValue() == 0.0f)
			{
				pObj->setZValue(m_dblMaxZValue);
				m_dblMaxZValue += 1.0f;
			}
		}
		else
		{
			Q_ASSERT(false);
		}

		if (pObj->GetWidgetType() == CShapeWidget::WIDGET_GROUP)
		{
			GroupWidgets(dynamic_cast<CGroupWidget*>(pObj));
			return;
		}
	}
}

/*! \fn bool CGraphicsLayer::EraseWidget( unsigned int nLayerIdx, CBaseWidget *pObj )
*********************************************************************************************************
** \brief  ��ָ��ͼ��ɾ��һ��ָ������
** \details
** \param nLayerIdx  ��0��ʼ
** \param pObj
** \return bool
** \author LiJin
** \date 2010��9��6��
** \note
********************************************************************************************************/
bool CGraphicsLayer::EraseWidget(CBaseWidget *pObj)
{
	Q_ASSERT(pObj);
	if (pObj)
	{
		std::vector <CBaseWidget*>::iterator it_find = std::find(m_arrWidgets.begin(), m_arrWidgets.end(), pObj);
		if (it_find != m_arrWidgets.end())
		{//�ҵ�
			//pObj->setParentItem(nullptr);
			m_arrWidgets.erase(it_find);
			// �����ڴ�
			delete pObj;
			return true;
		}
	}
	return false;
}
/*! \fn bool CGraphicsLayer::RemoveWidget(CBaseWidget *pObj)
********************************************************************************************************* 
** \brief CGraphicsLayer::RemoveWidget 
** \details �����ж�����ɾ��ָ��ͼԪ�����ǲ���������Դ
** \param pObj 
** \return bool 
** \author LiJin
** \date 2016��2��11�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::RemoveWidget(CBaseWidget *pObj)
{
	Q_ASSERT(pObj);
	if (pObj)
	{
		std::vector <CBaseWidget*>::iterator it_find = std::find(m_arrWidgets.begin(), m_arrWidgets.end(), pObj);
		if (it_find != m_arrWidgets.end())
		{//�ҵ�
			m_arrWidgets.erase(it_find);
			return true;
		}
	}
	return false;
}

void CGraphicsLayer::SetGraphSize(const QSize &sizeGraph)
{
//	qDebug() << "pos: " << this->pos();
//	this->setPos(sizeGraph.width(), sizeGraph.height());
	m_Size = sizeGraph;
//	this->setPos(0, 0);
}

std::vector <CBaseWidget*> & CGraphicsLayer::GetWidgets()
{
	return m_arrWidgets;
}

void CGraphicsLayer::SetVisible(bool bDsp)
{
	m_bIsVisible = bDsp;
	//this->setVisible(bDsp);
	for (auto i : m_arrWidgets)
	{
		i->setVisible(bDsp);
	}
}

void  CGraphicsLayer::SetLayerIdx(unsigned int nIdx)
{
	Q_ASSERT(nIdx > 0 && nIdx <= CGraphFile::MAX_LAYER_CNT);
	m_nIdx = nIdx;
}

void CGraphicsLayer::InvalidateWidget(CBaseWidget* pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return;
//	Q_ASSERT(m_fnInvalidWidget);
// 	if (m_fnInvalidWidget)
// 	{
// 		m_fnInvalidWidget(pWidget);
// 	}
	Q_ASSERT(m_pScene != nullptr);
	if (m_pScene)
	{
		m_pScene->update(pWidget->GetPosition());
	}

}
void CGraphicsLayer::InvalidateRect(const QRectF & rcClient)
{
// 	Q_ASSERT(m_fnInvalidRect);
// 	if (m_fnInvalidRect)
// 	{
// 		m_fnInvalidRect(rcClient);
// 	}
	Q_ASSERT(m_pScene != nullptr);
	if (m_pScene)
	{
		m_pScene->update(rcClient);
	}
}

void CGraphicsLayer::InvalidateLayer()
{
	Q_ASSERT(m_pScene != nullptr);
	if (m_pScene)
	{
		QRectF rcClient( QPointF(0.0f,0.0f),  m_Size);
		m_pScene->update(rcClient);
	}
}
/*! \fn void CGraphicsLayer::BuildTempGroupWidget()
********************************************************************************************************* 
** \brief CGraphicsLayer::BuildTempGroupWidget 
** \details ��ѡ�е�ͼԪ������ʱ�����ͼԪ
** \return void 
** \author LiJin
** \date 2016��2��15�� 
** \note 
********************************************************************************************************/
void CGraphicsLayer::BuildTempGroupWidget()
{
	bool bFlag = false;
	QRectF rcUnion;
	for (auto i : m_arrWidgets)
	{
		if (i->IsLocked() == false)
		{
			if (bFlag == false)
			{
				bFlag = true;
				rcUnion = i->GetPosition();
			}
			else
			{
				rcUnion = rcUnion.united(i->GetPosition());
			}
		}
	}
	m_pSelectedTempGrpWidget->Clear();
	QRectF rcLocation, rcRelativeLoc;
	for (auto i : m_arrWidgets)
	{
		if (i->IsLocked() == false)
		{
			rcLocation = i->GetPosition();
			rcRelativeLoc = BuildRelativeLocation(rcUnion, rcLocation);
			m_pSelectedTempGrpWidget->AddWidget(i, rcRelativeLoc);
		}
	}
	m_pSelectedTempGrpWidget->m_bSelected = false;
}

bool CGraphicsLayer::SelectAll( )
{
	m_arrSelection.clear();
	bool bRet = false;
	if (m_arrSelection != m_arrWidgets)
	{
		bRet = true;
	}
	m_arrSelection.clear();

	QRectF rcUnion;
	for (auto i: m_arrWidgets)
	{
		i->m_bSelected = true;
		m_arrSelection.push_back(i);
	}

	return bRet;
}
//����ѡ���ȡ��ѡ��
void CGraphicsLayer::Select(CBaseWidget* pObj)
{
	if (pObj == nullptr)
	{
		auto iter = m_arrSelection.begin();
		for (; iter != m_arrSelection.end(); ++iter)
		{
			(*iter)->m_bSelected = false;
			//	(*iter)->update();
			InvalidateWidget((*iter));
		}
		m_arrSelection.clear();
	}
	else
	{
		std::vector<CBaseWidget*>::iterator it_find = std::find(m_arrSelection.begin(), m_arrSelection.end(), pObj);
		Q_ASSERT(it_find == m_arrSelection.end());

		if (pObj->m_bSelected == false)
		{
			InvalidateWidget(pObj);     
			m_arrSelection.push_back(pObj);
			pObj->m_bSelected = true;
			InvalidateWidget(pObj);      
		}
	}
	BuildTempGroupWidget();
}
/*! \fn void CGraphicsLayer::Deselect(CBaseWidget* pObj)
*********************************************************************************************************
** \brief   CGraphicsLayer::Deselect
** \details ȡ��ͼԪ��ѡ��״̬
** \param   pObj
** \return void
** \author LiJin
** \date  2010��11��24��
** \note
********************************************************************************************************/
void CGraphicsLayer::Deselect(CBaseWidget* pObj)
{
	Q_ASSERT(pObj);
	if (pObj == nullptr)
		return;

	auto it_find = std::find(m_arrSelection.begin(), m_arrSelection.end(), pObj);
	if (it_find != m_arrSelection.end())
	{//�ҵ�
		InvalidateWidget(pObj);
		Q_ASSERT((*it_find)->m_bSelected);
		(*it_find)->m_bSelected = false;
		m_arrSelection.erase(it_find);
		InvalidateWidget(pObj);
	}
	BuildTempGroupWidget();
}
/*! \fn void CGraphicsLayer::SelectWidgetsInRect(const QRectF &rcSel)
*********************************************************************************************************
** \brief CGraphicsLayer::SelectWidgetsInRect
** \details ѡ����ο��ڵ�ͼԪ
** \param rcSel
** \return void
** \author LiJin
** \date 2016��1��21��
** \note
********************************************************************************************************/
void CGraphicsLayer::SelectWidgetsInRect(const QRectF &rcSel)
{
	if (rcSel.isEmpty() || rcSel.isNull())
		return;

	// ��� 
	Select(nullptr);

	auto iter = m_arrWidgets.begin();
	for (; iter != m_arrWidgets.end(); ++iter)
	{
		if ((*iter)->IsContained(rcSel) == true)
		{
			Select(*iter);
		}
	}
}
/*! \fn bool CGraphicsLayer::IsSelectedWidgetsCanMove(const QPointF & delta) const
********************************************************************************************************* 
** \brief CGraphicsLayer::IsSelectedWidgetsCanMove 
** \details ѡ�е�ͼԪ�Ƿ�����ƶ�
** \param delta 
** \return bool 
** \author LiJin 
** \date 2016��2��6�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::IsSelectedWidgetsCanMove(const QPointF & delta, bool bMoving) const
{
	bool bCanMove = true;
	// �ƶ�����С��һ������
	if (fabs(double(delta.x())) < 1.0f && fabs(double(delta.y())) < 1.0f)
	{
		return false;
	}

	if (m_arrSelection.empty() == true)
	{
		return false;
	}

	QRectF rcTemp;
	//auto iter = arrSelections.begin();	
	for (auto iter : m_arrSelection)
	{
		if ((iter)->IsLocked() == false)
		{
			if (bMoving)
			{
				rcTemp = iter->GetMovingPosition();
			}
			else
			{
				rcTemp = iter->GetPosition();
			}
            //��ת  ��ı߽�  TODO
//             QTransform tTransform;
//             tTransform.translate(rcTemp.x(), rcTemp.y());
// 
//             tTransform.rotate(iter->GetRotateAngle());
//             tTransform.translate(delta.x(),delta.y());
//             rcTemp = tTransform.mapRect(rcTemp);

			rcTemp.translate(delta);
			// �ж�
			if (rcTemp.x() < 0 || rcTemp.y() < 0 || rcTemp.right() > m_Size.width() || rcTemp.bottom() > m_Size.height())
			{
                if (!bMoving)
                {
                    //iter->SetMovingPosition(iter->GetPosition());
					iter->MoveDelta(QPointF(0,0));
					
                }
                
				bCanMove = false;
			}
		}
	}
	return bCanMove;
}
/*! \fn bool CGraphicsLayer::CanMoveWidgetDelta(CBaseWidget *pWidget, const QPointF & delta)const
********************************************************************************************************* 
** \brief CGraphicsLayer::CanMoveWidgetDelta 
** \details �ж�ĳ��ͼԪ����λ���Ƿ�����
** \param pWidget 
** \param delta 
** \return bool 
** \author LiJin
** \date 2016��2��15�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::CanMoveWidgetDelta(CBaseWidget *pWidget, const QPointF & delta)const
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return false;

	QRectF rcNewLoc = pWidget->GetPosition();
	rcNewLoc.translate(delta);

	// �ж�
	if (rcNewLoc.left() < 0 || rcNewLoc.top() < 0 || rcNewLoc.right() > m_Size.width() || rcNewLoc.bottom() > m_Size.height())
	{
		return false;
	}
	return true;
}

/*! \fn bool CGraphicsLayer::CanMoveWidget(CBaseWidget *pWidget, const QPointF & newPos) const
********************************************************************************************************* 
** \brief CGraphicsLayer::CanMoveWidget 
** \details �ж�ĳ��ͼԪ����λ���Ƿ�����            
** \param pWidget 
** \param newPos 
** \return bool 
** \author LiJin
** \date 2016��2��15�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::CanMoveWidget(CBaseWidget *pWidget, const QPointF & newPos) const
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return false;

	// �ж�
	if (newPos.x() < 0 || newPos.y() < 0 || newPos.x() > m_Size.width() || newPos.y() > m_Size.height())
	{
		return false;
	}

	QRectF rcWidget = pWidget->GetPosition();

	if ((newPos.x() + rcWidget.width()) > m_Size.width())
	{
		return false;
	}

	if ((newPos.y() + rcWidget.height()) > m_Size.height())
	{
		return false;
	}
	return true;
}
/*! \fn bool CGraphicsLayer::MoveWidgets(const QPointF & delta)
********************************************************************************************************* 
** \brief CGraphicsLayer::MoveWidgets 
** \details �ƶ�ѡ�е�ͼԪ
** \param delta 
** \return bool �Ƿ����ƶ�
** \author LiJin 
** \date 2016��2��6�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::MoveWidgets(const QPointF & delta)
{
	if (m_arrSelection.empty() == true)
		return false;

	bool bUpdate = false;
	for (auto it : m_arrSelection)
	{
		if (it->IsLocked() == false)
		{
			//	InvalidateWidget(it);
			it->MoveDelta(delta);
			//	InvalidateWidget(it);
			bUpdate = true;
		}
	}	
	return bUpdate;
}

bool CGraphicsLayer::MovingWidgets(const QPointF & delta)
{
	if (m_arrSelection.empty() == true)
		return false;

	bool bUpdate = false;
	for (auto it : m_arrSelection)
	{
		if (it->IsLocked() == false)
		{
			//	InvalidateWidget(it);
			it->MovingDelta(delta);
			//	InvalidateWidget(it);
			bUpdate = true;
		}
	}
	return bUpdate;
}

//ѭת
bool CGraphicsLayer::RotatingWidget(const QPointF & delta)
{
    if (m_arrSelection.empty() == true)
        return false;

    bool bUpdate = false;
    for (auto it : m_arrSelection)
    {
        if (it->IsLocked() == false)
        {
            //	InvalidateWidget(it);
            it->RotatingDelta(delta);
            //	InvalidateWidget(it);
            bUpdate = true;
        }
    }
    return bUpdate;
}

/*! \fn bool CGraphicsLayer::ResizeSelectionWidgets(int nDragHanle,const QPointF & ptInput)
********************************************************************************************************* 
** \brief CGraphicsLayer::ResizeSelectionWidgets 
** \details ����ͼԪ�Ĵ�С�ߴ�
** \param delta 
** \return bool 
** \author LiJin 
** \date 2016��2��6�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::ResizeSelectionWidgets(int nDragHanle,QPointF & ptInput)
{
	Q_ASSERT(nDragHanle > 0);
	if (nDragHanle <= 0)
		return false; 

	if (m_arrSelection.empty() == true)
		return false;
		
	if (ptInput.x() < 0)
	{
		ptInput.setX(0.f);
	}
	if (ptInput.y() < 0)
	{
		ptInput.setY(0.f);
	}
	if (ptInput.x() > m_Size.width())
	{
		ptInput.setX(m_Size.width());
	}
	if (ptInput.y() > m_Size.height())
	{
		ptInput.setY(m_Size.height());
	}

	if (m_arrSelection.size() == 1)
	{
		CBaseWidget *pWidget = m_arrSelection.front();
		pWidget->MoveHandleTo(nDragHanle, ptInput);
	}
	else
	{// ���ͼԪ���ƶ��������λ��
		Q_ASSERT(m_pSelectedTempGrpWidget->IsEmpty() == false);
		m_pSelectedTempGrpWidget->MoveHandleTo(nDragHanle, ptInput);
	}
	
	return true;
}

bool CGraphicsLayer::DeleteSelectedWidgets()
{
	if (m_arrSelection.empty() == true)
		return false;

	Q_ASSERT(m_pScene);
	if (m_pScene == nullptr)
		return false;

	// TODO ע��˴���ɾ���Ƿ���ȷ
	for (auto it : m_arrSelection)
	{
		if (GetCurrentWidget() == it)
		{
			SetCurrentWidget(nullptr);
		}
		//m_pScene->removeItem(it);		
		EraseWidget(it);
	}
	m_arrSelection.clear();
	return true;
}
/*! \fn void CGraphicsLayer::SelectMultiWidgets(CBaseWidget *pWidget)
********************************************************************************************************* 
** \brief CGraphicsLayer::SelectMultiWidgets 
** \details  SHIFT ���� ��ѡ
** \param pWidget 
** \return void 
** \author LiJin 
** \date 2016��2��6�� 
** \note 
********************************************************************************************************/
void CGraphicsLayer::SelectMultiWidgets(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);

	if (pWidget == nullptr)
		return;

	auto it_find = std::find(m_arrSelection.begin(), m_arrSelection.end(), pWidget);
	//  SHIFT ���� ��ѡ
	if (it_find != m_arrSelection.end())
	{// �����ͼԪΪ��ǰ��ѡ��ͼԪ����ȡ��ѡ��
		Deselect(pWidget);
	}
	else
	{// ����ѡ���ͼԪ
		Select(pWidget);
	} 	 
}
/*! \fn bool CGraphicsLayer::ReSelectWidget(CBaseWidget *pWidget)
********************************************************************************************************* 
** \brief CGraphicsLayer::ReSelectWidget 
** \details ����ѡ��ĳ��ͼԪ��
** \param pWidget 
** \return bool 
** \author LiJin
** \date 2016��2��6�� 
** \note ���ԭ���Ѿ���ѡ���ͼԪ����λ������ѡ��ָ����ͼԪ
********************************************************************************************************/
bool CGraphicsLayer::ReSelectWidget(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);

	if (pWidget == nullptr)
		return false;

	auto it_find = std::find(m_arrSelection.begin(), m_arrSelection.end(), pWidget);
	if (it_find == m_arrSelection.end())
	{//û���ҵ�
		if (m_arrSelection.empty() == false)
			Select(nullptr);
		Select(pWidget);
	}
	return true;
}
/*! \fn unsigned int CGraphicsLayer::IsSingleSelection()
********************************************************************************************************* 
** \brief CGraphicsLayer::IsSingleSelection 
** \details �жϵ�ǰ�ǵ�ѡ�����Ƕ�ѡ
** \return unsigned int 
** \author LiJin
** \date 2016��2��6�� 
** \note 
********************************************************************************************************/
unsigned int CGraphicsLayer::IsSingleSelection()
{
	auto nRet = m_arrSelection.size();
	if (nRet == 1)
	{
		return SINGLE_SELECTED;
	}
	else if (nRet == 0)
	{
		return NONE_SELECTED;
	}
	else
	{
		return MULTI_SELECTED;
	} 
}

bool  CGraphicsLayer::IsCurrentLayer()
{
	Q_ASSERT(m_pScene);
	if (m_pScene)
	{
		auto views = m_pScene->views();
		if (views.isEmpty() == false)
		{
			CGraphView *pView = dynamic_cast<CGraphView*> (views.first());
			Q_ASSERT(pView);
			if (pView)
			{
				if (pView->GetCurEditingLayerIdx() == m_nIdx)
				{
					return true;
				}
			}
		}
	}
	return false;
}
/*! \fn bool CGraphicsLayer::IsFirstSelectedWidget(CBaseWidget *pWidget)
********************************************************************************************************* 
** \brief CGraphicsLayer::IsFirstSelectedWidget 
** \details �ж��ǲ���ѡ���ͼԪ�еĵ�һ��ͼԪ
** \param pWidget 
** \return bool 
** \author LiJin
** \date 2016��2��10�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::IsFirstSelectedWidget(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return false;
	
	if (m_arrSelection.empty() == true)
		return false;

	auto pFirst = m_arrSelection.front();
	if (pFirst == pWidget)
	{
		return true;
	}
	return false;
}
/*! \fn void  CGraphicsLayer::DrawSelectionFrame(QPainter *pPainter)
********************************************************************************************************* 
** \brief CGraphicsLayer::DrawSelectionFrame 
** \details ѡ��ʱ���ⲿ��ѡ���
** \param pPainter 
** \return void 
** \author LiJin
** \date 2016��2��7�� 
** \note 
********************************************************************************************************/
void  CGraphicsLayer::DrawSelectionFrame(QPainter *pPainter)
{
//	Q_ASSERT(m_arrSelection.size() >= 2);
	if (m_arrSelection.size() < 2)
		return;
	
	QRectF rcBound;

	for (auto it : m_arrSelection)
	{
		rcBound = rcBound.united(it->GetPosition());
	}

	rcBound.adjust(-WIDGET_MULTI_SELECTED_MARGIN, -WIDGET_MULTI_SELECTED_MARGIN,
		WIDGET_MULTI_SELECTED_MARGIN, WIDGET_MULTI_SELECTED_MARGIN);

	QPen pen(Qt::darkGray);
	pen.setWidth(1);
	pen.setStyle(Qt::SolidLine);
	pPainter->save();
	pPainter->setPen(pen);
	// ���ⲿ���ο�
	pPainter->drawRect(rcBound);

	unsigned int i = 0;
	QRectF rcHandler;
	for (i = 1; i <= 8; i++)
	{
		rcHandler = GetSelectedBoxHandleRect(rcBound,i);
		pPainter->setBrush(Qt::white);
		pPainter->setPen(Qt::darkGray);
		pPainter->drawRect(rcHandler);
	}

	pPainter->restore();
}
/*! \fn void CGraphicsLayer::DrawMovingWidgets(QPainter *pPainter)
********************************************************************************************************* 
** \brief CGraphicsLayer::DrawMovingWidgets 
** \details ���ƶ��е�ͼԪ
** \param pPainter 
** \return void 
** \author LiJin
** \date 2016��2��8�� 
** \note 
********************************************************************************************************/
void CGraphicsLayer::DrawMovingWidgets(QPainter *pPainter)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;

	if (m_arrSelection.empty() == false)
	{
		if (m_arrSelection.size() == 1)
		{// ��ѡ�ƶ�
			for (auto i : m_arrSelection)
			{
				i->DrawMovingWidget(pPainter,true);
			}
		}
		else
		{
			QRectF rcBound;

			for (auto it : m_arrSelection)
			{
				rcBound = rcBound.united(it->GetMovingPosition());
			}

			rcBound.adjust(-5, -5, 5, 5);

			QPen pen(XGraphics::LightBlue);
			pen.setWidth(1);
			pen.setStyle(Qt::DashLine);
			pPainter->save();
			pPainter->setPen(pen);
			// ���ⲿ���ο�
			pPainter->drawRect(rcBound);
			pPainter->restore();
			
			for (auto i : m_arrSelection)
			{
				i->DrawMovingWidget(pPainter);
			}
		}
	}
}
/*! \fn QRectF CGraphicsLayer::GetSelectedBoxHandleRect(const QRectF & rcInput, int nHandle) const
********************************************************************************************************* 
** \brief CGraphicsLayer::GetSelectedBoxHandleRect 
** \details ����ѡ����ϵ�צ�ӵľ���λ��
** \param rcInput 
** \param nHandle 
** \return QT_NAMESPACE::QRectF 
** \author LiJin
** \date 2016��2��8�� 
** \note 
********************************************************************************************************/
QRectF CGraphicsLayer::GetSelectedBoxHandleRect(const QRectF & rcInput, int nHandle)const
{
	Q_ASSERT(rcInput.isNull() == false);
//	Q_ASSERT(rcInput.isEmpty() == false);
	Q_ASSERT(nHandle > 0 && nHandle <= 8);

	double x = 0.0, y = 0.0, xCenter = 0.0, yCenter = 0.0;
 
	xCenter = rcInput.x() + rcInput.width() / 2;
	yCenter = rcInput.y() + rcInput.height() / 2;

	switch (nHandle)
	{
	case 1:
		x = rcInput.x();
		y = rcInput.y();
		break;

	case 2:
		x = xCenter;
		y = rcInput.y();
		break;

	case 3:
		x = rcInput.right();
		y = rcInput.y();
		break;

	case 4:
		x = rcInput.right();
		y = yCenter;
		break;

	case 5:
		x = rcInput.right();
		y = rcInput.bottom();
		break;

	case 6:
		x = xCenter;
		y = rcInput.bottom();
		break;

	case 7:
		x = rcInput.x();
		y = rcInput.bottom();
		break;

	case 8:
		x = rcInput.x();
		y = yCenter;
		break;
	default:
		Q_ASSERT(false);
		break;
	}
	
	QRectF rcOutput;
//	QPointF point(x, y) ;
	rcOutput.setX(x - WIDGET_HANDLE_RECT_HALF_LEN);
	rcOutput.setY(y - WIDGET_HANDLE_RECT_HALF_LEN);
	rcOutput.setWidth(WIDGET_HANDLE_RECT_HALF_LEN * 2);
	rcOutput.setHeight(WIDGET_HANDLE_RECT_HALF_LEN * 2);

	return rcOutput;
}
/*! \fn std::pair <int, CBaseWidget *> CGraphicsLayer::HitTestSingleSelectedWidgetHandle(const QPointF & point) const
********************************************************************************************************* 
** \brief CGraphicsLayer::HitTestSingleSelectedWidgetHandle 
** \details �жϵ����ͼԪ�������ĸ�צ����
** \param point 
** \return int 
** \author LiJin
** \date 2016��2��9�� 
** \note 
********************************************************************************************************/
std::pair <int, CBaseWidget *> CGraphicsLayer::HitTestSingleSelectedWidgetHandle(const QPointF & point) const
{
	Q_ASSERT(m_arrSelection.size() == 1);
	if (m_arrSelection.size() == 1)
	{
		CBaseWidget *pWidget = m_arrSelection.front();
		int nHandle = pWidget->HitTestHandle(point);
		return std::make_pair(nHandle, pWidget);
	}
	return std::make_pair(0, nullptr);
}
/*! \fn int  CGraphicsLayer::HitTestMultiSelectedWidgetHandle(const QPointF &ptTemp)const
********************************************************************************************************* 
** \brief CGraphicsLayer::HitTestMultiSelectedWidgetHandle 
** \details �жϵ���Ķ��ͼԪ�������ĸ�צ����
** \param ptTemp 
** \return int 
** \author LiJin
** \date 2016��2��10�� 
** \note 
********************************************************************************************************/
int  CGraphicsLayer::HitTestMultiSelectedWidgetHandle(const QPointF &ptTemp)const
{
	Q_ASSERT(m_arrSelection.size() > 1);
	if (m_arrSelection.size() >1)
	{
		QRectF rcBound;
		for (auto it : m_arrSelection)
		{
			rcBound = rcBound.united(it->GetPosition());
		}
		rcBound.adjust(-WIDGET_MULTI_SELECTED_MARGIN, -WIDGET_MULTI_SELECTED_MARGIN, 
			WIDGET_MULTI_SELECTED_MARGIN, WIDGET_MULTI_SELECTED_MARGIN);

		QRectF rcTemp = rcBound;
		rcTemp.adjust(-20, -20, 20, 20);
		if (rcTemp.contains(ptTemp))
		{
			for (int nHandle = 1; nHandle <= 8; nHandle++)
			{
				rcTemp = GetSelectedBoxHandleRect(rcBound, nHandle);
				if (rcTemp.contains(ptTemp))
					return nHandle;
			}
		}
	}
	return 0;
}
/*! \fn CBaseWidget *CGraphicsLayer::HitTest(QPoint & pos)
********************************************************************************************************* 
** \brief CGraphicsLayer::HitTest 
** \details �ж�������Ƿ����ڵ�ͼԪ������
** \param pos 
** \return CBaseWidget * 
** \author LiJin 
** \date 2016��3��17�� 
** \note 
********************************************************************************************************/
CBaseWidget *CGraphicsLayer::HitTest(QPoint & pos)
{
	bool bRet = false;

	//������ĵط����ص���ͼԪʱ  ѡ�����ϲ��ͼԪ
	std::vector <CBaseWidget*> tHitWgts;

	for (auto it : m_arrWidgets)
	{
		bRet = it->HitTest(pos);
		if (bRet)
		{
			tHitWgts.push_back(it);
		}	 
	}

	CBaseWidget* pChooseWgt = nullptr;

	if (tHitWgts.size() > 0)
	{
		pChooseWgt = tHitWgts[0];
	}

	for (auto item : tHitWgts)
	{
		if (pChooseWgt->zValue() < item->zValue())
		{
			pChooseWgt = item;
		}
	}

	return pChooseWgt;
}

/*! \fn QRectF  CGraphicsLayer::BuildRelativePos(const QRectF & rcUnion, const QRectF & rcWidget)const
********************************************************************************************************* 
** \brief CGraphicsLayer::BuildRelativePos 
** \details �������λ��
** \param rcUnion 
** \param rcWidget 
** \return QT_NAMESPACE::QRectF 
** \author LiJin
** \date 2016��2��15�� 
** \note 
********************************************************************************************************/
QRectF  CGraphicsLayer::BuildRelativeLocation(const QRectF & rcUnion, const QRectF & rcWidget) const
{
	QRectF rcRelation;
	rcRelation.setX((rcWidget.x() - rcUnion.x()) / rcUnion.width());
	rcRelation.setY((rcWidget.y() - rcUnion.y()) / rcUnion.height());

	rcRelation.setWidth(rcWidget.width() / rcUnion.width());
	rcRelation.setHeight(rcWidget.height() / rcUnion.height());
	return rcRelation;
}
/*! \fn bool CGraphicsLayer::GroupWidgets()
********************************************************************************************************* 
** \brief CGraphicsLayer::GroupWidgets 
** \details ���ѡ�е�ͼԪ
** \return bool 
** \author LiJin
** \date 2016��2��11�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::GroupWidgets(CGroupWidget *pGroup)
{
	//Q_ASSERT(m_pScene);
	if (m_pScene == nullptr)
	{
		return false;
	}
	Q_ASSERT(pGroup);
	if (pGroup == nullptr)
	{
		return false;
	}

	if (m_arrSelection.size() <= 1)
		return false;
	
	std::vector<CBaseWidget *> arrSel;
	
	bool bFlag = false;
	QRectF rcUnion, rcPos;

	// ��������group��ȫ���ռ�
	for (auto it : m_arrSelection)
	{
		if ((it)->IsLocked())
			continue;

		if (bFlag == false)
		{
			bFlag = true;
			rcUnion = (it)->GetPosition();
		}
		else
		{
			rcUnion = rcUnion.united((it)->GetPosition());
		}
		arrSel.push_back(it);
	}
	Q_ASSERT(rcUnion.width() != 0 && rcUnion.height() != 0);
	if (rcUnion.width() == 0 || rcUnion.height() == 0)
		return false;

	// ���һ��group
	QRectF rcRelation;
	//CGroupWidget *pGroup = new CGroupWidget;
	
	for ( auto it2 : arrSel)
	{
		if (it2->IsLocked() == true)
			continue;
		// ÿ��ͼԪ������group�����λ��
		rcPos = (it2)->GetPosition();

		rcRelation = BuildRelativeLocation(rcUnion, rcPos);

		Deselect(it2);
		
		RemoveWidget(it2);
		
		
		m_pScene->removeItem(it2);
		
		pGroup->AddWidget(it2, rcRelation);


		//EraseWidget(it2);
	}
	//pGroup->MoveTo(rcUnion);

	//AddWidget(pGroup);
	//m_pScene->addItem(pGroup);
	Select(pGroup);
	SetCurrentWidget(pGroup);

	return true;
}

//���ָ����ͼԪ
CGroupWidget * CGraphicsLayer::GroupInputWidgets(const std::vector <CBaseWidget *> &arr, double dAngle,  GROUP_CHILD fFlag)
{
    Q_ASSERT(m_pScene);
    if (m_pScene == nullptr)
    {
        return nullptr;
    }

    if (arr.size() <= 1)
        return nullptr;

    std::vector<CBaseWidget *> arrSel;

    bool bFlag = false;
    QRectF rcUnion, rcPos;

    // ��������group��ȫ���ռ�
    for (auto it : arr)
    {
        if ((it)->IsLocked())
            continue;

        if (bFlag == false)
        {
            bFlag = true;
            rcUnion = (it)->GetPosition();
        }
        else
        {
            rcUnion = rcUnion.united((it)->GetPosition());
        }
        arrSel.push_back(it);
    }
    Q_ASSERT(rcUnion.width() != 0 && rcUnion.height() != 0);
    if (rcUnion.width() == 0 || rcUnion.height() == 0)
        return nullptr;

    // ���һ��group
    QRectF rcRelation;
    CGroupWidget *pGroup = new CGroupWidget;

    for (auto it2 : arrSel)
    {
        if (it2->IsLocked() == true)
            continue;
        // ÿ��ͼԪ������group�����λ��
        rcPos = (it2)->GetPosition();

        rcRelation = BuildRelativeLocation(rcUnion, rcPos);

        pGroup->AddWidget(it2, rcRelation);
    }

    AddWidget(pGroup);


    m_pScene->addItem(pGroup);
	if (fFlag == FATHER_GROUP)
	{
		Select(pGroup);
	}

    pGroup->SetRotateAngle(dAngle);

    SetCurrentWidget(pGroup);
    

    return pGroup;
}

bool CGraphicsLayer::CroupGroupWidgets(const std::vector <CBaseWidget *> &arr, double dAngle, CGroupWidget *pGroup)
{
	Q_ASSERT(m_pScene);
	if (m_pScene == nullptr)
	{
		return false;
	}

	Q_ASSERT(pGroup);
	if (pGroup == nullptr)
	{
		return false;
	}

	if (arr.size() <= 1)
		return false;

	std::vector<CBaseWidget *> arrSel;

	bool bFlag = false;
	QRectF rcUnion, rcPos;

	// ��������group��ȫ���ռ�
	for (auto it : arr)
	{
		if ((it)->IsLocked())
			continue;

		if (bFlag == false)
		{
			bFlag = true;
			rcUnion = (it)->GetPosition();
		}
		else
		{
			rcUnion = rcUnion.united((it)->GetPosition());
		}
		arrSel.push_back(it);
	}
	Q_ASSERT(rcUnion.width() != 0 && rcUnion.height() != 0);
	if (rcUnion.width() == 0 || rcUnion.height() == 0)
		return false;

	// ���һ��group
	QRectF rcRelation;

	for (auto it2 : arrSel)
	{
		if (it2->IsLocked() == true)
			continue;
		// ÿ��ͼԪ������group�����λ��
		rcPos = (it2)->GetPosition();

		rcRelation = BuildRelativeLocation(rcUnion, rcPos);

		pGroup->AddWidget(it2, rcRelation);
	}

	//AddWidget(pGroup);


	m_pScene->addItem(pGroup);
	//Select(pGroup);

	pGroup->SetRotateAngle(dAngle);

	//SetCurrentWidget(pGroup);


	return true;
}

bool CGraphicsLayer::CreateGroupWidgets(const std::vector <CBaseWidget *> &arr, double dAngle, CGroupWidget *pGroup)
{
	Q_ASSERT(m_pScene);
	if (m_pScene == nullptr)
	{
		return false;
	}

	Q_ASSERT(pGroup);
	if (pGroup == nullptr)
	{
		return false;
	}

	if (arr.size() <= 1)
		return false;

	std::vector<CBaseWidget *> arrSel;

	bool bFlag = false;
	QRectF rcUnion, rcPos;

	// ��������group��ȫ���ռ�
	for (auto it : arr)
	{
		if ((it)->IsLocked())
			continue;

		if (bFlag == false)
		{
			bFlag = true;
			rcUnion = (it)->GetPosition();
		}
		else
		{
			rcUnion = rcUnion.united((it)->GetPosition());
		}
		arrSel.push_back(it);
	}
	Q_ASSERT(rcUnion.width() != 0 && rcUnion.height() != 0);
	if (rcUnion.width() == 0 || rcUnion.height() == 0)
		return false;

	// ���һ��group
	QRectF rcRelation;

	for (auto it2 : arrSel)
	{
		if (it2->IsLocked() == true)
			continue;
		// ÿ��ͼԪ������group�����λ��
		rcPos = (it2)->GetPosition();

		rcRelation = BuildRelativeLocation(rcUnion, rcPos);

		pGroup->AddWidget(it2, rcRelation);
	}

	//AddWidget(pGroup);


	//m_pScene->addItem(pGroup);
	//Select(pGroup);

	pGroup->SetRotateAngle(dAngle);

	//SetCurrentWidget(pGroup);


	return true;
}

/*! \fn bool CGraphicsLayer::BreakGroupedWidgets()
********************************************************************************************************* 
** \brief CGraphicsLayer::BreakGroupedWidgets 
** \details ȡ������
** \return bool 
** \author LiJin
** \date 2016��2��11�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::BreakGroupedWidgets()
{
	Q_ASSERT(m_pScene);
	if (m_pScene == nullptr)
	{
		return false;
	}

	if (m_arrSelection.empty() == true)
		return false;

	CGroupWidget *pGrp = nullptr;
	CBaseWidget *pWidget = nullptr;
	auto arrSel = m_arrSelection;
	for (auto it : arrSel)
	{
		if (it->GetWidgetType() != CBaseWidget::WIDGET_GROUP)
			continue;

		pGrp = dynamic_cast<CGroupWidget*>(it);
		Q_ASSERT(pGrp);
		if (pGrp == nullptr)
			continue;

		unsigned int nCnt = (unsigned int)pGrp->GetWidgetCount();
		unsigned int i = 0;
		for (i = 0; i < nCnt; i++)
		{
			pWidget = pGrp->GetWidget(i);
			Q_ASSERT(pWidget);
			if (pWidget)
			{
				pWidget->SetGroup(nullptr);
				AddWidget( pWidget);
				m_pScene->addItem(pWidget);
				//Select(pWidget);
			}
		}
		Deselect(pGrp);
		pGrp->Clear();
		// ׼��ɾ�� ����ͼԪ
		if (GetCurrentWidget() == pGrp)
		{
			SetCurrentWidget(nullptr);
		}

		//m_pScene->removeItem(pGrp);
		EraseWidget(pGrp);
	}

	return true;
}

bool CGraphicsLayer::OnAutoAlign()
{
	if (m_arrSelection.size() <= 1)
		return false;

	return false;
}
/*! \fn bool CGraphicsLayer::OnLeftAlign()
********************************************************************************************************* 
** \brief CGraphicsLayer::OnLeftAlign 
** \details �����
** \return bool 
** \author LiJin
** \date 2016��2��15�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::OnLeftAlign()
{
	if (m_arrSelection.size() <= 1)
		return false;
	
	// ��Ҫ�ж��Ƿ�Խ��
	CBaseWidget *pFirstWidget = m_arrSelection.front();
	Q_ASSERT(pFirstWidget);
	if (pFirstWidget == nullptr)
		return false;

	bool bUpdate = false;
	QRectF rcFirst = pFirstWidget->GetPosition();
	QRectF rcOtherWidgetPos;
	QPointF ptDelta;

	for (auto it : m_arrSelection)
	{
		if (it->IsLocked())
			continue;
		if (it == pFirstWidget)
			continue;

		rcOtherWidgetPos = it->GetPosition();
		ptDelta.setX(rcFirst.left() - rcOtherWidgetPos.left());
		if (CanMoveWidgetDelta(it,ptDelta) == false)
		{
			qDebug() << QStringLiteral("λ��Խ�磬���ܶ���");
			continue;
		}
		it->MoveDelta(ptDelta);
		if (bUpdate == false)
		{
			bUpdate = true;
		}
	}

	return bUpdate;
}
/*! \fn bool CGraphicsLayer::OnRightAlign()
********************************************************************************************************* 
** \brief CGraphicsLayer::OnRightAlign 
** \details �Ҷ���
** \return bool 
** \author LiJin 
** \date 2016��2��16�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::OnRightAlign()
{
	if (m_arrSelection.size() <= 1)
		return false;

	// ��Ҫ�ж��Ƿ�Խ��
	CBaseWidget *pFirstWidget = m_arrSelection.front();
	Q_ASSERT(pFirstWidget);
	if (pFirstWidget == nullptr)
		return false;

	bool bUpdate = false;
	QRectF rcFirst = pFirstWidget->GetPosition();
	QRectF rcOtherWidgetPos;
	QPointF ptDelta;

	for (auto it : m_arrSelection)
	{
		if (it->IsLocked())
			continue;
		if (it == pFirstWidget)
			continue;

		rcOtherWidgetPos = it->GetPosition();
		ptDelta.setX(rcFirst.right() - rcOtherWidgetPos.right());
		if (CanMoveWidgetDelta(it, ptDelta) == false)
		{
			qDebug() << QStringLiteral("λ��Խ�磬���ܶ���");
			continue;
		}
		it->MoveDelta(ptDelta);
		if (bUpdate == false)
		{
			bUpdate = true;
		}
	}

	return bUpdate;
}
/*! \fn bool CGraphicsLayer::OnCenterHorzAlign()
********************************************************************************************************* 
** \brief CGraphicsLayer::OnCenterHorzAlign 
** \details ˮƽ���� ˮƽ�ƶ�->���ĵ���һ������
** \return bool 
** \author LiJin 
** \date 2016��2��16�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::OnCenterHorzAlign()
{
	if (m_arrSelection.size() <= 1)
		return false;

	// ��Ҫ�ж��Ƿ�Խ��
	CBaseWidget *pFirstWidget = m_arrSelection.front();
	Q_ASSERT(pFirstWidget);
	if (pFirstWidget == nullptr)
		return false;

	bool bUpdate = false;
	QRectF rcFirst = pFirstWidget->GetPosition();
	QRectF rcOtherWidgetPos;
	QPointF ptDelta;

	for (auto it : m_arrSelection)
	{
		if (it->IsLocked())
			continue;
		if (it == pFirstWidget)
			continue;

		rcOtherWidgetPos = it->GetPosition();
		ptDelta = rcFirst.center() - rcOtherWidgetPos.center();
		//ptDelta.setX(rcFirst.right() - rcOtherWidgetPos.right());
		ptDelta.setY(0.0f);

		if (CanMoveWidgetDelta(it, ptDelta) == false)
		{
			qDebug() << QStringLiteral("λ��Խ�磬���ܶ���");
			continue;
		}
		it->MoveDelta(ptDelta);
		if (bUpdate == false)
		{
			bUpdate = true;
		}
	}

	return bUpdate;
}

bool CGraphicsLayer::OnTopAlign()
{
	if (m_arrSelection.size() <= 1)
		return false;

	// ��Ҫ�ж��Ƿ�Խ��
	CBaseWidget *pFirstWidget = m_arrSelection.front();
	Q_ASSERT(pFirstWidget);
	if (pFirstWidget == nullptr)
		return false;

	bool bUpdate = false;
	QRectF rcFirst = pFirstWidget->GetPosition();
	QRectF rcOtherWidgetPos;
	QPointF ptDelta;

	for (auto it : m_arrSelection)
	{
		if (it->IsLocked())
			continue;
		if (it == pFirstWidget)
			continue;

		rcOtherWidgetPos = it->GetPosition();
		ptDelta.setX(rcFirst.top() - rcOtherWidgetPos.top());
		if (CanMoveWidgetDelta(it, ptDelta) == false)
		{
			qDebug() << QStringLiteral("λ��Խ�磬���ܶ���");
			continue;
		}
		it->MoveDelta(ptDelta);
		if (bUpdate == false)
		{
			bUpdate = true;
		}
	}

	return bUpdate;
}
/*! \fn bool CGraphicsLayer::OnCenterVertAlign()
********************************************************************************************************* 
** \brief CGraphicsLayer::OnCenterVertAlign 
** \details ��ֱ����
** \return bool 
** \author LiJin 
** \date 2016��2��16�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::OnCenterVertAlign()
{
	if (m_arrSelection.size() <= 1)
		return false;

	// ��Ҫ�ж��Ƿ�Խ��
	CBaseWidget *pFirstWidget = m_arrSelection.front();
	Q_ASSERT(pFirstWidget);
	if (pFirstWidget == nullptr)
		return false;

	bool bUpdate = false;
	QRectF rcFirst = pFirstWidget->GetPosition();
	QRectF rcOtherWidgetPos;
	QPointF ptDelta;

	for (auto it : m_arrSelection)
	{
		if (it->IsLocked())
			continue;
		if (it == pFirstWidget)
			continue;

		rcOtherWidgetPos = it->GetPosition();
		ptDelta = rcFirst.center() - rcOtherWidgetPos.center();
		//ptDelta.setX(rcFirst.right() - rcOtherWidgetPos.right());
		ptDelta.setY(0.0f);

		if (CanMoveWidgetDelta(it, ptDelta) == false)
		{
			qDebug() << QStringLiteral("λ��Խ�磬���ܶ���");
			continue;
		}
		it->MoveDelta(ptDelta);
		if (bUpdate == false)
		{
			bUpdate = true;
		}
	}

	return bUpdate;
}

bool CGraphicsLayer::OnButtomAlign()
{
	if (m_arrSelection.size() <= 1)
		return false;

	// ��Ҫ�ж��Ƿ�Խ��
	CBaseWidget *pFirstWidget = m_arrSelection.front();
	Q_ASSERT(pFirstWidget);
	if (pFirstWidget == nullptr)
		return false;

	bool bUpdate = false;
	QRectF rcFirst = pFirstWidget->GetPosition();
	QRectF rcOtherWidgetPos;
	QPointF ptDelta;

	for (auto it : m_arrSelection)
	{
		if (it->IsLocked())
			continue;
		if (it == pFirstWidget)
			continue;

		rcOtherWidgetPos = it->GetPosition();
		ptDelta.setY(rcFirst.bottom() - rcOtherWidgetPos.bottom());
		if (CanMoveWidgetDelta(it, ptDelta) == false)
		{
			qDebug() << QStringLiteral("λ��Խ�磬���ܶ���");
			continue;
		}
		it->MoveDelta(ptDelta);
		if (bUpdate == false)
		{
			bUpdate = true;
		}
	}

	return bUpdate;
}
/*! \fn bool CGraphicsLayer::MoveUpSelectedWidgets()
********************************************************************************************************* 
** \brief CGraphicsLayer::MoveUpSelectedWidgets 
** \details �����ƶ�һ��
** \return bool 
** \author LiJin 
** \date 2016��2��18�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::MoveUpSelectedWidgets()
{
	if (m_arrSelection.empty() == true)
		return false;

	bool bRet = false;
	bool bUpdate = false;

	auto arrSelection = m_arrSelection;

	auto func_sort = [](const CBaseWidget *pStart,const CBaseWidget *pEnd)->bool
	{
		return pStart->zValue() > pEnd->zValue();
	};
	std::sort(arrSelection.begin(), arrSelection.end(), func_sort);

#ifdef DEBUG
	for (auto it : arrSelection)
	{
		qDebug() << it->GetWidgetName().c_str() << " = " <<   it->zValue();
	}
#endif	 
	// �����ϲ�Ŀ�ʼ����
	for (auto it : arrSelection)
	{
		if (it->IsLocked() == true)
			continue;

		bRet = ChangeZStepValue(it, 1);
		if (bRet)
		{
			bUpdate = true;
		}
	}
	return bUpdate;
}
/*! \fn bool  CGraphicsLayer::MoveDownSelectedWidgets()
********************************************************************************************************* 
** \brief CGraphicsLayer::MoveDownSelectedWidgets 
** \details �����ƶ�һ��
** \return bool 
** \author LiJin 
** \date 2016��2��18�� 
** \note 
********************************************************************************************************/
bool  CGraphicsLayer::MoveDownSelectedWidgets()
{
#if 0
	if (m_arrSelection.empty() == true)	
		return false;

	if (m_arrSelection.size() == 1)
	{
		CBaseWidget *pWidget = m_arrSelection.front();
		Q_ASSERT(pWidget);
		if (pWidget == nullptr)
			return false;

		if (pWidget->IsLocked() == true)
			return false;

		return ChangeZStepValue(pWidget, -1);
	}
	else
	{
	//	BuildTempGroupWidget();
		//�ҳ���ײ�Ķ���
		//
	}
#endif
	if (m_arrSelection.empty() == true)
		return false;

	bool bRet = false;
	bool bUpdate = false;

	auto arrSelection = m_arrSelection;

	auto func_sort = [](const CBaseWidget *pStart, const CBaseWidget *pEnd)->bool
	{
		return pStart->zValue() < pEnd->zValue();
	};
	std::sort(arrSelection.begin(), arrSelection.end(), func_sort);

#ifdef DEBUG
	for (auto it : arrSelection)
	{
		qDebug() << it->GetWidgetName().c_str() << " = " << it->zValue();
	}
#endif	
	// �����²�Ŀ�ʼ����
	for (auto it : arrSelection)
	{
		if (it->IsLocked() == true)
			continue;

		bRet = ChangeZStepValue(it, -1);
		if (bRet)
		{
			bUpdate = true;
		}
	}
	return bUpdate;
}
/*! \fn bool CGraphicsLayer::ChangeZStepValue(CBaseWidget *pWidget, int nStepLevel)
********************************************************************************************************* 
** \brief CGraphicsLayer::ChangeZStepValue 
** \details 
** \param pWidget 
** \param nStepLevel ����Ϊ���ƣ�����Ϊ����
** \return bool 
** \author LiJin 
** \date 2016��2��17�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::ChangeZStepValue(CBaseWidget *pWidget, int nStepLevel)
{
	Q_ASSERT(pWidget);
	Q_ASSERT(nStepLevel != 0);
	if (pWidget == nullptr || nStepLevel == 0)
		return false;

	auto item_list = pWidget->collidingItems();
	// ���û���ص�
	if (item_list.empty() == true)
		return false;

	Q_ASSERT(pWidget->zValue() != 0.0f);

	bool bUpdate = false;
	CBaseWidget *pOtherItem = nullptr;
	CBaseWidget *pSwapItem = nullptr;
	double dblMinValue = DBL_MAX;
	double dblMaxValue = -DBL_MAX;
	double dblTempZ = 0.0f;

	for (auto it : item_list)
	{
		pOtherItem = dynamic_cast<CBaseWidget*>(it);
		Q_ASSERT(pOtherItem);
		if (pOtherItem == nullptr)
			continue;

		// ���Ҳ�����ڱ�ѡ���ͼԪ���У����޸�
// 		auto it_find = std::find(m_arrSelection.begin(), m_arrSelection.end(),pOtherItem);
// 		if (it_find != m_arrSelection.end())
// 		{// �ҵ�
// 			continue;
// 		}
		if (pOtherItem->IsSelected())
			continue;

		// ���ڱ�ͼ��
		if (pOtherItem->GetLayerIndex() == m_nIdx)
		{
			if (nStepLevel > 0)
			{// ����һ�� �ҵ�Z��������Ǹ�ͼԪ�������Ե���λ��
				Q_ASSERT(pOtherItem->zValue() != 0.0f);
				dblTempZ = pOtherItem->zValue();
				if (dblTempZ > pWidget->zValue())
				{
					if (dblTempZ < dblMinValue)
					{
						dblMinValue = dblTempZ;
						pSwapItem = pOtherItem;
					}					 
				}
			}	
			else if (nStepLevel < 0)
			{
				Q_ASSERT(pOtherItem->zValue() != 0.0f);
				dblTempZ = pOtherItem->zValue();
				if (dblTempZ < pWidget->zValue())
				{
					if (dblTempZ > dblMaxValue)
					{
						dblMaxValue = dblTempZ;
						pSwapItem = pOtherItem;
					}
				}
			}
		}
	}
	if (pSwapItem)
	{
		if (nStepLevel > 0)
		{// ����һ��
			dblTempZ = pWidget->zValue();
			pWidget->setZValue(pSwapItem->zValue());
			pSwapItem->setZValue(dblTempZ);		
			bUpdate = true;
		}
		else if (nStepLevel < 0)
		{//����
			dblTempZ = pWidget->zValue();
			pWidget->setZValue(pSwapItem->zValue());
			pSwapItem->setZValue(dblTempZ);
			bUpdate = true;
		}		 
	}
	return bUpdate;
}

bool CGraphicsLayer::MoveWidgetToTopLevel()
{
	if (m_arrSelection.empty() == true)
		return false;

	CBaseWidget *pWidget = m_arrSelection.front();
	auto item_list = pWidget->collidingItems();
	// ���û���ص�
	if (item_list.empty() == true)
		return false;
	CBaseWidget *pItem = nullptr;
	//item_list.push_back(pWidget);
	// �ҳ�Z����ϵ�ͼԪ
	std::vector<double> arrOldZValue;
	for (auto it:item_list)
	{
		pItem = dynamic_cast<CBaseWidget*>(it);
		Q_ASSERT(pItem);
		if (pItem == nullptr)
			continue;		
		// ���ڱ�ͼ��
		if (pItem->GetLayerIndex() != m_nIdx)
			continue;
		
		arrOldZValue.push_back(it->zValue());
	}
	// ���û�б�ͼ���ڵ��ص�
	if (arrOldZValue.empty() == true)
		return false;

	arrOldZValue.push_back(pWidget->zValue());
	// ����
	std::sort(arrOldZValue.begin(),arrOldZValue.end(),std::greater<double>());
	// �Ӹ���������
	auto func_sort = [](const QGraphicsItem *pLeft, const QGraphicsItem *pRight)->bool
	{
		return pLeft->zValue() > pRight->zValue();
	};

	qSort(item_list.begin(), item_list.end(), func_sort);
#ifdef DEBUG
	for (auto it : item_list)
	{
		pItem = dynamic_cast<CBaseWidget*>(it);
		Q_ASSERT(pItem);
		if (pItem == nullptr)
			continue;

		qDebug() << pItem->GetWidgetName().c_str() << " = " << pItem->zValue();
	}
#endif	
	
	auto it_zval = arrOldZValue.begin();
	pWidget->setZValue(*it_zval);
	std::advance(it_zval, 1);
	for (auto it_item:item_list)
	{
		it_item->setZValue(*it_zval);
		std::advance(it_zval, 1);
	}

	return true;
}
bool CGraphicsLayer::MoveWidgetToBottomLevel()
{

	return true;
}

void CGraphicsLayer::ClearBaseWgts()
{
	m_arrWidgets.clear();
	m_arrSelection.clear();
	m_pSelectedTempGrpWidget->Clear();
	m_pInsertObj = nullptr;
	m_pScene = nullptr;

}

/*! \fn bool CGraphicsLayer::MoveSelectedWidgetToBottomLevel(CBaseWidget *pWidget)
********************************************************************************************************* 
** \brief CGraphicsLayer::MoveSelectedWidgetToBottomLevel 
** \details 
** \param pWidget 
** \return bool 
** \author LiJin 
** \date 2016��2��19�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::MoveSelectedWidgetToBottomLevel(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return false;

	auto item_list = pWidget->collidingItems();
	// ���û���ص�
	if (item_list.empty() == true)
		return false;

	CBaseWidget *pItem = nullptr;

	// �ҳ�Z����µ�ͼԪ
	std::vector<double> arrOldZValue;
	for (auto it : item_list)
	{
		pItem = dynamic_cast<CBaseWidget*>(it);
		Q_ASSERT(pItem);
		if (pItem == nullptr)
			continue;
		// ���ڱ�ͼ��
		if (pItem->GetLayerIndex() != m_nIdx)
			continue;

		arrOldZValue.push_back(it->zValue());
	}
	// ���û�б�ͼ���ڵ��ص�
	if (arrOldZValue.empty() == true)
		return false;

	arrOldZValue.push_back(pWidget->zValue());
	// ����
	std::sort(arrOldZValue.begin(), arrOldZValue.end(), std::less<double>());
	// �ӵ���������
	auto func_sort = [](const QGraphicsItem *pLeft, const QGraphicsItem *pRight)->bool
	{
		return pLeft->zValue() < pRight->zValue();
	};

	qSort(item_list.begin(), item_list.end(), func_sort);
#ifdef DEBUG
	for (auto it : item_list)
	{
		pItem = dynamic_cast<CBaseWidget*>(it);
		Q_ASSERT(pItem);
		if (pItem == nullptr)
			continue;
		qDebug() << pItem->GetWidgetName().c_str() << " = " << pItem->zValue();
	}
#endif	
	auto it_zval = arrOldZValue.begin();
	pWidget->setZValue(*it_zval);
	std::advance(it_zval, 1);
	for (auto it_item : item_list)
	{
		Q_ASSERT(it_zval != arrOldZValue.end());
		if (it_zval != arrOldZValue.end())
		{
			it_item->setZValue(*it_zval);
			std::advance(it_zval, 1);
		}
	}
	return true;
}
/*! \fn bool CGraphicsLayer::MoveSelectedWidgetToTopLevel(CBaseWidget *pWidget)
********************************************************************************************************* 
** \brief CGraphicsLayer::MoveSelectedWidgetToTopLevel 
** \details �ƶ�ѡ�е�ͼԪ������
** \param pWidget 
** \return bool 
** \author LiJin 
** \date 2016��2��19�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::MoveSelectedWidgetToTopLevel(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return false;

	auto item_list = pWidget->collidingItems();
	// ���û���ص�
	if (item_list.empty() == true)
		return false;
	CBaseWidget *pItem = nullptr;

	// �ҳ�Z����ϵ�ͼԪ
	std::vector<double> arrOldZValue;
	for (auto it : item_list)
	{
		pItem = dynamic_cast<CBaseWidget*>(it);
		Q_ASSERT(pItem);
		if (pItem == nullptr)
			continue;
		// ���ڱ�ͼ��
		if (pItem->GetLayerIndex() != m_nIdx)
			continue;

		arrOldZValue.push_back(it->zValue());
	}
	// ���û�б�ͼ���ڵ��ص�
	if (arrOldZValue.empty() == true)
		return false;

	arrOldZValue.push_back(pWidget->zValue());
	// ����
	std::sort(arrOldZValue.begin(), arrOldZValue.end(), std::greater<double>());
	// �Ӹ���������
	auto func_sort = [](const QGraphicsItem *pLeft, const QGraphicsItem *pRight)->bool
	{
		return pLeft->zValue() > pRight->zValue();
	};

	qSort(item_list.begin(), item_list.end(), func_sort);
#ifdef DEBUG
	for (auto it : item_list)
	{
		pItem = dynamic_cast<CBaseWidget*>(it);
		Q_ASSERT(pItem);
		if (pItem == nullptr)
			continue;
		qDebug() << pItem->GetWidgetName().c_str() << " = " << pItem->zValue();
	}
#endif	
	auto it_zval = arrOldZValue.begin();
	pWidget->setZValue(*it_zval);
	std::advance(it_zval, 1);
	for (auto it_item : item_list)
	{
		Q_ASSERT(it_zval != arrOldZValue.end());
		if (it_zval != arrOldZValue.end())
		{
			it_item->setZValue(*it_zval);
			std::advance(it_zval, 1);
		}
	}
	return true;
}
/*! \fn bool CGraphicsLayer::MoveSelectedWidgetsToTopLevel()
********************************************************************************************************* 
** \brief CGraphicsLayer::MoveSelectedWidgetsToTopLevel 
** \details ���ڶ���
** \return bool 
** \author LiJin 
** \date 2016��2��18�� 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::MoveSelectedWidgetsToTopLevel()
{
	if (m_arrSelection.empty() == true)
		return false;

	bool bRet = false;
	bool bUpdate = false;

	auto arrSelection = m_arrSelection;
	
	auto func_sort = [](const CBaseWidget *pStart, const CBaseWidget *pEnd)->bool
	{
		return pStart->zValue() < pEnd->zValue();
	};
	std::sort(arrSelection.begin(), arrSelection.end(), func_sort);
	// �ӵ����� ��ʼ����->�����ڶ���
	for ( auto it: arrSelection)
	{
		bRet = MoveSelectedWidgetToTopLevel(it);
		if (bRet)
		{
			bUpdate = true;
		}
	}
#if 0
	using WidgetAndOldZVal = std::pair<CBaseWidget*, double>;

	std::deque<WidgetAndOldZVal> arrNewWidgets;
	CBaseWidget *pItem = nullptr;
	std::vector<CBaseWidget*> arrTemp1,arrTemp2;

	// �ӱ�ѡ���ͼԪ�У��ҳ�λ����²��ͼԪ�������ŵ���һ��ͼ�㣬�����ص�ͼԪ�ĸ�˳�����ظ���һ��
	for (auto it:arrSelection)
	{
		auto item_list = it->collidingItems();
		// ���û���ص�
		if (item_list.empty() == true)		
			continue;

		arrNewWidgets.clear();
		arrTemp1.clear();
		arrTemp2.clear();
		// �����Լ�
		item_list.append(it);
		// �ȼ�סZ��ֵ
		//arrNewWidgets.push_back(std::make_pair(nullptr, it->zValue()));
		for (auto it_coll : item_list)
		{
			arrNewWidgets.push_back(std::make_pair(nullptr, it_coll->zValue()));
		}
		// ���� �Ӹ�->������
		auto fnLessSort = [](const WidgetAndOldZVal & left, const WidgetAndOldZVal &right)->bool
		{
			return  left.second > right.second;
		};
		std::sort(arrNewWidgets.begin(), arrNewWidgets.end(), fnLessSort);
		//���������ͼԪ������Z��˳����������
		// �������Ǳ�ѡ�е�ͼԪ����
		for (auto it_coll : item_list)
		{
			pItem = dynamic_cast<CBaseWidget*>(it_coll);
			Q_ASSERT(pItem);
			if (pItem == nullptr)			 
				continue;			 
	 
			if (pItem->IsSelected() == true)
			{// ����Ǳ�ѡ�е�����£���ô��
				arrTemp1.push_back(pItem);				
			}
			else
			{
				arrTemp2.push_back(pItem);
			}
		}
		auto fnZLessSort = [](const CBaseWidget* pLeft, const CBaseWidget *pRight)->bool
		{
			return  pLeft->zValue() > pRight->zValue();
		};
		// ȷ���µ�˳��
		std::sort(arrTemp1.begin(), arrTemp1.end(), fnZLessSort);
		std::sort(arrTemp2.begin(), arrTemp2.end(), fnZLessSort);
		
		auto it1 = arrTemp1.begin();
		auto it2 = arrTemp2.begin();

		for (auto it_de : arrNewWidgets)
		{
			Q_ASSERT(it_de.first == nullptr);
			Q_ASSERT(it_de.second != 0.0f);

			// �Ȱѱ�ѡ�е�ͼԪ
			if (it1 != arrTemp1.end())
			{
				it_de.first = *it1;
				(*it1)->setZValue(it_de.second);

				std::advance(it1, 1);				 
			}
			else
			{// �Ǳ�ѡ�е�ͼԪ
				if (it2 != arrTemp2.end())
				{
					it_de.first = *it2;
					(*it2)->setZValue(it_de.second);

					std::advance(it2, 1);
				}
			}
		}
	}
#endif
	return bUpdate;
}

bool CGraphicsLayer::MoveSelectedWidgetsToBottomLevel()
{
	if (m_arrSelection.empty() == true)
		return false;

	bool bRet = false;
	bool bUpdate = false;

	auto arrSelection = m_arrSelection;

	auto func_sort = [](const CBaseWidget *pStart, const CBaseWidget *pEnd)->bool
	{
		return pStart->zValue() > pEnd->zValue();
	};
	std::sort(arrSelection.begin(), arrSelection.end(), func_sort);
	// �Ӹ����� ��ʼ����->�����ڵײ�
	for (auto it : arrSelection)
	{
		bRet = MoveSelectedWidgetToBottomLevel(it);
		if (bRet)
		{
			bUpdate = true;
		}
	}
	return bUpdate;
}
/** @}*/
