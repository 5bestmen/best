/*! @file graphview.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  graphview.cpp
文件实现功能 :  图形编辑
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   图形编辑
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

#include "graphview.h" 
#include "graphscene.h"
#include "widget_factory.h"

#include "qtpropertybrowser/QtVariantProperty"
#include "qtpropertybrowser/QtFontEditorFactory"
#include "qtpropertybrowser/QtCharEditorFactory"
#include "qtpropertybrowser/QtEnumEditorFactory"
#include "qtpropertybrowser/QtLineEditFactory"
#include "qtpropertybrowser/QtSliderFactory"
#include "qtpropertybrowser/QtScrollBarFactory"

#include "line_widget.h"
#include "shape_widget.h"
#include "background.h"
#include "graph_file.h"
#include "graphics_layer.h"
#include "graph_module.h"

#include "scadastudio/uimgr.h"
#include "attriwindow/attriwindow.h"

#include "qtpropertybrowser/qtpropertybrowser.h"
#include "qtpropertybrowser/QtTreePropertyBrowser"
#include "qtpropertybrowser/qtpropertymanager.h"

#include "axisframe.h"
#include "../scadastudio/uimodule/rightdock.h"
#include "group_widget.h"
#include "line_widget.h"
#include "bindinfoproperty.h"
#include "variant.h"
#include "thresholdchoosewgt.h"
#include "base_dync.h"
#include "dync_moving.h"
#include "dync_transparency.h"
#include "base_dync.h"
#include "dync_visible.h"
#include "dync_value.h"
#include "dync_text.h"
#include "group_move_line.h"
#include "graphtoolswgt.h"
#include "graph_serializer_factory.h"
#include "graph_serializer.h"
#include "xml_serializer.h"
#include "createnewtemplatewgt.h"
#include "added_widget.h"
#include "curveplot_wgt.h"

#include <QMouseEvent>
#include <QMessageBox>
#include <QMenu>
#include <QDebug>
#include <QScrollBar>
#include <QCoreApplication>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QXmlStreamWriter>

CGraphView::CGraphView(QWidget *parent) :
QGraphicsView(parent)
{

// 	setStyleSheet("background: transparent;border:0px");
// 	setWindowFlags(Qt::FramelessWindowHint);

	this->setViewportMargins(13, 13, 0, 0);
	m_pXFrame = new CAxisFrame(1, this);
	m_pYFrame = new CAxisFrame(2, this);

	m_pXFrame->setContentsMargins(0, 0, 0, 0);
	m_pYFrame->setContentsMargins(0, 0, 0, 0);

	this->resize(1920, 1200);

	m_nCurEditingLayerIndex = 1;
    //
    m_nViewPortWidth = 0;
    m_nViewPortHeight = 0;

	m_bDrawSelRect = false;
	m_pPorpWidget = nullptr;
	//m_scaleFactor = 40; 
    m_nCutCpyFlag = 0;
    //移动标志
    m_bMovingFlag = false;
	m_bViewEdable = true;
	m_nCurrentFile = 0;

	m_pTreeProperty = nullptr;

	m_pGroupMoveLine = nullptr;
//	m_pGraphFile = std::make_shared<CGraphFile>();

// 	connect(horizontalScrollBar(), &QScrollBar::sliderMoved, this, &CGraphView::slot_SliderMoved);
// 	connect(verticalScrollBar(), &QScrollBar::sliderMoved, this, &CGraphView::slot_SliderMoved);
	m_pPloyLine = nullptr;
	m_pIntManager    = new QtIntPropertyManager(this);
	m_pDoubleManager = new QtDoublePropertyManager(this);
	m_pStringManager = new QtStringPropertyManager(this);
	m_pColorManager  = new QtVariantPropertyManager(this);
	m_pFontManager 	 = new QtFontPropertyManager(this);
	m_pPointManager  = new QtPointPropertyManager(this);
	m_pSizeManager   = new QtSizePropertyManager(this);
	m_pGroupManager  = new QtGroupPropertyManager(this);

	m_pVariantManager       = new QtVariantPropertyManager(this);
	m_pVariantFactory       = new QtVariantEditorFactory(this);

	m_pDoubleSpinBoxFactory = new QtDoubleSpinBoxFactory(this);
	m_pCheckBoxFactory      = new QtCheckBoxFactory(this);
	m_pSpinBoxFactory       = new QtSpinBoxFactory(this);
	m_pLineEditFactory      = new QtLineEditFactory(this);
	m_pComboBoxFactory      = new QtEnumEditorFactory(this);
	m_pScrollBarFactory     = new QtScrollBarFactory(this);

	m_pCreateTemplateWgt = nullptr;

	connect(m_pDoubleManager, SIGNAL(valueChanged(QtProperty *, double)), this, SLOT(valueChanged(QtProperty *, double)));
	connect(m_pSizeManager, SIGNAL(valueChanged(QtProperty *, double)), this, SLOT(valueChanged(QtProperty *, double)));
	connect(m_pPointManager, SIGNAL(valueChanged(QtProperty *, double)), this, SLOT(valueChanged(QtProperty *, double)));

// 	connect(m_pDoubleManager,
// 		static_cast<void (QtDoublePropertyManager::*)(QtProperty *property, double val)> (&QtDoublePropertyManager::valueChanged),
// 		this,
// 		static_cast<void (CGraphView::*)(QtProperty *property, double val)> (&CGraphView::slot_valueChanged));
    //创建后保存一组数据
	AddHistFiles();

}

CGraphView::CGraphView(QGraphicsScene *pScene, QWidget * parent)
	:QGraphicsView(pScene, parent)
{
    this->setViewportMargins(13, 13, 0, 0);
    m_pXFrame = new CAxisFrame(1, this);
    m_pYFrame = new CAxisFrame(2, this);

	m_nCurrentFile = 0;
    m_pXFrame->setContentsMargins(0, 0, 0, 0);
    m_pYFrame->setContentsMargins(0, 0, 0, 0);
	
	m_pTreeProperty = nullptr;
	//m_scaleFactor = 50;
	m_nCurEditingLayerIndex = 1;
	m_bDrawSelRect = false;
	m_pPorpWidget = nullptr;
    m_nCutCpyFlag = 0;
	m_bViewEdable = true;

	m_pPloyLine = nullptr;
	m_pIntManager    = new QtIntPropertyManager(this);
	m_pDoubleManager = new QtDoublePropertyManager(this);
	m_pStringManager = new QtStringPropertyManager(this);
	m_pColorManager  = new QtVariantPropertyManager(this);
	m_pFontManager   = new QtFontPropertyManager(this);
	m_pPointManager  = new QtPointPropertyManager(this);
	m_pSizeManager   = new QtSizePropertyManager(this);
	m_pGroupManager  = new QtGroupPropertyManager(this);

	m_pBoolManager = new QtBoolPropertyManager(this);

	m_pComBoProperty = new QtEnumPropertyManager(this);
	//
	m_pBindInfoManager = new CBindInfoManager(this);
	m_pGroupMoveLine = nullptr;

	m_pCreateTemplateWgt = nullptr;
    
    setCacheMode(CacheBackground);
//	setRenderHint(QPainter::Antialiasing);
	setMouseTracking(true);
	//setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

// 	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
// 	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//  	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//  	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

// 	setStyleSheet("background: transparent;border:0px");
// 	setWindowFlags(Qt::FramelessWindowHint);
// 	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
// 	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      
//	m_pGraphFile = std::make_shared<CGraphFile>();

//	pScene->setSceneRect(m_pGraphFile->GetBackgroundRect());

	m_nDrawTool = DRAW_TOOLS_SEL;
	m_nMouseMode = WIDGET_MOUSE_SELECT;

	scale(1.0, 1.0);
// 	connect(horizontalScrollBar(), &QScrollBar::sliderMoved, this, &CGraphView::slot_SliderMoved);
// 	connect(verticalScrollBar(),   &QScrollBar::sliderMoved, this, &CGraphView::slot_SliderMoved);

//	connect(horizontalScrollBar(),)
	m_pVariantManager = nullptr;
	m_pVariantFactory = nullptr;

	m_pVariantManager = new QtVariantPropertyManager(this);
	m_pVariantFactory = new QtVariantEditorFactory(this);


	m_pDoubleSpinBoxFactory = new QtDoubleSpinBoxFactory(this);
	m_pCheckBoxFactory      = new QtCheckBoxFactory(this);
	m_pSpinBoxFactory       = new QtSpinBoxFactory(this);
	m_pLineEditFactory      = new QtLineEditFactory(this);
	m_pComboBoxFactory      = new QtEnumEditorFactory(this);
	m_pScrollBarFactory     = new QtScrollBarFactory(this);

	m_pBindInfoFactory = new CBindInfoFactory(this);

// 	connect(m_pDoubleManager,
// 		static_cast<void (QtDoublePropertyManager::*)(QtProperty *, double )> (&QtDoublePropertyManager::valueChanged),
// 		this,
// 		//&CGraphView::valueChanged);
// 		static_cast<void (CGraphView::*)(QtProperty *property, double val)> (&CGraphView::valueChanged));

// 	connect(stringManager, SIGNAL(valueChanged(QtProperty *, const QString &)),
// 		this, SLOT(valueChanged(QtProperty *, const QString &)));
// 	connect(colorManager, SIGNAL(valueChanged(QtProperty *, const QColor &)),
// 		this, SLOT(valueChanged(QtProperty *, const QColor &)));
// 	connect(fontManager, SIGNAL(valueChanged(QtProperty *, const QFont &)),
// 		this, SLOT(valueChanged(QtProperty *, const QFont &)));
// 	connect(pointManager, SIGNAL(valueChanged(QtProperty *, const QPoint &)),
// 		this, SLOT(valueChanged(QtProperty *, const QPoint &)));
// 	connect(sizeManager, SIGNAL(valueChanged(QtProperty *, const QSize &)),
// 		this, SLOT(valueChanged(QtProperty *, const QSize &)));

	connect(m_pDoubleManager, SIGNAL(valueChanged(QtProperty *, double)), this, SLOT(valueChanged(QtProperty *, double)) );
	connect(m_pSizeManager, SIGNAL(valueChanged(QtProperty *, QSize)), this, SLOT(valueChanged(QtProperty *, QSize)));
	connect(m_pPointManager, SIGNAL(valueChanged(QtProperty *, QPoint)), this, SLOT(valueChanged(QtProperty *, QPoint)));
	connect(m_pColorManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)), this, SLOT(valueChanged(QtProperty *, const QVariant &)));

	connect(m_pIntManager, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(valueChanged(QtProperty *, int)));

	connect(m_pComBoProperty, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(valueChanged(QtProperty *, int)));;

	connect(m_pStringManager, SIGNAL(valueChanged(QtProperty *, const QString &)), this, SLOT(valueChanged(QtProperty *, const QString &)));

	connect(m_pBoolManager, SIGNAL(valueChanged(QtProperty *, bool)), this, SLOT(Slot_BoolValueChange(QtProperty *, bool)));

	connect(m_pBindInfoManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)), this, SLOT(valueChanged(QtProperty *, const QVariant &)));
	
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(Slot_ScrollBarChange()));
    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(Slot_ScrollBarChange()));
 
	//创建后保存一组数据
	AddHistFiles();
	//缩放

	CGraphScene *pMScene = GetGraphicsScene();
	Q_ASSERT(pMScene);
	if (pMScene == nullptr)
		return;

	CGraphFile * pGraphFile = pMScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
	{
		return;
	}
	QMatrix old_matrix;
	old_matrix = this->matrix();
	this->resetMatrix();
	//this->translate(old_matrix.dx(), old_matrix.dy());
	this->scale(pGraphFile->GetSacled() / 50.0, pGraphFile->GetSacled() / 50.0);

}

CGraphView::~CGraphView()
{ 
	ClearPropMap();

	for (auto i : m_lstGraphFiles)
	{
		delete i;
	}
	m_lstGraphFiles.clear();

	if (m_pCreateTemplateWgt != nullptr)
	{
		m_pCreateTemplateWgt->deleteLater();
	}
}

CGraphScene *CGraphView::GetGraphicsScene() const
{
	return dynamic_cast<CGraphScene *>(scene());
}

void CGraphView::DrawRuler(QPainter *pPainter, const QRectF & rcViewPort, const QRectF & rcScene)
{
	Q_UNUSED(pPainter);
	Q_UNUSED(rcViewPort);
	Q_UNUSED(rcScene);

   // pPainter->drawPoint();
// 	QBrush br = pPainter->brush();
// 	br.setColor(Qt::blue);
// 	br.setStyle(Qt::SolidPattern);
// 	br.setTexture( (QPixmap("../product-ifix.jpg")) );
// 	pPainter->fillRect(rect, br);
// 	pPainter->drawRect(rect);
// 	const int LARGE_RULER = 50;
// 	const int LITTLE_RULER = 10;
// 	const int MARGIN_LEN = 30;
// 
// 	QLine lnHorz(rcViewPort.x() + MARGIN_LEN, rcViewPort.y() + MARGIN_LEN, rcViewPort.x() + rcViewPort.width(), rcViewPort.y() + MARGIN_LEN);
// 	QLine lnVert(rcViewPort.x() + MARGIN_LEN, rcViewPort.y() + MARGIN_LEN, rcViewPort.x() + MARGIN_LEN, rcViewPort.y() + rcViewPort.height());
// 
// 	std::vector<QLine> arrLines;
// 	QLine lnTemp;
// 	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
// 	int nLargeValue = 0, nLittleValue = 0;
// 
// 	// 水平上竖线
// 	unsigned int i = 0;
// 
// 	using PosAndValue = std::pair<QPoint, int>;
// 	std::vector<PosAndValue> arrPoints;
// 	for (;;)
// 	{	
// 		nLargeValue = ceil(rcViewPort.x() / LARGE_RULER) * LARGE_RULER + i * LARGE_RULER;
// 		// 底部的点
// 		y1 = rcViewPort.y() + MARGIN_LEN;
// 		x1 = ceil(rcViewPort.x() / LARGE_RULER) * LARGE_RULER + i * LARGE_RULER;
// 		i++;
// 
// 		if (x1 >= rcViewPort.right())
// 		{
// 			break;
// 		}
// 		// 顶部的点
// 		x2 = x1;
// 		y2 = y1 - 20;
// 
// 		lnTemp.setLine(x1, y1, x2, y2);
// 		if (x1 >= rcViewPort.x() + MARGIN_LEN)
// 		{
// 			arrLines.push_back(lnTemp);
// 			PosAndValue p_val = std::make_pair(QPoint(x1+2, y1 - 12), nLargeValue);
// 			arrPoints.push_back(p_val);
// 		}	
// 	}
// 
// 	i = 0;
// 	for (;;)
// 	{
// 		nLittleValue = ceil(rcViewPort.x() / LITTLE_RULER) * LITTLE_RULER + i * LITTLE_RULER;
// 		// 底部的点
// 		y1 = rcViewPort.y() + MARGIN_LEN;
// 		x1 = ceil(rcViewPort.x() / LITTLE_RULER) * LITTLE_RULER + i * LITTLE_RULER;
// 		i++;
// 
// 		if (x1 >= rcViewPort.right())
// 		{
// 			break;
// 		}
// 		// 顶部的点
// 		x2 = x1;
// 		y2 = y1 - 10;
// 
// 		lnTemp.setLine(x1, y1, x2, y2);
// 		if ((x1 >= rcViewPort.x() + MARGIN_LEN) && (nLittleValue % LARGE_RULER != 0))
// 		{
// 			arrLines.push_back(lnTemp);			
// 		}
// 	}
// 	// 垂直上横线
// 	i = 0;
// 	for (;;)
// 	{
// 		nLargeValue = ceil(rcViewPort.y() / LARGE_RULER) * LARGE_RULER + i * LARGE_RULER;
// 		// 右侧的点 
// 		x1 = rcViewPort.x() + MARGIN_LEN;
// 		y1 = ceil(rcViewPort.y() / LARGE_RULER) * LARGE_RULER + i * LARGE_RULER;		  
// 
// 		i++;
// 
// 		if (y1 >= rcViewPort.bottom())
// 		{
// 			break;
// 		}
// 		// 左侧的点
// 		x2 = x1 - 20 ;
// 		y2 = y1;
// 
// 		lnTemp.setLine(x1, y1, x2, y2);
// 		if (y1 >= rcViewPort.y() + MARGIN_LEN)
// 		{
// 			arrLines.push_back(lnTemp);
// // 			PosAndValue p_val = std::make_pair(QPoint(x1 + 2, y1 - 12), nLargeValue);
// // 			arrPoints.push_back(p_val);
// 		}
// 	}
// 	i = 0;
// 	for (;;)
// 	{
// 		nLittleValue = ceil(rcViewPort.y() / LITTLE_RULER) * LITTLE_RULER + i * LITTLE_RULER;
// 		// 右侧的点
// 		x1 = rcViewPort.x() + MARGIN_LEN;
// 		y1 = ceil(rcViewPort.y() / LITTLE_RULER) * LITTLE_RULER + i * LITTLE_RULER;
// 
// 		i++;
// 
// 		if (y1 >= rcViewPort.bottom())
// 		{
// 			break;
// 		}
// 		// 左侧的点
// 		x2 = x1 - 10;
// 		y2 = y1;
// 
// 		lnTemp.setLine(x1, y1, x2, y2);
// 		if ((y1 >= rcViewPort.y() + MARGIN_LEN) && (nLittleValue % LARGE_RULER != 0))
// 		{
// 			arrLines.push_back(lnTemp);
// 		}
// 	}
// 
// 	pPainter->save();
// 	pPainter->setPen(Qt::darkGray);
// 	pPainter->drawLine(lnHorz);
// 	pPainter->drawLine(lnVert);
// 	if (arrLines.empty() == false)
// 	{
// 		pPainter->drawLines(arrLines.data(), arrLines.size());
// 	}
// 	 
// 	for ( auto it:arrPoints)
// 	{
// 		pPainter->drawText(it.first,QString::number(it.second));
// 	}
// 	pPainter->restore();
}

void CGraphView::drawBackground(QPainter *pPainter, const QRectF &rect)
{


		CGraphScene *pScene = GetGraphicsScene();
		Q_ASSERT(pScene);
		if (pScene == nullptr)
			return;

		CGraphFile * pGraphFile = pScene->GetGraphFile();
		Q_ASSERT(pGraphFile);
		if (pGraphFile == nullptr)
		{
			return;
		}

		QRectF tRect; 

		if (pGraphFile->GetSacled() != 50)
		{

// 			if (rect.width()*m_scaleFactor / 50.0 == 0.0f && rect.height()*m_scaleFactor / 50.0 == 0.0f)
// 			{
// 				return;
// 			}
// 
// 			pGraphFile->SetGraphSize(QSize(rect.width()*m_scaleFactor / 50.0, rect.height()*m_scaleFactor / 50.0));
// 
// 			if (!m_bMovingFlag)
// 			{
// 				pGraphFile->SetGraphcsSize(QSize(rect.width()*m_scaleFactor / 50.0, rect.height()*m_scaleFactor / 50.0));
// 			}
// 			
// 
// 			tRect.setTopLeft(this->scene()->sceneRect().topLeft());
// 			tRect.setWidth(rect.width()*m_scaleFactor / 50.0);
// 			tRect.setHeight(rect.height()*m_scaleFactor / 50.0);


			tRect.setWidth(rect.width()*pGraphFile->GetSacled() / 50.0);
			tRect.setHeight(rect.height()*pGraphFile->GetSacled() / 50.0);

		}
		else
		{
			tRect = rect;
			tRect.setTopLeft(QPointF(0,0));
			tRect.setSize(QSize(pGraphFile->GetSize().second,pGraphFile->GetSize().first));
			setSceneRect(tRect);

		}
		
		tRect.setTopLeft(QPointF(0, 0));
		tRect.setSize(QSize(pGraphFile->GetSize().second, pGraphFile->GetSize().first));

		//pGraphFile->SetGraphcsSize(tRect.size().toSize());
        pScene->GetGraphFile()->DrawBackground(pPainter, tRect);
		

		//DrawRuler(pPainter, rect,sceneRect);
	

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

		CGraphFile * pGraphFile = pScene->GetGraphFile();
		Q_ASSERT(pGraphFile);
		if (pGraphFile == nullptr)
			return;

		if (m_nMouseMode == WIDGET_MOUSE_MOVE && m_bMovingFlag)
		{
			pGraphFile->DrawMovingWidgets(pPainter, m_nCurEditingLayerIndex);
		}

        if (m_bMovingFlag)
        {
            auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
            Q_ASSERT(pLayer != nullptr);
            if (pLayer != nullptr)
            {
                pLayer->DrawSelectionFrame(pPainter);
            }
        }

	}

 	DrawSelectBox(pPainter);
	QGraphicsView::drawForeground(pPainter, rect);
}
/*! \fn bool CGraphView::MoveWidgets(unsigned int nPixel, int nKey)
********************************************************************************************************* 
** \brief CGraphView::MoveWidgets 
** \details 键盘移动图元
** \param nPixel 
** \param nKey 
** \return bool 
** \author LiJin
** \date 2016年2月11日 
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
    else if (nKey == Qt::Key_X && kbModifiers == (Qt::ControlModifier))
    {
        //剪切
        OnCut();
    }
    else if (nKey == Qt::Key_C && kbModifiers == (Qt::ControlModifier))
    {
        //复制
        OnCopy();
    }
    else if (nKey == Qt::Key_V && kbModifiers == (Qt::ControlModifier))
    {
        //粘贴
        OnPaste();
    }
	else if (nKey == Qt::Key_Z && kbModifiers == (Qt::ControlModifier))
	{
		//回退
		m_nCurrentFile--;
		if (m_lstGraphFiles.size() + m_nCurrentFile - 1 < 0)
		{
			m_nCurrentFile++;
			return;
		}
		CGraphScene *pScene = GetGraphicsScene();
		Q_ASSERT(pScene);
		if (pScene == nullptr)
			return;

		CGraphFile * pGraphFile = pScene->GetGraphFile();
		Q_ASSERT(pGraphFile);
		if (pGraphFile == nullptr)
		{
			return;
		}

		if (m_lstGraphFiles[m_lstGraphFiles.size() + m_nCurrentFile - 1]->m_bVaildFlag == false)
		{
			m_nCurrentFile++;
			return;
		}

		*pGraphFile = *m_lstGraphFiles[m_lstGraphFiles.size()+m_nCurrentFile-1];

		scene()->update();
		
	}
	else if (nKey == Qt::Key_Y && kbModifiers == (Qt::ControlModifier))
	{
		//redo
		m_nCurrentFile++;
		if (m_nCurrentFile> 0)
		{
			m_nCurrentFile = 0;
			return;
		}

		CGraphScene *pScene = GetGraphicsScene();
		Q_ASSERT(pScene);
		if (pScene == nullptr)
			return;

		CGraphFile * pGraphFile = pScene->GetGraphFile();
		Q_ASSERT(pGraphFile);
		if (pGraphFile == nullptr)
		{
			return;
		}

		if (m_lstGraphFiles[m_lstGraphFiles.size() + m_nCurrentFile - 1]->m_bVaildFlag == false)
		{
			m_nCurrentFile--;
			return;
		}

		*pGraphFile = *m_lstGraphFiles[m_lstGraphFiles.size() + m_nCurrentFile-1];

		scene()->update();
	}
	else 
	{
		if ( // SHIFT + 移动键  移动10个像素
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
                scene()->update();
			}
		}
		else if ( // 纯粹的移动键  只移动1个像素
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
                scene()->update();
			}
		}
	}
}
/*! \fn void CGraphView::MoveWidgets(const QPointF & delta)
********************************************************************************************************* 
** \brief CGraphView::MoveWidgets 
** \details 移动选中的图元
** \param delta 
** \return void 
** \author LiJin 
** \date 2016年2月6日 
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

	CGraphFile * pGraphFile = pScene->GetGraphFile();
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
	{// 不能移动
		qDebug() << "图元移动出界";
        
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

	CGraphFile * pGraphFile = pScene->GetGraphFile();
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
	{// 不能移动
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
    //更新坐标轴当前位置显示
    m_pXFrame->UpdateCurrentHorizontal(e->pos().x());
    m_pYFrame->UpdateCurrentVorizontal(e->pos().y());

	if (m_pPloyLine != nullptr)
	{
		return;
	}
    
    //qDebug() << e->pos();
	QPointF ptTemp(mapToScene(e->pos()).x(), mapToScene(e->pos()).y());

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	if ( e->buttons() & Qt::LeftButton )
	{// 如果左键
		qDebug() << QStringLiteral("左键按下并移动");

        m_bMovingFlag = true;

		if (m_nDrawTool == DRAW_TOOLS_SEL)
		{
			if (m_nMouseMode == WIDGET_MOUSE_SELECT)
			{// 画选择框
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
				this->viewport()->update();
			}
			else if (m_nMouseMode == WIDGET_MOUSE_ROTATING)
			{
			//	ResizeWidget(pointf);
                //循转
                Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
                if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
                {
                    return;
                }

                CGraphScene *pScene = GetGraphicsScene();
                Q_ASSERT(pScene);
                if (pScene == nullptr)
                    return;

                CGraphFile * pGraphFile = pScene->GetGraphFile();
                Q_ASSERT(pGraphFile);
                if (pGraphFile == nullptr)
                    return;

                auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
                Q_ASSERT(pLayer != nullptr);
                if (pLayer == nullptr)
                {
                    return;
                }

                bool bUpdate = false;
                //QPointF delta = ptTemp - m_ptLast;
                bUpdate = pLayer->RotatingWidget(ptTemp);

                if (bUpdate)
                {
                    this->viewport()->update();
                }


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
	{// 左键没有按下
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

	QGraphicsView::mouseMoveEvent(e);
}

CBaseWidget *CGraphView::HitTest(QPoint & pos, unsigned int nLayerIdx)
{
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= CGraphFile::MAX_LAYER_CNT);
	if (nLayerIdx == 0 || nLayerIdx > CGraphFile::MAX_LAYER_CNT)
		return nullptr;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return nullptr;

	auto pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return nullptr;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer);
	if (pLayer == nullptr)
		return nullptr;

	return pLayer->HitTest(pos);
#if 0
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
#endif
}
/*! \fn int  CGraphView::GetMouseOnHandleIdx(const QPointF & pointf)
********************************************************************************************************* 
** \brief CGraphView::GetMouseOnHandleIdx 
** \details 获取鼠标在图元上的爪子位置
** \param pointf 
** \return int 
** \author LiJin
** \date 2016年2月9日 
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
	{// 单选
		CBaseWidget *pWidget = nullptr;
		std::tie(m_nDragHandle, pWidget) = pLayer->HitTestSingleSelectedWidgetHandle(pos);
		if (m_nDragHandle > 0 && pWidget !=nullptr && fabs(pWidget->GetRotateAngle()) < 30)
		{
			//修改光标
            if (m_nDragHandle == 1)
            {
                this->setCursor(QCursor(Qt::SizeFDiagCursor));
            }
            else if (m_nDragHandle == 2)
            {
                this->setCursor(QCursor(Qt::SizeVerCursor));
            }
            else if (m_nDragHandle == 3)
            {
                this->setCursor(QCursor(Qt::SizeBDiagCursor));
            }
            else if (m_nDragHandle == 8)
            {
                this->setCursor(QCursor(Qt::SizeHorCursor));
            }
            else if (m_nDragHandle == 4)
            {
                this->setCursor(QCursor(Qt::SizeHorCursor));
            }
            else if (m_nDragHandle == 7)
            {
                this->setCursor(QCursor(Qt::SizeBDiagCursor));
            }
            else if (m_nDragHandle == 6)
            {
                this->setCursor(QCursor(Qt::SizeVerCursor));
            }
            else if (m_nDragHandle == 5)
            {
                this->setCursor(QCursor(Qt::SizeFDiagCursor));
            }


			//
//             if (pWidget->IsRotate())
//             {
//                 this->setCursor(QCursor(Qt::ClosedHandCursor));
//                 m_nMouseMode = WIDGET_MOUSE_ROTATE;
//             }
// 			else
				m_nMouseMode = WIDGET_MOUSE_SIZE;
		}
        else
        {
            //qDebug() << mapToParent(pWidget->GetSpinHandlePos());

            if (pWidget != nullptr)
            {
                if (fabs((pWidget->GetSpinMovingHandlePos() - pos).x()) < 10 && fabs((pWidget->GetSpinMovingHandlePos() - pos).y()) < 10 )
                {
                    pWidget->SetRotate(true);
                    m_nMouseMode = WIDGET_MOUSE_ROTATE;
                    this->setCursor(QCursor(Qt::ClosedHandCursor));
                }
                else
                {
                    this->setCursor(QCursor(Qt::ArrowCursor));
                }
            }

            
        }

        

	}
	else if (nRet == CGraphicsLayer::MULTI_SELECTED)
	{// 多选 
		m_nDragHandle = pLayer->HitTestMultiSelectedWidgetHandle(pos);
		if (m_nDragHandle >0)
		{	
			//修改光标
			m_nMouseMode = WIDGET_MOUSE_SIZE;
		}
        else
        {
            this->setCursor(QCursor(Qt::ArrowCursor));

        }
	}

	return m_nDragHandle;
}

void CGraphView::OnLButtonDown(QMouseEvent * e)
{
	if (m_nCurEditingLayerIndex == 0)
	{
		QMessageBox::information(this, QStringLiteral("注意"), QStringLiteral("当前图层无法编辑..."));
		return;
	}

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	int nPosX = static_cast <int> (mapToScene(e->pos()).x());
	int nPosY = static_cast <int> (mapToScene(e->pos()).y());

	QPointF ptTemp(mapToScene(e->pos()).x(), mapToScene(e->pos()).y());
	m_ptFisrst = ptTemp;

	if (m_nDrawTool == DRAW_TOOLS_SEL)
	{
		if (m_nMouseMode == WIDGET_MOUSE_SIZE)//准备改变尺寸
		{
			m_nMouseMode = WIDGET_MOUSE_SIZING;
		}
		else if (m_nMouseMode == WIDGET_MOUSE_ROTATE)//准备旋转
		{
			m_nMouseMode = WIDGET_MOUSE_ROTATING;
		}
		else
		{
			//bool bHitTest = false;
			bool  bShiftKey = false, bCtrlKey = false;
			if (e->modifiers() == Qt::SHIFT)
			{
				bShiftKey = true;
			}
			if (e->modifiers() == Qt::CTRL)
			{
				bCtrlKey = true;
			}
            QPoint tBasePoint(ptTemp.x(),ptTemp.y());
            CBaseWidget *pObj = HitTest(tBasePoint, m_nCurEditingLayerIndex);
			if (pObj == nullptr)
			{// 一个图元也没有点击到
				pGraphFile->SelectNone(m_nCurEditingLayerIndex);
				m_nMouseMode = WIDGET_MOUSE_SELECT;
			}
			else
			{// 点击到图元
			
				if (bShiftKey == true)
				{//  SHIFT 按键 多选
					pGraphFile->SelectMultiWidgets(m_nCurEditingLayerIndex,pObj);
				}
				else if (bCtrlKey == true)
				{// CTRL 按键，单选，拖动为复制
					// 清空此前所选，
					pGraphFile->SelectNone(m_nCurEditingLayerIndex);
					// 选择新的图元
					pGraphFile->SelectWidget(m_nCurEditingLayerIndex, pObj); 
				}
				else
				{// 单选图元
					pGraphFile->ReSelectWidget(m_nCurEditingLayerIndex, pObj);
					pObj->SetPressFlag(true);
				}
				// 多选状态 -> 进入 移动
				m_nMouseMode = WIDGET_MOUSE_MOVE;

                pObj->update();
			}
		}
	}
	else
	{// 增加新的图元
		if (nPosX < 0 || nPosY < 0) //！TODO 还需要判断是否越右边的界限
		{// 提示使用者
			QMessageBox::information(this, QStringLiteral("超出范围"), QStringLiteral("你添加的图元超出画布范围，请注意..."));
			return;
		}
		else
		{
			QRectF rcPos(mapToScene(e->pos()), QSizeF(0, 0));
			bool bRet = AddNewWidget(rcPos, m_nDrawTool);
			if (bRet)
			{//!TODO 需要提示文档已修改

			}

		}
	}
	m_ptLast = ptTemp;

    
}


void CGraphView::mousePressEvent(QMouseEvent * e)
{
    qDebug() << "view:" << e->pos();
    qDebug() << "Scene" << mapToScene(e->pos());

    m_CurrentClickPoint = mapToScene(e->pos());

	if (!m_bViewEdable)
	{
		QGraphicsView::mousePressEvent(e);
		return;
	}

	if (m_pPloyLine != nullptr)
	{
		m_pPloyLine->InsertNewPoint(m_CurrentClickPoint);

		m_pPloyLine->UpdateAllPoints();
		return;
	}

    m_bMovingFlag = false;
	//int nPosX = static_cast <int> (mapToScene(e->pos()).x());
	//int nPosY = static_cast <int> (mapToScene(e->pos()).y());

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;
 
	QPointF ptTemp(mapToScene(e->pos()).x(), mapToScene(e->pos()).y());

	if (e->button() == Qt::LeftButton)
	{// 如果左键
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
    this->scene()->update();

	QGraphicsView::mousePressEvent(e);
}

void CGraphView::OnLButtonUp(QMouseEvent * e)
{
	//int nPosX = static_cast <int> (mapToScene(e->pos()).x());
	//int nPosY = static_cast <int> (mapToScene(e->pos()).y());

	QPointF ptCur(mapToScene(e->pos()).x(), mapToScene(e->pos()).y());

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	CGraphFile * pGraphFile = pScene->GetGraphFile();
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
			if (m_bMovingFlag)
			{
				if (e->modifiers() == Qt::CTRL)
				{
					OnCopy();
					pGraphFile->SelectNone(m_nCurEditingLayerIndex);
					OnPaste();
				}

				QPointF delta = ptCur - m_ptFisrst;
				MoveWidgets(delta);

				AddHistFiles();
			}
		}
		else if (m_nMouseMode == WIDGET_MOUSE_SIZING)
		{// 调整，鼠标松开时，调整结束。
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
			AddHistFiles();
		}
		else if (m_nMouseMode == WIDGET_MOUSE_ROTATING)
		{
			AddHistFiles();
		}
	}
	else  if (m_nDrawTool != DRAW_TOOLS_POLYLINE && m_nDrawTool != DRAW_TOOLS_POLYGON  &&m_nDrawTool != DRAW_TOOLS_PIPE )
	{
		CBaseWidget * pInserObj = pGraphFile->GetCurrentWidget(m_nCurEditingLayerIndex);
		if (pInserObj != nullptr)
		{
			if (m_ptFisrst == ptCur)
			{// 如果图元的位置空间为0，则取消掉这个图元。
				Q_ASSERT(pInserObj->m_bSelected);
				if (pInserObj->m_bSelected)
				{//先取消选择
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
			{// 如果图元的位置空间为0，则取消掉这个图元。
				Q_ASSERT(m_pInsertObj->m_bSelected);
				if (m_pInsertObj->m_bSelected)
				{//先取消选择
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

	if (!m_bViewEdable)
	{
		if (m_pGroupMoveLine == nullptr)
		{
			m_pGroupMoveLine = new CGroupMoveLine;
			m_pGroupMoveLine->setZValue(2);

			this->scene()->addItem(m_pGroupMoveLine);
			if (m_pPorpWidget != nullptr)
			{
				m_pGroupMoveLine->InsetNewPoint(m_pPorpWidget->GetRotationCenterPoint());


				CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_GROUP_MOVE);
				CDyncGroupMovingData *pGroupMoveData = dynamic_cast<CDyncGroupMovingData*>(pbaseDync);

				Q_ASSERT(pGroupMoveData);
				if (pGroupMoveData == nullptr)
				{
					return;
				}

				//保存的点为相对点
				pGroupMoveData->InsertPonit(m_pPorpWidget->GetRotationCenterPoint() - m_pPorpWidget->GetRotationCenterPoint());


			}

		}

		m_pGroupMoveLine->InsetNewPoint(mapToScene(e->pos()));

		if (m_pPorpWidget != nullptr)
		{
			//模拟色起始偏移
			CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_GROUP_MOVE);
			CDyncGroupMovingData *pGroupMoveData = dynamic_cast<CDyncGroupMovingData*>(pbaseDync);

			Q_ASSERT(pGroupMoveData);
			if (pGroupMoveData == nullptr)
			{
				return;
			}

			pGroupMoveData->InsertPonit(mapToScene(e->pos()) - m_pPorpWidget->GetRotationCenterPoint());

		}

		this->scene()->update();

		return;
	}


	if (e->button() == Qt::LeftButton)
	{// 如果左键
		OnLButtonUp(e);
	}
	m_nMouseMode = WIDGET_MOUSE_NOTHING;
    m_bMovingFlag = false;

    this->scene()->update();
	//update();
	//更新属性  值更新部分属性
	//旋转信息

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

// 	if (pLayer->IsSelectionEmpty() == true)
// 		return;

	CBaseWidget *pWidget = pLayer->GetFirstSelectedWidget();
	//Q_ASSERT(pWidget);
	m_pPorpWidget = pWidget;
	//redo undo
	//


	if (pWidget == nullptr)
	{
		SetGraphicItemAttrHidden();
		ChangeProperty();

		return;

	}



	pWidget->SetPressFlag(false);
	//保存历史操作数据
	//AddHistFiles();

	ChangeProperty();

	QGraphicsView::mouseReleaseEvent(e);
}

void CGraphView::dragEnterEvent(QDragEnterEvent *event)
{
    //接收文字类型的数据
    if (event->mimeData()->hasFormat("text/plain"))
    {
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }
    }
    else
    {
        event->ignore();
    }
}


void CGraphView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
    {
        //接收文字类型的数据
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }
    }
    else
    {
        event->ignore();
    }
}

void CGraphView::dropEvent(QDropEvent *event)
{
	//取文字类型的数据
	if (event->mimeData()->hasFormat("text/plain"))
	{
		//携带的数据
		QByteArray csvData = event->mimeData()->data("text/plain");
		int ChangeType = csvData.toInt();

		CGraphScene *pScene = GetGraphicsScene();
		Q_ASSERT(pScene);
		if (pScene == nullptr)
		{
			return;
		}
		QRectF rcPos(mapToScene(event->pos()), QSizeF(100, 100));


		AddNewWidget(rcPos, ChangeType);

	}
}


void CGraphView::wheelEvent(QWheelEvent *event)
{
	if (event->modifiers() != Qt::ControlModifier)
	{
		return;
	}

	bool bZoom = true;

	if (event->delta() > 0)
	{
		bZoom = true;
	}
	else
	{
		bZoom = false;
	}

// 	QRectF tOldRect = this->rect();

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
	{
		return;
	}

	if (bZoom && pGraphFile->GetSacled() >= 0)
	{
		pGraphFile->SetScaled(pGraphFile->GetSacled() + 2);
		//m_scaleFactor += 2;
		QMatrix old_matrix;
		old_matrix = this->matrix();
		this->resetMatrix();
		//this->translate(old_matrix.dx(), old_matrix.dy());
		this->scale(pGraphFile->GetSacled() / 50.0, pGraphFile->GetSacled() / 50.0);
		qDebug()<<"GGGGGG:"<<this->rect();
		
	}
	else if (!bZoom && pGraphFile->GetSacled() >= 0)
	{
		pGraphFile->SetScaled(pGraphFile->GetSacled() - 2);
		//m_scaleFactor -= 2;
		QMatrix old_matrix;
		old_matrix = this->matrix();
		this->resetMatrix();
		//this->translate(old_matrix.dx(), old_matrix.dy());

		this->scale(pGraphFile->GetSacled() / 50.0, pGraphFile->GetSacled() / 50.0);

	}
	else if (pGraphFile->GetSacled() < 0)
	{
		pGraphFile->SetScaled(0);
	}
	
	//this->scene()->setSceneRect(QRectF(0,0,this->width(),this->height()));
	
	ChangeProperty();

}

void CGraphView::mouseDoubleClickEvent(QMouseEvent * e)
{
	if (m_pPloyLine != nullptr)
	{
		m_pPloyLine->UpdateAllPoints();
		m_pPloyLine = nullptr;
		return;
	}

    Q_UNUSED(e);
    m_bMovingFlag = false;

	Slot_ShowItemProperty();

}

void CGraphView::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu;
	QAction *pAction = nullptr;
	
	pAction = menu.addAction(QStringLiteral("属性(&O)"), this, SLOT(Slot_ShowItemProperty()));
	//connect(pAction, &QAction::triggered, this, &CGraphView::OnAttr);

	menu.addSeparator();

	pAction = menu.addAction(QStringLiteral("剪切(&T)"), this, SLOT(OnCut()));
	//connect(pAction, &QAction::triggered, this, &CGraphView::OnCut);

	pAction = menu.addAction(QStringLiteral("复制(&C)"), this, SLOT(OnCopy()));
	//connect(pAction, &QAction::triggered, this, &CGraphView::OnCopy);

	pAction = menu.addAction(QStringLiteral("粘帖(&P)"), this, SLOT(OnPaste()));
	//connect(pAction, &QAction::triggered, this, &CGraphView::OnPaste);

	pAction = menu.addAction(QStringLiteral("删除(&D)"), this, SLOT(OnDelete()));
	//connect(pAction, &QAction::triggered, this, &CGraphView::OnDelete);

	menu.addSeparator();

	pAction = menu.addAction(QStringLiteral("组合(&G)"), this, SLOT(OnWidgetsGroup()));
	//connect(pAction, &QAction::triggered, this, &CGraphView::OnWidgetsGroup);
	pAction = menu.addAction(QStringLiteral("取消组合(&U)"), this, SLOT(OnWidgetsBreak()));
	//connect(pAction, &QAction::triggered, this, &CGraphView::OnWidgetsBreak);

	pAction = menu.addAction(tr("Save Library(&T)"), this, SLOT(OnSaveTemplate()));

	menu.addSeparator();
	pAction = menu.addAction(QStringLiteral("置于底层(&B)"), this, SLOT(MoveToBottomLevel()));
	//connect(pAction, &QAction::triggered, this, &CGraphView::MoveToBottomLevel);

	pAction = menu.addAction(QStringLiteral("置于顶层(&T)"), this, SLOT(MoveToTopLevel()));
	//connect(pAction, &QAction::triggered, this, &CGraphView::MoveToTopLevel);

	pAction = menu.addAction(QStringLiteral("下移一层(&S)"), this, SLOT(MoveDown()));
	//connect(pAction, &QAction::triggered, this, &CGraphView::MoveDown);

	pAction = menu.addAction(QStringLiteral("上移一层(&X)"), this, SLOT(MoveUp()));
	//connect(pAction, &QAction::triggered, this, &CGraphView::MoveUp);

	menu.addSeparator();
	pAction = menu.addAction(tr("UpLayer(&U)"), this, SLOT(OnLayerUp()));
	pAction = menu.addAction(tr("DownLayer(&D)"), this, SLOT(OnLayerDown()));
	menu.addSeparator();
	pAction = menu.addAction(QStringLiteral("导出"), this, SLOT(OnExport()));

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

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return false;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return false;

	CBaseWidget *pWidget = nullptr;
	if (nWidgetType == DRAW_TOOLS_TEMPLATE)
	{
		//加载图形库
		QString strFilename = GetModuleApi()->GetToolWgt()->currentItem()->data(0, Qt::UserRole + 1).toString();

		QFile file(qApp->applicationDirPath() + "/gtemplate/library/" + strFilename);

		if (!file.open(QFile::ReadOnly | QFile::Text))
		{
			qDebug() << "Error: Cannot write file: " << qPrintable(file.errorString());
			return false;
		}

		std::shared_ptr<QXmlStreamReader> pXml = std::make_shared<QXmlStreamReader>(&file);

		CGraphXmlSerializer pXmlReader;
		pXmlReader.LoadGraphTemplateInfo(pXml,pGraphFile,m_nCurEditingLayerIndex,rcPos);

	}
	else
	{
		pWidget = CWidgetFactory::CreateWidget(rcPos, nWidgetType);


		Q_ASSERT(pWidget);
		if (pWidget)
		{
			pLayer->SetCurrentWidget(pWidget);

			pLayer->Select(nullptr);
			pLayer->Select(pWidget);
		}

		if (pWidget && pGraphFile)
		{
			pGraphFile->AddWidget(m_nCurEditingLayerIndex, pWidget);

			//
			AddHistFiles();

		}

		if (nWidgetType == DRAW_TOOLS_POLYLINE || nWidgetType == DRAW_TOOLS_POLYGON || nWidgetType == DRAW_TOOLS_PIPE)
		{
			//折线  改变鼠标状态
			this->setCursor(QCursor(Qt::CrossCursor));
			m_pPloyLine = dynamic_cast<CLineWidget *>(pWidget);

			if (m_pPloyLine != nullptr)
			{
				m_pPloyLine->InsertNewPoint(rcPos.topLeft());
			}
		}

	}

	return true;
}
/*! \fn void CGraphView::SelectAll(unsigned int nLayerIdx)
********************************************************************************************************* 
** \brief CGraphView::SelectAll 
** \details 选择指定图层的所有图元
** \param nLayerIdx 
** \return void 
** \author LiJin 
** \date 2016年1月18日 
** \note 
********************************************************************************************************/
void CGraphView::SelectAll(unsigned int nLayerIdx)
{
	Q_ASSERT(nLayerIdx >= 0 && nLayerIdx < CGraphFile::MAX_LAYER_CNT);

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	CGraphFile * pGraphFile = pScene->GetGraphFile();
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
//单击选择或取消选择
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
** \details 取消图元的选中状态
** \param   pObj
** \return void
** \author LiJin
** \date  2010年11月24日
** \note
********************************************************************************************************/
void CGraphView::Deselect(CBaseWidget* pObj)
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
}
#endif

//重画pObj对象
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



void CGraphView::AddHistFiles()
{
	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	if (m_nCurrentFile != 0)
	{
		int nVaildFlag = m_nCurrentFile;

		for (int i = nVaildFlag; i < 0; i++)
		{
			m_lstGraphFiles[m_lstGraphFiles.count() + nVaildFlag]->m_bVaildFlag = false;
		}
	}


	if (m_lstGraphFiles.count() <= MAX_REDO_UNDO_TIMES)
	{
		if (m_nCurrentFile != 0)
		{
			*m_lstGraphFiles[m_lstGraphFiles.count() + m_nCurrentFile] = *pGraphFile;
			m_lstGraphFiles[m_lstGraphFiles.count() + m_nCurrentFile]->m_bVaildFlag = true;
		}
		else
		{
			CGraphFile* pHisotryFiles = new CGraphFile;
			//保持当前的历史数据
			*pHisotryFiles = *pGraphFile;
			m_lstGraphFiles.append(pHisotryFiles);

		}
	}
	else
	{
		if (m_nCurrentFile != 0)
		{
			*m_lstGraphFiles[m_lstGraphFiles.count() + m_nCurrentFile - 1] = *pGraphFile;
			m_lstGraphFiles[m_lstGraphFiles.count() + m_nCurrentFile - 1]->m_bVaildFlag = true;
		}
		else
		{
			//更新
			auto firstItem = m_lstGraphFiles.first();
			m_lstGraphFiles.removeFirst();
			m_lstGraphFiles.append(firstItem);

			//循环保存数据
			*firstItem = *pGraphFile;

		}
	}

	if (m_nCurrentFile != 0)
	{
		m_nCurrentFile++;
	}
}

void  CGraphView::OnCopy()
{
    m_pArrCpyCutWgt.clear();

    Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
    if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
        return;

    CGraphScene *pScene = GetGraphicsScene();
    Q_ASSERT(pScene);
    if (pScene == nullptr)
        return;

    CGraphFile * pGraphFile = pScene->GetGraphFile();
    Q_ASSERT(pGraphFile);
    if (pGraphFile == nullptr)
        return;

    auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
    Q_ASSERT(pLayer != nullptr);
    if (pLayer == nullptr)
        return;

    if (pLayer->IsSelectionEmpty() == true)
        return;

    //复制标签
    m_nCutCpyFlag = 1;
    
    m_pArrCpyCutWgt = pLayer->GetSelectedWidgets();

	for each (CBaseWidget * pBaseWgt in m_pArrCpyCutWgt)
	{
		pBaseWgt->SetMovingPosition(pBaseWgt->GetPosition());
	}

}

void  CGraphView::OnPaste()
{
    //创建对象
    Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
    if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
    {
        return;
    }

    CGraphScene *pScene = GetGraphicsScene();
    Q_ASSERT(pScene);
    if (pScene == nullptr)
    {
        return;
    }

    CGraphFile * pGraphFile = pScene->GetGraphFile();
    Q_ASSERT(pGraphFile);
    if (pGraphFile == nullptr)
    {
        return;
    }

    auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
    Q_ASSERT(pLayer != nullptr);
    if (pLayer == nullptr)
    {
        return;
    }

    if (m_pArrCpyCutWgt.empty())
    {
        return;
    }

    QPointF tpfStart = GetMutlStartPoint();

    for each (CBaseWidget * pBaseWgt in m_pArrCpyCutWgt)
    {
        QRectF tfNewRect(m_CurrentClickPoint.x() - tpfStart.x() + pBaseWgt->GetPosition().x(), m_CurrentClickPoint.y() - tpfStart.y() + pBaseWgt->GetPosition().y(), pBaseWgt->GetPosition().width(), pBaseWgt->GetPosition().height());

        CBaseWidget *pWidget = nullptr;
//         if (m_nCutCpyFlag == 1)
//         {
            //复制

        if (pBaseWgt->GetWidgetType() == CShapeWidget::WIDGET_GROUP)
        {
            //组合
            CGroupWidget *pGroupWgt = (CGroupWidget *)pBaseWgt;
            Q_ASSERT(pGroupWgt);

            if (pGroupWgt == nullptr)
            {
                return;
            }
            std::vector <CBaseWidget *> arrBaseWgt;
            
            for (int i = 0; i < pGroupWgt->GetWidgetCount();i++)
            {

				if (pGroupWgt->GetWidget(i)->GetWidgetType() == CShapeWidget::WIDGET_GROUP)
				{
					pWidget = CopyGroupInfo((CGroupWidget*)pGroupWgt->GetWidget(i));
				}
				else
				{
					pWidget = CWidgetFactory::CreateWidget(pGroupWgt->GetWidget(i)->GetPosition(), pGroupWgt->GetWidget(i)->GetWidgetType());

					Q_ASSERT(pWidget);

				}

				if (pWidget)
				{
					CopyBaseProperty(pWidget, pGroupWgt->GetWidget(i));
					CopyDynamicProperty(pWidget, pGroupWgt->GetWidget(i));
					//pWidget->SetRotateAngle(pGroupWgt->GetWidget(i)->GetRotateAngle());
					arrBaseWgt.push_back(pWidget);

					pGraphFile->RenameWidgetItem(m_nCurEditingLayerIndex, pWidget);
				}
            }
            QPointF delta = m_CurrentClickPoint - pBaseWgt->GetPosition().topLeft();

			CGroupWidget * pGroup = pLayer->GroupInputWidgets(arrBaseWgt, pGroupWgt->GetRotateAngle(), CGraphicsLayer::FATHER_GROUP);

			if (pGroup != nullptr)
			{
				pGraphFile->RenameWidgetItem(m_nCurEditingLayerIndex, pGroup);
				MoveWidgets(delta);
			}
        }
        else
        {
            pWidget = CWidgetFactory::CreateWidget(tfNewRect, pBaseWgt->GetWidgetType());

            Q_ASSERT(pWidget);
            if (pWidget)
            {
                //pWidget->SetRotateAngle(pBaseWgt->GetRotateAngle());

				CopyBaseProperty(pWidget, pBaseWgt);
				CopyDynamicProperty(pWidget, pBaseWgt);

                pLayer->SetCurrentWidget(pWidget);

                pLayer->Select(nullptr);
                pLayer->Select(pWidget);

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

						PloyLine->MoveTo(QRectF(tfNewRect.x(), tfNewRect.y(), PloyLine->GetPosition().width(), PloyLine->GetPosition().height()));
					}
				}
            }

            if (pWidget && pGraphFile)
            {
                pGraphFile->AddWidget(m_nCurEditingLayerIndex, pWidget);
            }

        }
            
            


//         }
//         else if (m_nCutCpyFlag == 2)
//         {
//             //剪切
//             pWidget = pBaseWgt;
//             pWidget->MoveTo(tfNewRect);
//             pWidget->setVisible(true);
// 
//             //m_nCutCpyFlag = 0;
// 
//         }
//         else
//         {
//             return;
//         }


    }

    if (m_nCutCpyFlag == 2)
    {
        //m_nCutCpyFlag = 0;
    }

    this->scene()->update();
}


//复制组
CGroupWidget *CGraphView::CopyGroupInfo(CGroupWidget* pGroupWgt)
{
	Q_ASSERT(pGroupWgt);
	if (pGroupWgt == nullptr)
	{
		return nullptr;
	}

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
	{
		return nullptr;
	}

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
	{
		return nullptr;
	}

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
	{
		return nullptr;
	}

	CGroupWidget *pGropWidget = new CGroupWidget;

	std::vector <CBaseWidget *> arrBaseWgt;

	CBaseWidget *pWidget = nullptr;

	for (int i = 0; i < pGroupWgt->GetWidgetCount(); i++)
	{

		if (pGroupWgt->GetWidget(i)->GetWidgetType() == CShapeWidget::WIDGET_GROUP)
		{
			pWidget = CopyGroupInfo((CGroupWidget*)pGroupWgt->GetWidget(i));
		}
		else
		{
			pWidget = CWidgetFactory::CreateWidget(pGroupWgt->GetWidget(i)->GetPosition(), pGroupWgt->GetWidget(i)->GetWidgetType());


		}

		Q_ASSERT(pWidget);
		if (pWidget)
		{
			CopyBaseProperty(pWidget, pGroupWgt->GetWidget(i));
			CopyDynamicProperty(pWidget, pGroupWgt->GetWidget(i));
			//pWidget->SetRotateAngle(pGroupWgt->GetWidget(i)->GetRotateAngle());
			arrBaseWgt.push_back(pWidget);

			pGraphFile->RenameWidgetItem(m_nCurEditingLayerIndex, pWidget);
		}
	}

	pLayer->CreateGroupWidgets(arrBaseWgt, pGroupWgt->GetRotateAngle(),pGropWidget);

	if (pGropWidget != nullptr)
	{
		pGraphFile->RenameWidgetItem(m_nCurEditingLayerIndex, pGropWidget);
	}

	return pGropWidget;

}

//复制基础属性
void CGraphView::CopyBaseProperty(CBaseWidget *pDescWgt, CBaseWidget *pSourceWgt)
{
// 	Q_ASSERT(pDescWgt);
// 	Q_ASSERT(pSourceWgt);
// 	if (pDescWgt == nullptr || pSourceWgt == nullptr)
// 	{
// 		return;
// 	}
	
	//旋转角度
	pDescWgt->SetRotateAngle(pSourceWgt->GetRotateAngle());
	pDescWgt->SetEndAngle(pSourceWgt->GetEndAngle());
	//pen属性
	pDescWgt->GetPenInfo()->SetColor(pSourceWgt->GetPenInfo()->GetColor());
	pDescWgt->GetPenInfo()->SetWidth(pSourceWgt->GetPenInfo()->GetWidth());
	pDescWgt->GetPenInfo()->SetStyle(pSourceWgt->GetPenInfo()->GetStyle());
	//brush属性
	pDescWgt->GetBrushInfo()->SetColor(pSourceWgt->GetBrushInfo()->GetColor());
	pDescWgt->GetBrushInfo()->SetStyle(pSourceWgt->GetBrushInfo()->GetStyle());
	pDescWgt->GetBrushInfo()->SetGradiendMode(pSourceWgt->GetBrushInfo()->GetGradiendMode());
	pDescWgt->GetBrushInfo()->SetEndColor(pSourceWgt->GetBrushInfo()->GetEndColor());

	//text属性
	pDescWgt->GetFontInfo()->SetColor(pSourceWgt->GetFontInfo()->GetColor());
	pDescWgt->GetFontInfo()->SetFont(pSourceWgt->GetFontInfo()->GetFont());
	pDescWgt->GetFontInfo()->SetFontLayout(pSourceWgt->GetFontInfo()->GetFontLayout());
	//
	pDescWgt->SetText(pSourceWgt->GetText());

	//背景图片属性
	pDescWgt->GetImageInfo()->SetBackgroundImage(pSourceWgt->GetImageInfo()->GetBackgroundImage());
	pDescWgt->GetImageInfo()->SetImageSize(pSourceWgt->GetImageInfo()->GetImageSize());
	pDescWgt->GetImageInfo()->SetImagePosition(pSourceWgt->GetImageInfo()->GetImagePosition());
	pDescWgt->GetImageInfo()->SetbTiling(pSourceWgt->GetImageInfo()->GetbTiling());
	pDescWgt->GetImageInfo()->SetStretch(pSourceWgt->GetImageInfo()->GetStretch());
	//

}


void CGraphView::CopyDynamicProperty(CBaseWidget *pDescWgt, CBaseWidget *pSourceWgt)
{
	pDescWgt->CopyDynamicPro(pSourceWgt->CreateDynamicPro());
}

void CGraphView::InsertGroupMovePoints()
{

	if (m_pGroupMoveLine == nullptr)
	{
		if (m_pPorpWidget != nullptr)
		{

			CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_GROUP_MOVE);
			CDyncGroupMovingData *pGroupMoveData = dynamic_cast<CDyncGroupMovingData*>(pbaseDync);

			Q_ASSERT(pGroupMoveData);
			if (pGroupMoveData == nullptr)
			{
				return;
			}

			if (pGroupMoveData->GetPoints().count() > 0)
			{
				m_pGroupMoveLine = new CGroupMoveLine;
				m_pGroupMoveLine->setZValue(2);

				CGraphScene *pScene = GetGraphicsScene();
				Q_ASSERT(pScene);
				if (pScene == nullptr)
					return;

				pScene->addItem(m_pGroupMoveLine);

				for each (QPointF var in pGroupMoveData->GetPoints())
				{
					m_pGroupMoveLine->InsetNewPoint(var + m_pPorpWidget->GetRotationCenterPoint());
				}

				this->scene()->update();

			}
			
		}
	}
}

void CGraphView::DeleteGroupMovePoints()
{
	if (m_pGroupMoveLine != nullptr)
	{
		delete m_pGroupMoveLine;

		m_pGroupMoveLine = nullptr;
	}
	this->scene()->update();
}

//设置图元属性隐藏
void CGraphView::SetGraphicItemAttrHidden()
{
	if (m_pTreeProperty == nullptr)
	{
		return;
	}

	//隐藏style属性
	for each (QtBrowserItem* var in m_pTreeProperty->items(m_mapIdToProperty[ITEM_CURVEPLOT_STYLE]))
	{
		m_pTreeProperty->setItemVisible(var, false);
	}

	//隐藏基本属性
	for each (QtBrowserItem* var in m_pTreeProperty->items(m_mapIdToProperty[ITEM_BASE_ID_NAME]))
	{
		m_pTreeProperty->setItemVisible(var, false);
	}

	//隐藏静态属性
	for each (QtBrowserItem* var in m_pTreeProperty->items(m_mapIdToProperty[ITEM_BASE_PRO]))
	{
		m_pTreeProperty->setItemVisible(var, false);
	}

	//隐藏动态属性
	for each (QtBrowserItem* var in m_pTreeProperty->items(m_mapIdToProperty[ITEM_DYNAMC_ATTR]))
	{
		m_pTreeProperty->setItemVisible(var, false);
	}


	//显示视图属性
	for each (QtBrowserItem* var in m_pTreeProperty->items(m_mapIdToProperty[ITEM_GRAPHICS_BASE]))
	{
		m_pTreeProperty->setItemVisible(var, true);
	}

}

//设置视图属性隐藏
void CGraphView::SetGraphicAttrHidden()
{
	Q_ASSERT(m_pTreeProperty);

	if (m_pTreeProperty == nullptr)
	{
		return;
	}

	Q_ASSERT(m_pTreeProperty);

	if (m_pTreeProperty == nullptr)
	{
		return;
	}

	//显示图元基本属性
	for each (QtBrowserItem* var in m_pTreeProperty->items(m_mapIdToProperty[ITEM_BASE_ID_NAME]))
	{
		m_pTreeProperty->setItemVisible(var, true);
	}

	//显示图元静态属性
	for each (QtBrowserItem* var in m_pTreeProperty->items(m_mapIdToProperty[ITEM_BASE_PRO]))
	{
		//一般属性全部显示
		m_pTreeProperty->setItemVisible(var, true);
	}

	for (auto item : m_pTreeProperty->items(m_mapIdToProperty[ITEM_END_ANGLE]))
	{

		if (m_pPorpWidget->GetWidgetType() == DRAW_TOOLS_CAMBER || m_pPorpWidget->GetWidgetType() == DRAW_TOOLS_PIE || m_pPorpWidget->GetWidgetType() == DRAW_TOOLS_CHORD)
		{
			m_pTreeProperty->setItemVisible(item, true);
		}
		else
		{
			m_pTreeProperty->setItemVisible(item, false);
		}
		//
	}

	//显示图元动态属性
	for each (QtBrowserItem* var in m_pTreeProperty->items(m_mapIdToProperty[ITEM_DYNAMC_ATTR]))
	{
		m_pTreeProperty->setItemVisible(var, true);
	}

	//隐藏视图属性
	for each (QtBrowserItem* var in m_pTreeProperty->items(m_mapIdToProperty[ITEM_GRAPHICS_BASE]))
	{
		m_pTreeProperty->setItemVisible(var, false);
	}

	//根据类型选择
	if (m_pPorpWidget != nullptr)
	{
		if (m_pPorpWidget->GetWidgetType() == DRAW_ADDR_CURE_PLOT)
		{
			//曲线图
			for (auto item : m_pTreeProperty->items(m_mapIdToProperty[ITEM_CURVEPLOT_STYLE]))
			{
				m_pTreeProperty->setItemVisible(item, true);
			}
		
		}
		else
		{
			//非曲线图
			for (auto item : m_pTreeProperty->items(m_mapIdToProperty[ITEM_CURVEPLOT_STYLE]))
			{
				m_pTreeProperty->setItemVisible(item, false);
			}
		}


		//按钮
// 		if (m_pPorpWidget->GetWidgetType()>= DRAW_TOOLS_BTN_NORMAL && m_pPorpWidget->GetWidgetType() <= DRAW_TOOLS_BTN_RED_LED)
// 		{
// 			//
// 			for (auto item : m_pTreeProperty->items(m_mapIdToProperty[ITEM_PHSUBBUTTON_EXEC]))
// 			{
// 				m_pTreeProperty->setItemVisible(item, true);
// 			}
// 
// 		}
// 		else
// 		{
// 			//
// 			for (auto item : m_pTreeProperty->items(m_mapIdToProperty[ITEM_PHSUBBUTTON_EXEC]))
// 			{
// 				m_pTreeProperty->setItemVisible(item, false);
// 			}
// 		}
	}


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

	CGraphFile * pGraphFile = pScene->GetGraphFile();
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
    m_pArrCpyCutWgt.clear();
    Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
    if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
        return;

    CGraphScene *pScene = GetGraphicsScene();
    Q_ASSERT(pScene);
    if (pScene == nullptr)
        return;

    CGraphFile * pGraphFile = pScene->GetGraphFile();
    Q_ASSERT(pGraphFile);
    if (pGraphFile == nullptr)
        return;

    auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
    Q_ASSERT(pLayer != nullptr);
    if (pLayer == nullptr)
        return;

    if (pLayer->IsSelectionEmpty() == true)
        return;

    //剪切标签
    m_nCutCpyFlag = 2;

    m_pArrCpyCutWgt = pLayer->GetSelectedWidgets();

    

    for each (CBaseWidget * pBaseWgt in m_pArrCpyCutWgt)
    {
        pBaseWgt->setVisible(false);
    }

    this->scene()->update();
}
#if 0
/*! \fn void CGraphView::SelectWidgetsInRect(const QRectF &rcSel)
********************************************************************************************************* 
** \brief CGraphView::SelectWidgetsInRect 
** \details 选择矩形框内的图元
** \param rcSel 
** \return void 
** \author LiJin 
** \date 2016年1月21日 
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

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	// 清空 
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
	// 需要判断起点是矩形的哪一个角
	if (ptCur.x() > m_ptFisrst.x() && ptCur.y() > m_ptFisrst.y())
	{// cur 在 fist 的右下角
		rcSel.setTopLeft(m_ptFisrst);
	//	rcSel.setBottomRight(m_ptLast);
		rcSel.setBottomRight(ptCur);
	}
	else if (ptCur.x() < m_ptFisrst.x() && ptCur.y() > m_ptFisrst.y())
	{// cur 在 fist 的左下角
		rcSel.setTopRight(m_ptFisrst);
	//	rcSel.setBottomLeft(m_ptLast);
		rcSel.setBottomLeft(ptCur);
	}
	else if (ptCur.x() > m_ptFisrst.x() && ptCur.y() < m_ptFisrst.y())
	{// cur 在 fist 的右上角
		rcSel.setBottomLeft(m_ptFisrst);
	//	rcSel.setTopLeft(m_ptLast);
		rcSel.setTopRight(ptCur);
	}
	else if (ptCur.x() < m_ptFisrst.x() && ptCur.y() < m_ptFisrst.y())
	{// cur 在 fist 的左上角
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
** \details 调整图元的大小尺寸
** \param delta 
** \return void 
** \author LiJin 
** \date 2016年2月6日 
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
** \details 图层显示控制
** \param nLayerIdx 
** \param bDisp 
** \return unsigned int 
** \author LiJin
** \date 2016年1月23日 
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

	CGraphFile * pGraphFile = pScene->GetGraphFile();
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
** \details 设置当前编辑的图层
** \param nLayerIdx 
** \return void 
** \author LiJin
** \date 2016年2月1日 
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

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	// 如果该图层不可见，那么不可修改
	if (nLayerIdx != m_nCurEditingLayerIndex)
	{// 如果图层发生改变

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

	if (pLayer->GetSelectedWidgets().size()<=1)
	{
		//少于等一一个不组合
		return;
	}

	//bool bRet = pLayer->GroupWidgets();



	CGroupWidget *pGroup = new CGroupWidget;

	pGraphFile->AddWidget(m_nCurEditingLayerIndex, pGroup);
// 	if (bRet)
// 	{
// 		invalidateScene();
// 	}
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

	this->viewport()->update();
}

void CGraphView::OnSaveTemplate()
{

	if (m_pCreateTemplateWgt == nullptr)
	{
		m_pCreateTemplateWgt = new CCreateNewTemplateWgt;
	}

	m_pCreateTemplateWgt->InitData();

	m_pCreateTemplateWgt->exec();

	if (m_pCreateTemplateWgt->GetLibraryName().isEmpty())
	{
		return;
	}

	//创建默认目录
	QString strRunPath = qApp->applicationDirPath();
	QDir tdir;
	tdir.mkdir(strRunPath + "/gtemplate/");
	tdir.mkdir(strRunPath + "/gtemplate/library/");
	//保存模板
	QString strFileName = strRunPath + "/gtemplate/library/" + m_pCreateTemplateWgt->GetLibraryName();

	if (strFileName != nullptr)
	{
		Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
		if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
			return;

		CGraphScene *pScene = GetGraphicsScene();
		Q_ASSERT(pScene);
		if (pScene == nullptr)
			return;

		CGraphFile * pGraphFile = pScene->GetGraphFile();
		Q_ASSERT(pGraphFile);
		if (pGraphFile == nullptr)
			return;

		auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
		Q_ASSERT(pLayer != nullptr);
		if (pLayer == nullptr)
			return;

		if (pLayer->IsSelectionEmpty() == true)
			return;

		if (pLayer->GetSelectedWidgets().size() > 1)
		{
			return;
		}

		QFile file(strFileName);

		if (!file.open(QFile::WriteOnly | QFile::Text))
		{
			qDebug() << "Error: Cannot write file: " << file.errorString();
			return;
		}

		std::shared_ptr<QXmlStreamWriter> pXmlWriter = std::make_shared<QXmlStreamWriter>(&file);

		pXmlWriter->setAutoFormatting(true);
		pXmlWriter->writeStartDocument();

		pXmlWriter->writeStartElement("Template");


		for each (CBaseWidget * pBaseWgt in pLayer->GetSelectedWidgets())
		{
			pBaseWgt->SaveXml(pXmlWriter);
		}

		pXmlWriter->writeEndElement();
		file.close();

		//库文件
		GetModuleApi()->GetToolWgt()->AnalyseTemplate(strRunPath + "/gtemplate/tlibrarymanger");
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

	this->scene()->update();
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

	this->scene()->update();
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

	this->scene()->update();
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

	this->scene()->update();
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

	this->scene()->update();
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

	this->scene()->update();
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

	this->scene()->update();
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

	CGraphFile * pGraphFile = pScene->GetGraphFile();
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

	CGraphFile * pGraphFile = pScene->GetGraphFile();
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

	CGraphFile * pGraphFile = pScene->GetGraphFile();
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
** \details 下移一层
** \return void
** \author LiJin
** \date 2016年1月22日
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

	CGraphFile * pGraphFile = pScene->GetGraphFile();
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

//图层上移
void CGraphView::OnLayerUp()
{
	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	pGraphFile->UploadItemLayer(m_nCurEditingLayerIndex);
}

//图层下移
void CGraphView::OnLayerDown()
{
	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	pGraphFile->DownItemLayer(m_nCurEditingLayerIndex);

}

//导出
void CGraphView::OnExport()
{
	QString strFilename = QFileDialog::getSaveFileName(nullptr,"","","Image Files(*.jpg *.png *.bmp *.gif)");
	if (strFilename.isEmpty())
	{
		return;
	}

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;


	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	QImage tImage(QSize(pGraphFile->GetSize().second, pGraphFile->GetSize().first), QImage::Format_RGB32);
	QPainter tPainter(&tImage);
	tPainter.setRenderHint(QPainter::Antialiasing);
	pScene->render(&tPainter, pGraphFile->GetBackgroundRect(), pGraphFile->GetBackgroundRect(), Qt::KeepAspectRatio);
	tImage.save(strFilename);



// 	QPixmap tPix = QPixmap::grabWidget(this, pGraphFile->GetGraphRect().toRect());
// 
// 	if (tPix.save(strFilename))
// 	{
// 		QMessageBox::information(0,tr("Success"), tr("Save Sucess!"));
// 	}
// 	else
// 	{
// 		QMessageBox::warning(0, tr("Error"), tr("Save Error!"));
// 	}
	
}

void CGraphView::ClearPropertyBrowser()
{
	IMainWindow *pMainFrm = GetMainWindow();
	Q_ASSERT(pMainFrm);
	if (pMainFrm == nullptr)
		return;

	QtTreePropertyBrowser *pTreeBrowser = pMainFrm->GetAtrri()->GetTreePropertyBrowser();
 
	QList<QtProperty *> listProperty = pTreeBrowser->properties();

	if (listProperty.empty() == false)
	{
		for (auto it : listProperty)
		{
			pTreeBrowser->removeProperty(it);
		}
	}
	listProperty.clear();
}

void CGraphView::ClearPropertyBrowser(QtTreePropertyBrowser *pTreeBrowser)
{
	Q_ASSERT(pTreeBrowser);
	if (pTreeBrowser == nullptr)
		return;
	QList<QtProperty *> listProperty = pTreeBrowser->properties();

	if (listProperty.empty() == false)
	{
		for (auto it : listProperty)
		{
			pTreeBrowser->removeProperty(it);
		}
	}
	listProperty.clear();	
}

void  CGraphView::OnAttr()
{
	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;
	
	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

// 	if (pLayer->IsSelectionEmpty() == true)
// 		return;

	CBaseWidget *pWidget = pLayer->GetFirstSelectedWidget();
// 	Q_ASSERT(pWidget);
// 	if (pWidget == nullptr)
// 		return;

	IMainWindow *pMainFrm = GetMainWindow();
	Q_ASSERT(pMainFrm);
	if (pMainFrm == nullptr)
		return;

	if (m_pTreeProperty == nullptr)
	{
		m_pTreeProperty = pMainFrm->GetAtrri()->GetTreePropertyBrowser();
	}

	m_pPorpWidget = pWidget;

		//
	if (m_pTreeProperty->properties().count() == 0)
	{
		ShowPropertyBrowser(pWidget, m_pTreeProperty);
	}
	

	//QtTreePropertyBrowser *pTreeBrowser = pMainFrm->GetAtrri()->GetTreePropertyBrowser();
	//QtGroupPropertyManager *pGroup = pMainFrm->GetAtrri()->GetGroupProperty();
	
	//ClearPropertyBrowser(m_pTreeProperty);
	//ClearPropMap();

	

    pMainFrm->GetRightDock()->setVisible(true);
    //pMainFrm->GetRightDock()->raise();

// 	QStringList labels;
// 	labels.append(QCoreApplication::translate("QtTreePropertyBrowser", "Property"));
// 	labels.append(QCoreApplication::translate("QtTreePropertyBrowser", "Value"));
// 	pTree->setHeaderLabels(labels);
	
// 	QString strTmp = QStringLiteral("静态属性");
// 	QtProperty *pStaticAttr = pGroup->addProperty(strTmp);

//	pTreeBrowser->addProperty(pStaticAttr);
}

void CGraphView::OnRotate()
{
    Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
    if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
        return;

    CGraphScene *pScene = GetGraphicsScene();
    Q_ASSERT(pScene);
    if (pScene == nullptr)
        return;

    CGraphFile * pGraphFile = pScene->GetGraphFile();
    Q_ASSERT(pGraphFile);
    if (pGraphFile == nullptr)
        return;

    auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
    Q_ASSERT(pLayer != nullptr);
    if (pLayer == nullptr)
        return;

    if (pLayer->IsSelectionEmpty() == true)
        return;

    CBaseWidget *pWidget = pLayer->GetFirstSelectedWidget();
    Q_ASSERT(pWidget);
    if (pWidget == nullptr)
        return;


    pWidget->SetRotate(true);
    
    

}

void CGraphView::OnSizeChange()
{
    Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
    if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
        return;

    CGraphScene *pScene = GetGraphicsScene();
    Q_ASSERT(pScene);
    if (pScene == nullptr)
        return;

    CGraphFile * pGraphFile = pScene->GetGraphFile();
    Q_ASSERT(pGraphFile);
    if (pGraphFile == nullptr)
        return;

    auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
    Q_ASSERT(pLayer != nullptr);
    if (pLayer == nullptr)
        return;

    if (pLayer->IsSelectionEmpty() == true)
        return;

    CBaseWidget *pWidget = pLayer->GetFirstSelectedWidget();
    Q_ASSERT(pWidget);
    if (pWidget == nullptr)
        return;

    pWidget->SetRotate(false);
}


#include <QPushButton>
void CGraphView::ShowPropertyBrowser(CBaseWidget *pWidget, QtTreePropertyBrowser *pTreeBrowser)
{
// 	Q_ASSERT(pWidget);
// 	if (pWidget == nullptr)
// 		return;

	Q_ASSERT(pTreeBrowser);
	if (pTreeBrowser == nullptr)
		return;

	InitGrpahicAttr(pTreeBrowser);

	m_pPorpWidget = pWidget;
//	pTreeBrowser->setFactoryForManager(boolManager, checkBoxFactory);
	pTreeBrowser->setFactoryForManager(m_pDoubleManager, m_pDoubleSpinBoxFactory);
	pTreeBrowser->setFactoryForManager(m_pIntManager, m_pSpinBoxFactory);
	pTreeBrowser->setFactoryForManager(m_pStringManager, m_pLineEditFactory);
	pTreeBrowser->setFactoryForManager(m_pSizeManager->subIntPropertyManager(), m_pSpinBoxFactory);
	pTreeBrowser->setFactoryForManager(m_pPointManager->subIntPropertyManager(), m_pSpinBoxFactory);
	pTreeBrowser->setFactoryForManager(m_pColorManager, m_pVariantFactory);
	pTreeBrowser->setFactoryForManager((QtVariantPropertyManager  *)m_pBindInfoManager,m_pBindInfoFactory);

	//pTreeBrowser->setFactoryForManager(m_pBindInfoManager, m_pBindInfoFactory);

	


	pTreeBrowser->setFactoryForManager(m_pComBoProperty, m_pComboBoxFactory);
	pTreeBrowser->setFactoryForManager(m_pBoolManager, m_pCheckBoxFactory);

	//pTreeBrowser->setResizeMode(QtTreePropertyBrowser::ResizeToContents);
	pTreeBrowser->setSplitterPosition(150);

	QRect rcLimit = QRect(0, 0, 50000, 50000);

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene)
	{
		CGraphFile *pFile = pScene->GetGraphFile();
		Q_ASSERT(pFile);
		if (pFile)
		{
			rcLimit = pFile->GetGraphRect().toRect();
		}
	}

	//视图基本属性

	//end
	
	QtProperty *pBaseItem = m_pGroupManager->addProperty(QStringLiteral("Base"));
	pTreeBrowser->addProperty(pBaseItem);
	AddPropertyToMap(pBaseItem, ITEM_BASE_ID_NAME);

	QtProperty *pWidgetNameItem = m_pStringManager->addProperty(QStringLiteral("Name"));
	//m_pStringManager->setValue(pWidgetNameItem, QString::fromLocal8Bit(pWidget->GetWidgetName().c_str()));
	m_pStringManager->setReadOnly(pWidgetNameItem, true);
	pBaseItem->addSubProperty(pWidgetNameItem);
	AddPropertyToMap(pWidgetNameItem, ITEM_NAME);

	QtProperty *pWidgetIDItem = m_pIntManager->addProperty(QStringLiteral("ID"));
	//m_pIntManager->setValue(pWidgetIDItem, pWidget->GetID());
	m_pIntManager->setReadOnly(pWidgetIDItem, true);
	pBaseItem->addSubProperty(pWidgetIDItem);
	AddPropertyToMap(pWidgetIDItem, ITEM_ID);

	//图表属性
	QtProperty *pCurvePlotItem = m_pGroupManager->addProperty(QStringLiteral("CurvePlotStyle"));
	pTreeBrowser->addProperty(pCurvePlotItem);
	AddPropertyToMap(pCurvePlotItem, ITEM_CURVEPLOT_STYLE);

	//边框宽度 
	QtProperty *pInterWidthItem = m_pIntManager->addProperty(tr("IntervalWidth"));
	//m_pIntManager->setValue(pLineWidthItem, pInfo->GetWidth());
	m_pIntManager->setMinimum(pInterWidthItem, 0);
	pCurvePlotItem->addSubProperty(pInterWidthItem);
	AddPropertyToMap(pInterWidthItem, ITEM_CURVEFRAM_WIDTH);


	//brush样式
	QtProperty *CurvePlotBrushStyleItem = m_pComBoProperty->addProperty(tr("BrushStyle"));
	
	QStringList lstCurvePlotBrushStyleTyps;
	lstCurvePlotBrushStyleTyps << tr("NoBrush") << tr("SolidPattern") << tr("Dense1Pattern")
		<< tr("Dense2Pattern") << tr("Dense3Pattern") << tr("Dense4Pattern") << tr("Dense5Pattern")
		<< tr("Dense6Pattern") << tr("Dense7Pattern") << tr("HorPattern") << tr("VerPattern")
		<< tr("CrossPattern") << tr("BDiagPattern") << tr("FDiagPattern") << tr("DiagCrossPattern");

	m_pComBoProperty->setEnumNames(CurvePlotBrushStyleItem, lstCurvePlotBrushStyleTyps);
	m_pComBoProperty->setValue(CurvePlotBrushStyleItem, 1);
	pCurvePlotItem->addSubProperty(CurvePlotBrushStyleItem);
	AddPropertyToMap(CurvePlotBrushStyleItem, ITEM_CUREVEPLOT_BRUSH_STYLE);

	//背景颜色  颜色属性
	QtProperty *pCurvePlotBrushColorItem = m_pColorManager->addProperty(QVariant::Color, QStringLiteral("Color"));
	//m_pColorManager->setValue(pBrushColorItem, pInfo->GetColor());
	pCurvePlotItem->addSubProperty(pCurvePlotBrushColorItem);
	AddPropertyToMap(pCurvePlotBrushColorItem, ITEM_CURVEPLOT_BURSH_COLOR);
	// 图表属性 end

	//pushbutton执行属性
	QtProperty *pPushBtnExecItem = m_pGroupManager->addProperty(QStringLiteral("Exec"));
	pTreeBrowser->addProperty(pPushBtnExecItem);
	AddPropertyToMap(pPushBtnExecItem, ITEM_PHSUBBUTTON_EXEC);

	//绑定
	QtProperty *pPushbuttonBind = m_pBindInfoManager->addProperty(ITEM_BIND_VAR, ("Variable"));
	pPushBtnExecItem->addSubProperty(pPushbuttonBind);
	AddPropertyToMap(pPushbuttonBind, TTEM_PUSHBUTTON_VAR);

	//pushtutton命令类型
	QtProperty *PushButtonOrderTypeItem = m_pComBoProperty->addProperty(tr("OrderType"));

	QStringList lstPushbuttonOrderTyps;
	lstPushbuttonOrderTyps << tr("ExecOrder") << tr("ON-OFF") << tr("Pulse");

	m_pComBoProperty->setEnumNames(PushButtonOrderTypeItem, lstPushbuttonOrderTyps);
	m_pComBoProperty->setValue(PushButtonOrderTypeItem, 0);
	pPushBtnExecItem->addSubProperty(PushButtonOrderTypeItem);
	AddPropertyToMap(PushButtonOrderTypeItem, ITEM_PUSHBUTTON_TYPE);


	//鼠标press
	QtProperty *pPushbuttonPressBind = m_pBindInfoManager->addProperty(ITEM_PUISHBUTTON_ORDER_CODE, ("PressOrder"));
	pPushBtnExecItem->addSubProperty(pPushbuttonPressBind);
	AddPropertyToMap(pPushbuttonPressBind, ITEM_PUSHBUTTON_PRESSORDER);

	//鼠标release
	QtProperty *pPushbuttonReleaseBind = m_pBindInfoManager->addProperty(ITEM_PUISHBUTTON_ORDER_CODE, ("ReleaseOrder"));
	pPushBtnExecItem->addSubProperty(pPushbuttonReleaseBind);
	AddPropertyToMap(pPushbuttonBind, ITEM_PUSHBUTTON_RELEASEORDER);
	//pushbutton执行属性  end

	QtProperty *pStaticItem = m_pGroupManager->addProperty(QStringLiteral("StaticProperty"));
	pTreeBrowser->addProperty(pStaticItem);
	AddPropertyToMap(pStaticItem, ITEM_BASE_PRO);

	//锁定
	QtProperty *pLockItem = m_pBoolManager->addProperty(tr("Lock"));
	m_pBoolManager->setTextVisible(pLockItem, false);
	AddPropertyToMap(pLockItem,ITEM_LOCK);
	pStaticItem->addSubProperty(pLockItem);

	//位置属性
	QtProperty *pPosItem = m_pPointManager->addProperty( QStringLiteral("Position"));
	//m_pPointManager->setValue(pPosItem, pWidget->GetPosition().topLeft().toPoint());
	AddPropertyToMap(pPosItem, ITEM_PSOITION);

	pStaticItem->addSubProperty(pPosItem);

	QtProperty *pSizeItem = m_pSizeManager->addProperty( QStringLiteral("Size"));
	//m_pSizeManager->setValue(pSizeItem, pWidget->GetPosition().size().toSize() );
	m_pSizeManager->setMaximum(pSizeItem, rcLimit.size());
	pStaticItem->addSubProperty(pSizeItem);
	AddPropertyToMap(pSizeItem, ITEM_SIZE);

	//旋转中心
	QtProperty *pRotateCenterItem = m_pComBoProperty->addProperty(QStringLiteral("RotateCenter"));
	//m_pDoubleManager->setValue(pRotateItem, pWidget->GetRotateAngle() );
	QStringList lstLIneTyps;
	lstLIneTyps << tr("mid_center") << tr("top_left") << tr("top_mid") << tr("top_right")
		<< tr("left_mid")  << tr("right_mid") << tr("left_bottom")
		<< tr("bottom_mid") << tr("right_bottom");

	m_pComBoProperty->setEnumNames(pRotateCenterItem, lstLIneTyps);
	m_pComBoProperty->setValue(pRotateCenterItem, 4);
	AddPropertyToMap(pRotateCenterItem, ITEM_ROTATE_CENTER);

	pStaticItem->addSubProperty(pRotateCenterItem);

	QtProperty *pRotateItem = m_pDoubleManager->addProperty(QStringLiteral("Rotate"));
	//m_pDoubleManager->setValue(pRotateItem, pWidget->GetRotateAngle() );
	m_pDoubleManager->setMaximum(pRotateItem, 360);
	m_pDoubleManager->setMinimum(pRotateItem, -360);
	AddPropertyToMap(pRotateItem, ITEM_ROTATE);

	pStaticItem->addSubProperty(pRotateItem);
	
	//终止角度
	QtProperty *pEndAngleItem = m_pDoubleManager->addProperty(QStringLiteral("EndAngle"));
	//m_pDoubleManager->setValue(pRotateItem, pWidget->GetRotateAngle() );
	m_pDoubleManager->setMaximum(pEndAngleItem, 360);
	m_pDoubleManager->setMinimum(pEndAngleItem, 0);
	AddPropertyToMap(pEndAngleItem, ITEM_END_ANGLE);

	pStaticItem->addSubProperty(pEndAngleItem);

	// 画笔---线条
	QtProperty *pLineItem = m_pStringManager->addProperty(QStringLiteral("Pen"));
	m_pStringManager->setReadOnly(pLineItem, true);
	

	AddPropertyToMap(pLineItem, ITEM_PEN);
	pStaticItem->addSubProperty(pLineItem);
	ShowPenProperty(pLineItem, nullptr);

	// 画刷---填充
	QtProperty *pBrushItem = m_pStringManager->addProperty(QStringLiteral("Brush"));
	m_pStringManager->setReadOnly(pBrushItem, true);

	pStaticItem->addSubProperty(pBrushItem);
	ShowBrushProperty(pBrushItem, nullptr);
	AddPropertyToMap(pBrushItem, ITEM_BRUSH);
	// 字体
	QtProperty *pTxtItem = m_pStringManager->addProperty(QStringLiteral("Text"));
	m_pStringManager->setReadOnly(pTxtItem, true);

	pStaticItem->addSubProperty(pTxtItem);
	ShowTextProperty(pTxtItem, QString(),nullptr);

	//填充图片
	QtProperty *pImageItem = m_pStringManager->addProperty(QStringLiteral("Image"));
	m_pStringManager->setReadOnly(pImageItem, true);

	pStaticItem->addSubProperty(pImageItem);
	AddPropertyToMap(pImageItem, ITEM_BACKGROUND_ITEM_IAMGE);
	ShowItemImageProperty(pImageItem);

	 
	//动态属性
	QtProperty *pDynamicItem = m_pGroupManager->addProperty(QStringLiteral("Dynamic"));
	AddPropertyToMap(pDynamicItem, ITEM_DYNAMC_ATTR);
	pTreeBrowser->addProperty(pDynamicItem);

	//动态颜色变化
	QtProperty *pDynamicColor = m_pStringManager->addProperty(QStringLiteral("DynamicColor"));
	pDynamicItem->addSubProperty(pDynamicColor);

	//动态pen
	QtProperty *pDynamicPenItem = m_pStringManager->addProperty(QStringLiteral("PenColor"));
	m_pStringManager->setReadOnly(pDynamicPenItem, true);
	pDynamicColor->addSubProperty(pDynamicPenItem);
	//创建pen二层结构
	ShowDynamicColorProperty(pDynamicPenItem, CBaseDyncData::DYNC_PEN_COLOR);

	//动态画刷
	QtProperty *pDynamicBrushItem = m_pStringManager->addProperty(QStringLiteral("BrushColor"));
	m_pStringManager->setReadOnly(pDynamicBrushItem, true);
	pDynamicColor->addSubProperty(pDynamicBrushItem);
	//动态画刷二层结构
    ShowDynamicColorProperty(pDynamicBrushItem, CBaseDyncData::DYNC_FILLING);

	//动态字体颜色
	QtProperty *pTextColorItem = m_pStringManager->addProperty(QStringLiteral("TextColor"));
	m_pStringManager->setReadOnly(pTextColorItem, true);
	pDynamicColor->addSubProperty(pTextColorItem);
	//动态字体颜色二层结构
	ShowDynamicColorProperty(pTextColorItem, CBaseDyncData::DYNC_TEXT_COLOR);

	//动态移动
	QtProperty *pDynamicMoving = m_pStringManager->addProperty(QStringLiteral("Moving"));
	pDynamicItem->addSubProperty(pDynamicMoving);

	//水平移动
	QtProperty *pHorMovingItem = m_pStringManager->addProperty(QStringLiteral("HorizontalMoving"));
	m_pStringManager->setReadOnly(pHorMovingItem, true);
	pDynamicMoving->addSubProperty(pHorMovingItem);
	//水平移动二层结构二层结构
	ShowDynamicMovintProperty(pHorMovingItem, CBaseDyncData::DYNC_HOR_MOVE);

	//竖直移动
	QtProperty *pVerMovingItem = m_pStringManager->addProperty(QStringLiteral("VerticalMoving"));
	m_pStringManager->setReadOnly(pVerMovingItem, true);
	pDynamicMoving->addSubProperty(pVerMovingItem);
	//水平移动二层结构二层结构
	ShowDynamicMovintProperty(pVerMovingItem, CBaseDyncData::DYNC_VIR_MOVE);

	//组合移动
	QtProperty *pGroupMovingItem = m_pStringManager->addProperty(QStringLiteral("ComposedMovement"));
	m_pStringManager->setReadOnly(pGroupMovingItem, true);
	pDynamicMoving->addSubProperty(pGroupMovingItem);
	//水平移动二层结构二层结构
	ShowDynamicGroupMovingProperty(pGroupMovingItem, CBaseDyncData::DYNC_GROUP_MOVE);

	//旋转
	QtProperty *pDyRotateItem = m_pStringManager->addProperty(QStringLiteral("Rotate"));
	m_pStringManager->setReadOnly(pDyRotateItem, true);
	pDynamicItem->addSubProperty(pDyRotateItem);
	//水平移动二层结构二层结构
	ShowDynamicMovintProperty(pDyRotateItem, CBaseDyncData::DYNC_ROTATION);

	//动态透明
	QtProperty *pTransparencyItem = m_pStringManager->addProperty(tr("Transparency"));
	m_pStringManager->setReadOnly(pTransparencyItem, true);
	pDynamicItem->addSubProperty(pTransparencyItem);
	//动态特命二层结构
	ShowDynamicTransparencyProperty(pTransparencyItem, CBaseDyncData::DYNC_TRANSPARENCY);

	//动态可见
	QtProperty *pDyncVisable = m_pStringManager->addProperty(tr("Visible"));
	m_pStringManager->setReadOnly(pDyncVisable,true);
	pDynamicItem->addSubProperty(pDyncVisable);
	//动态可见二层结构
	ShowDynamicVisableProperty(pDyncVisable, CBaseDyncData::DYNC_VISIBLE);

	//动态数值
	QtProperty *pDyncValue = m_pStringManager->addProperty(tr("DynamicValue"));
	m_pStringManager->setReadOnly(pDyncValue, true);
	pDynamicItem->addSubProperty(pDyncValue);
	//动态数值二层结构
	ShowDynamicValueProperty(pDyncValue, CBaseDyncData::DYNC_VALUE);

	//动态文字
	QtProperty *pDyncText = m_pStringManager->addProperty(tr("DynamicText"));
	m_pStringManager->setReadOnly(pDyncText, true);
	pDynamicItem->addSubProperty(pDyncText);
	//动态文本二层结构
	ShowDynamicTextProperty(pDyncText, CBaseDyncData::DYNC_STRING);

	//动态图片
	QtProperty *pDyncMap = m_pStringManager->addProperty(tr("DynamicImage"));
	m_pStringManager->setReadOnly(pDyncMap, true);
	pDynamicItem->addSubProperty(pDyncMap);
	//动态图片二层结构
	ShowDynamicMapProperty(pDyncMap, CBaseDyncData::DYNC_MAP);

	AddPropertyToMap(pDynamicPenItem, ITEM_DYNAMIC_PEN_COLOR);

	AddPropertyToMap(pDynamicBrushItem, ITEM_DYNAMIC_BRUSH);

	AddPropertyToMap(pTextColorItem, ITEM_DYNAMIC_TEXT_COLOR);

	AddPropertyToMap(pHorMovingItem, ITEM_DYNAMIC_HORIZONTAL);

	AddPropertyToMap(pVerMovingItem, ITEM_DYNAMIC_VERTICAL);
	//组合移动
	AddPropertyToMap(pGroupMovingItem, ITEM_DYNAMIC_GROUP_MOVING);

	AddPropertyToMap(pDyRotateItem, ITEM_DYNAMIC_ROTATE);
	//透明
	AddPropertyToMap(pTransparencyItem, ITEM_DYNAMIC_TRANSPARENCY);
	//可见
	AddPropertyToMap(pDyncVisable, ITEM_DYNAMIC_VISIBLE);
	//动态数值
	AddPropertyToMap(pDyncValue, ITEM_DYNAMIC_VALUE);
	//动态文本
	AddPropertyToMap(pDyncText, ITEM_DYNAMIC_TEXT);
	//动态图片
	AddPropertyToMap(pDyncMap, ITEM_DYNAMIC_MAP);

#if 0
	QtProperty *pStaticItem = m_pVariantManager->addProperty(QtVariantPropertyManager::groupTypeId(), QStringLiteral("静态属性"));

	QtVariantProperty *pItem = m_pVariantManager->addProperty(QVariant::String, QStringLiteral("名称"));
	pItem->setAttribute(QLatin1String("readOnly"), true);
	pItem->setValue(QString::fromLocal8Bit(pWidget->GetWidgetName().c_str()));
	pStaticItem->addSubProperty(pItem);

	pItem = m_pVariantManager->addProperty(QVariant::Rect, QStringLiteral("位置"));
	pItem->setValue(pWidget->GetPosition().toRect());
	pStaticItem->addSubProperty(pItem);
	
	QRect rcLimit = QRect(0, 0, 5000, 5000);
	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene)
	{
		CGraphFile *pFile = pScene->GetGraphFile();
		Q_ASSERT(pFile);
		if (pFile)
		{
			rcLimit = pFile->GetGraphRect().toRect();
			pItem->setAttribute(QLatin1String("constraint"), rcLimit);
		}
	}
	pItem->setAttribute(QLatin1String("constraint"), rcLimit);

	pItem = m_pVariantManager->addProperty(QVariant::Font, QStringLiteral("字体"));
	pStaticItem->addSubProperty(pItem);

	pTreeBrowser->setFactoryForManager(m_pVariantManager, m_pVariantFactory);
	pTreeBrowser->addProperty(pStaticItem);
		 
	QtProperty *pDynamicItem = m_pVariantManager->addProperty(QtVariantPropertyManager::groupTypeId(), QStringLiteral("动态属性"));
	pTreeBrowser->addProperty(pDynamicItem);
#endif

	pTreeBrowser->setPropertiesWithoutValueMarked(true);
	pTreeBrowser->setRootIsDecorated(false);

// 	connect(m_pDoubleManager, SIGNAL(valueChanged(QtProperty *, double)), this, SLOT(valueChanged(QtProperty *, double)));
// 	connect(m_pIntManager, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(valueChanged(QtProperty *, int)));
// 	connect(m_pStringManager, SIGNAL(valueChanged(QtProperty *, const QString &)), this, SLOT(valueChanged(QtProperty *, const QString &)));
	//connect(m_pColorManager, SIGNAL(valueChanged(QtProperty *, const QColor &)), this, SLOT(valueChanged(QtProperty *, const QColor &)));
//	connect(m_pStringManager, SIGNAL(valueChanged(QtProperty *, const QString &)), this, SLOT(valueChanged(QtProperty *, const QString &)));

// 	connect(m_pDoubleManager,
// 		static_cast<void (QtDoublePropertyManager::*)(QtProperty * , double  )> (&QtDoublePropertyManager::valueChanged),
// 		this,
// 		static_cast<void (CGraphView::*)(QtProperty * , double  )> (&CGraphView::valueChanged) );



}

void CGraphView::InitGrpahicAttr(QtTreePropertyBrowser *pTreeBrowser)
{
	Q_ASSERT(pTreeBrowser);
	if (pTreeBrowser == nullptr)
	{
		return;
	}

	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene)
	{
		CGraphFile *pFile = pScene->GetGraphFile();
		Q_ASSERT(pFile);

		if (pFile == nullptr)
		{
			return;
		}

		QtProperty *pBaseItem = m_pGroupManager->addProperty(tr("Graph_Base"));
		pTreeBrowser->addProperty(pBaseItem);
		AddPropertyToMap(pBaseItem, ITEM_GRAPHICS_BASE);

		//缩放比例
		QtProperty *pScaleItem = m_pIntManager->addProperty(tr("Scale"));
		m_pIntManager->setValue(pScaleItem, pFile->GetSacled());
		m_pIntManager->setMinimum(pScaleItem,0);
		AddPropertyToMap(pScaleItem, ITEM_GRAPHICS_SCALE);
		pBaseItem->addSubProperty(pScaleItem);

		QtProperty *pSizeItem = m_pSizeManager->addProperty(tr("Size"));
		m_pSizeManager->setValue(pSizeItem, QSize(pFile->GetSize().second,pFile->GetSize().first));
		m_pSizeManager->setMaximum(pSizeItem, QSize(10000,10000));
		pBaseItem->addSubProperty(pSizeItem);

		AddPropertyToMap(pSizeItem, ITEM_GREPHICS_SIZE);

		//背景色
		QtProperty *pBackgroundItem = m_pStringManager->addProperty(tr("Background"));
		pBackgroundItem->setModified(false);
		pBaseItem->addSubProperty(pBackgroundItem);
		AddPropertyToMap(pBackgroundItem, ITEM_BACKGROUND);

		//背景颜色
		QtProperty *pBackgroundColorItem = m_pStringManager->addProperty(tr("BackgroundColor"));
		pBackgroundColorItem->setModified(false);
		pBackgroundItem->addSubProperty(pBackgroundColorItem);
		AddPropertyToMap(pBackgroundColorItem, ITEM_BACKGROUND_COLOR);

		//背景颜色  颜色属性
		QtProperty *pBrushColorItem = m_pColorManager->addProperty(QVariant::Color, QStringLiteral("Color"));
		//m_pColorManager->setValue(pBrushColorItem, pInfo->GetColor());
		pBackgroundColorItem->addSubProperty(pBrushColorItem);
		AddPropertyToMap(pBrushColorItem, ITEM_BACKGROUND_COLOR_COLOR);

		//画刷类型
		QtProperty *pBrushSytleItem2 = m_pComBoProperty->addProperty(tr("Type"));
		QStringList lstLIneTyps;
		lstLIneTyps << tr("NoBrush") << tr("SolidPattern") << tr("Dense1Pattern")
			<< tr("Dense2Pattern") << tr("Dense3Pattern") << tr("Dense4Pattern") << tr("Dense5Pattern")
			<< tr("Dense6Pattern") << tr("Dense7Pattern") << tr("HorPattern") << tr("VerPattern")
			<< tr("CrossPattern") << tr("BDiagPattern") << tr("FDiagPattern") << tr("DiagCrossPattern");

		m_pComBoProperty->setEnumNames(pBrushSytleItem2, lstLIneTyps);
		m_pComBoProperty->setValue(pBrushSytleItem2, 1);
		pBackgroundColorItem->addSubProperty(pBrushSytleItem2);
		AddPropertyToMap(pBrushSytleItem2, ITEM_BACKGROUND_COLOR_TYPE);

		//画刷渐变类型
		QtProperty *pGradientItem = m_pComBoProperty->addProperty(tr("Gradient Type"));
		QStringList lstGradientTyps;
		lstGradientTyps << tr("NoGradient") << tr("Left-Right") << tr("Right-Left") << tr("Top-Bottom")
			<< tr("Bottom-Top");

		m_pComBoProperty->setEnumNames(pGradientItem, lstGradientTyps);
		m_pComBoProperty->setValue(pGradientItem, 0);

		pBackgroundColorItem->addSubProperty(pGradientItem);
		AddPropertyToMap(pGradientItem, ITEM_BACKGROUND_COLOR_GRADIENT_TYPE);

		//画刷 渐变色总之颜色
		QtProperty *pGradientColorItem = m_pColorManager->addProperty(QVariant::Color, QStringLiteral("GradientColor"));
		//m_pColorManager->setValue(pGradientColorItem, pInfo->GetColor());
		pBackgroundColorItem->addSubProperty(pGradientColorItem);
		AddPropertyToMap(pGradientColorItem, ITEM_BACKGROUND_COLOR_GRADIENT_COLOR);


		//背景图形
		QtProperty *pBackgroundImageItem = m_pStringManager->addProperty(tr("BackgroundImage"));
		pBackgroundImageItem->setModified(false);
		pBaseItem->addSubProperty(pBackgroundImageItem);
		AddPropertyToMap(pBackgroundImageItem, ITEM_BACKGROUND_IMAGE);

		//背景图片选择
		QtProperty *pBackgroundChooseImageItem = m_pBindInfoManager->addProperty(ITEM_BACKGROUND_IMAGE_CODE, tr("ChooseImage"));
		//m_pBindInfoManager->setValue(pBackgroundChooseImageItem, reinterpret_cast<long long>(m_pPorpWidget->GetDynamicPro(eDyncType)));
		pBackgroundImageItem->addSubProperty(pBackgroundChooseImageItem);
		AddPropertyToMap(pBackgroundChooseImageItem, ITEM_BACKGROUND_IMAGE_IMAGE);
		//图片坐标
		QtProperty *pPosItem = m_pPointManager->addProperty(tr("ImagePosition"));
		//m_pPointManager->setValue(pPosItem, pWidget->GetPosition().topLeft().toPoint());
		pBackgroundImageItem->addSubProperty(pPosItem);
		AddPropertyToMap(pPosItem, ITEM_BACKGROUND_IMAGE_XY);
		//图片大小
		QtProperty *pImageSizeItem = m_pSizeManager->addProperty(tr("ImageSize"));
		//m_pSizeManager->setValue(pSizeItem, QSize(pFile->GetSize().second, pFile->GetSize().first));
		m_pSizeManager->setMaximum(pImageSizeItem, QSize(10000, 10000));
		pBackgroundImageItem->addSubProperty(pImageSizeItem);
		AddPropertyToMap(pImageSizeItem, ITEM_BACKGROUND_IMAGE_SIZE);

		//平铺
		QtProperty *pTiling = m_pBoolManager->addProperty(tr("Tiling"));
		m_pBoolManager->setTextVisible(pTiling, false);
		pBackgroundImageItem->addSubProperty(pTiling);
		AddPropertyToMap(pTiling, ITEM_BACKGROUND_IMAGE_TILING);

		//拉伸
		QtProperty *pStretch = m_pBoolManager->addProperty(tr("ImageStretch"));
		m_pBoolManager->setTextVisible(pStretch, false);
		pBackgroundImageItem->addSubProperty(pStretch);
		AddPropertyToMap(pStretch, ITEM_BACKGROUND_IMAGE_STRETCH);

	}


}

//颜色
// void CGraphView::ChangeBrowserColor(QtBrowserItem *pBrowser, const QColor& color, int nIndex)
// {
// 	for each (QtBrowserItem* var in pBrowser->children())
// 	{
// 		if (var->children().count() > 0)
// 		{
// 			m_pTreeProperty->setBackgroundColor(var, QColor(255, 255, 0));
// 		}
// 		else
// 		{
// 			m_pTreeProperty->setBackgroundColor(var, QColor(255, 255, 0));
// 		}
// 		
// 	}
// 	
// 
// }


void CGraphView::ShowTextProperty(QtProperty *pParent, QString & szTxt, CFontInfo *pInfo)
{
	Q_UNUSED(pInfo);
	Q_ASSERT(pParent);
	if (pParent == nullptr)
		return;
// 	Q_ASSERT(pInfo);
// 	if (pInfo == nullptr)
// 		return;

	QtProperty *pColorItem = m_pColorManager->addProperty(QVariant::Color,QStringLiteral("Color"));
	//m_pColorManager->setValue(pColorItem, pInfo->GetColor());
	AddPropertyToMap(pColorItem, ITEM_TEXT_COLOR);
	pParent->addSubProperty(pColorItem);

	QtProperty *pFontItem = m_pColorManager->addProperty(QVariant::Font, tr("Font"));
	//m_pColorManager->setValue(pFontItem, pInfo->GetFont());
	AddPropertyToMap(pFontItem, ITEM_TEXT_FONT);
	pParent->addSubProperty(pFontItem);

	QtProperty *pTextPositionItem = m_pComBoProperty->addProperty(tr("Layout"));
	QStringList lstTextPosition;
	lstTextPosition << tr("Center") << tr("Top") << tr("Bottom")
		<< tr("Left") << tr("Right") << tr("Mid-Left") << tr("Mid-Right");

	m_pComBoProperty->setEnumNames(pTextPositionItem, lstTextPosition);
	m_pComBoProperty->setValue(pTextPositionItem, 0);
	AddPropertyToMap(pTextPositionItem, ITEM_TEXT_POSITION);
	pParent->addSubProperty(pTextPositionItem);

	QtProperty *pTxtItem = m_pStringManager->addProperty(QStringLiteral("Text"));
	m_pStringManager->setValue(pTxtItem, szTxt);
	pParent->addSubProperty(pTxtItem);
	AddPropertyToMap(pTxtItem, ITEM_TEXT);


}

void CGraphView::ShowItemImageProperty(QtProperty *pParent)
{
	//背景图片选择
	QtProperty *pBackgroundChooseImageItem = m_pBindInfoManager->addProperty(ITEM_BACKGROUND_IMAGE_CODE, tr("ChooseImage"));
	pParent->addSubProperty(pBackgroundChooseImageItem);
	AddPropertyToMap(pBackgroundChooseImageItem, ITEM_BACKGROUND_ITEM_IMAGE_IMAGE);

	//图片坐标
	QtProperty *pPosItem = m_pPointManager->addProperty(tr("ImagePosition"));
	pParent->addSubProperty(pPosItem);
	AddPropertyToMap(pPosItem, ITEM_BACKGROUND_ITEM_IMAGE_XY);
	//图片大小
	QtProperty *pImageSizeItem = m_pSizeManager->addProperty(tr("ImageSize"));
	m_pSizeManager->setMaximum(pImageSizeItem, QSize(10000, 10000));
	pParent->addSubProperty(pImageSizeItem);
	AddPropertyToMap(pImageSizeItem, ITEM_BACKGROUND_ITEM_IMAGE_SIZE);

	//平铺
	QtProperty *pTiling = m_pBoolManager->addProperty(tr("Tiling"));
	m_pBoolManager->setTextVisible(pTiling, false);
	pParent->addSubProperty(pTiling);
	AddPropertyToMap(pTiling, ITEM_BACKGROUND_ITEM_IMAGE_TILING);

	//拉伸
	QtProperty *pStretch = m_pBoolManager->addProperty(tr("ImageStretch"));
	m_pBoolManager->setTextVisible(pStretch, false);
	pParent->addSubProperty(pStretch);
	AddPropertyToMap(pStretch, ITEM_BACKGROUND_ITEM_IMAGE_STRETCH);

}

/*! \fn void CGraphView::ShowPenProperty(QtProperty *pParent, CPenInfo *pInfo)
********************************************************************************************************* 
** \brief CGraphView::ShowPenProperty 
** \details 显示画笔
** \param pParent 
** \param pInfo 
** \return void 
** \author LiJin 
** \date 2016年3月9日 
** \note 
********************************************************************************************************/
void CGraphView::ShowPenProperty(QtProperty *pParent, CPenInfo *pInfo)
{
	Q_UNUSED(pInfo);
	Q_ASSERT(pParent);
	if (pParent == nullptr)
		return;
// 	Q_ASSERT(pInfo);
// 	if (pInfo == nullptr)
// 		return;
	
	QtProperty *pLineColorItem = m_pColorManager->addProperty(QVariant::Color, QStringLiteral("Color"));
	//m_pColorManager->setValue(pLineColorItem, pInfo->GetColor());

	AddPropertyToMap(pLineColorItem, ITEM_PEN_COLOR);

	pParent->addSubProperty(pLineColorItem);

	QtProperty *pLineWidthItem = m_pIntManager->addProperty(tr("Width"));
	//m_pIntManager->setValue(pLineWidthItem, pInfo->GetWidth());
	m_pIntManager->setMaximum(pLineWidthItem, 100);
	m_pIntManager->setMinimum(pLineWidthItem, 0);
	AddPropertyToMap(pLineWidthItem, ITEM_PEN_WIDTH);

	pParent->addSubProperty(pLineWidthItem);

// 	QtProperty *pLineSytleItem = m_pIntManager->addProperty(tr("Type"));
// 	m_pIntManager->setValue(pLineSytleItem, pInfo->GetStyle());
// 	m_pIntManager->setMaximum(pLineSytleItem, Qt::CustomDashLine);
// 	m_pIntManager->setMinimum(pLineSytleItem, Qt::NoPen);
// 	AddPropertyToMap(pLineSytleItem, ITEM_PEN_TYPE);

	/////////////////////////////////
	QtProperty *pLineSytleItem2 = m_pComBoProperty->addProperty(tr("Type"));
	QStringList lstLIneTyps;
	lstLIneTyps << tr("NoPen") << tr("SolidLine") << tr("DashLine") 
		<< tr("DotLine") << tr("DashDotLine") << tr("DashDotDotLine") << tr("CustomDashLine");
	m_pComBoProperty->setEnumNames(pLineSytleItem2, lstLIneTyps);
	//m_pComBoProperty->setValue(pLineSytleItem2,1);
	AddPropertyToMap(pLineSytleItem2, ITEM_PEN_TYPE);

	////////////////////////////////
	

	//pParent->addSubProperty(pLineSytleItem);

	pParent->addSubProperty(pLineSytleItem2);
}
/*! \fn void CGraphView::ShowBrushProperty(QtProperty *pParent, CBrushInfo *pInfo)
********************************************************************************************************* 
** \brief CGraphView::ShowBrushProperty 
** \details 关于填充方面的设置
** \param pParent 
** \param pInfo 
** \return void 
** \author LiJin 
** \date 2016年3月9日 
** \note 
********************************************************************************************************/
void CGraphView::ShowBrushProperty(QtProperty *pParent, CBrushInfo *pInfo)
{
	Q_UNUSED(pInfo);
	Q_ASSERT(pParent);
	if (pParent == nullptr)
		return;
// 	Q_ASSERT(pInfo);
// 	if (pInfo == nullptr)
// 		return;

// 	QtProperty *pBrushSytleItem = m_pIntManager->addProperty(QStringLiteral("Type"));
// 	m_pIntManager->setValue(pBrushSytleItem, pInfo->GetStyle());
// 	m_pIntManager->setMaximum(pBrushSytleItem, Qt::ConicalGradientPattern);
// 	m_pIntManager->setMinimum(pBrushSytleItem, Qt::NoBrush);
// 	AddPropertyToMap(pBrushSytleItem, ITEM_BRUSH_TYPE);
// 	pParent->addSubProperty(pBrushSytleItem);


	QtProperty *pBrushSytleItem2 = m_pComBoProperty->addProperty(tr("Type"));
	QStringList lstLIneTyps;
	lstLIneTyps << tr("NoBrush") << tr("SolidPattern") << tr("Dense1Pattern")
		<< tr("Dense2Pattern") << tr("Dense3Pattern") << tr("Dense4Pattern") << tr("Dense5Pattern")
		<< tr("Dense6Pattern") << tr("Dense7Pattern") << tr("HorPattern") << tr("VerPattern")
		<< tr("CrossPattern") << tr("BDiagPattern") << tr("FDiagPattern") << tr("DiagCrossPattern");
		
	m_pComBoProperty->setEnumNames(pBrushSytleItem2, lstLIneTyps);
	m_pComBoProperty->setValue(pBrushSytleItem2, 1);
	AddPropertyToMap(pBrushSytleItem2, ITEM_BRUSH_TYPE);


	QtProperty *pBrushColorItem = m_pColorManager->addProperty(QVariant::Color,QStringLiteral("Color"));
	//m_pColorManager->setValue(pBrushColorItem, pInfo->GetColor());
	AddPropertyToMap(pBrushColorItem, ITEM_BURSH_COLOR);

	//画刷渐变类型
	QtProperty *pGradientItem = m_pComBoProperty->addProperty(tr("Gradient Type"));
	QStringList lstGradientTyps;
	lstGradientTyps <<tr("NoGradient")<< tr("Left-Right") << tr("Right-Left") << tr("Top-Bottom")
		<< tr("Bottom-Top") ;

	m_pComBoProperty->setEnumNames(pGradientItem, lstGradientTyps);
	m_pComBoProperty->setValue(pGradientItem, 0);
	AddPropertyToMap(pGradientItem, ITEM_BRUSH_GRADIENT);

	//画刷 渐变色总之颜色
	QtProperty *pGradientColorItem = m_pColorManager->addProperty(QVariant::Color, QStringLiteral("GradientColor"));
	//m_pColorManager->setValue(pGradientColorItem, pInfo->GetColor());
	AddPropertyToMap(pGradientColorItem, ITEM_BRUSH_ENDGRADIENTCOLOR);


	pParent->addSubProperty(pBrushColorItem);
	pParent->addSubProperty(pBrushSytleItem2);
	pParent->addSubProperty(pGradientItem);
	pParent->addSubProperty(pGradientColorItem);
}

void CGraphView::ShowDynamicColorProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE eDyncType)
{
	Q_ASSERT(pParent);
	if (pParent == nullptr)
		return;

	QtProperty *pBrushEnable = m_pBoolManager->addProperty(tr("Enable"));

	m_pBoolManager->setTextVisible(pBrushEnable, false);
	
// 	Q_ASSERT(m_pPorpWidget->GetDynamicPro(eDyncType));
// 	if (m_pPorpWidget->GetDynamicPro(eDyncType) == nullptr)
// 	{
// 		return;
// 	}

	QtProperty *pBrushColorItem = m_pBindInfoManager->addProperty(ITEM_COLOR_RANGE,tr("ColorRange"));
	//m_pBindInfoManager->setValue(pBrushColorItem, reinterpret_cast<long long>(m_pPorpWidget->GetDynamicPro(eDyncType)));

	QtProperty *pDynamicBind = m_pBindInfoManager->addProperty(ITEM_BIND_VAR,("Variable"));
	//m_pBindInfoManager->setValue(pDynamicBind, m_pPorpWidget->GetDynamicPro(CBaseDyncData::DYNC_FILLING)->m_szTagName.c_str());
	 	
	if (eDyncType == CBaseDyncData::DYNC_PEN_COLOR)
	{
		//pen颜色
		AddPropertyToMap(pBrushEnable, ITEM_DYNAMIC_PEN_COLOR_ENABLE);
		AddPropertyToMap(pBrushColorItem, ITEM_DYNAMIC_PEN_COLOR_COLOR);
		AddPropertyToMap(pDynamicBind, ITEM_DYNAMIC_PEN_COLOR_BINDATA);

		pParent->addSubProperty(pBrushEnable);
		pParent->addSubProperty(pDynamicBind);
		pParent->addSubProperty(pBrushColorItem);
	}
	else if (eDyncType == CBaseDyncData::DYNC_TEXT_COLOR)
	{
		//文本颜色
		AddPropertyToMap(pBrushEnable, ITEM_DYNAMIC_TEXT_COLOR_ENABLE);
		AddPropertyToMap(pBrushColorItem, ITEM_DYNAMIC_TEXT_COLOR_COLOR);
		AddPropertyToMap(pDynamicBind, ITEM_DYNAMIC_TEXT_COLOR_BINDATA);

		pParent->addSubProperty(pBrushEnable);
		pParent->addSubProperty(pDynamicBind);
		pParent->addSubProperty(pBrushColorItem);

	}
	else if (eDyncType == CBaseDyncData::DYNC_FILLING)
	{
		//填充
		//模拟色
		QtProperty *pAnalogColor = m_pBoolManager->addProperty(tr("AnalogColor"));
		m_pBoolManager->setTextVisible(pAnalogColor, false);
		//起始值
		QtProperty *pStartPro = m_pDoubleManager->addProperty(tr("StartValue"));
		//终止值
		QtProperty *pEndPro = m_pDoubleManager->addProperty(tr("EndValue"));
		//起始偏移量
		QtProperty *pStartOffset = m_pDoubleManager->addProperty(tr("StartOffset"));
		//终止偏移量
		QtProperty *pEndOffset = m_pDoubleManager->addProperty(tr("EndOffset"));
		//类型
		QtProperty *pTypePro = m_pComBoProperty->addProperty(tr("Type"));

		QStringList lstLIneTyps;
		lstLIneTyps << tr("Top->Bottom") << tr("Bottom->Top") << tr("Left->Right")<< tr("Right->Left");

		m_pComBoProperty->setEnumNames(pTypePro, lstLIneTyps);
		m_pComBoProperty->setValue(pTypePro, 0);


		AddPropertyToMap(pBrushEnable, ITEM_DYNAMIC_BRUSH_ENABLE);
		AddPropertyToMap(pBrushColorItem, ITEM_DYNAMIC_BRUSH_COLOR);
		AddPropertyToMap(pDynamicBind, ITEM_DYNAMIC_BRUSH_BINDPOINT);
		//
		AddPropertyToMap(pAnalogColor, ITEM_DYNAMIC_BRUSH_ANALOG_COLOR);
		AddPropertyToMap(pStartPro, ITEM_DYNAMIC_BRUSH_START_VALUE);
		AddPropertyToMap(pEndPro, ITEM_DYNAMIC_BRUSH_END_VALUE);
		AddPropertyToMap(pStartOffset, ITEM_DYNAMIC_BRUSH_START_OFFSET);

		AddPropertyToMap(pEndOffset, ITEM_DYNAMIC_BRUSH_END_OFFSET);
		AddPropertyToMap(pTypePro, ITEM_DYNAMIC_BRUSH_TYPE);

		pParent->addSubProperty(pBrushEnable);
		pParent->addSubProperty(pDynamicBind);
		pParent->addSubProperty(pBrushColorItem);


		pParent->addSubProperty(pAnalogColor);
		pParent->addSubProperty(pStartPro);
		pParent->addSubProperty(pEndPro);
		pParent->addSubProperty(pStartOffset);
		pParent->addSubProperty(pEndOffset);
		pParent->addSubProperty(pTypePro);
	}
	
}

void CGraphView::ShowDynamicMovintProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE eDyncType)
{
	Q_ASSERT(pParent);
	if (pParent == nullptr)
		return;

	QtProperty *pMovingEnable = m_pBoolManager->addProperty(tr("Enable"));
	m_pBoolManager->setTextVisible(pMovingEnable, false);

	QtProperty *pBackFlag = m_pBoolManager->addProperty(tr("Reverse"));
	m_pBoolManager->setTextVisible(pBackFlag, false);


// 	Q_ASSERT(m_pPorpWidget->GetDynamicPro(eDyncType));
// 
// 	if (m_pPorpWidget->GetDynamicPro(eDyncType) == nullptr)
// 	{
// 		return;
// 	}

	QtProperty *pDynamicBind = m_pBindInfoManager->addProperty(ITEM_BIND_VAR, ("Variable"));
	//m_pBindInfoManager->setValue(pDynamicBind, m_pPorpWidget->GetDynamicPro(eDyncType)->m_szTagName.c_str());

	QtProperty *pStartItem = m_pIntManager->addProperty(QStringLiteral("Start"));
	QtProperty *pEndItem = m_pIntManager->addProperty(QStringLiteral("End"));
	QtProperty *pStartOffsetItem = m_pIntManager->addProperty(QStringLiteral("StartOffset"));
	QtProperty *pEndOffsetItem = m_pIntManager->addProperty(QStringLiteral("EndOffset"));

	if (eDyncType == CBaseDyncData::DYNC_HOR_MOVE)
	{
		QtProperty *pDistanceItem = m_pDoubleManager->addProperty(tr("Distance"));

		//水平移动
		m_pIntManager->setValue(pStartItem, 0);
		m_pIntManager->setValue(pEndItem, 100);

		m_pIntManager->setValue(pStartOffsetItem, 0);
		m_pIntManager->setValue(pEndOffsetItem, 1);

 		AddPropertyToMap(pMovingEnable, ITEM_DYNAMIC_HORIZONTAL_ENABLE);
		AddPropertyToMap(pBackFlag, ITEM_DYNAMIC_HORIZONTAL_RESVESE);
		AddPropertyToMap(pDynamicBind, ITEM_DYNAMIC_HORIZONTAL_BINDDATA);

		AddPropertyToMap(pStartItem, ITEM_DYNAMIC_HORIZONTAL_START);
		AddPropertyToMap(pEndItem, ITEM_DYNAMIC_HORIZONTAL_END);
		AddPropertyToMap(pStartOffsetItem, ITEM_DYNAMIC_HORIZONTAL_STARTOFFSET);
		AddPropertyToMap(pEndOffsetItem, ITEM_DYNAMIC_HORIZONTAL_ENDOFFSET);
		AddPropertyToMap(pDistanceItem, ITEM_DYNAMIC_HORIZONTAL_DISTANCE);

		//是否动态画刷属性
		pParent->addSubProperty(pMovingEnable);
		pParent->addSubProperty(pBackFlag);
		pParent->addSubProperty(pDynamicBind);


		pParent->addSubProperty(pStartItem);
		pParent->addSubProperty(pEndItem);
		pParent->addSubProperty(pStartOffsetItem);
		pParent->addSubProperty(pEndOffsetItem);
		pParent->addSubProperty(pDistanceItem);

	}
	else if (eDyncType == CBaseDyncData::DYNC_VIR_MOVE)
	{
		QtProperty *pDistanceItem = m_pDoubleManager->addProperty(tr("Distance"));

		//竖直移动
		m_pIntManager->setValue(pStartItem, 0);
		m_pIntManager->setValue(pEndItem, 100);

		m_pIntManager->setValue(pStartOffsetItem, 0);
		m_pIntManager->setValue(pEndOffsetItem, 1);

		AddPropertyToMap(pMovingEnable, ITEM_DYNAMIC_VERTICAL_ENABLE);
		AddPropertyToMap(pBackFlag, ITEM_DYNAMIC_VERTICAL_RESVESE);
		AddPropertyToMap(pDynamicBind, ITEM_DYNAMIC_VERTICAL_BINDDATA);

		AddPropertyToMap(pStartItem, ITEM_DYNAMIC_VERTICAL_START);
		AddPropertyToMap(pEndItem, ITEM_DYNAMIC_VERTICAL_END);
		AddPropertyToMap(pStartOffsetItem, ITEM_DYNAMIC_VERTICAL_STARTOFFSET);
		AddPropertyToMap(pEndOffsetItem, ITEM_DYNAMIC_VERTICAL_ENDOFFSET);
		AddPropertyToMap(pDistanceItem, ITEM_DYNAMIC_VERTICAL_DISTANCE);

		//是否动态画刷属性
		pParent->addSubProperty(pMovingEnable);
		pParent->addSubProperty(pBackFlag);
		pParent->addSubProperty(pDynamicBind);


		pParent->addSubProperty(pStartItem);
		pParent->addSubProperty(pEndItem);
		pParent->addSubProperty(pStartOffsetItem);
		pParent->addSubProperty(pEndOffsetItem);
		pParent->addSubProperty(pDistanceItem);
	}
	else if (eDyncType == CBaseDyncData::DYNC_ROTATION)
	{
		//旋转
		QtProperty *pStartAngelItem = m_pIntManager->addProperty(tr("StartAngle"));
		QtProperty *pEndAngelItem = m_pIntManager->addProperty(tr("StartAngle"));

		m_pIntManager->setValue(pStartItem, 0);
		m_pIntManager->setValue(pEndItem, 360);

		m_pIntManager->setValue(pStartOffsetItem, 0);
		m_pIntManager->setValue(pEndOffsetItem, 1);

		AddPropertyToMap(pMovingEnable, ITEM_DYNAMIC_ROTATE_ENABLE);
		AddPropertyToMap(pBackFlag, ITEM_DYNAMIC_ROTATE_RESVESE);
		AddPropertyToMap(pDynamicBind, ITEM_DYNAMIC_ROTATE_BINDDATA);

		AddPropertyToMap(pStartItem, ITEM_DYNAMIC_ROTATE_START);
		AddPropertyToMap(pEndItem, ITEM_DYNAMIC_ROTATE_END);
		AddPropertyToMap(pStartOffsetItem, ITEM_DYNAMIC_ROTATE_STARTOFFSET);
		AddPropertyToMap(pEndOffsetItem, ITEM_DYNAMIC_ROTATE_ENDOFFSET);
		//
		AddPropertyToMap(pStartAngelItem, ITEM_DYNAMIC_ROTATE_STARTANGLE);
		AddPropertyToMap(pEndAngelItem, ITEM_DYNAMIC_ROTATE_ENDANGLE);

		//是否动态画刷属性
		pParent->addSubProperty(pMovingEnable);
		pParent->addSubProperty(pBackFlag);
		pParent->addSubProperty(pDynamicBind);


		pParent->addSubProperty(pStartItem);
		pParent->addSubProperty(pEndItem);
		pParent->addSubProperty(pStartOffsetItem);
		pParent->addSubProperty(pEndOffsetItem);
		pParent->addSubProperty(pStartAngelItem);
		pParent->addSubProperty(pEndAngelItem);
	}

}

//组合移动二层结构
void CGraphView::ShowDynamicGroupMovingProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE eDyncType)
{
	Q_UNUSED(eDyncType);
	Q_ASSERT(pParent);
	if (pParent == nullptr)
		return;

	QtProperty *pMovingEnable = m_pBoolManager->addProperty(tr("Enable"));
	m_pBoolManager->setTextVisible(pMovingEnable, false);

// 	Q_ASSERT(m_pPorpWidget->GetDynamicPro(eDyncType));
// 
// 	if (m_pPorpWidget->GetDynamicPro(eDyncType) == nullptr)
// 	{
// 		return;
// 	}

	QtProperty *pDynamicBind = m_pBindInfoManager->addProperty(ITEM_BIND_VAR, ("Variable"));
// 	m_pBindInfoManager->setValue(pDynamicBind, m_pPorpWidget->GetDynamicPro(eDyncType)->m_szTagName.c_str());

	QtProperty *pStartItem = m_pIntManager->addProperty(QStringLiteral("Start"));
	QtProperty *pEndItem = m_pIntManager->addProperty(QStringLiteral("End"));


	QtProperty *pEditStateItem = m_pBoolManager->addProperty(QStringLiteral("Edit"));
	m_pBoolManager->setTextVisible(pEditStateItem, false);


	QtProperty *pReset = m_pBoolManager->addProperty(QStringLiteral("Reset"));
	m_pBoolManager->setTextVisible(pReset, false);

	m_pIntManager->setValue(pStartItem, 0);
	m_pIntManager->setValue(pEndItem, 100);


	AddPropertyToMap(pMovingEnable, ITEM_DYNAMIC_GROUP_MOVING_ENABLE);
	AddPropertyToMap(pDynamicBind, ITEM_DYNAMIC_GROUP_MOVING_BINDDATA);

	AddPropertyToMap(pStartItem, ITEM_DYNAMIC_GROUP_MOVING_START);
	AddPropertyToMap(pEndItem, ITEM_DYNAMIC_GROUP_MOVING_END);

	AddPropertyToMap(pEditStateItem, ITEM_DYNAMIC_GROUP_MOVING_EDIT);
	AddPropertyToMap(pReset, ITEM_DYNAMIC_GROUP_MOVING_RESET);


	//是否动态画刷属性
	pParent->addSubProperty(pMovingEnable);
	pParent->addSubProperty(pDynamicBind);

	pParent->addSubProperty(pStartItem);
	pParent->addSubProperty(pEndItem);

	pParent->addSubProperty(pEditStateItem);
	pParent->addSubProperty(pReset);
}



//动态透明二层结构
void CGraphView::ShowDynamicTransparencyProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE eDyncType)
{
	Q_UNUSED(eDyncType);
	Q_ASSERT(pParent);
	if (pParent == nullptr)
		return;

	QtProperty *pBrushEnable = m_pBoolManager->addProperty(tr("Enable"));
	m_pBoolManager->setTextVisible(pBrushEnable, false);

// 	Q_ASSERT(m_pPorpWidget->GetDynamicPro(eDyncType));
// 	if (m_pPorpWidget->GetDynamicPro(eDyncType) == nullptr)
// 	{
// 		return;
// 	}

	QtProperty *pDynamicBind = m_pBindInfoManager->addProperty(ITEM_BIND_VAR, tr("Variable"));
// 	m_pBindInfoManager->setValue(pDynamicBind, m_pPorpWidget->GetDynamicPro(eDyncType)->m_szTagName.c_str());

	QtProperty *pStartItem = m_pIntManager->addProperty(tr("Start"));
	m_pIntManager->setValue(pStartItem, 0);
	m_pIntManager->setMaximum(pStartItem, 255);
	m_pIntManager->setMinimum(pStartItem, 0);

	QtProperty *pEndItem = m_pIntManager->addProperty(tr("End"));
	m_pIntManager->setValue(pEndItem, 0);
	m_pIntManager->setMaximum(pEndItem, 255);
	m_pIntManager->setMinimum(pEndItem, 0);

	AddPropertyToMap(pBrushEnable, ITEM_DYNAMIC_TRANSPARENCY_ENABLE);
	AddPropertyToMap(pDynamicBind, ITEM_DYNAMIC_TRANSPARENCY_BINDDATA);

	AddPropertyToMap(pStartItem, ITEM_DYNAMIC_TRANSPARENCY_START);
	AddPropertyToMap(pEndItem, ITEM_DYNAMIC_TRANSPARENCY_END);

	//动态透明
	pParent->addSubProperty(pBrushEnable);
	pParent->addSubProperty(pDynamicBind);

	pParent->addSubProperty(pStartItem);
	pParent->addSubProperty(pEndItem);
}

//动态可见二层结构
void CGraphView::ShowDynamicVisableProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE eDyncType)
{
	Q_UNUSED(eDyncType);
	Q_ASSERT(pParent);
	if (pParent == nullptr)
		return;

	QtProperty *pVisableEnable = m_pBoolManager->addProperty(tr("Enable"));
	m_pBoolManager->setTextVisible(pVisableEnable, false);

// 	Q_ASSERT(m_pPorpWidget->GetDynamicPro(eDyncType));
// 	if (m_pPorpWidget->GetDynamicPro(eDyncType) == nullptr)
// 	{
// 		return;
// 	}

	QtProperty *pDynamicBind = m_pBindInfoManager->addProperty(ITEM_BIND_VAR, tr("Variable"));
// 	m_pBindInfoManager->setValue(pDynamicBind, m_pPorpWidget->GetDynamicPro(eDyncType)->m_szTagName.c_str());

	QtProperty *pVisableValueItem = m_pDoubleManager->addProperty(tr("Value"));
	m_pDoubleManager->setValue(pVisableValueItem, 0);

	QtProperty *pZoneAreaItem = m_pDoubleManager->addProperty(tr("DeadZone"));
	m_pDoubleManager->setValue(pZoneAreaItem, 0);

	QtProperty *pConditionItem = m_pComBoProperty->addProperty(tr("Condition"));

	QStringList lstConditionTyps;
	lstConditionTyps << tr("=") << tr("!=") << tr(">=") << tr(">") << tr("<=") << tr("<");

	m_pComBoProperty->setEnumNames(pConditionItem, lstConditionTyps);
	m_pComBoProperty->setValue(pConditionItem, 0);

	QtProperty *pFlashTypeItem = m_pComBoProperty->addProperty(tr("FlashingType"));

	QStringList lstFlashTypeTyps;
	lstFlashTypeTyps << tr("Null") << tr("Slow") << tr("Middle") << tr("Fast")<<tr("Customize");

	m_pComBoProperty->setEnumNames(pFlashTypeItem, lstFlashTypeTyps);
	m_pComBoProperty->setValue(pFlashTypeItem, 0);

	QtProperty *pFlashTimeItem = m_pIntManager->addProperty(tr("FlashingTime"));
	m_pIntManager->setMinimum(pFlashTimeItem, 0);


	AddPropertyToMap(pVisableEnable, ITEM_DYNAMIC_VISABLE_ENABLE);
	AddPropertyToMap(pDynamicBind, ITEM_DYNAMIC_VISABLE_BINDDATA);

	AddPropertyToMap(pVisableValueItem, ITEM_DYNAMIC_VISAVLE_VALUE);
	AddPropertyToMap(pZoneAreaItem, ITEM_DYNAMCI_VISABLE_AREA);

	AddPropertyToMap(pConditionItem, ITEM_DYNAMIC_VISABLE_COMPARE);

	AddPropertyToMap(pFlashTypeItem, ITEM_DYNAMIC_VISABLE_FLASHTYPE);
	AddPropertyToMap(pFlashTimeItem, ITEM_DYNAMIC_VISABLE_CYCLE);


	//动态可见
	pParent->addSubProperty(pVisableEnable);
	pParent->addSubProperty(pDynamicBind);

	pParent->addSubProperty(pVisableValueItem);
	pParent->addSubProperty(pZoneAreaItem);

	pParent->addSubProperty(pConditionItem);

	pParent->addSubProperty(pFlashTypeItem);

	pParent->addSubProperty(pFlashTimeItem);

}

//动态数值二层结构
void CGraphView::ShowDynamicValueProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE eDyncType)
{
	Q_UNUSED(eDyncType);
	Q_ASSERT(pParent);
	if (pParent == nullptr)
		return;

	QtProperty *pValueEnable = m_pBoolManager->addProperty(tr("Enable"));
	m_pBoolManager->setTextVisible(pValueEnable, false);

// 	Q_ASSERT(m_pPorpWidget->GetDynamicPro(eDyncType));
// 	if (m_pPorpWidget->GetDynamicPro(eDyncType) == nullptr)
// 	{
// 		return;
// 	}

	QtProperty *pDynamicBind = m_pBindInfoManager->addProperty(ITEM_BIND_VAR, tr("Variable"));
// 	m_pBindInfoManager->setValue(pDynamicBind, m_pPorpWidget->GetDynamicPro(eDyncType)->m_szTagName.c_str());


	QtProperty *pFormatItem = m_pComBoProperty->addProperty(tr("Format"));

	QStringList lstFormatTyps;
	lstFormatTyps << tr("X") << tr("XX") << tr("XXX") << tr("XXXX") << tr("XXXXX") << tr("X.XX")
		<<tr("X.XXX")<<tr("X.XXXX")<<tr("X.XXXXX");

	m_pComBoProperty->setEnumNames(pFormatItem, lstFormatTyps);
	m_pComBoProperty->setValue(pFormatItem, 0);


	AddPropertyToMap(pValueEnable, ITEM_DYNAMIC_VALUE_ENABLE);
	AddPropertyToMap(pDynamicBind, ITEM_DYNAMIC_VALUE_BINDDATA);

	AddPropertyToMap(pFormatItem, ITEM_DYNAMIC_VALUE_VALUE_FORMAT);

	//动态数值
	pParent->addSubProperty(pValueEnable);
	pParent->addSubProperty(pDynamicBind);

	pParent->addSubProperty(pFormatItem);
}

//动态文本二层结构
void CGraphView::ShowDynamicTextProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE eDyncType)
{
	Q_UNUSED(eDyncType);
	Q_ASSERT(pParent);
	if (pParent == nullptr)
		return;

	QtProperty *pDynamicTextEnable = m_pBoolManager->addProperty(tr("Enable"));

	m_pBoolManager->setTextVisible(pDynamicTextEnable, false);

// 	Q_ASSERT(m_pPorpWidget->GetDynamicPro(eDyncType));
// 	if (m_pPorpWidget->GetDynamicPro(eDyncType) == nullptr)
// 	{
// 		return;
// 	}

	QtProperty *pTextThresholdItem = m_pBindInfoManager->addProperty(ITEM_TEXT_RANGE, tr("TextThreshold"));
// 	m_pBindInfoManager->setValue(pTextThresholdItem, reinterpret_cast<long long>(m_pPorpWidget->GetDynamicPro(eDyncType)));

	QtProperty *pDynamicBind = m_pBindInfoManager->addProperty(ITEM_BIND_VAR, ("Variable"));
// 	m_pBindInfoManager->setValue(pDynamicBind, m_pPorpWidget->GetDynamicPro(CBaseDyncData::DYNC_STRING)->m_szTagName.c_str());

	//
	AddPropertyToMap(pDynamicTextEnable, ITEM_DYNAMIC_TEXT_ENABLE);
	AddPropertyToMap(pTextThresholdItem, ITEM_DYNAMIC_TEXT_VALUES);
	AddPropertyToMap(pDynamicBind, ITEM_DYNAMIC_TEXT_BINDDATA);

	pParent->addSubProperty(pDynamicTextEnable);
	pParent->addSubProperty(pDynamicBind);
	pParent->addSubProperty(pTextThresholdItem);
	
}

//动态图片二层结构
void CGraphView::ShowDynamicMapProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE eDyncType)
{
	Q_UNUSED(eDyncType);
	Q_ASSERT(pParent);
	if (pParent == nullptr)
		return;

	QtProperty *pDynamicMapEnable = m_pBoolManager->addProperty(tr("Enable"));

	m_pBoolManager->setTextVisible(pDynamicMapEnable, false);

// 	Q_ASSERT(m_pPorpWidget->GetDynamicPro(eDyncType));
// 	if (m_pPorpWidget->GetDynamicPro(eDyncType) == nullptr)
// 	{
// 		return;
// 	}

	QtProperty *pMapThresholdItem = m_pBindInfoManager->addProperty(ITEM_MAP_RANGE, tr("ImageThreshold"));
// 	m_pBindInfoManager->setValue(pMapThresholdItem, reinterpret_cast< long long>(m_pPorpWidget->GetDynamicPro(eDyncType)));

	QtProperty *pDynamicBind = m_pBindInfoManager->addProperty(ITEM_BIND_VAR, ("Variable"));
// 	m_pBindInfoManager->setValue(pDynamicBind, m_pPorpWidget->GetDynamicPro(CBaseDyncData::DYNC_MAP)->m_szTagName.c_str());

	//
	AddPropertyToMap(pDynamicMapEnable, ITEM_DYNAMIC_MAP_ENABLE);
	AddPropertyToMap(pMapThresholdItem, ITEM_DYNAMIC_MAP_VALUES);
	AddPropertyToMap(pDynamicBind, ITEM_DYNAMIC_MAP_BINIDDATA);

	pParent->addSubProperty(pDynamicMapEnable);
	pParent->addSubProperty(pDynamicBind);
	pParent->addSubProperty(pMapThresholdItem);

}



void CGraphView::SetBindDataTypeProperty(QtProperty *pBindType)
{
	QtProperty *pBinTypeNumm = m_pBoolManager->addProperty(tr("Null"));
	AddPropertyToMap(pBinTypeNumm, ITEM_DYNAMIC_BRUSH_DATA_TYPE_NULL);
	pBindType->addSubProperty(pBinTypeNumm);

	QtProperty *pBinTypeBool = m_pBoolManager->addProperty(tr("Bool"));
	AddPropertyToMap(pBinTypeBool, ITEM_DYNAMIC_BRUSH_DATA_TYPE_BOOL);
	pBindType->addSubProperty(pBinTypeBool);


	QtProperty *pBinTypeChar = m_pBoolManager->addProperty(tr("Char"));
	AddPropertyToMap(pBinTypeChar, ITEM_DYNAMIC_BRUSH_DATA_TYPE_CHAR);
	pBindType->addSubProperty(pBinTypeChar);


	QtProperty *pBinTypeByte = m_pBoolManager->addProperty(tr("Byte"));
	AddPropertyToMap(pBinTypeByte, ITEM_DYNAMIC_BRUSH_DATA_TYPE_BYTE);
	pBindType->addSubProperty(pBinTypeByte);

	QtProperty *pBinTypeShort = m_pBoolManager->addProperty(tr("Short"));
	AddPropertyToMap(pBinTypeShort, ITEM_DYNAMIC_BRUSH_DATA_TYPE_SHORT);
	pBindType->addSubProperty(pBinTypeShort);

	QtProperty *pBinTypeWord = m_pBoolManager->addProperty(tr("Word"));
	AddPropertyToMap(pBinTypeWord, ITEM_DYNAMIC_BRUSH_DATA_TYPE_WORD);
	pBindType->addSubProperty(pBinTypeWord);

	QtProperty *pBinTypeInt = m_pBoolManager->addProperty(tr("Int"));
	AddPropertyToMap(pBinTypeInt, ITEM_DYNAMIC_BRUSH_DATA_TYPE_INT);
	pBindType->addSubProperty(pBinTypeInt);

	QtProperty *pBinTypeDword = m_pBoolManager->addProperty(tr("Dword"));
	AddPropertyToMap(pBinTypeDword, ITEM_DYNAMIC_BRUSH_DATA_TYPE_DWORD);
	pBindType->addSubProperty(pBinTypeDword);

	QtProperty *pBinTypeLlong = m_pBoolManager->addProperty(tr("Llong"));
	AddPropertyToMap(pBinTypeLlong, ITEM_DYNAMIC_BRUSH_DATA_TYPE_LLONG);
	pBindType->addSubProperty(pBinTypeLlong);

	QtProperty *pBinTypeLword = m_pBoolManager->addProperty(tr("Lword"));
	AddPropertyToMap(pBinTypeLword, ITEM_DYNAMIC_BRUSH_DATA_TYPE_LWORD);
	pBindType->addSubProperty(pBinTypeLword);

	QtProperty *pBinTypeFloat = m_pBoolManager->addProperty(tr("Float"));
	AddPropertyToMap(pBinTypeFloat, ITEM_DYNAMIC_BRUSH_DATA_TYPE_FOLAT);
	pBindType->addSubProperty(pBinTypeFloat);

	QtProperty *pBinTypeDouble = m_pBoolManager->addProperty(tr("Double"));
	AddPropertyToMap(pBinTypeDouble, ITEM_DYNAMIC_BRUSH_DATA_TYPE_DOUBLE);
	pBindType->addSubProperty(pBinTypeDouble);

	QtProperty *pBinTypeString = m_pBoolManager->addProperty(tr("String"));
	AddPropertyToMap(pBinTypeString, ITEM_DYNAMIC_BRUSH_DATA_TYPE_STRING);
	pBindType->addSubProperty(pBinTypeString);

	QtProperty *pBinTypeBlock = m_pBoolManager->addProperty(tr("Block"));
	AddPropertyToMap(pBinTypeBlock, ITEM_DYNAMIC_BRUSH_DATA_TYPE_BLOCK);
	pBindType->addSubProperty(pBinTypeBlock);

	QtProperty *pBinTypeMax = m_pBoolManager->addProperty(tr("Max"));
	AddPropertyToMap(pBinTypeMax, ITEM_DYNAMIC_BRUSH_DATA_TYPE_MAX);
	pBindType->addSubProperty(pBinTypeMax);
}


void CGraphView::AddPropertyToMap(QtProperty *pProp, int nProCode)
{
	Q_ASSERT(pProp != nullptr);
	if (pProp == nullptr)
	{
		return;
	}

	m_mapPropertyToId[pProp] = nProCode;
	m_mapIdToProperty[nProCode] = pProp;
}

void  CGraphView::ClearPropMap()
{
	m_mapIdToProperty.clear();
	m_mapPropertyToId.clear();
}

void CGraphView::resizeEvent(QResizeEvent * event)
{
	Q_UNUSED(event);
	//Q_ASSERT(event);
	viewport()->update();
}

void CGraphView::slot_SliderMoved()
{
	viewport()->update();
}

void CGraphView::valueChanged(QtProperty *pProperty, double dblValue)
{
	Q_ASSERT(pProperty);
	if (pProperty == nullptr)
		return;

	if (m_mapPropertyToId.contains(pProperty) == false)
		return;

	int szID = m_mapPropertyToId[pProperty];
	if (szID == ITEM_ROTATE)
	{
		//旋转
		m_pPorpWidget->SetRotateAngle(dblValue);
	}
	else if (szID == ITEM_END_ANGLE)
	{
		//结束角度
		m_pPorpWidget->SetEndAngle(dblValue);
	}
	else if (szID == ITEM_DYNAMIC_VISAVLE_VALUE)
	{
		//可见 value
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VISIBLE);
		CDyncVisibleData *pVisableData = dynamic_cast<CDyncVisibleData*>(pbaseDync);

		Q_ASSERT(pVisableData);
		if (pVisableData == nullptr)
		{
			return;
		}

		pVisableData->SetVisableValue(dblValue);

	}
	else if (szID == ITEM_DYNAMCI_VISABLE_AREA)
	{
		//死区  deadzone 
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VISIBLE);
		CDyncVisibleData *pVisableData = dynamic_cast<CDyncVisibleData*>(pbaseDync);

		Q_ASSERT(pVisableData);
		if (pVisableData == nullptr)
		{
			return;
		}

		pVisableData->SetDeadBand(dblValue);

	}
	else if (szID == ITEM_DYNAMIC_BRUSH_START_VALUE)
	{
		//模拟色起始值
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_FILLING);
		CDyncFillingData *pFillingData = dynamic_cast<CDyncFillingData*>(pbaseDync);

		Q_ASSERT(pFillingData);
		if (pFillingData == nullptr)
		{
			return;
		}

		pFillingData->m_dMinVal = dblValue;

	}
	else if (szID == ITEM_DYNAMIC_BRUSH_END_VALUE)
	{
		//模拟色终止值
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_FILLING);
		CDyncFillingData *pFillingData = dynamic_cast<CDyncFillingData*>(pbaseDync);

		Q_ASSERT(pFillingData);
		if (pFillingData == nullptr)
		{
			return;
		}

		pFillingData->m_dMaxVal = dblValue;

	}
	else if (szID == ITEM_DYNAMIC_BRUSH_START_OFFSET)
	{
		//模拟色起始偏移
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_FILLING);
		CDyncFillingData *pFillingData = dynamic_cast<CDyncFillingData*>(pbaseDync);

		Q_ASSERT(pFillingData);
		if (pFillingData == nullptr)
		{
			return;
		}

		pFillingData->m_dStartPos = dblValue;

	}
	else if (szID == ITEM_DYNAMIC_BRUSH_END_OFFSET)
	{
		//模拟色终止偏移
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_FILLING);
		CDyncFillingData *pFillingData = dynamic_cast<CDyncFillingData*>(pbaseDync);

		Q_ASSERT(pFillingData);
		if (pFillingData == nullptr)
		{
			return;
		}

		pFillingData->m_dEndPos = dblValue;
	}
	else if (szID == ITEM_DYNAMIC_HORIZONTAL_DISTANCE)
	{
		//水平位移
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_HOR_MOVE);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetDistance(dblValue);
	}
	else if (szID == ITEM_DYNAMIC_VERTICAL_DISTANCE)
	{
		//竖直位移
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VIR_MOVE);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetDistance(dblValue);
	}
	 
	invalidateScene();
	//保存历史操作数据
// 	AddHistFiles();

	this->viewport()->update();

}

void CGraphView::valueChanged(QtProperty *pProperty, int nValue)
{
	Q_ASSERT(pProperty);
	if (pProperty == nullptr)
		return;
		
	if (m_mapPropertyToId.contains(pProperty) == false)
		return;

	if (m_pPorpWidget == nullptr)
	{
		int szID = m_mapPropertyToId[pProperty];
		setCacheMode(CacheNone);

		if (szID == ITEM_BACKGROUND_COLOR_TYPE)
		{
			//画刷类型
			std::shared_ptr<CBackground> tBackground = GetGraphicsScene()->GetGraphFile()->GetBackground();
			tBackground->m_nBrushStyle = nValue;

		}
		else if (szID == ITEM_BACKGROUND_COLOR_GRADIENT_TYPE)
		{
			//渐变类型
			std::shared_ptr<CBackground> tBackground = GetGraphicsScene()->GetGraphFile()->GetBackground();
			tBackground->m_nGradiendMode = nValue;

		}
		else if (szID == ITEM_GRAPHICS_SCALE)
		{
			//缩放比例
			/////////////////////////////////////////////////////////////////////////
			CGraphScene *pScene = GetGraphicsScene();
			Q_ASSERT(pScene);
			if (pScene == nullptr)
				return;

			CGraphFile * pGraphFile = pScene->GetGraphFile();
			Q_ASSERT(pGraphFile);
			if (pGraphFile == nullptr)
			{
				return;
			}

			pGraphFile->SetScaled(nValue);
			//m_scaleFactor += 2;
			QMatrix old_matrix;
			old_matrix = this->matrix();
			this->resetMatrix();
			//this->translate(old_matrix.dx(), old_matrix.dy());
			this->scale(pGraphFile->GetSacled() / 50.0, pGraphFile->GetSacled() / 50.0);


			/////////////////////////////////////////////////////////////////////////
		}

		this->viewport()->update();
		//保存历史操作数据
// 		AddHistFiles();

		return;
	}

//	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	int szID = m_mapPropertyToId[pProperty];

	if (szID == ITEM_PEN_WIDTH)
	{
		m_pPorpWidget->GetPenInfo()->SetWidth(static_cast<unsigned int>(nValue));
	}
	else if (szID == ITEM_PEN_TYPE)
	{
		m_pPorpWidget->GetPenInfo()->SetStyle(static_cast<unsigned int>(nValue));
	}
	else if (szID == ITEM_ROTATE_CENTER)
	{
		//旋转中心
		m_pPorpWidget->SetRotateCenter(nValue);
	}
	else if (szID == ITEM_BRUSH_TYPE)
	{
		//画刷
		m_pPorpWidget->GetBrushInfo()->SetStyle(static_cast<unsigned int>(nValue));
	}
	else if (szID == ITEM_BRUSH_GRADIENT)
	{
		//画刷渐变类型
		m_pPorpWidget->GetBrushInfo()->SetGradiendMode(nValue);
	}
	else if (szID == ITEM_TEXT_POSITION)
	{
		//字体layout
		m_pPorpWidget->GetFontInfo()->SetFontLayout(nValue);
	}
	else if (szID == ITEM_DYNAMIC_HORIZONTAL_START)
	{
		//起始
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_HOR_MOVE);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetStartData(nValue);
	}
	else if (szID == ITEM_DYNAMIC_HORIZONTAL_END)
	{
		//结束
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_HOR_MOVE);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetEndData(nValue);
	}
	else if (szID == ITEM_DYNAMIC_HORIZONTAL_STARTOFFSET)
	{
		//起始偏移
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_HOR_MOVE);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetStartoffset(nValue);
	}
	else if (szID == ITEM_DYNAMIC_HORIZONTAL_ENDOFFSET)
	{
		//结束偏移
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_HOR_MOVE);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetEndoffset(nValue);
	}////////////////////////////////////////////////////////////////////////////
	else if (szID == ITEM_DYNAMIC_VERTICAL_START)
	{
		//起始
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VIR_MOVE);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetStartData(nValue);
	}
	else if (szID == ITEM_DYNAMIC_VERTICAL_END)
	{
		//结束
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VIR_MOVE);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetEndData(nValue);
	}
	else if (szID == ITEM_DYNAMIC_VERTICAL_STARTOFFSET)
	{
		//起始偏移
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VIR_MOVE);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetStartoffset(nValue);
	}
	else if (szID == ITEM_DYNAMIC_VERTICAL_ENDOFFSET)
	{
		//结束偏移
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VIR_MOVE);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetEndoffset(nValue);
	}///////////////////////////////////
	else if (szID == ITEM_DYNAMIC_ROTATE_START)
	{
		//起始
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_ROTATION);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetStartData(nValue);
	}
	else if (szID == ITEM_DYNAMIC_ROTATE_END)
	{
		//旋转结束
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_ROTATION);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetEndData(nValue);
	}
	else if (szID == ITEM_DYNAMIC_VERTICAL_END)
	{
		//结束
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_ROTATION);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetEndData(nValue);
	}
	else if (szID == ITEM_DYNAMIC_ROTATE_STARTOFFSET)
	{
		//起始偏移
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_ROTATION);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetStartoffset(nValue);
	}
	else if (szID == ITEM_DYNAMIC_ROTATE_ENDOFFSET)
	{
		//结束偏移
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_ROTATION);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetEndoffset(nValue);
	}
	else if (szID == ITEM_DYNAMIC_ROTATE_STARTANGLE)
	{
		//旋转起始角度
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_ROTATION);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}
		pMovingData->SetStartAngel(nValue);
	}
	else if (szID == ITEM_DYNAMIC_ROTATE_ENDANGLE)
	{
		//旋转结束角度
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_ROTATION);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}
		pMovingData->SetEndAngle(nValue);
	}
	else if (szID == ITEM_DYNAMIC_GROUP_MOVING_START)
	{
		//组合起始起始
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_GROUP_MOVE);
		CDyncGroupMovingData *pMovingData = dynamic_cast<CDyncGroupMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetStartData(nValue);
	}
	else if (szID == ITEM_DYNAMIC_GROUP_MOVING_END)
	{
		//组合结束
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_GROUP_MOVE);
		CDyncGroupMovingData *pMovingData = dynamic_cast<CDyncGroupMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetEndData(nValue);
	}
	else if (szID == ITEM_DYNAMIC_TRANSPARENCY_START)
	{
		//透明起始
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_TRANSPARENCY);
		CDyncTransparencyData *pTranData = dynamic_cast<CDyncTransparencyData*>(pbaseDync);

		Q_ASSERT(pTranData);
		if (pTranData == nullptr)
		{
			return;
		}

		pTranData->SetMinValue(nValue);

	}
	else if (szID == ITEM_DYNAMIC_TRANSPARENCY_END)
	{
		//透明结束
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_TRANSPARENCY);
		CDyncTransparencyData *pTranData = dynamic_cast<CDyncTransparencyData*>(pbaseDync);

		Q_ASSERT(pTranData);
		if (pTranData == nullptr)
		{
			return;
		}

		pTranData->SetMaxValue(nValue);

	}
	else if (szID == ITEM_DYNAMIC_VISABLE_COMPARE)
	{
		//比较条件
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VISIBLE);
		CDyncVisibleData *pVisableData = dynamic_cast<CDyncVisibleData*>(pbaseDync);

		Q_ASSERT(pVisableData);
		if (pVisableData == nullptr)
		{
			return;
		}

		pVisableData->SetCondition(nValue);
	}
	else if (szID == ITEM_DYNAMIC_VISABLE_FLASHTYPE)
	{
		//闪烁类型
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VISIBLE);
		CDyncVisibleData *pVisableData = dynamic_cast<CDyncVisibleData*>(pbaseDync);

		Q_ASSERT(pVisableData);
		if (pVisableData == nullptr)
		{
			return;
		}

		pVisableData->SetFlashType(nValue);
	}
	else if (szID == ITEM_DYNAMIC_VISABLE_CYCLE)
	{
		//闪烁类型
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VISIBLE);
		CDyncVisibleData *pVisableData = dynamic_cast<CDyncVisibleData*>(pbaseDync);

		Q_ASSERT(pVisableData);
		if (pVisableData == nullptr)
		{
			return;
		}

		pVisableData->SetFlashTime(nValue);
	}
	else if (szID == ITEM_DYNAMIC_VALUE_VALUE_FORMAT)
	{
		//数值格式
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VALUE);
		CDyncValueData *pValueData = dynamic_cast<CDyncValueData*>(pbaseDync);

		Q_ASSERT(pValueData);
		if (pValueData == nullptr)
		{
			return;
		}

		pValueData->SetFormat((CDyncValueData::VAL_FORMAT)nValue);

	}
	else if (szID == ITEM_DYNAMIC_BRUSH_TYPE)
	{
		//动态模拟色类型
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_FILLING);
		CDyncFillingData *pValueData = dynamic_cast<CDyncFillingData*>(pbaseDync);

		Q_ASSERT(pValueData);
		if (pValueData == nullptr)
		{
			return;
		}

		pValueData->m_nFillingPolicy = (CDyncFillingData::DYNC_FILLING_POLICY)(nValue);

	}
	else if (szID == ITEM_CUREVEPLOT_BRUSH_STYLE)
	{
		//曲线图表画刷类型
		if (m_pPorpWidget->GetWidgetType() == DRAW_ADDR_CURE_PLOT)
		{
			CAddedWidget *taddWgt = dynamic_cast<CAddedWidget*>(m_pPorpWidget);

			Q_ASSERT(taddWgt);
			if (taddWgt == nullptr)
			{
				return;
			}

			taddWgt->GetPlotWgt()->GetBrushInfo()->SetStyle(nValue);

			//直接刷新曲线样式
			taddWgt->GetPlotWgt()->UpdateChart();


		}
	}
	else if (szID == ITEM_CURVEFRAM_WIDTH)
	{
		if (m_pPorpWidget->GetWidgetType() == DRAW_ADDR_CURE_PLOT)
		{
			//曲线图表边框
			CAddedWidget *taddWgt = dynamic_cast<CAddedWidget*>(m_pPorpWidget);

			Q_ASSERT(taddWgt);
			if (taddWgt == nullptr)
			{
				return;
			}

			taddWgt->SetInterval(nValue);

		}
	}

	invalidateScene();

	this->viewport()->update();

	//保存历史操作数据
// 	AddHistFiles();
}

void CGraphView::valueChanged(QtProperty *pProperty, const QString &szValue)
{
	Q_ASSERT(pProperty);
	if (pProperty == nullptr)
		return;

 	if (m_mapPropertyToId.contains(pProperty) == false)
		return;

	if (m_pPorpWidget == nullptr)
	{

		return;
	}

	//	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	int szID = m_mapPropertyToId[pProperty];

	if (szID == ITEM_TEXT)
	{
		m_pPorpWidget->SetText(szValue);
	}
	else if (szID == ITEM_DYNAMIC_BRUSH_BINDPOINT)
	{
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_FILLING)->m_szTagName = szValue.toLocal8Bit().data();
	}
	else if (szID == ITEM_DYNAMIC_PEN_COLOR_BINDATA)
	{
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_PEN_COLOR)->m_szTagName = szValue.toLocal8Bit().data();
	}
	else if (szID == ITEM_DYNAMIC_TEXT_COLOR_BINDATA)
	{
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_TEXT_COLOR)->m_szTagName = szValue.toLocal8Bit().data();
	}
	else if (szID == ITEM_DYNAMIC_HORIZONTAL_BINDDATA)
	{
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_HOR_MOVE)->m_szTagName = szValue.toLocal8Bit().data();
	}
	else if (szID == ITEM_DYNAMIC_VERTICAL_BINDDATA)
	{
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VIR_MOVE)->m_szTagName = szValue.toLocal8Bit().data();
	}
	else if (szID == ITEM_DYNAMIC_ROTATE_BINDDATA)
	{
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_ROTATION)->m_szTagName = szValue.toLocal8Bit().data();
	}
 
	invalidateScene();
	//保存历史操作数据
// 	AddHistFiles();
	this->viewport()->update();
}

void CGraphView::valueChanged(QtProperty *pProperty, const QVariant &tValue)
{
	Q_ASSERT(pProperty);
	if (pProperty == nullptr)
		return;

	if (m_mapPropertyToId.contains(pProperty) == false)
		return;

	if (m_pPorpWidget == nullptr)
	{
		int szID = m_mapPropertyToId[pProperty];

		setCacheMode(CacheNone);

		if (szID == ITEM_BACKGROUND_COLOR_COLOR)
		{
			
			std::shared_ptr<CBackground> tBackground = GetGraphicsScene()->GetGraphFile()->GetBackground();
			tBackground->m_clrBackground = tValue.value<QColor>();
		}
		else if (szID == ITEM_BACKGROUND_COLOR_GRADIENT_COLOR)
		{
			std::shared_ptr<CBackground> tBackground = GetGraphicsScene()->GetGraphFile()->GetBackground();
			tBackground->m_cloEndColor = tValue.value<QColor>();
		}
		else if (szID == ITEM_BACKGROUND_IMAGE_IMAGE)
		{
			//背景图片
			std::shared_ptr<CBackground> tBackground = GetGraphicsScene()->GetGraphFile()->GetBackground();
			tBackground->m_strBackgroundImage = tValue.toString();
			tBackground->m_szImage = QSize(0,0);

		}
		////////////////


		this->viewport()->update();

		//保存历史操作数据
// 		AddHistFiles();

		return;
	}

//	Q_ASSERT(m_nCurEditingLayerIndex > 0 && m_nCurEditingLayerIndex <= CGraphFile::MAX_LAYER_CNT);
	if (m_nCurEditingLayerIndex == 0 || m_nCurEditingLayerIndex > CGraphFile::MAX_LAYER_CNT)
		return;

	int szID = m_mapPropertyToId[pProperty];

	if (szID == ITEM_PEN_COLOR)
	{
		m_pPorpWidget->GetPenInfo()->SetColor(tValue.value<QColor>());
	}
	else if (szID == ITEM_BURSH_COLOR)
	{
		m_pPorpWidget->GetBrushInfo()->SetColor(tValue.value<QColor>());
	}
	else if (szID == ITEM_BRUSH_ENDGRADIENTCOLOR)
	{
		//渐变终止颜色
		m_pPorpWidget->GetBrushInfo()->SetEndColor(tValue.value<QColor>());
	}
	else if (szID == ITEM_TEXT_COLOR)
	{
		//文本颜色
		m_pPorpWidget->GetFontInfo()->SetColor(tValue.value<QColor>());
	}
	else if (szID == ITEM_TEXT_FONT)
	{
		//文字属性
		m_pPorpWidget->GetFontInfo()->SetFont(tValue.value<QFont>());
	}
	else if (szID == ITEM_DYNAMIC_BRUSH_BINDPOINT)
	{

		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_FILLING)->m_szTagName = tValue.toByteArray().data();
		//m_pPorpWidget->GetDynamicBrushInfo()->GetBindPointInfo()->SetBindValue(tValue.toString());
	}
	else if (szID == ITEM_DYNAMIC_PEN_COLOR_BINDATA)
	{

		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_PEN_COLOR)->m_szTagName = tValue.toByteArray().data();
		//m_pPorpWidget->GetDynamicBrushInfo()->GetBindPointInfo()->SetBindValue(tValue.toString());
	}
	else if (szID == ITEM_DYNAMIC_TEXT_COLOR_BINDATA)
	{

		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_TEXT_COLOR)->m_szTagName = tValue.toByteArray().data();
		//m_pPorpWidget->GetDynamicBrushInfo()->GetBindPointInfo()->SetBindValue(tValue.toString());
	}
	else if (szID == ITEM_DYNAMIC_HORIZONTAL_BINDDATA)
	{
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_HOR_MOVE)->m_szTagName = tValue.toByteArray().data();
	}
	else if (szID == ITEM_DYNAMIC_VERTICAL_BINDDATA)
	{
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VIR_MOVE)->m_szTagName = tValue.toByteArray().data();
	}
	else if (szID == ITEM_DYNAMIC_ROTATE_BINDDATA)
	{
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_ROTATION)->m_szTagName = tValue.toByteArray().data();
	}
	else if (szID == ITEM_DYNAMIC_TRANSPARENCY_BINDDATA)
	{
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_TRANSPARENCY)->m_szTagName = tValue.toByteArray().data();
	}
	else if (szID == ITEM_DYNAMIC_VISABLE_BINDDATA)
	{
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VISIBLE)->m_szTagName = tValue.toByteArray().data();
	}
	else if (szID == ITEM_DYNAMIC_VALUE_BINDDATA)
	{
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VALUE)->m_szTagName = tValue.toByteArray().data();
	}
	else if (szID == ITEM_DYNAMIC_TEXT_BINDDATA)
	{
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_STRING)->m_szTagName = tValue.toByteArray().data();
	}
	else if (szID == ITEM_DYNAMIC_MAP_BINIDDATA)
	{
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_MAP)->m_szTagName = tValue.toByteArray().data();
	}
	else if (szID == ITEM_DYNAMIC_GROUP_MOVING_BINDDATA)
	{
		//组合移动
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_GROUP_MOVE)->m_szTagName = tValue.toByteArray().data();
	}
	else if (szID == ITEM_BACKGROUND_ITEM_IMAGE_IMAGE)
	{
		//图元背景图片
		m_pPorpWidget->GetImageInfo()->SetBackgroundImage(tValue.toString());

		//m_pPorpWidget->GetImageInfo()->SetImageSize(QSize(0, 0));
	}
	else if (szID == ITEM_CURVEPLOT_BURSH_COLOR)
	{
		//曲线图表 brush color
		if (m_pPorpWidget->GetWidgetType() == DRAW_ADDR_CURE_PLOT)
		{
			CAddedWidget *taddWgt = dynamic_cast<CAddedWidget*>(m_pPorpWidget);

			Q_ASSERT(taddWgt);
			if (taddWgt == nullptr)
			{
				return;
			}

			taddWgt->GetPlotWgt()->GetBrushInfo()->SetColor(tValue.value<QColor>());

			//直接刷新曲线样式
			taddWgt->GetPlotWgt()->UpdateChart();


		}

	}
	invalidateScene();

	//保存历史操作数据
// 	AddHistFiles();
	this->viewport()->update();

}

void CGraphView::valueChanged(QtProperty *property, const QFont &value)
{
	Q_UNUSED(property);
	Q_UNUSED(value);
}

void CGraphView::valueChanged(QtProperty *pProperty, const QPoint &value)
{
	Q_ASSERT(pProperty);
	if (pProperty == nullptr)
		return;

	if (m_mapPropertyToId.contains(pProperty) == false)
		return;

	if (m_pPorpWidget == nullptr)
	{
		int szID = m_mapPropertyToId[pProperty];

		setCacheMode(CacheNone);

		if (szID == ITEM_BACKGROUND_IMAGE_XY)
		{
			//背景坐标
			std::shared_ptr<CBackground> tBackground = GetGraphicsScene()->GetGraphFile()->GetBackground();
			tBackground->m_pfImage = value;
		}
		////////////////

		this->viewport()->update();

		//保存历史操作数据
// 		AddHistFiles();

		return;
	}

	int szID = m_mapPropertyToId[pProperty];
	if (szID == ITEM_PSOITION)
	{
		//大小
		m_pPorpWidget->MoveTo(value);
	}
	else if (szID == ITEM_BACKGROUND_ITEM_IMAGE_XY)
	{
		m_pPorpWidget->GetImageInfo()->SetImagePosition(value);
	}

	invalidateScene();

	this->viewport()->update();
}

void CGraphView::valueChanged(QtProperty *pProperty, const QSize &value)
{
	Q_ASSERT(pProperty);
	if (pProperty == nullptr)
		return;

	if (m_mapPropertyToId.contains(pProperty) == false)
		return;

	if (m_pPorpWidget == nullptr)
	{
		int szID = m_mapPropertyToId[pProperty];

		setCacheMode(CacheNone);

		if (szID == ITEM_BACKGROUND_IMAGE_SIZE)
		{
			//背景坐标
			std::shared_ptr<CBackground> tBackground = GetGraphicsScene()->GetGraphFile()->GetBackground();
			tBackground->m_szImage = value;
		}
		else if (szID == ITEM_GREPHICS_SIZE)
		{
			CGraphScene *pScene = GetGraphicsScene();
			Q_ASSERT(pScene);
			if (pScene)
			{
				CGraphFile *pFile = pScene->GetGraphFile();
				Q_ASSERT(pFile);
				if (pFile)
				{
					pFile->SetGraphSize(QSize(value));
					//
					pFile->SetGraphcsSize(QSize(value));
					this->scene()->setSceneRect(0, 0, value.width(), value.height());
					
				}

				this->scene()->update();
			}
		}
		////////////////

		this->viewport()->update();

		//保存历史操作数据
// 		AddHistFiles();

		return;
	}

	int szID = m_mapPropertyToId[pProperty];
	if (szID == ITEM_SIZE)
	{
		//大小
		m_pPorpWidget->MoveTo(QRectF(m_pPorpWidget->GetPosition().topLeft(),value));
	}
	else if (szID == ITEM_BACKGROUND_ITEM_IMAGE_SIZE)
	{
		//图元背景图大小
		m_pPorpWidget->GetImageInfo()->SetImageSize(value);
	}


	invalidateScene();

	this->viewport()->update();
}

void CGraphView::Slot_BoolValueChange(QtProperty *pProperty, bool bFlag)
{
	Q_ASSERT(pProperty);
	if (pProperty == nullptr)
	{
		return;
	}

	if (m_mapPropertyToId.contains(pProperty) == false)
	{
		return;
	}

	if (m_pPorpWidget == nullptr)
	{
		int szID = m_mapPropertyToId[pProperty];

		setCacheMode(CacheNone);

		if (szID == ITEM_BACKGROUND_IMAGE_TILING)
		{
			//平铺
			std::shared_ptr<CBackground> tBackground = GetGraphicsScene()->GetGraphFile()->GetBackground();

			tBackground->m_bTiling = bFlag;
			
		}
		else if (szID == ITEM_BACKGROUND_IMAGE_STRETCH)
		{
			//拉伸
			std::shared_ptr<CBackground> tBackground = GetGraphicsScene()->GetGraphFile()->GetBackground();

			tBackground->m_bStretch = bFlag;


			
		}

		////////////////

		this->viewport()->update();

		//保存历史操作数据
// 		AddHistFiles();

		return;
	}

	int szID = m_mapPropertyToId[pProperty];
	if (szID == ITEM_DYNAMIC_BRUSH_ENABLE)
	{
		//画刷启动
		//m_pPorpWidget->GetDynamicBrushInfo()->SetEnableFlag(bFlag);
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_FILLING)->m_bEnable = bFlag;
	}
	else if (szID == ITEM_LOCK)
	{
		//锁定
		m_pPorpWidget->SetLocked(bFlag);
	}
	else if (szID == ITEM_DYNAMIC_PEN_COLOR_ENABLE)
	{
		//画刷启动
		//m_pPorpWidget->GetDynamicBrushInfo()->SetEnableFlag(bFlag);
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_PEN_COLOR)->m_bEnable = bFlag;
	}
	else if (szID == ITEM_DYNAMIC_TEXT_COLOR_ENABLE)
	{
		//画刷启动
		//m_pPorpWidget->GetDynamicBrushInfo()->SetEnableFlag(bFlag);
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_TEXT_COLOR)->m_bEnable = bFlag;
	}
	else if (szID == ITEM_DYNAMIC_HORIZONTAL_ENABLE)
	{
		//水平移动启动
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_HOR_MOVE)->m_bEnable = bFlag;
	}
	else if (szID == ITEM_DYNAMIC_HORIZONTAL_RESVESE)
	{
		//水平移动反转启动
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_HOR_MOVE);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetBackFlag(bFlag);
	}
	else if (szID == ITEM_DYNAMIC_VERTICAL_ENABLE)
	{
		//竖直移动启动
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VIR_MOVE)->m_bEnable = bFlag;
	}
	else if (szID == ITEM_DYNAMIC_VERTICAL_RESVESE)
	{
		//竖直移动反转启动
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VIR_MOVE);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetBackFlag(bFlag);
	}
	else if (szID == ITEM_DYNAMIC_GROUP_MOVING_ENABLE)
	{
		//组合移动启动
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_GROUP_MOVE)->m_bEnable = bFlag;
	}
	else if (szID == ITEM_DYNAMIC_GROUP_MOVING_EDIT)
	{
		//组合移动编辑状态  
		if (bFlag)
		{
			m_bViewEdable = false;
			//加载点
			InsertGroupMovePoints();
		}
		else
		{
			m_bViewEdable = true;
			//去除点
			DeleteGroupMovePoints();

		}

	}
	else if (szID == ITEM_DYNAMIC_GROUP_MOVING_RESET)
	{
		//组合移动 复位  
		DeleteGroupMovePoints();

		if (m_pPorpWidget != nullptr)
		{

			CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_GROUP_MOVE);
			CDyncGroupMovingData *pGroupMoveData = dynamic_cast<CDyncGroupMovingData*>(pbaseDync);

			Q_ASSERT(pGroupMoveData);
			if (pGroupMoveData == nullptr)
			{
				return;
			}

			pGroupMoveData->ClearAllPoints();

		}

	}
	else if (szID == ITEM_DYNAMIC_ROTATE_ENABLE)
	{
		//旋转启动
		m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_ROTATION)->m_bEnable = bFlag;
	}
	else if (szID == ITEM_DYNAMIC_ROTATE_RESVESE)
	{
		//旋转反转启动
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_ROTATION);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		pMovingData->SetBackFlag(bFlag);
	}
	else if (szID == ITEM_DYNAMIC_TRANSPARENCY_ENABLE)
	{
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_TRANSPARENCY);
		Q_ASSERT(pbaseDync);

		if (pbaseDync == nullptr)
		{
			return;
		}

		pbaseDync->m_bEnable = bFlag;
	}
	else if (szID == ITEM_DYNAMIC_VISABLE_ENABLE)
	{
		//可见  
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VISIBLE);
		Q_ASSERT(pbaseDync);

		if (pbaseDync == nullptr)
		{
			return;
		}

		pbaseDync->m_bEnable = bFlag;
	}
	else if (szID == ITEM_DYNAMIC_VALUE_ENABLE)
	{
		//动态数值  
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VALUE);
		Q_ASSERT(pbaseDync);

		if (pbaseDync == nullptr)
		{
			return;
		}

		pbaseDync->m_bEnable = bFlag;

	}
	else if (szID == ITEM_DYNAMIC_BRUSH_ANALOG_COLOR)
	{
		//动态画刷模拟色
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_FILLING);
		CDyncFillingData *pFillData = dynamic_cast<CDyncFillingData*>(pbaseDync);

		Q_ASSERT(pFillData);
		if (pFillData == nullptr)
		{
			return;
		}

		pFillData->m_bAnalogColor = bFlag;

	}
	else if (szID == ITEM_DYNAMIC_TEXT_ENABLE)
	{
		//是否启用  动态文本  
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_STRING);
		Q_ASSERT(pbaseDync);

		if (pbaseDync == nullptr)
		{
			return;
		}

		pbaseDync->m_bEnable = bFlag;

	}
	else if (szID == ITEM_DYNAMIC_MAP_ENABLE)
	{
		//是否启用  动态图片  
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_MAP);
		Q_ASSERT(pbaseDync);

		if (pbaseDync == nullptr)
		{
			return;
		}

		pbaseDync->m_bEnable = bFlag;

	}
	else if (szID == ITEM_BACKGROUND_ITEM_IMAGE_TILING)
	{
		//图元图片  平铺
		m_pPorpWidget->GetImageInfo()->SetbTiling(bFlag);

	}
	else if (szID == ITEM_BACKGROUND_ITEM_IMAGE_STRETCH)
	{
		//图元图片拉伸
		m_pPorpWidget->GetImageInfo()->SetStretch(bFlag);
	}

	invalidateScene();		

	//保存历史操作数据
// 	AddHistFiles();
	this->viewport()->update();

}


void CGraphView::paintEvent(QPaintEvent *event)
{
    if (viewport()->width() != m_nViewPortWidth || viewport()->height() != m_nViewPortHeight)
    {
        //当视图大小发送变化时,需要重绘背景
        m_nViewPortWidth = viewport()->width();
        m_nViewPortHeight = viewport()->height();

        setCacheMode(CacheNone);
    }
    else
    {
        //当没有变化时
        setCacheMode(CacheBackground);
    }
    QGraphicsView::paintEvent(event);
}

QPointF CGraphView::GetMutlStartPoint()
{
    QPointF pfStart;
    double dStartX = 0;
    double dStartY = 0;

    for each (CBaseWidget * baseWgt in m_pArrCpyCutWgt)
    {
        Q_ASSERT(baseWgt);

        if (baseWgt == nullptr)
        {
            return pfStart;
        }
        if (baseWgt->GetPosition().x() > dStartX)
        {
            dStartX = baseWgt->GetPosition().x();
        }

        if (baseWgt->GetPosition().y() > dStartY)
        {
            dStartY = baseWgt->GetPosition().y();
        }
    }

    pfStart.setX(dStartX);
    pfStart.setY(dStartY);

    return pfStart;
}

void CGraphView::Slot_ScrollBarChange()
{
    setCacheMode(CacheNone);
}

void CGraphView::Slot_ShowItemProperty()
{
	OnAttr();
	ChangeProperty();

	IMainWindow *pMainFrm = GetMainWindow();
	Q_ASSERT(pMainFrm);
	if (pMainFrm == nullptr)
		return;
	pMainFrm->GetRightDock()->raise();
}

void CGraphView::ChangeProperty()
{
	CGraphScene *pScene = GetGraphicsScene();
	Q_ASSERT(pScene);
	if (pScene == nullptr)
		return;

	CGraphFile * pGraphFile = pScene->GetGraphFile();
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	auto pLayer = pGraphFile->GetLayer(m_nCurEditingLayerIndex);
	Q_ASSERT(pLayer != nullptr);
	if (pLayer == nullptr)
		return;

	if (pLayer->IsSelectionEmpty() == true)
	{
		if (m_mapIdToProperty.contains(ITEM_GREPHICS_SIZE) && m_mapIdToProperty.contains(ITEM_BACKGROUND)
			&& m_mapIdToProperty.contains(ITEM_BACKGROUND_COLOR_COLOR))
		{
			//视图属性

			std::shared_ptr<CBackground> tBackground = pGraphFile->GetBackground();
			//大小
			m_pSizeManager->setValue(m_mapIdToProperty[ITEM_GREPHICS_SIZE], QSize(pGraphFile->GetSize().second, pGraphFile->GetSize().first));
			//缩放比例
			m_pIntManager->setValue(m_mapIdToProperty[ITEM_GRAPHICS_SCALE], pGraphFile->GetSacled());
			//颜色
			m_pColorManager->setValue(m_mapIdToProperty[ITEM_BACKGROUND_COLOR_COLOR], tBackground->m_clrBackground);
			//画刷类型
			m_pIntManager->setValue(m_mapIdToProperty[ITEM_BACKGROUND_COLOR_TYPE], tBackground->m_nBrushStyle);
			//渐变类型
			m_pIntManager->setValue(m_mapIdToProperty[ITEM_BACKGROUND_COLOR_GRADIENT_TYPE], tBackground->m_nGradiendMode);

			//背景图片
			m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_BACKGROUND_IMAGE_IMAGE], tBackground->m_strBackgroundImage);

			//背景图片位置
			m_pPointManager->setValue(m_mapIdToProperty[ITEM_BACKGROUND_IMAGE_XY], tBackground->m_pfImage);
			//背景图片大小
			m_pSizeManager->setValue(m_mapIdToProperty[ITEM_BACKGROUND_IMAGE_SIZE], tBackground->m_szImage);
			//拉伸
			m_pBoolManager->setValue(m_mapIdToProperty[ITEM_BACKGROUND_IMAGE_STRETCH], tBackground->m_bStretch);
			//平铺
			m_pBoolManager->setValue(m_mapIdToProperty[ITEM_BACKGROUND_IMAGE_TILING], tBackground->m_bTiling);

			//AddHistFiles();
		}
		return;
	}

	CBaseWidget *pWidget = pLayer->GetFirstSelectedWidget();
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return;


	if (m_mapIdToProperty.contains(ITEM_CUREVEPLOT_BRUSH_STYLE) && m_mapIdToProperty.contains(ITEM_CURVEPLOT_BURSH_COLOR))
	{
		//曲线图属性
		if (pWidget->GetWidgetType() == DRAW_ADDR_CURE_PLOT)
		{
			CAddedWidget *taddWgt = dynamic_cast<CAddedWidget*>(pWidget);

			Q_ASSERT(taddWgt);
			if (taddWgt == nullptr)
			{
				return;
			}

			m_pComBoProperty->setValue(m_mapIdToProperty[ITEM_CUREVEPLOT_BRUSH_STYLE], taddWgt->GetPlotWgt()->GetBrushInfo()->GetStyle());

			m_pColorManager->setValue(m_mapIdToProperty[ITEM_CURVEPLOT_BURSH_COLOR], taddWgt->GetPlotWgt()->GetBrushInfo()->GetColor());

			m_pIntManager->setValue(m_mapIdToProperty[ITEM_CURVEFRAM_WIDTH], taddWgt->GetInterval());


		}

	}

	if (m_mapIdToProperty.contains(ITEM_PSOITION) && m_mapIdToProperty.contains(ITEM_SIZE)
		&& m_mapIdToProperty.contains(ITEM_ROTATE) && m_mapIdToProperty.contains(ITEM_ROTATE) && m_mapIdToProperty.contains(ITEM_ROTATE)
		&& m_mapIdToProperty.contains(ITEM_PEN_COLOR) && m_mapIdToProperty.contains(ITEM_PEN_WIDTH) && m_mapIdToProperty.contains(ITEM_PEN_TYPE)
		&& m_mapIdToProperty.contains(ITEM_BRUSH_TYPE) && m_mapIdToProperty.contains(ITEM_BURSH_COLOR) && m_mapIdToProperty.contains(ITEM_BRUSH_GRADIENT)
		&& m_mapIdToProperty.contains(ITEM_BRUSH_ENDGRADIENTCOLOR) && m_mapIdToProperty.contains(ITEM_TEXT_COLOR) && m_mapIdToProperty.contains(ITEM_TEXT)
		&& m_mapIdToProperty.contains(ITEM_TEXT_FONT) && m_mapIdToProperty.contains(ITEM_TEXT_POSITION))
	{
		//名称
		m_pStringManager->setValue(m_mapIdToProperty[ITEM_NAME], QString::fromLocal8Bit(pWidget->GetWidgetName().c_str()));
		//ID
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_ID], pWidget->GetID());
		//锁定
		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_LOCK], pWidget->IsLocked());
		//位置
		m_pPointManager->setValue(m_mapIdToProperty[ITEM_PSOITION], pWidget->GetPosition().topLeft().toPoint());
		//大小
		m_pSizeManager->setValue(m_mapIdToProperty[ITEM_SIZE], pWidget->GetPosition().size().toSize());
		//旋转角度
		m_pDoubleManager->setValue(m_mapIdToProperty[ITEM_ROTATE], pWidget->GetRotateAngle());
		//弧度结束角度
		m_pDoubleManager->setValue(m_mapIdToProperty[ITEM_END_ANGLE], pWidget->GetEndAngle());

		//画笔颜色
		m_pColorManager->setValue(m_mapIdToProperty[ITEM_PEN_COLOR], pWidget->GetPenInfo()->GetColor());
		//边框宽度
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_PEN_WIDTH], pWidget->GetPenInfo()->GetWidth());
		//边框类型
		m_pComBoProperty->setValue(m_mapIdToProperty[ITEM_PEN_TYPE], pWidget->GetPenInfo()->GetStyle());

		//画刷类型
		m_pComBoProperty->setValue(m_mapIdToProperty[ITEM_BRUSH_TYPE], pWidget->GetBrushInfo()->GetStyle());
		//画刷颜色
		m_pColorManager->setValue(m_mapIdToProperty[ITEM_BURSH_COLOR], pWidget->GetBrushInfo()->GetColor());
		//画刷渐变类型
		m_pComBoProperty->setValue(m_mapIdToProperty[ITEM_BRUSH_GRADIENT], pWidget->GetBrushInfo()->GetGradiendMode());
		//画刷渐变终止颜色
		m_pColorManager->setValue(m_mapIdToProperty[ITEM_BRUSH_ENDGRADIENTCOLOR], pWidget->GetBrushInfo()->GetEndColor());

		//字体颜色
		m_pColorManager->setValue(m_mapIdToProperty[ITEM_TEXT_COLOR], pWidget->GetFontInfo()->GetColor());
		m_pColorManager->setValue(m_mapIdToProperty[ITEM_TEXT_FONT], pWidget->GetFontInfo()->GetFont());
		m_pComBoProperty->setValue(m_mapIdToProperty[ITEM_TEXT_POSITION], pWidget->GetFontInfo()->GetFontLayout());
		m_pStringManager->setValue(m_mapIdToProperty[ITEM_TEXT], pWidget->GetText());

		//旋转中西
		m_pComBoProperty->setValue(m_mapIdToProperty[ITEM_ROTATE_CENTER], pWidget->GetRotateCenter());
	}


	if (m_mapIdToProperty.contains(ITEM_DYNAMIC_BRUSH_ENABLE) && m_mapIdToProperty.contains(ITEM_DYNAMIC_BRUSH_BINDPOINT))
	{
		//
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_FILLING);
		CDyncFillingData *pFillingData = dynamic_cast<CDyncFillingData*>(pbaseDync);

		Q_ASSERT(pFillingData);
		if (pFillingData == nullptr)
		{
			return;
		}
		//动态画刷是否启动
		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_BRUSH_ENABLE], pFillingData->m_bEnable);

		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_BRUSH_BINDPOINT], pFillingData->m_szTagName.c_str());

		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_BRUSH_COLOR], reinterpret_cast<long long>(pWidget->CreateDynamicPro(CBaseDyncData::DYNC_FILLING)));

		//模拟色
		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_BRUSH_ANALOG_COLOR], pFillingData->m_bAnalogColor);
		//起始值
		m_pDoubleManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_BRUSH_START_VALUE], pFillingData->m_dMinVal);
		//结束值
		m_pDoubleManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_BRUSH_END_VALUE], pFillingData->m_dMaxVal);
		//起始偏移
		m_pDoubleManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_BRUSH_START_OFFSET], pFillingData->m_dStartPos);
		//终止偏移
		m_pDoubleManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_BRUSH_END_OFFSET], pFillingData->m_dEndPos);
		//模拟色类型
		m_pComBoProperty->setValue(m_mapIdToProperty[ITEM_DYNAMIC_BRUSH_TYPE], pFillingData->m_nFillingPolicy);
	}

	if (m_mapIdToProperty.contains(ITEM_DYNAMIC_PEN_COLOR_ENABLE) && m_mapIdToProperty.contains(ITEM_DYNAMIC_PEN_COLOR_BINDATA))
	{
		//动态pen是否启动
		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_PEN_COLOR_ENABLE], pWidget->CreateDynamicPro(CBaseDyncData::DYNC_PEN_COLOR)->m_bEnable);

		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_PEN_COLOR_BINDATA], pWidget->CreateDynamicPro(CBaseDyncData::DYNC_PEN_COLOR)->m_szTagName.c_str());

		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_PEN_COLOR_COLOR], reinterpret_cast<long long>(pWidget->CreateDynamicPro(CBaseDyncData::DYNC_PEN_COLOR)));
	}

	if (m_mapIdToProperty.contains(ITEM_DYNAMIC_TEXT_COLOR_ENABLE) && m_mapIdToProperty.contains(ITEM_DYNAMIC_TEXT_COLOR_BINDATA))
	{
		//动态Text是否启动
		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_TEXT_COLOR_ENABLE], pWidget->CreateDynamicPro(CBaseDyncData::DYNC_TEXT_COLOR)->m_bEnable);

		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_TEXT_COLOR_BINDATA], pWidget->CreateDynamicPro(CBaseDyncData::DYNC_TEXT_COLOR)->m_szTagName.c_str());

		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_TEXT_COLOR_COLOR], reinterpret_cast<long long>(pWidget->CreateDynamicPro(CBaseDyncData::DYNC_TEXT_COLOR)));
	}

	if (m_mapIdToProperty.contains(ITEM_DYNAMIC_HORIZONTAL_ENABLE) && m_mapIdToProperty.contains(ITEM_DYNAMIC_HORIZONTAL_RESVESE) && m_mapIdToProperty.contains(ITEM_DYNAMIC_HORIZONTAL_BINDDATA)
		&& m_mapIdToProperty.contains(ITEM_DYNAMIC_HORIZONTAL_START) && m_mapIdToProperty.contains(ITEM_DYNAMIC_HORIZONTAL_END) && m_mapIdToProperty.contains(ITEM_DYNAMIC_HORIZONTAL_STARTOFFSET)
		&& m_mapIdToProperty.contains(ITEM_DYNAMIC_HORIZONTAL_ENDOFFSET) && m_mapIdToProperty.contains(ITEM_DYNAMIC_VERTICAL_ENABLE) && m_mapIdToProperty.contains(ITEM_DYNAMIC_VERTICAL_RESVESE)
		&& m_mapIdToProperty.contains(ITEM_DYNAMIC_VERTICAL_BINDDATA) && m_mapIdToProperty.contains(ITEM_DYNAMIC_VERTICAL_START) && m_mapIdToProperty.contains(ITEM_DYNAMIC_VERTICAL_END))
	{
		//竖直移动反转启动
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_HOR_MOVE);
		CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		//移动
		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_HORIZONTAL_ENABLE], pMovingData->m_bEnable);
		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_HORIZONTAL_RESVESE], pMovingData->GetBackFlag());
		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_HORIZONTAL_BINDDATA], pMovingData->m_szTagName.c_str());
		//
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_HORIZONTAL_START], pMovingData->GetStartData());
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_HORIZONTAL_END], pMovingData->GetEndData());
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_HORIZONTAL_STARTOFFSET], pMovingData->GetStartOffset());
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_HORIZONTAL_ENDOFFSET], pMovingData->GetEndOffset());
		m_pDoubleManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_HORIZONTAL_DISTANCE],pMovingData->GetDistance());


		//竖直移动反转启动
		pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VIR_MOVE);
		pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}
		//移动
		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VERTICAL_ENABLE], pMovingData->m_bEnable);
		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VERTICAL_RESVESE], pMovingData->GetBackFlag());
		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VERTICAL_BINDDATA], pMovingData->m_szTagName.c_str());
		//
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VERTICAL_START], pMovingData->GetStartData());
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VERTICAL_END], pMovingData->GetEndData());
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VERTICAL_STARTOFFSET], pMovingData->GetStartOffset());
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VERTICAL_ENDOFFSET], pMovingData->GetEndOffset());
		m_pDoubleManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VERTICAL_DISTANCE], pMovingData->GetDistance());

		//竖直移动反转启动
		pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_ROTATION);
		pMovingData = dynamic_cast<CDyncMovingData*>(pbaseDync);

		Q_ASSERT(pMovingData);
		if (pMovingData == nullptr)
		{
			return;
		}

		//移动
		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_ROTATE_ENABLE], pMovingData->m_bEnable);
		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_ROTATE_RESVESE], pMovingData->GetBackFlag());
		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_ROTATE_BINDDATA], pMovingData->m_szTagName.c_str());
		//
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_ROTATE_START], pMovingData->GetStartData());
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_ROTATE_END], pMovingData->GetEndData());
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_ROTATE_STARTOFFSET], pMovingData->GetStartOffset());
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_ROTATE_ENDOFFSET], pMovingData->GetEndOffset());
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_ROTATE_STARTANGLE], pMovingData->GetStartAngle());
		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_ROTATE_ENDANGLE], pMovingData->GetEndAngel());
	}

	if (m_mapIdToProperty.contains(ITEM_DYNAMIC_TRANSPARENCY_ENABLE) && m_mapIdToProperty.contains(ITEM_DYNAMIC_TRANSPARENCY_BINDDATA) 
		&& m_mapIdToProperty.contains(ITEM_DYNAMIC_TRANSPARENCY_START) && m_mapIdToProperty.contains(ITEM_DYNAMIC_TRANSPARENCY_END))
	{
		//动态画刷是否启动
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_TRANSPARENCY);
		CDyncTransparencyData *pTransparencyData = dynamic_cast<CDyncTransparencyData*>(pbaseDync);

		Q_ASSERT(pTransparencyData);
		if (pTransparencyData == nullptr)
		{
			return;
		}

		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_TRANSPARENCY_ENABLE], pTransparencyData->m_bEnable);

		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_TRANSPARENCY_BINDDATA], pTransparencyData->m_szTagName.c_str());

		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_TRANSPARENCY_START], pTransparencyData->GetMinValue());

		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_TRANSPARENCY_END], pTransparencyData->GetMaxValue());

	}

	if (m_mapIdToProperty.contains(ITEM_DYNAMIC_VISABLE_ENABLE) && m_mapIdToProperty.contains(ITEM_DYNAMIC_VISABLE_BINDDATA)
		&& m_mapIdToProperty.contains(ITEM_DYNAMIC_VISAVLE_VALUE) && m_mapIdToProperty.contains(ITEM_DYNAMCI_VISABLE_AREA))
	{
		//可见
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VISIBLE);
		CDyncVisibleData *pVisableData = dynamic_cast<CDyncVisibleData*>(pbaseDync);

		Q_ASSERT(pVisableData);
		if (pVisableData == nullptr)
		{
			return;
		}

		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VISABLE_ENABLE], pVisableData->m_bEnable);

		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VISABLE_BINDDATA], pVisableData->m_szTagName.c_str());

		m_pDoubleManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VISAVLE_VALUE], pVisableData->GetVisableValue());
		m_pDoubleManager->setValue(m_mapIdToProperty[ITEM_DYNAMCI_VISABLE_AREA], pVisableData->GetDeadBand());


		m_pComBoProperty->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VISABLE_COMPARE], pVisableData->GetCondition());

		m_pComBoProperty->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VISABLE_FLASHTYPE], pVisableData->GetFlashType());



		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VISABLE_CYCLE], pVisableData->GetFlashTime());

	}

	if (m_mapIdToProperty.contains(ITEM_DYNAMIC_VALUE_ENABLE) && m_mapIdToProperty.contains(ITEM_DYNAMIC_VALUE_BINDDATA)
		&& m_mapIdToProperty.contains(ITEM_DYNAMIC_VALUE_VALUE_FORMAT))
	{
		//动态数值
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_VALUE);
		CDyncValueData *pValueData = dynamic_cast<CDyncValueData*>(pbaseDync);

		Q_ASSERT(pValueData);
		if (pValueData == nullptr)
		{
			return;
		}

		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VALUE_ENABLE], pValueData->m_bEnable);

		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VALUE_BINDDATA], pValueData->m_szTagName.c_str());

		m_pComBoProperty->setValue(m_mapIdToProperty[ITEM_DYNAMIC_VALUE_VALUE_FORMAT], pValueData->GetFormat());
	}

	if (m_mapIdToProperty.contains(ITEM_DYNAMIC_TEXT_ENABLE) && m_mapIdToProperty.contains(ITEM_DYNAMIC_TEXT_BINDDATA)
		&& m_mapIdToProperty.contains(ITEM_DYNAMIC_TEXT_VALUES))
	{
		//动态文本
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_STRING);
		CDyncText *pValueData = dynamic_cast<CDyncText*>(pbaseDync);

		Q_ASSERT(pValueData);
		if (pValueData == nullptr)
		{
			return;
		}

		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_TEXT_ENABLE], pValueData->m_bEnable);

		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_TEXT_BINDDATA], pValueData->m_szTagName.c_str());

		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_TEXT_VALUES], reinterpret_cast<long long>(pWidget->CreateDynamicPro(CBaseDyncData::DYNC_STRING)));
	}

	if (m_mapIdToProperty.contains(ITEM_DYNAMIC_MAP_ENABLE) && m_mapIdToProperty.contains(ITEM_DYNAMIC_MAP_BINIDDATA)
		&& m_mapIdToProperty.contains(ITEM_DYNAMIC_MAP_VALUES))
	{
		//动态 图片
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_MAP);
		CDyncText *pValueData = dynamic_cast<CDyncText*>(pbaseDync);

		Q_ASSERT(pValueData);
		if (pValueData == nullptr)
		{
			return;
		}

		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_MAP_ENABLE], pValueData->m_bEnable);

		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_MAP_BINIDDATA], pValueData->m_szTagName.c_str());

		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_MAP_VALUES], reinterpret_cast<long long>(pWidget->CreateDynamicPro(CBaseDyncData::DYNC_MAP)));
	}

	//图元背景图片属性
	if (m_mapIdToProperty.contains(ITEM_BACKGROUND_ITEM_IAMGE) && m_mapIdToProperty.contains(ITEM_BACKGROUND_ITEM_IMAGE_IMAGE)
		&& m_mapIdToProperty.contains(ITEM_BACKGROUND_ITEM_IMAGE_XY))
	{
		CImageInfo *pImageInfo = m_pPorpWidget->GetImageInfo();
		Q_ASSERT(pImageInfo);
		if (pImageInfo == nullptr)
		{
			return;
		}

		//背景图片
		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_BACKGROUND_ITEM_IMAGE_IMAGE], pImageInfo->GetBackgroundImage());

		//背景图片位置
		m_pPointManager->setValue(m_mapIdToProperty[ITEM_BACKGROUND_ITEM_IMAGE_XY], pImageInfo->GetImagePosition());
		//背景图片大小
		m_pSizeManager->setValue(m_mapIdToProperty[ITEM_BACKGROUND_ITEM_IMAGE_SIZE], pImageInfo->GetImageSize());
		//拉伸
		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_BACKGROUND_ITEM_IMAGE_TILING], pImageInfo->GetbTiling());
		//平铺
		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_BACKGROUND_ITEM_IMAGE_STRETCH], pImageInfo->GetStretch());
	}

	if (m_mapIdToProperty.contains(ITEM_DYNAMIC_GROUP_MOVING_ENABLE) && m_mapIdToProperty.contains(ITEM_DYNAMIC_GROUP_MOVING_BINDDATA)
		&& m_mapIdToProperty.contains(ITEM_DYNAMIC_GROUP_MOVING_START))
	{
		//组合移动
		CBaseDyncData *pbaseDync = m_pPorpWidget->CreateDynamicPro(CBaseDyncData::DYNC_GROUP_MOVE);
		CDyncGroupMovingData *pValueData = dynamic_cast<CDyncGroupMovingData*>(pbaseDync);

		Q_ASSERT(pValueData);
		if (pValueData == nullptr)
		{
			return;
		}

		m_pBoolManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_GROUP_MOVING_ENABLE], pValueData->m_bEnable);

		m_pBindInfoManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_GROUP_MOVING_BINDDATA], pValueData->m_szTagName.c_str());

		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_GROUP_MOVING_START], pValueData->GetStartData());

		m_pIntManager->setValue(m_mapIdToProperty[ITEM_DYNAMIC_GROUP_MOVING_END], pValueData->GetEndData());

		//挂一个下面
		SetGraphicAttrHidden();

		//AddHistFiles();
	}


}
/** @}*/
