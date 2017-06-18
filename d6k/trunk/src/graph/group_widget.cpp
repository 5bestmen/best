/*! @file group_widget.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  group_widget.cpp
�ļ�ʵ�ֹ��� :  ���ͼԪ
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ���ͼԪ
*  @author  LiJin
*  @version 1.0
*  @date    2016.02.3
*******************************************************************************/
#include "group_widget.h" 
#include "colour_define.h"

#include <QPainter>
#include <QtMath>

CGroupWidget::CGroupWidget() :CBaseWidget()
{
	m_nObjType = WIDGET_GROUP;
	m_szName = "GROUP";
// 	m_szTxt = QStringLiteral("�������ͼԪ");

	m_rcMovingPos = m_rcPos;
}

CGroupWidget::~CGroupWidget()
{
	std::vector<GRP_MEMBER>::iterator iter = m_arrWidgets.begin();
	for (; iter != m_arrWidgets.end(); ++iter)
	{
		Q_ASSERT((*iter).WidgetPtr);
		if ((*iter).WidgetPtr)
		{
			delete (*iter).WidgetPtr;
			(*iter).WidgetPtr = nullptr;
		}
	}

	m_arrWidgets.clear();
}

void CGroupWidget::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;
	Q_UNUSED(widget);
//	pPainter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    

	for (auto it : m_arrWidgets)
	{
		Q_ASSERT(it.WidgetPtr != nullptr);
		if (it.WidgetPtr != nullptr)
		{
			if (it.WidgetPtr->isVisible())
			{
//                 it.WidgetPtr->SetRotateAngle(m_dblRotateAngle + it.WidgetPtr->GetRotateAngle());
                pPainter->save();
                if (m_dblRotateAngle != 0.0f)
                {
                    pPainter->translate(GetRotationCenterPoint());
                    pPainter->rotate(m_dblRotateAngle);
                    pPainter->translate(-GetRotationCenterPoint());
                }

				it.WidgetPtr->paint(pPainter, option, widget);
                pPainter->restore();
			}
		}
	}
	QPen pen(Qt::darkGray);
	pen.setWidthF(m_PenInfo.GetWidth());
	pen.setStyle(Qt::SolidLine);
    

	pPainter->save();	
	//m_szTxt = QString::fromLocal8Bit(m_szWidgetName.c_str());
	if (m_szTxt.length())
	{
		//pPainter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
		QTextOption txtOpt;
		//txtOpt.
		pPainter->setRenderHint(QPainter::TextAntialiasing);
		pPainter->drawText(m_rcPos, m_szTxt, txtOpt);
	}
	pPainter->restore();
	// ��ѡ���
	DrawSelectedTracker(pPainter);
}

QRectF CGroupWidget::boundingRect() const
{
	QRectF rcBounding = m_rcPos.normalized();	
	double dPenWidth = m_PenInfo.GetWidth()/2;

	if (dPenWidth > 0.0f)
	{	// �������߿�
		rcBounding.adjust(0 - dPenWidth, 0 - dPenWidth, dPenWidth, dPenWidth);
	}

	return rcBounding;
}

/*! \fn void CGroupWidget::MoveHandleTo(int nHandle, const QPointF &ptInput)
********************************************************************************************************* 
** \brief CGroupWidget::MoveHandleTo 
** \details  �ƶ�ĳ��צ�ӵ��µ�λ�ã�����ͼԪ�Ĵ�С
** \param nHandle 
** \param point 
** \return void 
** \author LiJin
** \date 2016��2��15�� 
** \note 
********************************************************************************************************/
void CGroupWidget::MoveHandleTo(int nHandle, const QPointF &ptInput)
{
	QRectF rcLoc = m_rcPos;
	QPointF ptTemp = ptInput;

	//���ڴ�����ת�����ͼ��,�������еĵ������¶��ǵȱ�������

	switch (nHandle)
	{
	case POS_TOP_LEFT:
		//  �ƶ����Ͻǲ��ܳ����ұ߼��±�
// 		rcLoc.setTopLeft(ptTemp);

		if (ptTemp.x() > rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}

		ptTemp.setY(rcLoc.bottomRight().y() - (rcLoc.bottomRight().x() - ptTemp.x())*1.0 / (rcLoc.width())*(rcLoc.height()));

		rcLoc.setLeft(ptTemp.x());
		rcLoc.setTop(ptTemp.y());
		break;
	case POS_TOP_CENTER:
		//  �ƶ��ϱ߲��ܳ����±�

		if (ptTemp.y() > rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}

		ptTemp.setX(rcLoc.bottomLeft().x() + (rcLoc.bottomLeft().y() - ptTemp.y())*1.0 / (rcLoc.height())*(rcLoc.width()));


		rcLoc.setRight(ptTemp.x());
		rcLoc.setTop(ptTemp.y());
		break;
	case POS_TOP_RIGHT:
		//  �ƶ����Ͻ� ���ܳ������½�

		if (ptTemp.x() < rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}

		ptTemp.setY(rcLoc.topLeft().y() + (ptTemp.x() - rcLoc.topLeft().x())*1.0 / (rcLoc.width())*(rcLoc.height()));


		rcLoc.setRight(ptTemp.x());
		rcLoc.setBottom(ptTemp.y());
		break;
		break;
	case POS_RIGHT_CENTER:
		// �ƶ��ұ� ���ܳ��� ���

		if (ptTemp.x() < rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		ptTemp.setY(rcLoc.topLeft().y() + (ptTemp.x() - rcLoc.topLeft().x())*1.0 / (rcLoc.width())*(rcLoc.height()));


		rcLoc.setRight(ptTemp.x());
		rcLoc.setBottom(ptTemp.y());
		break;
	case POS_BOTTOM_RIGHT:
		// �ƶ����½� ���ܳ������Ͻ�

		if (ptTemp.x() < rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		ptTemp.setY(rcLoc.topLeft().y() + (ptTemp.x() - rcLoc.topLeft().x())*1.0 / (rcLoc.width())*(rcLoc.height()));


		rcLoc.setRight(ptTemp.x());
		rcLoc.setBottom(ptTemp.y());

		break;

	case POS_BOTTOM_CENTER:


		// �ƶ��±� ���ܳ����ϱ�
		if (ptTemp.y() < rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		ptTemp.setX(rcLoc.topLeft().x() + (ptTemp.y() - rcLoc.topLeft().y())*1.0 / (rcLoc.height())*(rcLoc.width()));

		rcLoc.setRight(ptTemp.x());
		rcLoc.setBottom(ptTemp.y());

		break;
	case POS_BOTTOM_LEFT:
		// �ƶ����½� ���ܳ������Ͻ�


		if (ptTemp.x() > rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}

		ptTemp.setY(rcLoc.bottomRight().y() - (rcLoc.bottomRight().x() - ptTemp.x())*1.0 / (rcLoc.width())*(rcLoc.height()));


		rcLoc.setLeft(ptTemp.x());
		rcLoc.setTop(ptTemp.y());
		break;
	case POS_LEFT_CENTER:
		// �ƶ���� ���ܳ����ұ�
		if (ptTemp.x() > rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		ptTemp.setY(rcLoc.bottomRight().y() - (rcLoc.bottomRight().x() - ptTemp.x())*1.0 / (rcLoc.width())*(rcLoc.height()));


		rcLoc.setLeft(ptTemp.x());
		rcLoc.setTop(ptTemp.y());
		break;
	}
	rcLoc = rcLoc.normalized();

	MoveTo(rcLoc);	
}


void CGroupWidget::GetPath(QPainterPath& path)
{
	QRectF rcShape = m_rcPos.normalized();
	path.addRect(rcShape); 
}

void  CGroupWidget::GetMovingPath(QPainterPath& path)
{
	QRectF rcShape = m_rcMovingPos.normalized();
	path.addRect(rcShape);
} 
/*! \fn void CGroupWidget::AddWidget(CBaseWidget *pWidget, const QRectF & rcRelat)
********************************************************************************************************* 
** \brief CGroupWidget::AddWidget 
** \details ���һ��ͼԪ��������
** \param pWidget 
** \param rcRelat 
** \return void 
** \author LiJin
** \date 2016��2��11�� 
** \note 
********************************************************************************************************/

void CGroupWidget::AddWidget(CBaseWidget *pWidget, const QRectF & rcRelat)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return;
	QRectF rcBound = m_rcPos;
	std::vector<GRP_MEMBER>::iterator iter = m_arrWidgets.begin();
	for (; iter != m_arrWidgets.end(); ++iter)
	{
		if ((*iter).WidgetPtr && (*iter).WidgetPtr == pWidget)
		{// ���ظ�
			Q_ASSERT(false);
			return;
		}
	}
	if (m_arrWidgets.empty() == true)
	{
        if (pWidget->GetRotateAngle() != 0.0f)
        {
//             QRectF tNoRotate = pWidget->GetPosition();
//             QPointF tCenter = pWidget->GetRotationCenterPoint();
//             double dLength = (tNoRotate.x() - tCenter.x())*(tNoRotate.x() - tCenter.x())
//                 + (tNoRotate.y() - tCenter.y()) *(tNoRotate.y() - tCenter.y());
// 
//             dLength = sqrt(dLength);
// 
//             m_rcPos.setX(tCenter.x() - dLength);
//             m_rcPos.setY(tCenter.y() - dLength);
// 
//             m_rcPos.setWidth(dLength*2);
//             m_rcPos.setHeight(dLength*2);

            QPointF tCenter = pWidget->GetRotationCenterPoint();
            QTransform tr_temp;
            tr_temp.translate(tCenter.x(), tCenter.y());
            tr_temp = tr_temp.rotate(pWidget->GetRotateAngle());
            //	QRectF rcRet = tr_temp.mapRect(rcInput);
            tr_temp.translate(-tCenter.x(), -tCenter.y());

            m_rcPos = tr_temp.mapRect(pWidget->GetPosition());

        }
        else
        {
            m_rcPos = pWidget->GetPosition();
        }
		
	}
	else
	{
        QRectF tfRect;
        if (pWidget->GetRotateAngle() != 0.0f)
        {
            QPointF tCenter = pWidget->GetRotationCenterPoint();
            QTransform tr_temp;
            tr_temp.translate(tCenter.x(), tCenter.y());
            tr_temp = tr_temp.rotate(pWidget->GetRotateAngle());
            //	QRectF rcRet = tr_temp.mapRect(rcInput);
            tr_temp.translate(-tCenter.x(), -tCenter.y());

            tfRect = tr_temp.mapRect(pWidget->GetPosition());

        }
        else
        {
            tfRect = pWidget->GetPosition();
        }

        m_rcPos = m_rcPos.united(tfRect);
	}

	GRP_MEMBER member = { pWidget, rcRelat };

	if (m_arrWidgets.empty())
	{
		m_arrWidgets.push_back(member);
	}
	else
	{
		std::vector <GRP_MEMBER>::iterator it = m_arrWidgets.begin();

		while (it != m_arrWidgets.end())
		{
			if (it->WidgetPtr->zValue() > member.WidgetPtr->zValue())
			{
				m_arrWidgets.insert(it, member);
				break;
			}

			if (it == m_arrWidgets.end()-1)
			{
				m_arrWidgets.push_back(member);
				break;
			}

			it++;
		}
	}


	
	m_rcMovingPos = m_rcPos;
}
/*! \fn void CGroupWidget::MoveTo(const QRectF & rcPosition)
********************************************************************************************************* 
** \brief CGroupWidget::MoveTo 
** \details ȷ��ͼԪ����λ��
** \param rcPosition 
** \return void 
** \author LiJin
** \date 2016��2��11�� 
** \note 
********************************************************************************************************/
void CGroupWidget::MoveTo(const QRectF & rcPosition)
{
	QRectF tTemp = m_rcPos;
	m_rcPos = rcPosition;
	m_rcMovingPos = m_rcPos;

	QRectF rect, scale;
	std::vector<GRP_MEMBER>::iterator iter = m_arrWidgets.begin();
	for (; iter != m_arrWidgets.end(); ++iter)
	{
		Q_ASSERT((*iter).WidgetPtr);
		if ((*iter).WidgetPtr)
		{
			scale = (iter)->RelativePos;

            if ((*iter).WidgetPtr->GetRotateAngle() != 0.0f)
            {       
				double dScale = (1.0*m_rcPos.width() / tTemp.width());

				rect.setX(m_rcPos.x() + scale.x()*m_rcPos.width() + dScale*((*iter).WidgetPtr->GetPosition().topLeft().x() - tTemp.x() - scale.x()*tTemp.width()));
				rect.setY(m_rcPos.y() + scale.y()*m_rcPos.height() + dScale*((*iter).WidgetPtr->GetPosition().topLeft().y() - tTemp.y() - scale.y()*tTemp.height()));

				rect.setWidth((*iter).WidgetPtr->GetPosition().width()*dScale);

				rect.setHeight((*iter).WidgetPtr->GetPosition().height()*dScale);
            }
            else
            {
                rect.setX(m_rcPos.x() + scale.x()*m_rcPos.width());
                rect.setY(m_rcPos.y() + scale.y()*m_rcPos.height());
                rect.setWidth(scale.width()*m_rcPos.width());
                rect.setHeight(scale.height()*m_rcPos.height());

            }

			(*iter).WidgetPtr->MoveTo(rect);
		}
	}
}
void CGroupWidget::MoveDelta(const QPointF &delta)
{
//	qDebug() << delta;
	m_rcPos.translate(delta);
	m_rcMovingPos = m_rcPos;
	for (auto it : m_arrWidgets)
	{
		Q_ASSERT(it.WidgetPtr != nullptr);
		if (it.WidgetPtr != nullptr)
		{
			it.WidgetPtr->MoveDelta(delta);		 
		}
	}
}

void CGroupWidget::MovingDelta(const QPointF &delta)
{
//	qDebug() << delta;
	m_rcMovingPos.translate(delta);
	for (auto it : m_arrWidgets)
	{
		Q_ASSERT(it.WidgetPtr != nullptr);
		if (it.WidgetPtr != nullptr)
		{
			it.WidgetPtr->MovingDelta(delta);
		}
	}
}
/*! \fn void CGroupWidget::DrawMovingWidget(QPainter *pPainter, bool bSingleSelected)
********************************************************************************************************* 
** \brief CGroupWidget::DrawMovingWidget 
** \details ���ƶ��е����ͼԪ 
** \param pPainter 
** \param bSingleSelected 
** \return void 
** \author LiJin
** \date 2016��2��11�� 
** \note ��ѡ��ͼԪ״̬���ⲿѡ���Ϊ����ɫ���������Ƕ�ѡ״̬ԭ����ɶ��������ɶ��
********************************************************************************************************/
void CGroupWidget::DrawMovingWidget(QPainter *pPainter, bool bSingleSelected)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;	

    pPainter->save();


    if (m_dblRotateAngle != 0.0f)
    {
        pPainter->translate(m_rcMovingPos.center());
        pPainter->rotate(m_dblRotateAngle);
        pPainter->translate(-m_rcMovingPos.center());
    }

	for (auto it : m_arrWidgets)
	{
		Q_ASSERT(it.WidgetPtr != nullptr);
		if (it.WidgetPtr != nullptr)
		{
			//if (it.WidgetPtr->isVisible())
			{
                //pPainter->save();


				it.WidgetPtr->DrawMovingWidget(pPainter, false);
                //pPainter->restore();
			}
		}
	}


	// ���ⲿѡ���
	//pPainter->save();
	if (bSingleSelected)
	{
		QPen pen(XGraphics::LightBlue);
		pen.setWidthF(m_PenInfo.GetWidth());
		pen.setStyle(Qt::DashLine);

		pPainter->setPen(pen);
	}
	else
	{// ��ѡ���ƶ� ��ԭ����ɶ��������ɶ��
		QPen pen(Qt::darkGray);
		pen.setWidth(1);
		pen.setStyle(Qt::SolidLine);
		pPainter->setPen(pen);
	}
	QPainterPath path;
	GetMovingPath(path);
	pPainter->drawPath(path);
	pPainter->restore();

    
}


#if 0
void CGroupWidget::DrawSingleSelectedTracker(QPainter *pPainter)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;

	QRectF rcFrame = this->GetPosition();  //m_rcPos.normalized();
	//	rcFrame.adjust(-1, -1, 1, 1);
	// ��
	QPen pen(Qt::darkGray);
	pen.setWidth(1);
	pen.setStyle(Qt::SolidLine);

	// צ
	unsigned int nHandles = GetHandleCount();

	QPointF ptSpinStart = GetSpinHandlePos(); // �������ο�ͼԪ������ʼ��
	QPointF ptSpinEnd(ptSpinStart);
	ptSpinEnd.setY(ptSpinStart.y() - 15);
	QLine lnSpin(ptSpinStart.toPoint(), ptSpinEnd.toPoint());

	pPainter->save();
	pPainter->setPen(pen);
	// ���ⲿ���ο�
	pPainter->drawRect(rcFrame);

	pPainter->drawLine(lnSpin);
	unsigned int i = 0;
	QRectF  rcHandler;

	if (false) // ��ת
	{

	}
	else
	{
		//QBrush br(Qt::green);
		for (i = 1; i <= nHandles; i++)
		{
			rcHandler = GetRawHandleRect(i);
			pPainter->setBrush(Qt::white);
			pPainter->setPen(Qt::darkGray);
			pPainter->drawRect(rcHandler);
		}
	}

	pen.setWidth(1);
	pen.setStyle(Qt::SolidLine);
	pen.setColor(Qt::darkGray);

	pPainter->setRenderHint(QPainter::Antialiasing);
	pPainter->setPen(pen);
	QRectF rcSpin(ptSpinEnd.x() - 7, ptSpinEnd.y() - 14, 14, 14);
	pPainter->drawArc(rcSpin, -60 * 16, -300 * 16);
	// ��С3������
	rcSpin.adjust(3, 3, -3, -3);

	// 	pen.setWidth(1);
	// 	pen.setColor(Qt::white);
	// 	pPainter->setPen(pen);
	pPainter->drawArc(rcSpin, -60 * 16, -300 * 16);


	double fAngle = 30;
	double fRadius = 7;

	double ptCenterX = ptSpinEnd.x();
	double ptCenterY = ptSpinEnd.y() - 7;

	double ptx = ptCenterX + fRadius * sin(fAngle* 3.14 / 180);
	double pty = ptCenterY + fRadius * cos(fAngle* 3.14 / 180);

	QPointF ptStart(ptx, pty);

	fRadius = 4;
	ptx = ptCenterX + fRadius * sin(fAngle* 3.14 / 180);
	pty = ptCenterY + fRadius * cos(fAngle* 3.14 / 180);
	QPointF ptEnd(ptx, pty);

	pPainter->drawLine(ptStart, ptEnd);

	QPointF ptArraw;
	ptStart.setX(ptCenterX + 7 + 2);
	ptStart.setY(ptCenterY);

	ptEnd = ptStart;
	ptEnd.setX(ptStart.x() - 4 - 3);

	ptArraw = ptStart;
	ptArraw.setY(ptStart.y() + 3);
	ptArraw.setX(ptEnd.x() + (ptStart.x() - ptEnd.x()) / 2);

	pPainter->drawLine(ptStart, ptArraw);

	pPainter->drawLine(ptEnd, ptArraw);

	//	pPainter->drawLine(ptStart, ptEnd);
	QPointF ptStart1(ptStart);
	ptStart1.setX(ptStart.x() - 2);
	pPainter->drawLine(ptStart, ptStart1);

	QPointF ptEnd1(ptEnd);
	ptEnd1.setX(ptEnd.x() + 2);
	pPainter->drawLine(ptEnd, ptEnd1);

	// 	QBrush brTmp;
	// 	brTmp.setColor(Qt::red);
	//  pPainter->drawPoint(ptTmp);

	pPainter->restore();
}

/*! \fn void CGroupWidget::DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected)
********************************************************************************************************* 
** \brief CGroupWidget::DrawMultiSelectedTracker 
** \details ��ѡ״̬�£���ͼԪ�����
** \param pPainter 
** \param bFirstSelected 
** \return void 
** \author LiJin
** \date 2016��2��11�� 
** \note ����ǵ�һ����ѡ�еĻ��������Ӵ�
********************************************************************************************************/
void CGroupWidget::DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;

	QRectF rcFrame = this->GetPosition();  
	//	rcFrame.adjust(-1, -1, 1, 1);
	// ��
	QPen pen(XGraphics::DarkBlue);
	if (bFirstSelected)
	{
		pen.setWidth(3);
	}
	else
	{
		pen.setWidth(2);
	}
	pen.setStyle(Qt::SolidLine);

	pPainter->save();
	pPainter->setPen(pen);
	// ���ⲿ���ο�
	pPainter->drawRect(rcFrame);
	pPainter->restore();
}
#endif

/** @}*/

