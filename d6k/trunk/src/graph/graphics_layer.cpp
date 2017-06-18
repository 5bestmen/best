/*! @file graphics_layer.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  graphics_layer.cpp
文件实现功能 :  图层
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   图层
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
	// 清空资源之后，就不会重复回收资源
	m_pSelectedTempGrpWidget->Clear();

	if (m_pScene == nullptr)
	{
		//在没有scene时  需要手动删除
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
	//清空原有的数据
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
			//组合
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

				// 计算整个group的全部空间
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

				// 添加一个group
				QRectF rcRelation;
				CGroupWidget *pGroup = new CGroupWidget;

				for (auto it2 : arrSel)
				{
					if (it2->IsLocked() == true)
					{
						continue;
					}

					// 每个图元与整个group的相对位置
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
** \details 添加一个图元到图层中
** \param pObj 
** \return void 
** \author LiJin 
** \date 2016年1月18日 
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
** \brief  在指定图层删除一个指定对象
** \details
** \param nLayerIdx  从0开始
** \param pObj
** \return bool
** \author LiJin
** \date 2010年9月6日
** \note
********************************************************************************************************/
bool CGraphicsLayer::EraseWidget(CBaseWidget *pObj)
{
	Q_ASSERT(pObj);
	if (pObj)
	{
		std::vector <CBaseWidget*>::iterator it_find = std::find(m_arrWidgets.begin(), m_arrWidgets.end(), pObj);
		if (it_find != m_arrWidgets.end())
		{//找到
			//pObj->setParentItem(nullptr);
			m_arrWidgets.erase(it_find);
			// 回收内存
			delete pObj;
			return true;
		}
	}
	return false;
}
/*! \fn bool CGraphicsLayer::RemoveWidget(CBaseWidget *pObj)
********************************************************************************************************* 
** \brief CGraphicsLayer::RemoveWidget 
** \details 从现有队列中删除指定图元，但是并不回收资源
** \param pObj 
** \return bool 
** \author LiJin
** \date 2016年2月11日 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::RemoveWidget(CBaseWidget *pObj)
{
	Q_ASSERT(pObj);
	if (pObj)
	{
		std::vector <CBaseWidget*>::iterator it_find = std::find(m_arrWidgets.begin(), m_arrWidgets.end(), pObj);
		if (it_find != m_arrWidgets.end())
		{//找到
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
** \details 将选中的图元生产临时的组合图元
** \return void 
** \author LiJin
** \date 2016年2月15日 
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
//单击选择或取消选择
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
** \details 取消图元的选中状态
** \param   pObj
** \return void
** \author LiJin
** \date  2010年11月24日
** \note
********************************************************************************************************/
void CGraphicsLayer::Deselect(CBaseWidget* pObj)
{
	Q_ASSERT(pObj);
	if (pObj == nullptr)
		return;

	auto it_find = std::find(m_arrSelection.begin(), m_arrSelection.end(), pObj);
	if (it_find != m_arrSelection.end())
	{//找到
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
** \details 选择矩形框内的图元
** \param rcSel
** \return void
** \author LiJin
** \date 2016年1月21日
** \note
********************************************************************************************************/
void CGraphicsLayer::SelectWidgetsInRect(const QRectF &rcSel)
{
	if (rcSel.isEmpty() || rcSel.isNull())
		return;

	// 清空 
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
** \details 选中的图元是否可以移动
** \param delta 
** \return bool 
** \author LiJin 
** \date 2016年2月6日 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::IsSelectedWidgetsCanMove(const QPointF & delta, bool bMoving) const
{
	bool bCanMove = true;
	// 移动距离小于一个像素
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
            //旋转  后的边界  TODO
//             QTransform tTransform;
//             tTransform.translate(rcTemp.x(), rcTemp.y());
// 
//             tTransform.rotate(iter->GetRotateAngle());
//             tTransform.translate(delta.x(),delta.y());
//             rcTemp = tTransform.mapRect(rcTemp);

			rcTemp.translate(delta);
			// 判断
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
** \details 判断某个图元的新位置是否允许
** \param pWidget 
** \param delta 
** \return bool 
** \author LiJin
** \date 2016年2月15日 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::CanMoveWidgetDelta(CBaseWidget *pWidget, const QPointF & delta)const
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return false;

	QRectF rcNewLoc = pWidget->GetPosition();
	rcNewLoc.translate(delta);

	// 判断
	if (rcNewLoc.left() < 0 || rcNewLoc.top() < 0 || rcNewLoc.right() > m_Size.width() || rcNewLoc.bottom() > m_Size.height())
	{
		return false;
	}
	return true;
}

/*! \fn bool CGraphicsLayer::CanMoveWidget(CBaseWidget *pWidget, const QPointF & newPos) const
********************************************************************************************************* 
** \brief CGraphicsLayer::CanMoveWidget 
** \details 判断某个图元的新位置是否允许            
** \param pWidget 
** \param newPos 
** \return bool 
** \author LiJin
** \date 2016年2月15日 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::CanMoveWidget(CBaseWidget *pWidget, const QPointF & newPos) const
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return false;

	// 判断
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
** \details 移动选中的图元
** \param delta 
** \return bool 是否有移动
** \author LiJin 
** \date 2016年2月6日 
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

//循转
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
** \details 调整图元的大小尺寸
** \param delta 
** \return bool 
** \author LiJin 
** \date 2016年2月6日 
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
	{// 多个图元，移动的是相对位置
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

	// TODO 注意此处的删除是否正确
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
** \details  SHIFT 按键 多选
** \param pWidget 
** \return void 
** \author LiJin 
** \date 2016年2月6日 
** \note 
********************************************************************************************************/
void CGraphicsLayer::SelectMultiWidgets(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);

	if (pWidget == nullptr)
		return;

	auto it_find = std::find(m_arrSelection.begin(), m_arrSelection.end(), pWidget);
	//  SHIFT 按键 多选
	if (it_find != m_arrSelection.end())
	{// 点击的图元为此前所选的图元，则取消选择
		Deselect(pWidget);
	}
	else
	{// 增加选择该图元
		Select(pWidget);
	} 	 
}
/*! \fn bool CGraphicsLayer::ReSelectWidget(CBaseWidget *pWidget)
********************************************************************************************************* 
** \brief CGraphicsLayer::ReSelectWidget 
** \details 重新选择某个图元，
** \param pWidget 
** \return bool 
** \author LiJin
** \date 2016年2月6日 
** \note 如果原来已经有选择的图元，则复位，重新选择指定的图元
********************************************************************************************************/
bool CGraphicsLayer::ReSelectWidget(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);

	if (pWidget == nullptr)
		return false;

	auto it_find = std::find(m_arrSelection.begin(), m_arrSelection.end(), pWidget);
	if (it_find == m_arrSelection.end())
	{//没有找到
		if (m_arrSelection.empty() == false)
			Select(nullptr);
		Select(pWidget);
	}
	return true;
}
/*! \fn unsigned int CGraphicsLayer::IsSingleSelection()
********************************************************************************************************* 
** \brief CGraphicsLayer::IsSingleSelection 
** \details 判断当前是单选，还是多选
** \return unsigned int 
** \author LiJin
** \date 2016年2月6日 
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
** \details 判断是不是选择的图元中的第一个图元
** \param pWidget 
** \return bool 
** \author LiJin
** \date 2016年2月10日 
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
** \details 选中时，外部的选择框
** \param pPainter 
** \return void 
** \author LiJin
** \date 2016年2月7日 
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
	// 画外部矩形框
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
** \details 画移动中的图元
** \param pPainter 
** \return void 
** \author LiJin
** \date 2016年2月8日 
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
		{// 单选移动
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
			// 画外部矩形框
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
** \details 计算选择框上的爪子的矩形位置
** \param rcInput 
** \param nHandle 
** \return QT_NAMESPACE::QRectF 
** \author LiJin
** \date 2016年2月8日 
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
** \details 判断点击的图元，是在哪个爪子上
** \param point 
** \return int 
** \author LiJin
** \date 2016年2月9日 
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
** \details 判断点击的多个图元，是在哪个爪子上
** \param ptTemp 
** \return int 
** \author LiJin
** \date 2016年2月10日 
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
** \details 判断鼠标点击是否所在的图元区域内
** \param pos 
** \return CBaseWidget * 
** \author LiJin 
** \date 2016年3月17日 
** \note 
********************************************************************************************************/
CBaseWidget *CGraphicsLayer::HitTest(QPoint & pos)
{
	bool bRet = false;

	//当点击的地方有重叠的图元时  选择最上层的图元
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
** \details 生成相对位置
** \param rcUnion 
** \param rcWidget 
** \return QT_NAMESPACE::QRectF 
** \author LiJin
** \date 2016年2月15日 
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
** \details 组合选中的图元
** \return bool 
** \author LiJin
** \date 2016年2月11日 
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

	// 计算整个group的全部空间
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

	// 添加一个group
	QRectF rcRelation;
	//CGroupWidget *pGroup = new CGroupWidget;
	
	for ( auto it2 : arrSel)
	{
		if (it2->IsLocked() == true)
			continue;
		// 每个图元与整个group的相对位置
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

//组合指定的图元
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

    // 计算整个group的全部空间
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

    // 添加一个group
    QRectF rcRelation;
    CGroupWidget *pGroup = new CGroupWidget;

    for (auto it2 : arrSel)
    {
        if (it2->IsLocked() == true)
            continue;
        // 每个图元与整个group的相对位置
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

	// 计算整个group的全部空间
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

	// 添加一个group
	QRectF rcRelation;

	for (auto it2 : arrSel)
	{
		if (it2->IsLocked() == true)
			continue;
		// 每个图元与整个group的相对位置
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

	// 计算整个group的全部空间
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

	// 添加一个group
	QRectF rcRelation;

	for (auto it2 : arrSel)
	{
		if (it2->IsLocked() == true)
			continue;
		// 每个图元与整个group的相对位置
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
** \details 取消分组
** \return bool 
** \author LiJin
** \date 2016年2月11日 
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
		// 准备删除 成组图元
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
** \details 左对齐
** \return bool 
** \author LiJin
** \date 2016年2月15日 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::OnLeftAlign()
{
	if (m_arrSelection.size() <= 1)
		return false;
	
	// 需要判断是否越界
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
			qDebug() << QStringLiteral("位置越界，不能对齐");
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
** \details 右对齐
** \return bool 
** \author LiJin 
** \date 2016年2月16日 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::OnRightAlign()
{
	if (m_arrSelection.size() <= 1)
		return false;

	// 需要判断是否越界
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
			qDebug() << QStringLiteral("位置越界，不能对齐");
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
** \details 水平居中 水平移动->中心点在一条线上
** \return bool 
** \author LiJin 
** \date 2016年2月16日 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::OnCenterHorzAlign()
{
	if (m_arrSelection.size() <= 1)
		return false;

	// 需要判断是否越界
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
			qDebug() << QStringLiteral("位置越界，不能对齐");
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

	// 需要判断是否越界
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
			qDebug() << QStringLiteral("位置越界，不能对齐");
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
** \details 垂直居中
** \return bool 
** \author LiJin 
** \date 2016年2月16日 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::OnCenterVertAlign()
{
	if (m_arrSelection.size() <= 1)
		return false;

	// 需要判断是否越界
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
			qDebug() << QStringLiteral("位置越界，不能对齐");
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

	// 需要判断是否越界
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
			qDebug() << QStringLiteral("位置越界，不能对齐");
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
** \details 向上移动一层
** \return bool 
** \author LiJin 
** \date 2016年2月18日 
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
	// 从最上层的开始上移
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
** \details 向下移动一层
** \return bool 
** \author LiJin 
** \date 2016年2月18日 
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
		//找出碰撞的队列
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
	// 从最下层的开始下移
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
** \param nStepLevel 正数为上移，负数为下移
** \return bool 
** \author LiJin 
** \date 2016年2月17日 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::ChangeZStepValue(CBaseWidget *pWidget, int nStepLevel)
{
	Q_ASSERT(pWidget);
	Q_ASSERT(nStepLevel != 0);
	if (pWidget == nullptr || nStepLevel == 0)
		return false;

	auto item_list = pWidget->collidingItems();
	// 如果没有重叠
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

		// 如果也是属于被选择的图元队列，则不修改
// 		auto it_find = std::find(m_arrSelection.begin(), m_arrSelection.end(),pOtherItem);
// 		if (it_find != m_arrSelection.end())
// 		{// 找到
// 			continue;
// 		}
		if (pOtherItem->IsSelected())
			continue;

		// 属于本图层
		if (pOtherItem->GetLayerIndex() == m_nIdx)
		{
			if (nStepLevel > 0)
			{// 上移一层 找到Z轴最靠近的那个图元，和他对调下位置
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
		{// 上移一层
			dblTempZ = pWidget->zValue();
			pWidget->setZValue(pSwapItem->zValue());
			pSwapItem->setZValue(dblTempZ);		
			bUpdate = true;
		}
		else if (nStepLevel < 0)
		{//下移
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
	// 如果没有重叠
	if (item_list.empty() == true)
		return false;
	CBaseWidget *pItem = nullptr;
	//item_list.push_back(pWidget);
	// 找出Z轴最靠上的图元
	std::vector<double> arrOldZValue;
	for (auto it:item_list)
	{
		pItem = dynamic_cast<CBaseWidget*>(it);
		Q_ASSERT(pItem);
		if (pItem == nullptr)
			continue;		
		// 属于本图层
		if (pItem->GetLayerIndex() != m_nIdx)
			continue;
		
		arrOldZValue.push_back(it->zValue());
	}
	// 如果没有本图层内的重叠
	if (arrOldZValue.empty() == true)
		return false;

	arrOldZValue.push_back(pWidget->zValue());
	// 排序
	std::sort(arrOldZValue.begin(),arrOldZValue.end(),std::greater<double>());
	// 从高往低排序
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
** \date 2016年2月19日 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::MoveSelectedWidgetToBottomLevel(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return false;

	auto item_list = pWidget->collidingItems();
	// 如果没有重叠
	if (item_list.empty() == true)
		return false;

	CBaseWidget *pItem = nullptr;

	// 找出Z轴最靠下的图元
	std::vector<double> arrOldZValue;
	for (auto it : item_list)
	{
		pItem = dynamic_cast<CBaseWidget*>(it);
		Q_ASSERT(pItem);
		if (pItem == nullptr)
			continue;
		// 属于本图层
		if (pItem->GetLayerIndex() != m_nIdx)
			continue;

		arrOldZValue.push_back(it->zValue());
	}
	// 如果没有本图层内的重叠
	if (arrOldZValue.empty() == true)
		return false;

	arrOldZValue.push_back(pWidget->zValue());
	// 排序
	std::sort(arrOldZValue.begin(), arrOldZValue.end(), std::less<double>());
	// 从低往高排序
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
** \details 移动选中的图元到顶层
** \param pWidget 
** \return bool 
** \author LiJin 
** \date 2016年2月19日 
** \note 
********************************************************************************************************/
bool CGraphicsLayer::MoveSelectedWidgetToTopLevel(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return false;

	auto item_list = pWidget->collidingItems();
	// 如果没有重叠
	if (item_list.empty() == true)
		return false;
	CBaseWidget *pItem = nullptr;

	// 找出Z轴最靠上的图元
	std::vector<double> arrOldZValue;
	for (auto it : item_list)
	{
		pItem = dynamic_cast<CBaseWidget*>(it);
		Q_ASSERT(pItem);
		if (pItem == nullptr)
			continue;
		// 属于本图层
		if (pItem->GetLayerIndex() != m_nIdx)
			continue;

		arrOldZValue.push_back(it->zValue());
	}
	// 如果没有本图层内的重叠
	if (arrOldZValue.empty() == true)
		return false;

	arrOldZValue.push_back(pWidget->zValue());
	// 排序
	std::sort(arrOldZValue.begin(), arrOldZValue.end(), std::greater<double>());
	// 从高往低排序
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
** \details 置于顶层
** \return bool 
** \author LiJin 
** \date 2016年2月18日 
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
	// 从低往高 开始排序->并置于顶层
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

	// 从被选择的图元中，找出位置最靠下层的图元，将他放到第一个图层，调整重叠图元的各顺序，再重复下一个
	for (auto it:arrSelection)
	{
		auto item_list = it->collidingItems();
		// 如果没有重叠
		if (item_list.empty() == true)		
			continue;

		arrNewWidgets.clear();
		arrTemp1.clear();
		arrTemp2.clear();
		// 加上自己
		item_list.append(it);
		// 先记住Z轴值
		//arrNewWidgets.push_back(std::make_pair(nullptr, it->zValue()));
		for (auto it_coll : item_list)
		{
			arrNewWidgets.push_back(std::make_pair(nullptr, it_coll->zValue()));
		}
		// 排序 从高->低排序
		auto fnLessSort = [](const WidgetAndOldZVal & left, const WidgetAndOldZVal &right)->bool
		{
			return  left.second > right.second;
		};
		std::sort(arrNewWidgets.begin(), arrNewWidgets.end(), fnLessSort);
		//将排序过的图元按照新Z轴顺序填入数组
		// 先挑出是被选中的图元出来
		for (auto it_coll : item_list)
		{
			pItem = dynamic_cast<CBaseWidget*>(it_coll);
			Q_ASSERT(pItem);
			if (pItem == nullptr)			 
				continue;			 
	 
			if (pItem->IsSelected() == true)
			{// 如果是被选中的情况下，那么，
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
		// 确立新的顺序
		std::sort(arrTemp1.begin(), arrTemp1.end(), fnZLessSort);
		std::sort(arrTemp2.begin(), arrTemp2.end(), fnZLessSort);
		
		auto it1 = arrTemp1.begin();
		auto it2 = arrTemp2.begin();

		for (auto it_de : arrNewWidgets)
		{
			Q_ASSERT(it_de.first == nullptr);
			Q_ASSERT(it_de.second != 0.0f);

			// 先把被选中的图元
			if (it1 != arrTemp1.end())
			{
				it_de.first = *it1;
				(*it1)->setZValue(it_de.second);

				std::advance(it1, 1);				 
			}
			else
			{// 非被选中的图元
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
	// 从高往低 开始排序->并置于底层
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
