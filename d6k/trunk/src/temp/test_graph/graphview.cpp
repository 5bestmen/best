/*! @file graphview.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       : graphview.cpp
�ļ�ʵ�ֹ��� : ͼ�α༭
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  ͼ��
*  @author LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

#include "graphview.h" 
#include "graphscene.h"

#include <QMouseEvent>
#include <QMessageBox>
#include <QMenu>
#include <QDebug>

#include "shape_widget.h"

#include "background.h"
#include "graph_file.h"
#include "graphics_layer.h"

CGraphView::CGraphView(QWidget *parent) :
QGraphicsView(parent)
{
	m_nCurEditingLayerIndex = 1;

	m_bDrawSelRect = false;
//	m_pGraphFile = std::make_shared<CGraphFile>();
}

CGraphView::CGraphView(QGraphicsScene *pScene, QWidget * parent)
	:QGraphicsView(pScene, parent)
{
	m_nCurEditingLayerIndex = 1;
	m_bDrawSelRect = false;

	setCacheMode(CacheBackground);
//	setRenderHint(QPainter::Antialiasing);
	setMouseTracking(true);

// 	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
// 	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
// 	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
// 	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	setStyleSheet("background: transparent;border:0px");
	setWindowFlags(Qt::FramelessWindowHint);
// 	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
// 	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

//	m_pGraphFile = std::make_shared<CGraphFile>();

//	pScene->setSceneRect(m_pGraphFile->GetBackgroundRect());

	m_nDrawTool = DRAW_TOOLS_SEL;
	m_nMouseMode = WIDGET_MOUSE_SELECT;

	scale(1.0, 1.0);
}

CGraphView::~CGraphView()
{ 
}

CGraphScene *CGraphView::GetGraphicsScene() const
{
	return dynamic_cast<CGraphScene *>(scene());
}

void CGraphView::DrawRuler(QPainter *pPainter, const QRectF & rcViewPort, const QRectF & rcScene)
{
// 	QBrush br = pPainter->brush();
// 	br.setColor(Qt::blue);
// 	br.setStyle(Qt::SolidPattern);
// 	// 	br.setTexture( (QPixmap("../product-ifix.jpg")) );
// 	pPainter->fillRect(rect, br);
// 	pPainter->drawRect(rect);
	const int LARGE_RULER = 50;
	const int LITTLE_RULER = 10;
	const int MARGIN_LEN = 30;

	QLine lnHorz(rcViewPort.x() + MARGIN_LEN, rcViewPort.y() + MARGIN_LEN, rcViewPort.x() + rcViewPort.width(), rcViewPort.y() + MARGIN_LEN);
	QLine lnVert(rcViewPort.x() + MARGIN_LEN, rcViewPort.y() + MARGIN_LEN, rcViewPort.x() + MARGIN_LEN, rcViewPort.y() + rcViewPort.height());

	std::vector<QLine> arrLines;
	QLine lnTemp;
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	int nLargeValue = 0, nLittleValue = 0;

	// ˮƽ������
	unsigned int i = 0;

	using PosAndValue = std::pair<QPoint, int>;
	std::vector<PosAndValue> arrPoints;
	for (;;)
	{	
		nLargeValue = ceil(rcViewPort.x() / LARGE_RULER) * LARGE_RULER + i * LARGE_RULER;
		// �ײ��ĵ�
		y1 = rcViewPort.y() + MARGIN_LEN;
		x1 = ceil(rcViewPort.x() / LARGE_RULER) * LARGE_RULER + i * LARGE_RULER;
		i++;

		if (x1 >= rcViewPort.right())
		{
			break;
		}
		// �����ĵ�
		x2 = x1;
		y2 = y1 - 20;

		lnTemp.setLine(x1, y1, x2, y2);
		if (x1 >= rcViewPort.x() + MARGIN_LEN)
		{
			arrLines.push_back(lnTemp);
			PosAndValue p_val = std::make_pair(QPoint(x1+2, y1 - 12), nLargeValue);
			arrPoints.push_back(p_val);
		}	
	}

	i = 0;
	for (;;)
	{
		nLittleValue = ceil(rcViewPort.x() / LITTLE_RULER) * LITTLE_RULER + i * LITTLE_RULER;
		// �ײ��ĵ�
		y1 = rcViewPort.y() + MARGIN_LEN;
		x1 = ceil(rcViewPort.x() / LITTLE_RULER) * LITTLE_RULER + i * LITTLE_RULER;
		i++;

		if (x1 >= rcViewPort.right())
		{
			break;
		}
		// �����ĵ�
		x2 = x1;
		y2 = y1 - 10;

		lnTemp.setLine(x1, y1, x2, y2);
		if ((x1 >= rcViewPort.x() + MARGIN_LEN) && (nLittleValue % LARGE_RULER != 0))
		{
			arrLines.push_back(lnTemp);			
		}
	}
	// ��ֱ�Ϻ���
	i = 0;
	for (;;)
	{
		nLargeValue = ceil(rcViewPort.y() / LARGE_RULER) * LARGE_RULER + i * LARGE_RULER;
		// �Ҳ�ĵ� 
		x1 = rcViewPort.x() + MARGIN_LEN;
		y1 = ceil(rcViewPort.y() / LARGE_RULER) * LARGE_RULER + i * LARGE_RULER;		  

		i++;

		if (y1 >= rcViewPort.bottom())
		{
			break;
		}
		// ���ĵ�
		x2 = x1 - 20 ;
		y2 = y1;

		lnTemp.setLine(x1, y1, x2, y2);
		if (y1 >= rcViewPort.y() + MARGIN_LEN)
		{
			arrLines.push_back(lnTemp);
// 			PosAndValue p_val = std::make_pair(QPoint(x1 + 2, y1 - 12), nLargeValue);
// 			arrPoints.push_back(p_val);
		}
	}
	i = 0;
	for (;;)
	{
		nLittleValue = ceil(rcViewPort.y() / LITTLE_RULER) * LITTLE_RULER + i * LITTLE_RULER;
		// �Ҳ�ĵ�
		x1 = rcViewPort.x() + MARGIN_LEN;
		y1 = ceil(rcViewPort.y() / LITTLE_RULER) * LITTLE_RULER + i * LITTLE_RULER;

		i++;

		if (y1 >= rcViewPort.bottom())
		{
			break;
		}
		// ���ĵ�
		x2 = x1 - 10;
		y2 = y1;

		lnTemp.setLine(x1, y1, x2, y2);
		if ((y1 >= rcViewPort.y() + MARGIN_LEN) && (nLittleValue % LARGE_RULER != 0))
		{
			arrLines.push_back(lnTemp);
		}
	}

	pPainter->save();
	pPainter->setPen(Qt::darkGray);
	pPainter->drawLine(lnHorz);
	pPainter->drawLine(lnVert);
	if (arrLines.empty() == false)
	{
		pPainter->drawLines(arrLines.data(), arrLines.size());
	}
	 
	for ( auto it:arrPoints)
	{
		pPainter->drawText(it.first,QString::number(it.second));
	}
	pPainter->restore();
}

void CGraphView::drawBackground(QPainter *pPainter, const QRectF &rect)
{
//	if (m_pGraphFile)
	{			
		QRectF sceneRect = this->sceneRect();
		CGraphScene *pScene = GetGraphicsScene();
		Q_ASSERT(pScene);
		if (pScene)
		{
			pScene->GetGraphFile()->DrawBackground(pPainter, sceneRect);
		}
		DrawRuler(pPainter, rect,sceneRect);
	}

	QGraphicsView::drawBackground(pPainter, rect);
	return;
}

void CGraphView::drawForeground(QPainter *pPainter, const QRectF &rect)
{
// 	QRectF sceneRect = this->sceneRect();
// 	CGraphScene *pScene = GetGraphicsScene();
// 	Q_ASSERT(pScene);
// 	if (pScene)
// 	{
// 		pScene->GetGraphFile()->DrawBackground(pPainter, sceneRect);
// 	}

	if (m_nCurEditingLayerIndex >0 && m_nCurEditingLayerIndex<= CGraphFile::MAX_LAYER_CNT)
	{
		CGraphScene *pScene = GetGraphicsScene();
		Q_ASSERT(pScene);
		if (pScene == nullptr)
			return;

		std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
		Q_ASSERT(pGraphFile);
		if (pGraphFile == nullptr)
			return;

		if (m_nMouseMode == WIDGET_MOUSE_MOVE)
		{
			pGraphFile->DrawMovingWidgets(pPainter, m_nCurEditingLayerIndex);
		}

		auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
		Q_ASSERT(pLayer != nullptr);
		if (pLayer!=nullptr)
		{
			pLayer->DrawSelectionFrame(pPainter);
		}
	}

	DrawSelectBox(pPainter);
	QGraphicsView::drawForeground(pPainter, rect);
}
/*! \fn bool CGraphView::MoveWidgets(unsigned int nPixel, int nKey)
********************************************************************************************************* 
** \brief CGraphView::MoveWidgets 
** \details �����ƶ�ͼԪ
** \param nPixel 
** \param nKey 
** \return bool 
** \author LiJin
** \date 2016��2��11�� 
** \note 
********************************************************************************************************/
bool CGraphView::MoveWidgets(unsigned int nPixel, int nKey)
{
	Q_ASSERT(nPixel > 0);
	if (m_nCurEditingLayerIndex > 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
	{
		CGraphScene *pScene = GetGraphicsScene();
		Q_ASSERT(pScene);
		if (pScene == nullptr)
			return false;

		auto pGraphFile = pScene->GetGraphFile();
		Q_ASSERT(pGraphFile);
		if (pGraphFile == nullptr)
			return false;

		auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
		Q_ASSERT(pLayer != nullptr);
		if (pLayer == nullptr)
			return false;

		QPointF delta;
		delta.setX(0.0f);
		delta.setY(0.0f);

		double fMoveDist = nPixel*1.0f;

		switch (nKey)
		{
		case Qt::Key_Up:
			delta.setX(0.0f);
			delta.setY(-fMoveDist);
			break;
		case Qt::Key_Down:
			delta.setX(0.0f);
			delta.setY(fMoveDist);
			break;
		case Qt::Key_Left:
			delta.setX(-fMoveDist);
			delta.setY(0.0f);
			break;
		case Qt::Key_Right:
			delta.setX(fMoveDist);
			delta.setY(0.0f);
			break;
		default:
			Q_ASSERT(false);
			break;
		}
		bool bRet = pLayer->IsSelectedWidgetsCanMove(delta, false);
		if (bRet)
		{
			return pLayer->MoveWidgets(delta);
		}
	}
	return false;
}


void CGraphView::keyPressEvent(QKeyEvent *e)
{
	Qt::KeyboardModifiers kbModifiers = e->modifiers();
	int nKey = e->key();

	if (kbModifiers == (Qt::ControlModifier) && nKey == Qt::Key_A)
	{
		SelectAll(m_nCurEditingLayerIndex);
	}
	else if ( nKey == Qt::Key_Delete)
	{
		OnDelete();
	}
	else 
	{
		if ( // SHIFT + �ƶ���  �ƶ�10������
			(kbModifiers == (Qt::ShiftModifier) && nKey == Qt::Key_Up) ||
			(kbModifiers == (Qt::ShiftModifier) && nKey == Qt::Key_Down) ||
			(kbModifiers == (Qt::ShiftModifier) && nKey == Qt::Key_Left) ||
			(kbModifiers == (Qt::ShiftModifier) && nKey == Qt::Key_Right)
			)
		{
			bool bRet = MoveWidgets(10, nKey);	
			if (bRet)
			{
				invalidateScene();
			}
		}
		else if ( // ������ƶ���  ֻ�ƶ�1������
			nKey == Qt::Key_Up   ||
			nKey == Qt::Key_Down ||
			nKey == Qt::Key_Left ||
			nKey == Qt::Key_Right
			)
		{
			bool bRet = MoveWidgets(1, nKey);
			if (bRet)
			{
				invalidateScene();
			}
		}
	}
}
/*! \fn void CGraphView::MoveWidgets(const QPointF & delta)
********************************************************************************************************* 
** \brief CGraphView::MoveWidgets 
** \details �ƶ�ѡ�е�ͼԪ
** \param delta 
** \return void 
** \author LiJin 
** \date 2016��2��6�� 
** \note 
********************************************************************************************************/
void CGraphView::MoveWidgets(const QPointF & delta)
{ 
	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
	{
		return;
	}

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;	 

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
 	Q_ASSERT(pGraphFile);
 	if (pGraphFile == nullptr)
 		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
	{
		return;
	}

	bool bCanMove = pLayer->IsSelectedWidgetsCanMove(delta,false);
	if (bCanMove == false)
	{// �����ƶ�
		qDebug() << "ͼԪ�ƶ�����";
		return;
	}

	bool bUpdate = false;
	
	bUpdate = pLayer->MoveWidgets(delta);

	if (bUpdate)
	{
		this->viewport()->update();
	}
}

void CGraphView::MovingWidgets(const QPointF & delta)
{
	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
	{
		return;
	}

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
	{
		return;
	}

	bool bCanMove = pLayer->IsSelectedWidgetsCanMove(delta);
	if (bCanMove == false)
	{// �����ƶ�
		return;
	}

	bool bUpdate = false;

	bUpdate = pLayer->MovingWidgets(delta);

	if (bUpdate)
	{
		this->viewport()->update();
	}
}

void CGraphView::mouseMoveEvent(QMouseEvent * e)
{
	QPointF ptTemp(mapToScene(e->pos()).x(), mapToScene(e->pos()).y());

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	if ( e->buttons() & Qt::LeftButton )
	{// ������
		qDebug() << QStringLiteral("������²��ƶ�");

		if (m_nDrawTool == DRAW_TOOLS_SEL)
		{
			if (m_nMouseMode == WIDGET_MOUSE_SELECT)
			{// ��ѡ���
				BuildSelectBox(ptTemp);
			}
			else if ( m_nMouseMode == WIDGET_MOUSE_MOVE )
			{
				QPointF delta = ptTemp - m_ptLast;
				if (pGraphFile)
				{
				//	MoveWidgets(delta);	
					MovingWidgets(delta);
				}
			}
			else if (m_nMouseMode == WIDGET_MOUSE_SIZING)
			{
// 				if (pointf.X < 0)
// 					pointf.X = 0;
// 				if (pointf.Y < 0)
// 					pointf.Y = 0;
// 				if (pointf.X > size.cx)
// 					pointf.X = (float)size.cx;
// 				if (pointf.Y > size.cy)
// 					pointf.Y = (float)size.cy;
				ResizeSelectionWidgets(ptTemp);
			}
			else if (m_nMouseMode == WIDGET_MOUSE_ROTATING)
			{
			//	ResizeWidget(pointf);
			}
		}
		else
		{
			CBaseWidget *pObj = pGraphFile->GetCurrentWidget(m_nCurEditingLayerIndex);
			if ( pObj )
			{				
				pObj->MoveHandleTo(5, ptTemp);
				//m_pInsertObj->update();
				viewport()->update();
			}
		}
	}
	else
	{// ���û�а���
		if (m_nDrawTool == DRAW_TOOLS_SEL)
		{
			m_nMouseMode = WIDGET_MOUSE_NOTHING;
			//	CBaseWidget *pHitWidget = m_pGraph->HitTest(0,pointf);
			m_nDragHandle = GetMouseOnHandleIdx(ptTemp);
			//qDebug() << "DragHandle =" << m_nDragHandle;
		}
		else
		{
#if 0
			if (m_nDrawTool == DRAW_POLYLINE || m_nDrawTool == DRAW_POLYGON)
			{
				if (m_pInsertObj != NULL)
				{
					if (pointf.X < 0)
						pointf.X = 0;
					if (pointf.Y < 0)
						pointf.Y = 0;
					if (pointf.X > size.cx)
						pointf.X = (float)size.cx;
					if (pointf.Y > size.cy)
						pointf.Y = (float)size.cy;
					InvalidateWidget(m_pInsertObj);
					m_pInsertObj->MoveHandleTo(m_nDragHandle, pointf);
					InvalidateWidget(m_pInsertObj);
				}
			}
			else if (m_nDrawTool == DRAW_PIPE)
			{
			}
#endif
		}
	}

	m_ptLast = ptTemp;
}

CBaseWidget *CGraphView::HitTest(QPoint & pos, unsigned int nLayerIdx)
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx == 0 || nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return nullptr;

	CBaseWidget *pObj = dynamic_cast <CBaseWidget*> (this->itemAt( pos ));
	if (pObj == nullptr)
	{
		return nullptr;
	}

	if (pObj->GetLayerIndex() == nLayerIdx)
	{
		return pObj;
	}

	return nullptr;
}
/*! \fn int  CGraphView::GetMouseOnHandleIdx(const QPointF & pointf)
********************************************************************************************************* 
** \brief CGraphView::GetMouseOnHandleIdx 
** \details ��ȡ�����ͼԪ�ϵ�צ��λ��
** \param pointf 
** \return int 
** \author LiJin
** \date 2016��2��9�� 
** \note 
********************************************************************************************************/
int  CGraphView::GetMouseOnHandleIdx(const QPointF & pos)
{
	Q_ASSERT(m_nCurEditingLayerIndex >= 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
  		return 0;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return 0;
	
	auto pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return 0;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer);
	if (pLayer == nullptr)
		return 0;

	auto nRet = pLayer->IsSingleSelection();
	if (nRet == CGraphicsLayer::SINGLE_SELECTED)
	{// ��ѡ
		CBaseWidget *pWidget = nullptr;
		std::tie(m_nDragHandle, pWidget) = pLayer->HitTestSingleSelectedWidgetHandle(pos);
		if (m_nDragHandle > 0 && pWidget !=nullptr)
		{
			//�޸Ĺ��

			//
			if (pWidget->IsRotate())
				m_nMouseMode = WIDGET_MOUSE_ROTATE;
			else
				m_nMouseMode = WIDGET_MOUSE_SIZE;
		}
	}
	else if (nRet == CGraphicsLayer::MULTI_SELECTED)
	{// ��ѡ 
		m_nDragHandle = pLayer->HitTestMultiSelectedWidgetHandle(pos);
		if (m_nDragHandle >0)
		{	
			//�޸Ĺ��
			m_nMouseMode = WIDGET_MOUSE_SIZE;
		}
	}
	return m_nDragHandle;
}

void CGraphView::OnLButtonDown(QMouseEvent * e)
{
	if (m_nCurEditingLayerIndex == 0)
	{
		QMessageBox::information(this, QStringLiteral("ע��"), QStringLiteral("��ǰͼ���޷��༭..."));
		return;
	}

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	int nPosX = static_cast <int> (mapToScene(e->pos()).x());
	int nPosY = static_cast <int> (mapToScene(e->pos()).y());

	QPointF ptTemp(mapToScene(e->pos()).x(), mapToScene(e->pos()).y());
	m_ptFisrst = ptTemp;

	if (m_nDrawTool == DRAW_TOOLS_SEL)
	{
		if (m_nMouseMode == WIDGET_MOUSE_SIZE)//׼���ı�ߴ�
		{
			m_nMouseMode = WIDGET_MOUSE_SIZING;
		}
		else if (m_nMouseMode == WIDGET_MOUSE_ROTATE)//׼����ת
		{
			m_nMouseMode = WIDGET_MOUSE_ROTATING;
		}
		else
		{
			bool bHitTest = false;
			bool  bShiftKey = false, bCtrlKey = false;
			if (e->modifiers() == Qt::SHIFT)
			{
				bShiftKey = true;
			}
			if (e->modifiers() == Qt::CTRL)
			{
				bCtrlKey = true;
			}

			CBaseWidget *pObj = HitTest(e->pos(), m_nCurEditingLayerIndex);
			if (pObj == nullptr)
			{// һ��ͼԪҲû�е����
				pGraphFile->SelectNone(m_nCurEditingLayerIndex);
				m_nMouseMode = WIDGET_MOUSE_SELECT;
			}
			else
			{// �����ͼԪ
			
				if (bShiftKey == true)
				{//  SHIFT ���� ��ѡ
					pGraphFile->SelectMultiWidgets(m_nCurEditingLayerIndex,pObj);
				}
				else if (bCtrlKey == true)
				{// CTRL ��������ѡ���϶�Ϊ����
					// ��մ�ǰ��ѡ��
					pGraphFile->SelectNone(m_nCurEditingLayerIndex);
					// ѡ���µ�ͼԪ
					pGraphFile->SelectWidget(m_nCurEditingLayerIndex, pObj); 
				}
				else
				{// ��ѡͼԪ
					pGraphFile->ReSelectWidget(m_nCurEditingLayerIndex, pObj);
				}
				// ��ѡ״̬ -> ���� �ƶ�
				m_nMouseMode = WIDGET_MOUSE_MOVE;
			}
		}
	}
	else
	{// �����µ�ͼԪ
		if (nPosX < 0 || nPosY < 0) //��TODO ����Ҫ�ж��Ƿ�Խ�ұߵĽ���
		{// ��ʾʹ����
			QMessageBox::information(this, QStringLiteral("������Χ"), QStringLiteral("����ӵ�ͼԪ����������Χ����ע��..."));
			return;
		}
		else
		{
			QRectF rcPos(mapToScene(e->pos()), QSizeF(0, 0));
			bool bRet = AddNewWidget(rcPos, m_nDrawTool);
			if (bRet)
			{//!TODO ��Ҫ��ʾ�ĵ����޸�

			}

		}
	}
	m_ptLast = ptTemp;
}


void CGraphView::mousePressEvent(QMouseEvent * e)
{
	int nPosX = static_cast <int> (mapToScene(e->pos()).x());
	int nPosY = static_cast <int> (mapToScene(e->pos()).y());

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;
 
	QPointF ptTemp(mapToScene(e->pos()).x(), mapToScene(e->pos()).y());

	if (e->button() == Qt::LeftButton)
	{// ������
		qDebug() << QStringLiteral("�������");
		OnLButtonDown(e);
	}
	else if (e->button() == Qt::RightButton )
	{
	//	m_nMouseMode = WIDGET_MOUSE_NOTHING;
	//	pGraphFile->SelectWidget(m_nCurEditingLayerIndex, nullptr);
	//	Select(nullptr);

	//	if (m_nDrawTool == DRAW_POLYLINE || m_nDrawTool == DRAW_POLYGON)
		{

		}
	}
}

void CGraphView::OnLButtonUp(QMouseEvent * e)
{
	int nPosX = static_cast <int> (mapToScene(e->pos()).x());
	int nPosY = static_cast <int> (mapToScene(e->pos()).y());

	QPointF ptCur(mapToScene(e->pos()).x(), mapToScene(e->pos()).y());

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	if (m_nDrawTool == DRAW_TOOLS_SEL)
	{
		if (m_nMouseMode == WIDGET_MOUSE_SELECT)
		{
			QRectF rcBox(m_ptFisrst, m_ptLast);
			rcBox = rcBox.normalized();
			BuildSelectBox(rcBox);
			m_bDrawSelRect = false;

			QRectF rcSel(m_ptFisrst, ptCur);
			rcSel = rcSel.normalized();
			pGraphFile->SelectWidgetsInRect(m_nCurEditingLayerIndex,rcSel);
		}
		else if (m_nMouseMode == WIDGET_MOUSE_MOVE)
		{
			QPointF delta = ptCur - m_ptFisrst;

			MoveWidgets(delta);			 
		}
		else if (m_nMouseMode == WIDGET_MOUSE_SIZING)
		{// ����������ɿ�ʱ������������
			//Q_ASSERT(false);
#if 0
			if (!m_arrSelection.empty())
			{
				CBaseWidget *pObj = m_arrSelection.front();
				Q_ASSERT(pObj);
				// 					if (pObj)
				// 						pObj->NormalizePosition();
			}
#endif

		}
		else if (m_nMouseMode == WIDGET_MOUSE_ROTATING)
		{

		}
	}
	else  if (m_nDrawTool != DRAW_TOOLS_POLYLINE && m_nDrawTool != DRAW_TOOLS_POLYGON  &&m_nDrawTool != DRAW_TOOLS_PIPE)
	{
		CBaseWidget * pInserObj = pGraphFile->GetCurrentWidget(m_nCurEditingLayerIndex);
		if (pInserObj != nullptr)
		{
			if (m_ptFisrst == ptCur)
			{// ���ͼԪ��λ�ÿռ�Ϊ0����ȡ�������ͼԪ��
				Q_ASSERT(pInserObj->m_bSelected);
				if (pInserObj->m_bSelected)
				{//��ȡ��ѡ��
					pGraphFile->Deselect(m_nCurEditingLayerIndex,pInserObj);
				}
				pGraphFile->EraseWidget(m_nCurEditingLayerIndex, pInserObj);
				pGraphFile->SetCurrentWidget(m_nCurEditingLayerIndex, nullptr);
			}
			else
			{
				pInserObj->NormalizePosition();
			}
			pGraphFile->SetCurrentWidget(m_nCurEditingLayerIndex, nullptr);			 
		}
#if 0
		if (m_pInsertObj != nullptr)
		{
			if (m_ptFisrst == ptCur)
			{// ���ͼԪ��λ�ÿռ�Ϊ0����ȡ�������ͼԪ��
				Q_ASSERT(m_pInsertObj->m_bSelected);
				if (m_pInsertObj->m_bSelected)
				{//��ȡ��ѡ��
					Deselect(m_pInsertObj);
				}
				pGraphFile->EraseWidget(m_nCurEditingLayerIndex, m_pInsertObj);
				//scene()->removeItem(m_pInsertObj);
				m_pInsertObj = nullptr;
			}
			else
			{
				m_pInsertObj->NormalizePosition();
				//	if(theApp.m_nGridType>0)
				//		m_pInsertObj->SnapGrid();
				//	CInsertAction *pAction=new CInsertAction;
				//	pAction->m_pObj=m_pInsertObj;
				//	AddDrawAction(pAction);
			}
			m_pInsertObj = nullptr;
		}
#endif

		m_nDrawTool = DRAW_TOOLS_SEL;
	}
}

void CGraphView::mouseReleaseEvent(QMouseEvent * e)
{	
	if (e->button() == Qt::LeftButton)
	{// ������
		qDebug() << QStringLiteral("����ɿ�");
		OnLButtonUp(e);
	}
	m_nMouseMode = WIDGET_MOUSE_NOTHING;
	update();
}

void CGraphView::mouseDoubleClickEvent(QMouseEvent * e)
{

}
void CGraphView::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu;
	QAction *pAction = nullptr;
	
	pAction = menu.addAction(QStringLiteral("����(&T)"), this, SLOT(OnCut()));
	connect(pAction, &QAction::triggered, this, &CGraphView::OnCut);

	pAction = menu.addAction(QStringLiteral("����(&C)"), this, SLOT(OnCopy()));
	connect(pAction, &QAction::triggered, this, &CGraphView::OnCopy);

	pAction = menu.addAction(QStringLiteral("ճ��(&P)"), this, SLOT(OnPaste()));
	connect(pAction, &QAction::triggered, this, &CGraphView::OnPaste);

	pAction = menu.addAction(QStringLiteral("ɾ��(&D)"), this, SLOT(OnDelete()));
	connect(pAction, &QAction::triggered, this, &CGraphView::OnDelete);

	menu.addSeparator();

	pAction = menu.addAction(QStringLiteral("���(&G)"), this, SLOT(OnWidgetsGroup()));
	connect(pAction, &QAction::triggered, this, &CGraphView::OnWidgetsGroup);
	pAction = menu.addAction(QStringLiteral("ȡ�����(&U)"), this, SLOT(OnWidgetsBreak()));
	connect(pAction, &QAction::triggered, this, &CGraphView::OnWidgetsBreak);

	menu.addSeparator();
	pAction = menu.addAction(QStringLiteral("���ڵײ�(&B)"), this, SLOT(MoveToBottomLevel()));
	connect(pAction, &QAction::triggered, this, &CGraphView::MoveToBottomLevel);

	pAction = menu.addAction(QStringLiteral("���ڶ���(&T)"), this, SLOT(MoveToTopLevel()));
	connect(pAction, &QAction::triggered, this, &CGraphView::MoveToTopLevel);

	pAction = menu.addAction(QStringLiteral("����һ��(&S)"), this, SLOT(MoveDown()));
	connect(pAction, &QAction::triggered, this, &CGraphView::MoveDown);

	pAction = menu.addAction(QStringLiteral("����һ��(&X)"), this, SLOT(MoveUp()));
	connect(pAction, &QAction::triggered, this, &CGraphView::MoveUp);

	menu.addSeparator();
	pAction = menu.addAction(QStringLiteral("����"), this, SLOT(OnCut()));

	menu.exec(event->globalPos());
} 

bool CGraphView::AddNewWidget(const QRectF & rcPos, unsigned int nWidgetType)
{
	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
	{
		return false;
	}

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return false;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return false;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return false;

	CShapeWidget *pWidget = nullptr;
	switch (nWidgetType)
	{
	case DRAW_TOOLS_LINE:

		break;
	case  DRAW_TOOLS_RECTANGLE:
	{
		pWidget = new CShapeWidget(rcPos,CShapeWidget::RECTANGLE);
		pLayer->SetCurrentWidget(pWidget);
		
		pLayer->Select(nullptr);
		pLayer->Select(pWidget);
	}
	break;
	case DRAW_TOOLS_ROUND:
	{
		pWidget = new CShapeWidget(rcPos, CShapeWidget::ROUND);
		pLayer->SetCurrentWidget(pWidget);

		pLayer->Select(nullptr);
		pLayer->Select(pWidget);
	}
	break;
	default:
		break;
	}

	if (pWidget && pGraphFile)
	{
		pGraphFile->AddWidget(m_nCurEditingLayerIndex, pWidget);
	}

	return true;
}
/*! \fn void CGraphView::SelectAll(unsigned int nLayerIdx)
********************************************************************************************************* 
** \brief CGraphView::SelectAll 
** \details ѡ��ָ��ͼ�������ͼԪ
** \param nLayerIdx 
** \return void 
** \author LiJin 
** \date 2016��1��18�� 
** \note 
********************************************************************************************************/
void CGraphView::SelectAll(unsigned int nLayerIdx)
{
	Q_ASSERT(nLayerIdx >= 0 && nLayerIdx < CGraphFile::MAX_LAYER_CNT);

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	bool bRet = pGraphFile->SelectAll(nLayerIdx);
	if (bRet)
	{
		viewport()->update();
	}
// 	auto widets_list = pGraphFile->m_arrLayers[nLayerIdx]->GetWidgets();
// 
// 	m_arrSelection.clear();
// 	auto iter = widets_list.begin();
// 	for (; iter != widets_list.end(); ++iter)
// 	{
// 		(*iter)->m_bSelected = true;
// 		m_arrSelection.push_back(*iter);
// 		//	(*iter)->update();
// 		InvalidateWidget((*iter));
// 	}
	//m_arrSelection.clear();
}
#if 0
//����ѡ���ȡ��ѡ��
void CGraphView::Select(CBaseWidget* pObj)
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
}
/*! \fn void CGraphView::Deselect(CBaseWidget* pObj)
*********************************************************************************************************
** \brief   CGraphView::Deselect
** \details ȡ��ͼԪ��ѡ��״̬
** \param   pObj
** \return void
** \author LiJin
** \date  2010��11��24��
** \note
********************************************************************************************************/
void CGraphView::Deselect(CBaseWidget* pObj)
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
}
#endif

//�ػ�pObj����
void CGraphView::InvalidateWidget(CBaseWidget* pObj)
{
	Q_ASSERT(pObj);
	if (pObj == nullptr)
		return;

//	pObj->update();
	QRectF rcUpdate = pObj->GetPosition();

	rcUpdate.adjust(-2, -2, 2, 2);
	pObj->update(rcUpdate);
// 	QRectF rect = pObj->GetClipRect();
// 	 
// 	rect.InflateRect(5, 5);
// 	InvalidateRect(rect, FALSE);
}

void  CGraphView::OnCopy()
{

}

void  CGraphView::OnPaste()
{

}

void  CGraphView::OnDelete()
{
	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	bool bRet = pLayer->DeleteSelectedWidgets();
	if (bRet)
	{
		this->viewport()->update();
	}	 
}

void CGraphView::OnCut()
{

}
#if 0
/*! \fn void CGraphView::SelectWidgetsInRect(const QRectF &rcSel)
********************************************************************************************************* 
** \brief CGraphView::SelectWidgetsInRect 
** \details ѡ����ο��ڵ�ͼԪ
** \param rcSel 
** \return void 
** \author LiJin 
** \date 2016��1��21�� 
** \note 
********************************************************************************************************/
void CGraphView::SelectWidgetsInRect(const QRectF &rcSel)
{
	if (rcSel.isEmpty() || rcSel.isNull())
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	// ��� 
	Select(nullptr);

	auto widets_list = pGraphFile->m_arrLayers[m_nCurEditingLayerIndex-1]->GetWidgets();

	auto iter = widets_list.begin();
	for (; iter != widets_list.end(); ++iter)
	{
		if ((*iter)->IsContained(rcSel) == true)
		{
			Select(*iter);
		}
	}
}
#endif

void CGraphView::BuildSelectBox(const QRectF & rcSel)
{
//	qDebug() << QStringLiteral("build select box rect");

	if (m_rcSelectBox != rcSel)
	{
		m_rcOldSelBox = m_rcSelectBox;
		m_rcSelectBox = rcSel;
		QRectF rcUpdate = m_rcSelectBox;
		rcUpdate.united(m_rcOldSelBox);
		//update(rcUpdate.toRect());
		viewport()->update();
	}
}

void CGraphView::BuildSelectBox(const QPointF & ptCur)
{
//	qDebug() << QStringLiteral("build select box point");

	//QRectF rcSel(m_ptFisrst, m_ptLast);
	QRectF rcSel;
	// ��Ҫ�ж�����Ǿ��ε���һ����
	if (ptCur.x() > m_ptFisrst.x() && ptCur.y() > m_ptFisrst.y())
	{// cur �� fist �����½�
		rcSel.setTopLeft(m_ptFisrst);
	//	rcSel.setBottomRight(m_ptLast);
		rcSel.setBottomRight(ptCur);
	}
	else if (ptCur.x() < m_ptFisrst.x() && ptCur.y() > m_ptFisrst.y())
	{// cur �� fist �����½�
		rcSel.setTopRight(m_ptFisrst);
	//	rcSel.setBottomLeft(m_ptLast);
		rcSel.setBottomLeft(ptCur);
	}
	else if (ptCur.x() > m_ptFisrst.x() && ptCur.y() < m_ptFisrst.y())
	{// cur �� fist �����Ͻ�
		rcSel.setBottomLeft(m_ptFisrst);
	//	rcSel.setTopLeft(m_ptLast);
		rcSel.setTopRight(ptCur);
	}
	else if (ptCur.x() < m_ptFisrst.x() && ptCur.y() < m_ptFisrst.y())
	{// cur �� fist �����Ͻ�
		rcSel.setBottomRight(m_ptFisrst);
	//	rcSel.setTopLeft(m_ptLast);
		rcSel.setTopLeft(ptCur);
	}
	else
	{
		rcSel.setTopLeft(m_ptFisrst);
		rcSel.setBottomRight(ptCur);
	}

	rcSel= rcSel.normalized();

	if (rcSel != m_rcOldSelBox)
	{
		m_rcOldSelBox = m_rcSelectBox;
	}
	m_rcSelectBox = rcSel;

	if (rcSel.isEmpty() == false)
	{
		m_bDrawSelRect = true;
	}
	else
	{
		m_bDrawSelRect = false;
	}
	rcSel |= m_rcOldSelBox;

	//this->update(rcSel.toRect());
	viewport()->update();
}

void CGraphView::DrawSelectBox(QPainter *pPainter)
{
	if (m_bDrawSelRect && !m_rcSelectBox.isEmpty())
	{
		m_rcSelectBox = m_rcSelectBox.normalized();

		QColor crBrush(Qt::lightGray);
		crBrush.setAlpha(100);

		QPen pen;
		pen.setWidth(1);
		pen.setColor(Qt::darkBlue);
		pen.setStyle(Qt::DotLine);
		
// 		QBrush brush;
// 		brush.setColor(crBrush);

	//  pen.setBrush(brush);
		
		pPainter->save();
		pPainter->setPen(pen);
	//	pPainter->setBrush(brush);
	//	pPainter->setPen(Qt::black);

	//	pPainter->setPen(Qt::darkBlue);
		pPainter->fillRect(m_rcSelectBox, crBrush);
		pPainter->drawRect( m_rcSelectBox );
	 
		pPainter->restore();
		return;
	}
}
/*! \fn void  CGraphView::ResizeSelectionWidgets( QPointF & delta)
********************************************************************************************************* 
** \brief CGraphView::ResizeSelectionWidgets 
** \details ����ͼԪ�Ĵ�С�ߴ�
** \param delta 
** \return void 
** \author LiJin 
** \date 2016��2��6�� 
** \note 
********************************************************************************************************/
void  CGraphView::ResizeSelectionWidgets( QPointF & ptInput)
{
	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return ;

	auto pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return ;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	bool bRet = pLayer->ResizeSelectionWidgets(m_nDragHandle,ptInput);
	if (bRet)
	{
		invalidateScene();
	}
}
/*! \fn void CGraphView::SetLayerVisible(unsigned int nLayerIdx, bool bDisp)
********************************************************************************************************* 
** \brief CGraphView::SetLayerVisible 
** \details ͼ����ʾ����
** \param nLayerIdx 
** \param bDisp 
** \return unsigned int 
** \author LiJin
** \date 2016��1��23�� 
** \note 
********************************************************************************************************/
unsigned int CGraphView::SetLayerVisible(unsigned int nLayerIdx, bool bDisp)
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx > CGraphFile::MAX_LAYER_CNT || nLayerIdx == 0)
		return 0;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return 0;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return 0;

	unsigned int nCurEditingLayer = m_nCurEditingLayerIndex;
	pGraphFile->SetLayerVisible(nLayerIdx, bDisp, nCurEditingLayer);

	SetCurEditingLayer(nCurEditingLayer);

	invalidateScene();
	return m_nCurEditingLayerIndex;
}
/*! \fn void CGraphView::SetCurEditingLayer(unsigned int nLayerIdx)
********************************************************************************************************* 
** \brief CGraphView::SetCurEditingLayer 
** \details ���õ�ǰ�༭��ͼ��
** \param nLayerIdx 
** \return void 
** \author LiJin
** \date 2016��2��1�� 
** \note 
********************************************************************************************************/
void CGraphView::SetCurEditingLayer(unsigned int nLayerIdx)
{
	Q_ASSERT(nLayerIdx >= 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	// �����ͼ�㲻�ɼ�����ô�����޸�
	if (nLayerIdx != m_nCurEditingLayerIndex)
	{// ���ͼ�㷢���ı�

	}
	m_nCurEditingLayerIndex = nLayerIdx;
}

void CGraphView::OnWidgetsGroup()
{
	Q_ASSERT(m_nCurEditingLayerIndex >= 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	auto pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	bool bRet = pLayer->GroupWidgets();
	if (bRet)
	{
		invalidateScene();
	}
}

void CGraphView::OnWidgetsBreak()
{
	Q_ASSERT(m_nCurEditingLayerIndex >= 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	auto pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	bool bRet = pLayer->BreakGroupedWidgets();
	if (bRet)
	{
		invalidateScene();
	}
}

void CGraphView::OnAutoAlign()
{
	Q_ASSERT(m_nCurEditingLayerIndex >= 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	auto pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	bool bRet = pLayer->OnAutoAlign();
	if (bRet)
	{
		invalidateScene();
	}
}

void CGraphView::OnLeftAlign()
{
	Q_ASSERT(m_nCurEditingLayerIndex >= 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	auto pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	bool bRet = pLayer->OnLeftAlign();
	if (bRet)
	{
		invalidateScene();
	}
}

void CGraphView::OnRightAlign()
{
	Q_ASSERT(m_nCurEditingLayerIndex >= 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	auto pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	bool bRet = pLayer->OnRightAlign();
	if (bRet)
	{
		invalidateScene();
	}
}

void CGraphView::OnCenterHorzAlign()
{
	Q_ASSERT(m_nCurEditingLayerIndex >= 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	auto pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	bool bRet = pLayer->OnCenterHorzAlign();
	if (bRet)
	{
		invalidateScene();
	}
}

void CGraphView::OnTopAlign()
{
	Q_ASSERT(m_nCurEditingLayerIndex >= 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	auto pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	bool bRet = pLayer->OnTopAlign();
	if (bRet)
	{
		invalidateScene();
	}
}

void CGraphView::OnCenterVertAlign()
{
	Q_ASSERT(m_nCurEditingLayerIndex >= 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	auto pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	bool bRet = pLayer->OnCenterVertAlign();
	if (bRet)
	{
		invalidateScene();
	}
}

void CGraphView::OnButtomAlign()
{
	Q_ASSERT(m_nCurEditingLayerIndex >= 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	auto pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	bool bRet = pLayer->OnButtomAlign();
	if (bRet)
	{
		invalidateScene();
	}
}


void CGraphView::MoveToBottomLevel()
{
	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	if (pLayer->MoveSelectedWidgetsToBottomLevel())
	{
		invalidateScene();
	}
}
void CGraphView::MoveToTopLevel()
{
	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	if (pLayer->MoveSelectedWidgetsToTopLevel())
	{
		invalidateScene();
	}	
}

void CGraphView::MoveUp()
{
	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	if (pLayer->IsSelectionEmpty() == true)
		return;

	if (pLayer->MoveUpSelectedWidgets())
	{
		invalidateScene();
	}
}
/*! \fn void CGraphView::DownOneLevel()
*********************************************************************************************************
** \brief CGraphView::DownOneLevel
** \details ����һ��
** \return void
** \author LiJin
** \date 2016��1��22��
** \note
********************************************************************************************************/
void CGraphView::MoveDown()
{
	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	std::shared_ptr<CGraphFile> pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	if (pLayer->IsSelectionEmpty() == true)
		return;	
	
	if (pLayer->MoveDownSelectedWidgets())
	{
		invalidateScene();
	}	
}

/** @}*/
