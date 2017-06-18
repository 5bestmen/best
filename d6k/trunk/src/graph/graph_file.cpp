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
*  @brief   图形文件.
*  @author  LiJin
*  @version 1.0
*  @date    2014.10.26
*******************************************************************************/

#include "graph_file.h"
#include "graph_folder.h"

#include "base_widget.h"
#include "background.h"
#include "graphics_layer.h"
#include "graphscene.h"

//#include "xml_serializer.h"
#include "graph_serializer.h"
#include "graph_serializer_factory.h"

#include <QFileInfo>

#include <QDebug>
#include <memory>
#include <QXmlStreamWriter>
#include <QtCore/QtMath>

CGraphFileVerInfo::CGraphFileVerInfo()
{
	m_nMajorVer = 1;
	m_nMinorVer = 0;
	m_nBuild = 0;

}

CGraphFileVerInfo::~CGraphFileVerInfo()
{

}

/*! \fn bool CGraphFileVerInfo::SaveFile(QXmlStreamWriter *pWriter) const
********************************************************************************************************* 
** \brief CGraphFileVerInfo::SaveFile 
** \details 保存文件头
** \param pWriter 
** \return bool 
** \author LiJin 
** \date 2016年4月5日 
** \note 
********************************************************************************************************/
bool CGraphFileVerInfo::SaveFile(QXmlStreamWriter *pWriter) const
{
	Q_ASSERT(pWriter != nullptr);
	if (pWriter == nullptr)
		return false;

	pWriter->writeStartElement("FileInfo");

	{
		pWriter->writeStartElement("Version");
		pWriter->writeAttribute("MajorVersion", QString::number(m_nMajorVer));
		pWriter->writeAttribute("MinorVersion", QString::number(m_nMinorVer));
		pWriter->writeAttribute("Build", QString::number(m_nBuild));
		pWriter->writeEndElement();

	//	pWriter->writeStartElement("User");
	

		pWriter->writeEndElement();
	}
	pWriter->writeEndElement();

	return true;
}


  
CGraphFile::CGraphFile()
{
	m_pScene = nullptr;
	m_pFolder = nullptr;
	m_nMaxWidgetID = 0;
	m_pScene = nullptr;
	m_szGraphName = "图形1";
	m_bFlashFlag = false;
	m_scaleFactor = 50;

	m_Size = QSize(1920, 1200);
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
	m_bVaildFlag = true;

}

CGraphFile::CGraphFile(CGraphScene *pScene) :m_pScene(pScene)
{
	m_pFolder = nullptr;
	m_nMaxWidgetID = 0;
	m_bFlashFlag = false;
	m_scaleFactor = 50;

	m_Size = QSize(1400, 800);
	m_szGraphName = "画面1";

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

	m_bVaildFlag = true;

}

CGraphFile::CGraphFile(CGraphFolder *pFolder)
{
	Q_UNUSED(pFolder);
	m_bFlashFlag = false;
	m_Size = QSize(1600, 800);
	// 建立图层
	unsigned int nIdx = 1;
	m_scaleFactor = 50;
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
// 	if (m_pScene != nullptr)
// 	{
// 		m_pScene->deleteLater();
// 	}
}

//重写赋值函数 
const CGraphFile& CGraphFile::operator=(const CGraphFile& src)
{
	for (int i=0; i<MAX_LAYER_CNT; i++)
	{
		//
		*m_arrLayers[i] = *(src.GetLayer(i+1));
	}

	//TODO...................................

	return *this;
}

void CGraphFile::SetGraphSize(QSize &sizeGraph)
{
	Q_ASSERT(sizeGraph.height() && sizeGraph.width());

	if (sizeGraph.height() == 0 && sizeGraph.width() == 0)
	{
		return;
	}

	if (m_pBackground)
	{
		m_pBackground->SetSize(sizeGraph);
	}

	for (auto i : m_arrLayers)
	{
	 	Q_ASSERT(i != nullptr);
	 	if (i != nullptr)
		{
		 	//i->SetGraphSize(sizeGraph);
		}
	}
}

void CGraphFile::SetGraphcsSize(QSize &sizeGraph)
{
	for (auto i : m_arrLayers)
	{
		Q_ASSERT(i != nullptr);
		if (i != nullptr)
		{
			i->SetGraphSize(sizeGraph);
		}
	}

	m_Size = sizeGraph;
}

QRectF CGraphFile::GetGraphRect()const
{
	if (m_pBackground)
	{
		return m_pBackground->GetBackgroundRect();
	}
	QRectF rcNull(0,0,0,0);
	return rcNull;
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
** \date 2016年1月18日   m
** \note 
********************************************************************************************************/
void CGraphFile::AddWidget(unsigned int nLayerIdx, CBaseWidget *pObj)
{
	Q_ASSERT(pObj);
	if (pObj == nullptr)
		return;
	connect(pObj, &CBaseWidget::UpdateItemRect, this, &CGraphFile::slot_on_updateScene);
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

//命名
void CGraphFile::RenameWidgetItem(unsigned int nLayerIdx, CBaseWidget *pObj)
{
	Q_ASSERT(pObj);
	if (pObj == nullptr)
		return;
	connect(pObj, &CBaseWidget::UpdateItemRect, this, &CGraphFile::slot_on_updateScene);
	Q_ASSERT(nLayerIdx > 0 && nLayerIdx <= MAX_LAYER_CNT);
	if (nLayerIdx > MAX_LAYER_CNT)
		return;

	std::string szFolderName;
	if (m_pFolder)
	{
		szFolderName = m_pFolder->GetFolderName();
	}

	pObj->BuildWidgetName(++m_nMaxWidgetID, nLayerIdx, m_szGraphName, szFolderName);
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

std::shared_ptr<CGraphicsLayer> CGraphFile::GetLayer(unsigned int nIdx) const
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

void CGraphFile::ClearLayer()
{
	for each (std::shared_ptr<CGraphicsLayer> var in m_arrLayers)
	{
		var->ClearBaseWgts();
		m_pScene = nullptr;
	}
}

/*! \fn bool CGraphFile::SaveFile(const QString &szFilePath)
*********************************************************************************************************
** \brief CGraphFile::SaveFile
** \details 保存图形文件
** \param szFileName
** \return bool
** \author LiJin
** \date 2016年2月19日
** \note
********************************************************************************************************/
bool CGraphFile::SaveFile(const QString &szFilePath) 
{ 
	Q_UNUSED(szFilePath);
	if (GetFilePath().isEmpty())
		return false;

	Q_ASSERT(m_szGraphName.empty() == false);
	if (m_szGraphName.empty() == true)
		return false;

// 	QString szNewFileName = szFilePath + ("//");
// 	szNewFileName.append( QString::fromLocal8Bit((char*)(m_szGraphName).c_str()) );
// 
// 	QFileInfo nInfo(szNewFileName);
// 	if (nInfo.suffix() != "pic")
// 	{
// 		szNewFileName += ".pic";
// 	}

	CGraphSerializer *pXmlWriter = CGraphSerializerFactory::CreateSerializer(CGraphSerializerFactory::XML_TYPE);
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter)
	{	
		bool bRet = pXmlWriter->SaveToFile(this, GetFilePath());
		if (bRet == false)
		{ // todo -> log

		}
		else
		{

		}

		CGraphSerializerFactory::DestroySerializer(pXmlWriter);
	}

	// 	views().first()->setWindowTitle(fileName);
	// 	m_fileName = fileName;
	// 	m_sceneDirty = FALSE;
	return true;
}
//加载图形文件
bool CGraphFile::LoadFile(const QString &szFileName)
{
	
	if (szFileName.isEmpty())
	{
		return false;
	}

	CGraphSerializer *pXmlWriter = CGraphSerializerFactory::CreateSerializer(CGraphSerializerFactory::XML_TYPE);
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter)
	{
		bool bRet = pXmlWriter->LoadFromFile(szFileName,this);
		if (bRet == false)
		{ // todo -> log

		}
		else
		{

		}

		CGraphSerializerFactory::DestroySerializer(pXmlWriter);
	}
	return true;
}

void CGraphFile::slot_on_updateScene(CBaseWidget* pObj, const  QRectF& rect, const QRectF& rect1)
{
	Q_ASSERT(pObj);
	if (!pObj)
	{
		return;
	}
	if (!rect.isValid() ||!rect1.isValid())
	{
		return ;
	}
	if (rect.width()==0 && rect.height()==0 )
	{
		return;
	}
	if (!m_pScene)
	{
		return;
	}
	if (m_pScene)
	{
		if (rect==rect1)
		{
			m_pScene->update(rect);
		}
		else
		{
		//	QRectF rectf =  QRectF(pObj->GetOldPosition().topLeft(), QPointF(rect.bottomRight().x(), rect.bottomRight().y()));
			QRectF rectf = GetNewRect(rect, pObj->GetOldPosition());
			m_pScene->update(rectf);
		}		
	}
}
QRectF CGraphFile::GetNewRect(const QRectF& rect1, const QRectF& rect2)
{	
	//包含情况
	if (rect1.contains(rect2))
	{
		return rect1;
	}
	if (rect2.contains(rect1))
	{
		return rect2;
	}
	//不包含或者部分重叠情况
	QRectF valRect;
	//1<<rectangle1在rectangle2的左侧（两矩形大小一致）  左上
	if (rect1.topLeft().x()<=rect2.topLeft().x()  && rect1.topLeft().y()<=rect2.topLeft().y())
	{
		 valRect=QRectF(rect1.topLeft(),rect2.bottomRight());
	}
	//2<<rectangle1在rectangle2的左侧（两矩形大小一致）  左下
	if (rect1.topLeft().x() < rect2.topLeft().x() && rect1.topLeft().y() > rect2.topLeft().y())
	{
		 valRect=QRectF(QPointF(rect1.topLeft().x(),rect2.topLeft().y()),QPointF(rect2.bottomRight().x(),rect1.bottomRight().y()));
	}
	//3<<rectangle1在rectangle2的右侧（两矩形大小一致）  右上
	if (rect1.topLeft().x() >= rect2.topLeft().x() && rect1.topLeft().y() <= rect2.topLeft().y())
	{
		 valRect=QRectF(QPointF(rect2.topLeft().x(), rect1.topLeft().y()), QPointF(rect1.bottomRight().x(), rect2.bottomRight().y()));
	}
	//4<<rectangle1在rectangle2的右侧（两矩形大小一致）  右下
	if (rect1.topLeft().x() > rect2.topLeft().x() && rect1.topLeft().y() > rect2.topLeft().y())
	{
		  valRect=QRectF(rect2.topLeft(), rect1.bottomRight());
	}
	//按照实际情况，边界范围需要扩大（√2 -1）*2
	qreal dx = (qSqrt(2) - 1) * 2 * qMax(rect1.width(),rect2.width());
	qreal dy = (qSqrt(2) - 1) * 2 * qMax(rect1.height(),rect2.height());

	valRect.setWidth(valRect.width()+dx);
	valRect.setHeight(valRect.height() + dy);

	return valRect;
}

//图元下移一个图层
void CGraphFile::DownItemLayer(unsigned int nLayer)
{
	auto layer = GetLayer(nLayer);
	auto arrSelectItems = layer->GetSelectedWidgets();

	//最下层不做处理
	if (nLayer == MAX_LAYER_CNT)
	{
		return;
	}

	for (auto item : arrSelectItems)
	{
		layer->RemoveWidget(item);
		auto upLayer = GetLayer(nLayer + 1);
		upLayer->AddWidget(item);
	}
}

//图元上移一个图层
void CGraphFile::UploadItemLayer(unsigned int nLayer)
{
	auto layer = GetLayer(nLayer);
	auto arrSelectItems = layer->GetSelectedWidgets();

	if (nLayer == 1)
	{
		//最上层不做处理
		return;
	}

	for (auto item : arrSelectItems)
	{
		layer->RemoveWidget(item);
		auto upLayer = GetLayer(nLayer - 1);
		upLayer->AddWidget(item);
	}
}

/*@}*/
