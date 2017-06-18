/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  logicxmlwriter.cpp
�ļ�ʵ�ֹ��� :  ����ģ��������  д�������ļ�
����         :  ww
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��> �߼��༭
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author ww
*  @version 1.0
*  @date    2017.1.4
*/
#include <QFile>
#include "logicxmlwriter.h"
#include "logicmodel.h"
#include "logiccontainter.h"

CLogicXmlWriter::CLogicXmlWriter()
{

}

CLogicXmlWriter::~CLogicXmlWriter()
{

}

/*! \fn CLogicPropertyWgt::SaveDataToXml(const QString &strFilename,std::shared_ptr<CLogicObjectModel> pObjectModel)
*********************************************************************************************************
** \brief CLogicPropertyWgt::SaveDataToXml(const QString &strFilename,std::shared_ptr<CLogicObjectModel> pObjectModel)
** \details ���������������ļ�
** \param  
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
bool CLogicXmlWriter::SaveDataToXml(const QString &strFilename,std::shared_ptr<CLogicObjectModel> pObjectModel)
{
    QFile xmlFile(strFilename);

    if (xmlFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QXmlStreamWriter writer(&xmlFile);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("graph");
        //��װ������
        SaveItemData(writer,pObjectModel->GetBlockProperty());
        //��
        SaveLineData(writer, pObjectModel->GetLineProperty());
        //ע��
        SaveCommentData(writer, pObjectModel->GetCommentProperty());

        writer.writeEndElement();
        xmlFile.close();
    }
    else
    {
        return false;
    }

    return true;
}

/*! \fn CLogicPropertyWgt::SaveItemData(QXmlStreamWriter &xmlWriter, std::vector<std::shared_ptr<CBlockProperty>> tBlock)
*********************************************************************************************************
** \brief CLogicPropertyWgt::SaveItemData(QXmlStreamWriter &xmlWriter, std::vector<std::shared_ptr<CBlockProperty>> tBlock)
** \details �����߼�������
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogicXmlWriter::SaveItemData(QXmlStreamWriter &xmlWriter, std::vector<std::shared_ptr<CBlockProperty>> tBlock)
{
    for each(std::shared_ptr<CBlockProperty> itemInfo in tBlock)
    {
        xmlWriter.writeStartElement("logicItem");
        xmlWriter.writeAttribute("createNum", QString::number(itemInfo->GetCreateNum()));
        xmlWriter.writeAttribute("execNum", QString::number(itemInfo->GetExecNum()));
        xmlWriter.writeAttribute("itemTypeNum", QString::number(itemInfo->GetItemType()));
        xmlWriter.writeAttribute("exFlag", QString::number(itemInfo->GetEnoShowFlag() ? 1 : 0));

        xmlWriter.writeStartElement("rect");
        QString strRect = QString::number(itemInfo->GetBlockRect().x()) + "," + QString::number(itemInfo->GetBlockRect().y())
            + "," + QString::number(itemInfo->GetBlockRect().width()) + "," + QString::number(itemInfo->GetBlockRect().height());
        xmlWriter.writeCharacters(strRect);
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("itemcomment");
        xmlWriter.writeCharacters(itemInfo->GetComment());
        xmlWriter.writeEndElement();

		//�����Ϣ
		for (auto item : itemInfo->GetPinValues())
		{
			
			if (item->GetCellPro() != nullptr)
			{
				xmlWriter.writeStartElement("INPUTBIND");
				xmlWriter.writeAttribute("PinIndex", QString::number(item->GetPinIndex()));
				xmlWriter.writeAttribute("PinName", item->GetName());
				xmlWriter.writeAttribute("BindValue", item->GetCellPro()->GetCellName());
				xmlWriter.writeEndElement();
			}
			
			
		}

        xmlWriter.writeEndElement();
    }
}

/*! \fn CLogicPropertyWgt::SaveLineData(QXmlStreamWriter &xmlWriter, std::vector<std::shared_ptr<CLineProperty>> tLine)
*********************************************************************************************************
** \brief CLogicPropertyWgt::SaveLineData(QXmlStreamWriter &xmlWriter, std::vector<std::shared_ptr<CLineProperty>> tLine)
** \details ����ֱ������
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogicXmlWriter::SaveLineData(QXmlStreamWriter &xmlWriter, std::vector<std::shared_ptr<CLineProperty>> tLine)
{
    for each(std::shared_ptr<CLineProperty> lineItem in tLine)
    {
        xmlWriter.writeStartElement("linerounter");

        xmlWriter.writeStartElement("item");
        xmlWriter.writeAttribute("beginitem", QString::number(lineItem->GetBeginItemIndex()));
        xmlWriter.writeAttribute("enditem", QString::number(lineItem->GetEndItemIndex()));
        xmlWriter.writeAttribute("beginIndex", QString::number(lineItem->GetBeginPinIndex()));
        xmlWriter.writeAttribute("endIndex", QString::number(lineItem->GetEndPinIndex()));
        xmlWriter.writeEndElement();

        for each(std::shared_ptr<QPointF> pfItem in lineItem->GetAllPoints())
        {
            QString strPointf = QString::number(pfItem->x()) + "," + QString::number(pfItem->y());
            xmlWriter.writeStartElement("point");
            xmlWriter.writeCharacters(strPointf);
            xmlWriter.writeEndElement();
        }

        xmlWriter.writeEndElement();
    }
}

/*! \fn CLogicPropertyWgt::SaveCommentData(QXmlStreamWriter &xmlWriter, std::vector<std::shared_ptr<CCommentProperty>> tComment)
*********************************************************************************************************
** \brief CLogicPropertyWgt::SaveCommentData(QXmlStreamWriter &xmlWriter, std::vector<std::shared_ptr<CCommentProperty>> tComment)
** \details ����ע������
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogicXmlWriter::SaveCommentData(QXmlStreamWriter &xmlWriter, std::vector<std::shared_ptr<CCommentProperty>> tComment)
{
    for each (std::shared_ptr<CCommentProperty> commentItem in tComment)
    {
        xmlWriter.writeStartElement("detailComment");

        xmlWriter.writeStartElement("pos");
        QString strPos = QString::number(commentItem->GetCommentPos().x()) + "," + QString::number(commentItem->GetCommentPos().y());
        xmlWriter.writeCharacters(strPos);
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("comment");
        xmlWriter.writeCharacters(commentItem->GetComment());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("rect");
        QString strRect = QString::number(commentItem->GetRect().x()) + "," + QString::number(commentItem->GetRect().y()) + "," +
            QString::number(commentItem->GetRect().width()) + "," + QString::number(commentItem->GetRect().height());
        xmlWriter.writeCharacters(strRect);
        xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();
    }
}
