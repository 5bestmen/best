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
*  @brief   ͼ���ļ�.
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
** \details �����ļ�ͷ
** \param pWriter 
** \return bool 
** \author LiJin 
** \date 2016��4��5�� 
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
	m_szGraphName = "ͼ��1";
	m_bFlashFlag = false;
	m_scaleFactor = 50;

	m_Size = QSize(1920, 1200);
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
	m_bVaildFlag = true;

}

CGraphFile::CGraphFile(CGraphScene *pScene) :m_pScene(pScene)
{
	m_pFolder = nullptr;
	m_nMaxWidgetID = 0;
	m_bFlashFlag = false;
	m_scaleFactor = 50;

	m_Size = QSize(1400, 800);
	m_szGraphName = "����1";

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
	// ����ͼ��
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

//��д��ֵ���� 
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
** \details ��ָ��ͼ������һ������
** \param nLayerIdx  ��1��ʼ
** \param pObj 
** \return void 
** \author LiJin 
** \date 2016��1��18��   m
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

//����
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
** \details ����ͼ���ļ�
** \param szFileName
** \return bool
** \author LiJin
** \date 2016��2��19��
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
//����ͼ���ļ�
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
	//�������
	if (rect1.contains(rect2))
	{
		return rect1;
	}
	if (rect2.contains(rect1))
	{
		return rect2;
	}
	//���������߲����ص����
	QRectF valRect;
	//1<<rectangle1��rectangle2����ࣨ�����δ�Сһ�£�  ����
	if (rect1.topLeft().x()<=rect2.topLeft().x()  && rect1.topLeft().y()<=rect2.topLeft().y())
	{
		 valRect=QRectF(rect1.topLeft(),rect2.bottomRight());
	}
	//2<<rectangle1��rectangle2����ࣨ�����δ�Сһ�£�  ����
	if (rect1.topLeft().x() < rect2.topLeft().x() && rect1.topLeft().y() > rect2.topLeft().y())
	{
		 valRect=QRectF(QPointF(rect1.topLeft().x(),rect2.topLeft().y()),QPointF(rect2.bottomRight().x(),rect1.bottomRight().y()));
	}
	//3<<rectangle1��rectangle2���Ҳࣨ�����δ�Сһ�£�  ����
	if (rect1.topLeft().x() >= rect2.topLeft().x() && rect1.topLeft().y() <= rect2.topLeft().y())
	{
		 valRect=QRectF(QPointF(rect2.topLeft().x(), rect1.topLeft().y()), QPointF(rect1.bottomRight().x(), rect2.bottomRight().y()));
	}
	//4<<rectangle1��rectangle2���Ҳࣨ�����δ�Сһ�£�  ����
	if (rect1.topLeft().x() > rect2.topLeft().x() && rect1.topLeft().y() > rect2.topLeft().y())
	{
		  valRect=QRectF(rect2.topLeft(), rect1.bottomRight());
	}
	//����ʵ��������߽緶Χ��Ҫ���󣨡�2 -1��*2
	qreal dx = (qSqrt(2) - 1) * 2 * qMax(rect1.width(),rect2.width());
	qreal dy = (qSqrt(2) - 1) * 2 * qMax(rect1.height(),rect2.height());

	valRect.setWidth(valRect.width()+dx);
	valRect.setHeight(valRect.height() + dy);

	return valRect;
}

//ͼԪ����һ��ͼ��
void CGraphFile::DownItemLayer(unsigned int nLayer)
{
	auto layer = GetLayer(nLayer);
	auto arrSelectItems = layer->GetSelectedWidgets();

	//���²㲻������
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

//ͼԪ����һ��ͼ��
void CGraphFile::UploadItemLayer(unsigned int nLayer)
{
	auto layer = GetLayer(nLayer);
	auto arrSelectItems = layer->GetSelectedWidgets();

	if (nLayer == 1)
	{
		//���ϲ㲻������
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
