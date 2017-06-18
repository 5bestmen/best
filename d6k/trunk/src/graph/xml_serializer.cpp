/*! @file xml_serializer.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  xml_serializer.cpp
�ļ�ʵ�ֹ��� :  ͼ���ļ����л� xml ��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ͼ���ļ����л� xml ��
*  @author  LiJin
*  @version 1.0
*  @date    2016.08.28
*******************************************************************************/

#include "graph_file.h"
#include "xml_serializer.h"
#include "graphscene.h"
#include "graphview.h"
#include "widget_factory.h"
#include "line_widget.h"
#include "base_dync.h"
#include "dync_color.h"
#include "dync_moving.h"
#include "dync_transparency.h"
#include "dync_visible.h"
#include "dync_value.h"
#include "dync_text.h"
#include "background.h"
#include "base_widget.h"
#include "group_widget.h"

#include <QFile> 
#include <QDebug>
#include <QXmlStreamWriter>
#include <QDir>

/*! \fn bool CGraphXmlSerializer::SaveToFile(CGraphFile *pGraph,,const QString & szFilePath)
*********************************************************************************************************
** \brief CGraphXmlSerializer::SaveToFile
** \details
** \param pGraph
** \param szFilePath
** \return bool
** \author LiJin
** \date 2016��8��29��
** \note
********************************************************************************************************/
bool CGraphXmlSerializer::SaveToFile(CGraphFile *pGraph, const QString & szFilePath)
{
	Q_ASSERT(pGraph);
	if (pGraph == nullptr)
		return false;
	Q_ASSERT(szFilePath.length());
	if (szFilePath.isEmpty() == true)
	{
		return false;
	}

	QFile file(szFilePath);

	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
	//	qDebug() << "Error: Cannot write file: " << qPrintable(file.errorString());
		qDebug() << "Error: Cannot write file: " <<  (file.errorString().toLocal8Bit().data());
		return false;
	}

	std::shared_ptr<QXmlStreamWriter> pXmlWriter = std::make_shared<QXmlStreamWriter>(&file);
	pXmlWriter->setAutoFormatting(true);
	pXmlWriter->writeStartDocument();

	pXmlWriter->writeStartElement("Graph");

	// �����ļ�ͷ 
	SaveFileVerInfo(pXmlWriter, pGraph->GetFileVerInfo());

	pXmlWriter->writeStartElement("Data");

	pXmlWriter->writeAttribute("Descript", pGraph->GetDescript().c_str());
	pXmlWriter->writeAttribute("Height", QString::number(pGraph->GetSize().first));
	pXmlWriter->writeAttribute("Width", QString::number(pGraph->GetSize().second));
	//���ű���
	pXmlWriter->writeAttribute("Scale", QString::number(pGraph->GetSacled()));


	// ��������

	pXmlWriter->writeStartElement("Backgroud");

	pXmlWriter->writeAttribute("Background-Color", pGraph->GetBackground()->m_clrBackground.name(QColor::HexArgb));
	pXmlWriter->writeAttribute("Background-type", QString::number(pGraph->GetBackground()->m_nBrushStyle));;
	pXmlWriter->writeAttribute("Background-Gradient-Color", QString::number(pGraph->GetBackground()->m_nGradiendMode));
	pXmlWriter->writeAttribute("Gradient-Color", pGraph->GetBackground()->m_cloEndColor.name(QColor::HexArgb));

	pXmlWriter->writeAttribute("background-Image", pGraph->GetBackground()->m_strBackgroundImage);
	pXmlWriter->writeAttribute("Image.x", QString::number(pGraph->GetBackground()->m_pfImage.x()));;
	pXmlWriter->writeAttribute("Image.y", QString::number(pGraph->GetBackground()->m_pfImage.y()));;

	pXmlWriter->writeAttribute("Image.width", QString::number(pGraph->GetBackground()->m_szImage.width()));;
	pXmlWriter->writeAttribute("Image.height", QString::number(pGraph->GetBackground()->m_szImage.height()));;

	pXmlWriter->writeAttribute("Tiling", QString::number(pGraph->GetBackground()->m_bTiling));;
	pXmlWriter->writeAttribute("Stretch", QString::number(pGraph->GetBackground()->m_bStretch));;

	pXmlWriter->writeEndElement();

	// ��
	QString szTagName;
	unsigned int nIdx = 1;
	for (auto it : pGraph->m_arrLayers)
	{
		szTagName = QString("Layer%1").arg(nIdx);
		pXmlWriter->writeStartElement(szTagName);
		pXmlWriter->writeAttribute("Count", QString::number(it->GetWidgetCount()));

		SaveLayer(pXmlWriter, it);

		pXmlWriter->writeEndElement();

		nIdx++;
	}

	pXmlWriter->writeEndElement();
	pXmlWriter->writeEndElement();

	pXmlWriter->writeEndDocument();

	file.close();
	if (file.error())
	{
		qDebug() << "Error: Cannot write file: " << qPrintable(file.errorString());
		return false;
	}


	return true;
}
/*! \fn void CGraphXmlSerializer::SaveFileVerInfo(std::shared_ptr<QXmlStreamWriter> pXmlWriter, const CGraphFileVerInfo *pInfo)const
*********************************************************************************************************
** \brief CGraphXmlSerializer::SaveFileVerInfo
** \details �����ļ�ͷ��Ϣ
** \param pXmlWriter
** \param pInfo
** \return void
** \author LiJin
** \date 2016��8��29��
** \note
********************************************************************************************************/
void CGraphXmlSerializer::SaveFileVerInfo(std::shared_ptr<QXmlStreamWriter> pXmlWriter, const CGraphFileVerInfo *pInfo)const
{
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter == nullptr)
		return;
	Q_ASSERT(pInfo);
	if (pInfo == nullptr)
		return;

	pXmlWriter->writeStartElement("FileInfo");

	pXmlWriter->writeStartElement("Version");
	pXmlWriter->writeAttribute("MajorVersion", QString::number(pInfo->GetMajorVer()));
	pXmlWriter->writeAttribute("MinorVersion", QString::number(pInfo->GetMinorVer()));
	pXmlWriter->writeAttribute("Build", QString::number(pInfo->GetBuild()));
	pXmlWriter->writeEndElement();

	pXmlWriter->writeEndElement();
}
/*! \fn void CGraphXmlSerializer::SaveLayer(std::shared_ptr<QXmlStreamWriter> pXmlWriter, std::shared_ptr<CGraphicsLayer> pLayer) const
*********************************************************************************************************
** \brief CGraphXmlSerializer::SaveLayer
** \details ͼ�㱣��
** \param pXmlWriter
** \param pLayer
** \return void
** \author LiJin
** \date 2016��8��31��
** \note
********************************************************************************************************/
void CGraphXmlSerializer::SaveLayer(std::shared_ptr<QXmlStreamWriter> pXmlWriter, std::shared_ptr<CGraphicsLayer> pLayer) const
{
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter == nullptr)
		return;
	Q_ASSERT(pLayer);
	if (pLayer == nullptr)
		return;

	std::vector <CBaseWidget*> & arrWidgets = pLayer->GetWidgets();
	if (arrWidgets.empty() == true)
	{
		return;
	}

	for (auto i : arrWidgets)
	{
		i->SaveXml(pXmlWriter);
	}
}

bool  CGraphXmlSerializer::LoadFromFile(const QString & szFilePath, CGraphFile *pGraph)
{
	Q_ASSERT(pGraph);
	if (pGraph == nullptr)
	{
		return false;
	}

	Q_ASSERT(szFilePath.length());

	if (szFilePath.isEmpty() == true)
	{
		return false;
	}

	CreatePath(szFilePath);

	QFile file(szFilePath);

	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		qDebug() << "Error: Cannot write file: " << (file.errorString().toUtf8().data());
		return false;
	}

	std::shared_ptr<QXmlStreamReader> pXmlReader = std::make_shared<QXmlStreamReader>(&file);


	//�������� ��ͼ
	CGraphScene *pGraphScene = new CGraphScene(pGraph);

	pGraph->SetScene(pGraphScene);


	//////////////////////////////////
	while (!pXmlReader->atEnd() && (!pXmlReader->hasError()))
	{
		//������������
		pXmlReader->readNext();

		if (pXmlReader->isStartElement())
		{
			if (pXmlReader->name() == "Version")
			{
				pGraph->GetFileVerInfo()->SetMajorVer(pXmlReader->attributes().value("MajorVersion").toString().toUInt());
				pGraph->GetFileVerInfo()->SetMinorVer(pXmlReader->attributes().value("MinorVersion").toString().toUInt());
				pGraph->GetFileVerInfo()->SetBuild(pXmlReader->attributes().value("Build").toString().toUInt());
			}
			else if (pXmlReader->name() == "Data")
			{
				pGraph->SetGraphcsSize(QSize(pXmlReader->attributes().value("Width").toString().toInt(), pXmlReader->attributes().value("Height").toString().toInt()));
				pGraph->SetGraphSize(QSize(pXmlReader->attributes().value("Width").toString().toInt(), pXmlReader->attributes().value("Height").toString().toInt()));
				pGraph->SetScaled(pXmlReader->attributes().value("Scale").toString().toInt());

				LoadGraphcInfo(pXmlReader, pGraph);
			}

		}

	}
	//////////////////////////////////

	return true;
}

void CGraphXmlSerializer::LoadGraphcInfo(std::shared_ptr<QXmlStreamReader> pXml, CGraphFile *pFile)
{
	Q_ASSERT(pFile);
	if (pFile == nullptr)
	{
		return;
	}

	while (!pXml->atEnd())
	{
		//������������
		pXml->readNext();

		if (pXml->isStartElement())
		{
			QString strLayer = pXml->name().toString();
			if (strLayer.contains("Layer"))
			{
				int nLayer = strLayer.remove("Layer").toInt();
				LoadGraphItemInfo(pXml, pFile, nLayer);
			}
			else if (strLayer == "Backgroud")
			{
				//��ͼ����
				pFile->GetBackground()->m_clrBackground = QColor(pXml->attributes().value("Background-Color").toString());
				pFile->GetBackground()->m_nBrushStyle = pXml->attributes().value("Background-type").toString().toInt();
				pFile->GetBackground()->m_nGradiendMode = pXml->attributes().value("Background-Gradient-Color").toString().toInt();
				pFile->GetBackground()->m_cloEndColor = QColor(pXml->attributes().value("Gradient-Color").toString());

				//����ͼ����
				pFile->GetBackground()->m_strBackgroundImage = pXml->attributes().value("background-Image").toString();

				pFile->GetBackground()->m_pfImage.setX(pXml->attributes().value("Image.x").toString().toInt());
				pFile->GetBackground()->m_pfImage.setY(pXml->attributes().value("Image.y").toString().toInt());

				pFile->GetBackground()->m_szImage.setWidth(pXml->attributes().value("Image.width").toString().toInt());
				pFile->GetBackground()->m_szImage.setHeight(pXml->attributes().value("Image.height").toString().toInt());

				pFile->GetBackground()->m_bTiling = pXml->attributes().value("Tiling").toString().toInt();
				pFile->GetBackground()->m_bStretch = pXml->attributes().value("Stretch").toString().toInt();
			}
		}

		if (pXml->isEndElement())
		{
			if (pXml->name().toString() == "Data")
			{
				return;
			}
		}

	}
}

void CGraphXmlSerializer::LoadGraphItemInfo(std::shared_ptr<QXmlStreamReader> pXml, CGraphFile *pFile, int nLayer)
{
	Q_ASSERT(pFile);
	if (pFile == nullptr)
	{
		return;
	}

	CBaseWidget *pWidget = nullptr;

	QRectF tzLine;

	while (!pXml->atEnd())
	{
		//������������
		pXml->readNext();

		if (pXml->isStartElement())
		{
			if (pXml->name().toString() == "Widget" && pXml->attributes().value("Type").toInt() != CBaseWidget::WIDGET_GROUP)
			{
				//�����ͼԪ
				qreal posx = pXml->attributes().value("Pos.x").toDouble();
				qreal posy = pXml->attributes().value("Pos.y").toDouble();
				qreal rWidth = pXml->attributes().value("Width").toDouble();
				qreal rHeight = pXml->attributes().value("Height").toDouble();
				int nItemType = pXml->attributes().value("Type").toInt();
				tzLine = QRectF(posx, posy, rWidth, rHeight);

				pWidget = CWidgetFactory::CreateWidget(QRectF(posx, posy, rWidth, rHeight), nItemType);
				Q_ASSERT(pWidget);
				if (pWidget == nullptr)
				{
					LogMsg(QObject::tr("Create Graphic Item Error!").toLocal8Bit().data(), 1);
					continue;
				}


				pFile->AddWidget(nLayer, pWidget);

				pWidget->SetRotateAngle(pXml->attributes().value("Rotate").toDouble());
				pWidget->SetRotateCenter(pXml->attributes().value("RotateCenter").toInt());
				pWidget->SetEndAngle(pXml->attributes().value("EndAngle").toDouble());
				pWidget->SetWidgetName(pXml->attributes().value("WidgetName").toLocal8Bit().toStdString());
				pWidget->SetName(pXml->attributes().value("Name").toLocal8Bit().toStdString());
				pWidget->SetID(pXml->attributes().value("ID").toUInt());
				//z value
				if (pXml->attributes().hasAttribute("ZValue"))
				{
					pWidget->setZValue(pXml->attributes().value("ZValue").toUInt());
				}
				//����
				pWidget->SetLocked(pXml->attributes().value("Lock").toInt());
				//��������
				LoadAttrInfo(pXml, pWidget);

			}
			else if (pXml->name().toString() == "Widget" && pXml->attributes().value("Type").toInt() == CBaseWidget::WIDGET_GROUP)
			{
				//���ͼԪ
				qreal posx = pXml->attributes().value("Pos.x").toDouble();
				qreal posy = pXml->attributes().value("Pos.y").toDouble();
				qreal rWidth = pXml->attributes().value("Width").toDouble();
				qreal rHeight = pXml->attributes().value("Height").toDouble();
				//int nItemType = pXml->attributes().value("Type").toInt();
				tzLine = QRectF(posx, posy, rWidth, rHeight);

				pWidget = new CGroupWidget;
				Q_ASSERT(pWidget);
				if (pWidget == nullptr)
				{
					LogMsg(QObject::tr("Create Graphic Item Error!").toLocal8Bit().data(), 1);
					continue;
				}


				//ֻ��������
				pFile->RenameWidgetItem(nLayer, pWidget);

				//pFile->AddWidget(nLayer, pWidget);

				pWidget->SetRotateAngle(pXml->attributes().value("Rotate").toDouble());
				pWidget->SetRotateCenter(pXml->attributes().value("RotateCenter").toInt());
				pWidget->SetEndAngle(pXml->attributes().value("EndAngle").toDouble());
				pWidget->SetWidgetName(pXml->attributes().value("WidgetName").toLocal8Bit().toStdString());
				pWidget->SetName(pXml->attributes().value("Name").toLocal8Bit().toStdString());
				pWidget->SetID(pXml->attributes().value("ID").toUInt());
				//z value
				if (pXml->attributes().hasAttribute("ZValue"))
				{
					pWidget->setZValue(pXml->attributes().value("ZValue").toUInt());
				}
				//����
				pWidget->SetLocked(pXml->attributes().value("Lock").toInt());

				std::vector <CBaseWidget *> arrChildWgt;
				//����groupinfo
				LoadGroupItemInfo(pXml, pFile, nLayer, (CGroupWidget*)pWidget, 1, arrChildWgt);

				pFile->AddWidget(nLayer, pWidget);

				//pFile->GetLayer(nLayer)->AddSingleWidget(pWidget);
			}
		}

		if (pXml->isEndElement())
		{
			if (pXml->name().toString() == "Widget")
			{
				if (pWidget != nullptr && (pWidget->GetWidgetType() == DRAW_TOOLS_POLYLINE || pWidget->GetWidgetType() == DRAW_TOOLS_POLYGON || pWidget->GetWidgetType() == DRAW_TOOLS_PIPE))
				{
					CLineWidget *PloyLine = dynamic_cast<CLineWidget *>(pWidget);

					if (PloyLine != nullptr)
					{
						PloyLine->UpdateAllPoints();
						PloyLine->MoveTo(tzLine);
					}

				}

			}

			if (pXml->name().toString().contains("Layer"))
			{
				return;
			}
		}

	}
}


void CGraphXmlSerializer::LoadGroupItemInfo(std::shared_ptr<QXmlStreamReader> pXml, CGraphFile *pFile, int nLayer, CGroupWidget *pGroupWgt, int nChild, std::vector <CBaseWidget *> arrChild)
{
	//��������

	Q_ASSERT(pGroupWgt);
	if (pGroupWgt == nullptr)
	{
		return;
	}

	LoadAttrInfo(pXml, pGroupWgt);

	CBaseWidget *pWidget = nullptr;
	QRectF tzLine;


	while (!pXml->atEnd())
	{
		if (pXml->isStartElement())
		{


			if (pXml->name().toString() == "Widget" && pXml->attributes().value("Type").toInt() != CBaseWidget::WIDGET_GROUP)
			{

				nChild++;
				//
				qreal posx = pXml->attributes().value("Pos.x").toDouble();
				qreal posy = pXml->attributes().value("Pos.y").toDouble();
				qreal rWidth = pXml->attributes().value("Width").toDouble();
				qreal rHeight = pXml->attributes().value("Height").toDouble();
				int nItemType = pXml->attributes().value("Type").toInt();
				tzLine = QRectF(posx, posy, rWidth, rHeight);

				pWidget = CWidgetFactory::CreateWidget(QRectF(posx, posy, rWidth, rHeight), nItemType);
				Q_ASSERT(pWidget);
				if (pWidget == nullptr)
				{
					LogMsg(QObject::tr("Create Graphic Item Error!").toLocal8Bit().data(), 1);
					continue;
				}


				arrChild.push_back(pWidget);

				pWidget->SetRotateAngle(pXml->attributes().value("Rotate").toDouble());
				pWidget->SetRotateCenter(pXml->attributes().value("RotateCenter").toInt());
				pWidget->SetEndAngle(pXml->attributes().value("EndAngle").toDouble());
				pWidget->SetWidgetName(pXml->attributes().value("WidgetName").toLocal8Bit().toStdString());
				pWidget->SetName(pXml->attributes().value("Name").toLocal8Bit().toStdString());
				pWidget->SetID(pXml->attributes().value("ID").toUInt());
				if (pXml->attributes().hasAttribute("ZValue"))
				{
					pWidget->setZValue(pXml->attributes().value("ZValue").toUInt());
				}
				//����
				pWidget->SetLocked(pXml->attributes().value("Lock").toInt());

				//��������
				LoadAttrInfo(pXml, pWidget);

				//����ͼԪ
				//pFile->AddWidget(nLayer, pWidget);

			}
			else if (pXml->name().toString() == "Widget" && pXml->attributes().value("Type").toInt() == CBaseWidget::WIDGET_GROUP)
			{
				nChild++;
				//���ͼԪ
				qreal posx = pXml->attributes().value("Pos.x").toDouble();
				qreal posy = pXml->attributes().value("Pos.y").toDouble();
				qreal rWidth = pXml->attributes().value("Width").toDouble();
				qreal rHeight = pXml->attributes().value("Height").toDouble();
				//int nItemType = pXml->attributes().value("Type").toInt();
				tzLine = QRectF(posx, posy, rWidth, rHeight);

				pWidget = new CGroupWidget;
				Q_ASSERT(pWidget);
				if (pWidget == nullptr)
				{
					LogMsg(QObject::tr("Create Graphic Item Error!").toLocal8Bit().data(), 1);
					continue;
				}


				//ֻ��������
				pFile->RenameWidgetItem(nLayer, pWidget);

				//pFile->AddWidget(nLayer, pWidget);

				pWidget->SetRotateAngle(pXml->attributes().value("Rotate").toDouble());
				pWidget->SetRotateCenter(pXml->attributes().value("RotateCenter").toInt());
				pWidget->SetEndAngle(pXml->attributes().value("EndAngle").toDouble());
				pWidget->SetWidgetName(pXml->attributes().value("WidgetName").toLocal8Bit().toStdString());
				pWidget->SetName(pXml->attributes().value("Name").toLocal8Bit().toStdString());
				pWidget->SetID(pXml->attributes().value("ID").toUInt());
				if (pXml->attributes().hasAttribute("ZValue"))
				{
					pWidget->setZValue(pXml->attributes().value("ZValue").toUInt());
				}
				//����
				pWidget->SetLocked(pXml->attributes().value("Lock").toInt());

				arrChild.push_back(pWidget);

				//��������
				//LoadAttrInfo(pXml, pWidget);

				std::vector <CBaseWidget *> arrChild2;
				//����groupinfo
				LoadGroupItemInfo(pXml, pFile, nLayer, (CGroupWidget*)pWidget, 1, arrChild2);
				//���ͼԪ


			}
		}
		if (pXml->isEndElement())
		{
			if (pXml->name().toString() == "Widget")
			{

				if (pWidget != nullptr && (pWidget->GetWidgetType() == DRAW_TOOLS_POLYLINE || pWidget->GetWidgetType() == DRAW_TOOLS_POLYGON || pWidget->GetWidgetType() == DRAW_TOOLS_PIPE))
				{
					CLineWidget *PloyLine = dynamic_cast<CLineWidget *>(pWidget);

					if (PloyLine != nullptr)
					{
						PloyLine->UpdateAllPoints();
						PloyLine->MoveTo(tzLine);
					}

				}

				nChild--;
				if (nChild == 0)
				{

					if (arrChild.size() > 0)
					{
						auto pLayer = pFile->GetLayer(nLayer);
						Q_ASSERT(pLayer != nullptr);
						if (pLayer == nullptr)
						{
							return;
						}

						pLayer->CreateGroupWidgets(arrChild, pGroupWgt->GetRotateAngle(), pGroupWgt);

						arrChild.clear();
					}


					return;
				}
			}
		}
		//
		pXml->readNext();
	}
}

void CGraphXmlSerializer::LoadAttrInfo(std::shared_ptr<QXmlStreamReader> pXml, CBaseWidget *pWidget)
{
	while (!pXml->atEnd())
	{
		//��ȡ����ͼԪ������
		pXml->readNext();

		if (pXml->name().toString() == "Widget")
		{
			return;
		}

		if (pXml->isStartElement())
		{
			if (pXml->name().toString() == "BasePen")
			{
				if (pWidget != nullptr)
				{
					pWidget->GetPenInfo()->SetColor(QColor(pXml->attributes().value("Color").toString()));

					pWidget->GetPenInfo()->SetWidth(pXml->attributes().value("PenWidth").toInt());

					pWidget->GetPenInfo()->SetStyle(pXml->attributes().value("PenType").toInt());

				}
			}
			else if (pXml->name().toString() == "BaseBrush")
			{
				if (pWidget != nullptr)
				{
					pWidget->GetBrushInfo()->SetColor(pXml->attributes().value("Color").toString());

					pWidget->GetBrushInfo()->SetStyle(pXml->attributes().value("BrushType").toInt());

					pWidget->GetBrushInfo()->SetGradiendMode(pXml->attributes().value("GradientType").toInt());
					pWidget->GetBrushInfo()->SetEndColor(pXml->attributes().value("GradientColor").toString());

				}
			}
			else if (pXml->name().toString() == "BaseText")
			{
				if (pWidget != nullptr)
				{
					pWidget->GetFontInfo()->SetColor(pXml->attributes().value("Color").toString());

					qDebug() << pXml->attributes().value("Font").toString();

					QFont textFont;
					textFont.fromString(pXml->attributes().value("Font").toString());
					pWidget->GetFontInfo()->SetFont(textFont);

					pWidget->GetFontInfo()->SetFontLayout(pXml->attributes().value("Layout").toInt());

					pWidget->SetText(pXml->attributes().value("Text").toString());
				}
			}
			else if (pXml->name().toString() == "ItemBackgroud")
			{
				if (pWidget != nullptr)
				{
					pWidget->GetImageInfo()->SetBackgroundImage(pXml->attributes().value("Item-background-Image").toString());
					int nx = pXml->attributes().value("Item-Image.x").toInt();
					int ny = pXml->attributes().value("Item-Image.y").toInt();
					pWidget->GetImageInfo()->SetImagePosition(QPoint(nx, ny));

					int nWidth = pXml->attributes().value("Item-Image.width").toInt();
					int nHeight = pXml->attributes().value("Item-Image.height").toInt();

					pWidget->GetImageInfo()->SetImageSize(QSize(nWidth, nHeight));

					bool bTiling;
					if (pXml->attributes().value("Item-Tiling").toInt() == 1)
					{
						bTiling = true;
					}
					else
					{
						bTiling = false;
					}

					pWidget->GetImageInfo()->SetbTiling(bTiling);

					bool bStretch;
					if (pXml->attributes().value("Item-Stretch").toInt() == 1)
					{
						bStretch = true;
					}
					else
					{
						bStretch = false;
					}
					pWidget->GetImageInfo()->SetStretch(bStretch);
				}
			}
			else if (pXml->name().toString() == "DynamicBrush" || pXml->name().toString() == "DynamicPenColor" || pXml->name().toString() == "DynamicTextColor")
			{
				if (pWidget != nullptr)
				{
					CBaseDyncData *baseData = nullptr;
					if (pXml->name().toString() == "DynamicBrush")
					{
						baseData = pWidget->CreateDynamicPro(CBaseDyncData::DYNC_FILLING);

						CDyncFillingData *pFillingData = dynamic_cast<CDyncFillingData*>(baseData);

						Q_ASSERT(pFillingData);

						if (pFillingData == nullptr)
						{
							return;
						}

						//ģ��ɫ
						pFillingData->m_bAnalogColor = pXml->attributes().value("AnalogColor").toInt();

						pFillingData->m_dMinVal = pXml->attributes().value("StartValue").toDouble();
						pFillingData->m_dMaxVal = pXml->attributes().value("EndValue").toDouble();
						pFillingData->m_dStartPos = pXml->attributes().value("StartOffset").toDouble();
						pFillingData->m_dEndPos = pXml->attributes().value("EndOffset").toDouble();

						pFillingData->m_nFillingPolicy = CDyncFillingData::DYNC_FILLING_POLICY(pXml->attributes().value("ColorType").toInt());
					}
					else if (pXml->name().toString() == "DynamicPenColor")
					{
						baseData = pWidget->CreateDynamicPro(CBaseDyncData::DYNC_PEN_COLOR);

						CDyncClrData *pColorData = dynamic_cast<CDyncClrData*>(baseData);

						pColorData->m_nDyncClrType = CDyncClrData::DYNC_FILL_CLR;
					}
					else if (pXml->name().toString() == "DynamicTextColor")
					{
						baseData = pWidget->CreateDynamicPro(CBaseDyncData::DYNC_TEXT_COLOR);

						CDyncClrData *pColorData = dynamic_cast<CDyncClrData*>(baseData);

						pColorData->m_nDyncClrType = CDyncClrData::DYNC_TEXT_CLR;
					}

					CDyncClrData *pColorData = dynamic_cast<CDyncClrData*>(baseData);

					Q_ASSERT(pColorData);

					if (pColorData == nullptr)
					{
						return;
					}

					pColorData->m_szTagName = pXml->attributes().value("Variable").toLocal8Bit().data();

					pColorData->m_bEnable = pXml->attributes().value("Enable").toInt();

					while (!pXml->atEnd())
					{
						pXml->readNext();

						if (pXml->isStartElement())
						{
							if (pXml->name().toString() == "ColorRange")
							{
								//
								CDyncClrValueInfo *pclrValue = pColorData->CreateClrData();
								Q_ASSERT(pclrValue);
								if (pclrValue == nullptr)
								{
									break;
								}

								pclrValue->m_dwColor = pXml->attributes().value("Color").toString();
								pclrValue->m_dwBlinkingColor = pXml->attributes().value("FlashColor").toString();
								pclrValue->m_nBlinkingType = CDyncClrValueInfo::BLINKING_TYPE(pXml->attributes().value("FlashMode").toString().toInt());

								pclrValue->m_nBlinkInterval = pXml->attributes().value("FlashTime").toString().toInt();

								pclrValue->m_dValue = pXml->attributes().value("ThresholdData").toString().toDouble();
							}
						}

						if (pXml->isEndElement())
						{

							if (pXml->name().toString() == "DynamicBrush" || pXml->name().toString() == "DynamicPenColor" || pXml->name().toString() == "DynamicTextColor")
							{
								break;
							}
						}
					}
					//pWidget->GetFontInfo->SetFont(pXml->attributes().value("Variable").toString());

				}
			}
			else if (pXml->name().toString() == "HorizonalMove")
			{
				CBaseDyncData *baseData = pWidget->CreateDynamicPro(CBaseDyncData::DYNC_HOR_MOVE);

				CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(baseData);

				Q_ASSERT(pMovingData);

				if (pMovingData != nullptr)
				{
					pMovingData->m_bEnable = pXml->attributes().value("Enable").toInt();;
					pMovingData->SetBackFlag(pXml->attributes().value("Reverse").toInt());
					pMovingData->m_szTagName = pXml->attributes().value("Variable").toLocal8Bit().data();
					pMovingData->SetStartData(pXml->attributes().value("MoveStart").toString().toInt());
					pMovingData->SetEndData(pXml->attributes().value("MoveEnd").toString().toInt());

					pMovingData->SetStartoffset(pXml->attributes().value("StartOffset").toString().toInt());
					pMovingData->SetEndoffset(pXml->attributes().value("EndOffset").toString().toInt());
					pMovingData->SetDistance(pXml->attributes().value("Diatance").toString().toDouble());
				}


			}
			else if (pXml->name().toString() == "VerticalMove")
			{
				CBaseDyncData *baseData = pWidget->CreateDynamicPro(CBaseDyncData::DYNC_VIR_MOVE);

				CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(baseData);

				Q_ASSERT(pMovingData);

				if (pMovingData != nullptr)
				{
					pMovingData->m_bEnable = pXml->attributes().value("Enable").toInt();
					pMovingData->SetBackFlag(pXml->attributes().value("Reverse").toInt());
					pMovingData->m_szTagName = pXml->attributes().value("Variable").toLocal8Bit().data();
					pMovingData->SetStartData(pXml->attributes().value("MoveStart").toString().toInt());
					pMovingData->SetEndData(pXml->attributes().value("MoveEnd").toString().toInt());

					pMovingData->SetStartoffset(pXml->attributes().value("StartOffset").toString().toInt());
					pMovingData->SetEndoffset(pXml->attributes().value("EndOffset").toString().toInt());
					pMovingData->SetDistance(pXml->attributes().value("Diatance").toString().toDouble());
				}

			}
			else if (pXml->name().toString() == "DyncGroupMove")
			{
				CBaseDyncData *baseData = pWidget->CreateDynamicPro(CBaseDyncData::DYNC_GROUP_MOVE);

				CDyncGroupMovingData *pMovingData = dynamic_cast<CDyncGroupMovingData*>(baseData);

				Q_ASSERT(pMovingData);

				if (pMovingData != nullptr)
				{
					pMovingData->m_bEnable = pXml->attributes().value("Enable").toInt();
					pMovingData->m_szTagName = pXml->attributes().value("Variable").toLocal8Bit().data();
					pMovingData->SetStartData(pXml->attributes().value("MoveStart").toString().toInt());
					pMovingData->SetEndData(pXml->attributes().value("MoveEnd").toString().toInt());


					while (!pXml->atEnd())
					{
						pXml->readNext();

						if (pXml->isStartElement())
						{
							if (pXml->name().toString() == "GroupPoint")
							{
								//
								QString strPoint = pXml->readElementText();

								double pfx = strPoint.split(",").first().toDouble();
								double pfy = strPoint.split(",").last().toDouble();

								pMovingData->InsertPonit(QPointF(pfx, pfy));
							}
						}

						if (pXml->isEndElement())
						{

							if (pXml->name().toString() == "DyncGroupMove")
							{
								break;
							}
						}
					}
				}
			}
			else if (pXml->name().toString() == "RotateMove")
			{
				CBaseDyncData *baseData = pWidget->CreateDynamicPro(CBaseDyncData::DYNC_ROTATION);

				CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(baseData);

				Q_ASSERT(pMovingData);

				if (pMovingData != nullptr)
				{
					pMovingData->m_bEnable = pXml->attributes().value("Enable").toInt();
					pMovingData->SetBackFlag(pXml->attributes().value("Reverse").toInt());
					pMovingData->m_szTagName = pXml->attributes().value("Variable").toLocal8Bit().data();
					pMovingData->SetStartData(pXml->attributes().value("MoveStart").toString().toInt());
					pMovingData->SetEndData(pXml->attributes().value("MoveEnd").toString().toInt());

					pMovingData->SetStartoffset(pXml->attributes().value("StartOffset").toString().toInt());
					pMovingData->SetEndoffset(pXml->attributes().value("EndOffset").toString().toInt());

					pMovingData->SetStartAngel(pXml->attributes().value("StartAngel").toString().toInt());
					pMovingData->SetEndAngle(pXml->attributes().value("EndAngel").toString().toInt());
				}
			}
			else if (pXml->name().toString() == "Transparency")
			{
				CBaseDyncData *baseData = pWidget->CreateDynamicPro(CBaseDyncData::DYNC_TRANSPARENCY);

				CDyncTransparencyData *pTransData = dynamic_cast<CDyncTransparencyData*>(baseData);

				Q_ASSERT(pTransData);

				if (pTransData != nullptr)
				{
					pTransData->m_bEnable = pXml->attributes().value("Enable").toInt();
					pTransData->m_szTagName = pXml->attributes().value("Variable").toLocal8Bit().data();

					pTransData->SetMinValue(pXml->attributes().value("TransStart").toString().toInt());
					pTransData->SetMaxValue(pXml->attributes().value("TransEnd").toString().toInt());

				}

			}
			else if (pXml->name().toString() == "DyncVisable")
			{
				CBaseDyncData *baseData = pWidget->CreateDynamicPro(CBaseDyncData::DYNC_VISIBLE);

				CDyncVisibleData *pVisableData = dynamic_cast<CDyncVisibleData*>(baseData);

				Q_ASSERT(pVisableData);

				if (pVisableData != nullptr)
				{
					pVisableData->m_bEnable = pXml->attributes().value("Enable").toInt();
					pVisableData->m_szTagName = pXml->attributes().value("Variable").toLocal8Bit().data();

					pVisableData->SetVisableValue(pXml->attributes().value("VisableValue").toDouble());
					pVisableData->SetDeadBand(pXml->attributes().value("DeadZone").toDouble());

					pVisableData->SetCondition(pXml->attributes().value("Condition").toInt());
					pVisableData->SetFlashType(pXml->attributes().value("FlashingType").toInt());
					pVisableData->SetFlashTime(pXml->attributes().value("FlashingTime").toInt());
				}

			}
			else if (pXml->name().toString() == "DyncValue")
			{
				CBaseDyncData *baseData = pWidget->CreateDynamicPro(CBaseDyncData::DYNC_VALUE);

				CDyncValueData *pValueData = dynamic_cast<CDyncValueData*>(baseData);

				Q_ASSERT(pValueData);

				if (pValueData != nullptr)
				{
					pValueData->m_bEnable = pXml->attributes().value("Enable").toInt();
					pValueData->m_szTagName = pXml->attributes().value("Variable").toLocal8Bit().data();

					pValueData->SetFormat(CDyncValueData::VAL_FORMAT(pXml->attributes().value("DynamicValue").toInt()));
				}
			}
			else if (pXml->name().toString() == "DynamicTextContext")
			{
				if (pWidget != nullptr)
				{
					CBaseDyncData *baseData = nullptr;

					baseData = pWidget->CreateDynamicPro(CBaseDyncData::DYNC_STRING);

					CDyncText *pTextData = dynamic_cast<CDyncText*>(baseData);

					Q_ASSERT(pTextData);

					if (pTextData == nullptr)
					{
						return;
					}

					pTextData->m_szTagName = pXml->attributes().value("Variable").toLocal8Bit().data();

					pTextData->m_bEnable = pXml->attributes().value("Enable").toInt();

					while (!pXml->atEnd())
					{
						pXml->readNext();

						if (pXml->isStartElement())
						{
							if (pXml->name().toString() == "TextRange")
							{
								//
								CDyncTextValueInfo *pTextValue = pTextData->CreateTextData();

								Q_ASSERT(pTextValue);
								if (pTextValue == nullptr)
								{
									break;
								}

								pTextValue->SetOrigData(pXml->attributes().value("Source").toString());
								pTextValue->SetDescData(pXml->attributes().value("Desc").toString());
							}
						}

						if (pXml->isEndElement())
						{

							if (pXml->name().toString() == "DynamicTextContext")
							{
								break;
							}
						}
					}
					//pWidget->GetFontInfo->SetFont(pXml->attributes().value("Variable").toString());

				}
			}
			else if (pXml->name().toString() == "DynamicMap")
			{
				if (pWidget != nullptr)
				{
					CBaseDyncData *baseData = nullptr;

					baseData = pWidget->CreateDynamicPro(CBaseDyncData::DYNC_MAP);

					CDyncText *pTextData = dynamic_cast<CDyncText*>(baseData);

					Q_ASSERT(pTextData);

					if (pTextData == nullptr)
					{
						return;
					}

					pTextData->m_szTagName = pXml->attributes().value("Variable").toLocal8Bit().data();

					pTextData->m_bEnable = pXml->attributes().value("Enable").toInt();

					while (!pXml->atEnd())
					{
						pXml->readNext();

						if (pXml->isStartElement())
						{
							if (pXml->name().toString() == "TextRange")
							{
								//
								CDyncTextValueInfo *pTextValue = pTextData->CreateTextData();

								Q_ASSERT(pTextValue);
								if (pTextValue == nullptr)
								{
									break;
								}

								pTextValue->SetOrigData(pXml->attributes().value("Source").toString());
								pTextValue->SetDescData(pXml->attributes().value("Desc").toString());
							}
						}

						if (pXml->isEndElement())
						{

							if (pXml->name().toString() == "DynamicMap")
							{
								break;
							}
						}
					}
					//pWidget->GetFontInfo->SetFont(pXml->attributes().value("Variable").toString());

				}
			}
			else if (pXml->name().toString() == "LinePoint")
			{

				//pWidget->SetRotateAngle(pBaseWgt->GetRotateAngle());

				if (pWidget->GetWidgetType() == DRAW_TOOLS_POLYLINE || pWidget->GetWidgetType() == DRAW_TOOLS_POLYGON || pWidget->GetWidgetType() == DRAW_TOOLS_PIPE)
				{
					CLineWidget *PloyLine = dynamic_cast<CLineWidget *>(pWidget);

					if (PloyLine != nullptr)
					{
						QString strPoint = pXml->readElementText();
						PloyLine->InsertNewPoint(QPointF(strPoint.split(",").first().toDouble(), strPoint.split(",").last().toDouble()));
					}
				}

			}
		}
	}
}



void CGraphXmlSerializer::CreatePath(const QString &strFilePath)
{
	QString strPath = strFilePath;
	strPath.remove(strFilePath.length() - strFilePath.split("/").last().length(), strFilePath.split("/").last().length());
	QDir tDir;
	tDir.mkdir(strPath);
}

void CGraphXmlSerializer::LoadGraphTemplateInfo(std::shared_ptr<QXmlStreamReader> pXml, CGraphFile *pFile, int nLayer, const QRectF & rcPos)
{
	Q_ASSERT(pFile);
	if (pFile == nullptr)
	{
		return;
	}

	CBaseWidget *pWidget = nullptr;

	QRectF tzLine;

	while (!pXml->atEnd())
	{
		//������������
		pXml->readNext();

		if (pXml->isStartElement())
		{
			if (pXml->name().toString() == "Widget" && pXml->attributes().value("Type").toInt() != CBaseWidget::WIDGET_GROUP)
			{
				//�����ͼԪ
				qreal posx = pXml->attributes().value("Pos.x").toDouble();
				qreal posy = pXml->attributes().value("Pos.y").toDouble();
				qreal rWidth = pXml->attributes().value("Width").toDouble();
				qreal rHeight = pXml->attributes().value("Height").toDouble();
				int nItemType = pXml->attributes().value("Type").toInt();
				tzLine = QRectF(posx, posy, rWidth, rHeight);

				pWidget = CWidgetFactory::CreateWidget(QRectF(posx, posy, rWidth, rHeight), nItemType);
				Q_ASSERT(pWidget);
				if (pWidget == nullptr)
				{
					LogMsg(QObject::tr("Create Graphic Item Error!").toLocal8Bit().data(), 1);
					continue;
				}


				pFile->AddWidget(nLayer, pWidget);

				pWidget->SetRotateAngle(pXml->attributes().value("Rotate").toDouble());
				pWidget->SetRotateCenter(pXml->attributes().value("RotateCenter").toInt());
				pWidget->SetEndAngle(pXml->attributes().value("EndAngle").toDouble());
				pWidget->SetWidgetName(pXml->attributes().value("WidgetName").toLocal8Bit().toStdString());
				pWidget->SetName(pXml->attributes().value("Name").toLocal8Bit().toStdString());
				pWidget->SetID(pXml->attributes().value("ID").toUInt());
				if (pXml->attributes().hasAttribute("ZValue"))
				{
					pWidget->setZValue(pXml->attributes().value("ZValue").toUInt());
				}
				//����
				pWidget->SetLocked(pXml->attributes().value("Lock").toInt());
				//��������
				LoadAttrInfo(pXml, pWidget);

			}
			else if (pXml->name().toString() == "Widget" && pXml->attributes().value("Type").toInt() == CBaseWidget::WIDGET_GROUP)
			{
				//���ͼԪ
				qreal posx = pXml->attributes().value("Pos.x").toDouble();
				qreal posy = pXml->attributes().value("Pos.y").toDouble();
				qreal rWidth = pXml->attributes().value("Width").toDouble();
				qreal rHeight = pXml->attributes().value("Height").toDouble();
				//int nItemType = pXml->attributes().value("Type").toInt();
				tzLine = QRectF(posx, posy, rWidth, rHeight);

				pWidget = new CGroupWidget;
				Q_ASSERT(pWidget);
				if (pWidget == nullptr)
				{
					LogMsg(QObject::tr("Create Graphic Item Error!").toLocal8Bit().data(), 1);
					continue;
				}


				//ֻ��������
				pFile->RenameWidgetItem(nLayer, pWidget);

				//pFile->AddWidget(nLayer, pWidget);

				pWidget->SetRotateAngle(pXml->attributes().value("Rotate").toDouble());
				pWidget->SetRotateCenter(pXml->attributes().value("RotateCenter").toInt());
				pWidget->SetEndAngle(pXml->attributes().value("EndAngle").toDouble());
				pWidget->SetWidgetName(pXml->attributes().value("WidgetName").toLocal8Bit().toStdString());
				pWidget->SetName(pXml->attributes().value("Name").toLocal8Bit().toStdString());
				pWidget->SetID(pXml->attributes().value("ID").toUInt());
				//z value
				if (pXml->attributes().hasAttribute("ZValue"))
				{
					pWidget->setZValue(pXml->attributes().value("ZValue").toUInt());
				}
				
				//����
				pWidget->SetLocked(pXml->attributes().value("Lock").toInt());

				std::vector <CBaseWidget *> arrChildWgt;
				//����groupinfo
				LoadGroupItemInfo(pXml, pFile, nLayer, (CGroupWidget*)pWidget, 1, arrChildWgt);

				pFile->AddWidget(nLayer, pWidget);

				//pFile->GetLayer(nLayer)->AddSingleWidget(pWidget);
			}
		}

		if (pXml->isEndElement())
		{
			if (pXml->name().toString() == "Widget")
			{
				if (pWidget != nullptr && (pWidget->GetWidgetType() == DRAW_TOOLS_POLYLINE || pWidget->GetWidgetType() == DRAW_TOOLS_POLYGON || pWidget->GetWidgetType() == DRAW_TOOLS_PIPE))
				{
					CLineWidget *PloyLine = dynamic_cast<CLineWidget *>(pWidget);

					if (PloyLine != nullptr)
					{
						PloyLine->UpdateAllPoints();
						PloyLine->MoveTo(tzLine);
					}

				}

				pWidget->MoveTo(QRectF(rcPos.topLeft().x(), rcPos.topLeft().y(), tzLine.width(), tzLine.height()));


			}

			if (pXml->name().toString().contains("Layer"))
			{
				return;
			}
		}

	}
	
}

/** @}*/

