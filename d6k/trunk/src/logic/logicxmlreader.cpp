/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  logicxmlreader.cpp
�ļ�ʵ�ֹ��� :  ��ȡ���� ��������ģ��
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
#include "logicxmlreader.h"
#include "logiccontainter.h"
#include "logicmodel.h"

CLogicXmlReader::CLogicXmlReader()
{

}

CLogicXmlReader::~CLogicXmlReader()
{

}

/*! \fn CLogicPropertyWgt::OpenLogicXmlFile(const QString &fileName, std::shared_ptr<CLogicObjectModel> pObjectModel)
*********************************************************************************************************
** \brief CLogicPropertyWgt::OpenLogicXmlFile(const QString &fileName, std::shared_ptr<CLogicObjectModel> pObjectModel)
** \details ��ȡ�����ļ�  ��������ģ��
** \param  fileName:�����ļ�����   pObjectModel������ģ��
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
bool CLogicXmlReader::OpenLogicXmlFile(const QString &fileName, std::shared_ptr<CLogicObjectModel> pObjectModel)
{
    if (fileName.isEmpty())
    {
        return false;
    }

    QFile xmlFile(fileName);

    if (xmlFile.open(QFile::ReadOnly | QFile::Text))
    {
        QXmlStreamReader xmlReader(&xmlFile);

        while (!xmlReader.atEnd())
        {
            xmlReader.readNext();

            if (xmlReader.isStartElement())
            {
                if (xmlReader.name().toString() == "logicItem")
                {
                    AnalyseLogicItemData(xmlReader, pObjectModel);
                }
                else if (xmlReader.name().toString() == "linerounter")
                {
                    AnalyseLogicLineData(xmlReader, pObjectModel);
                }
                else if (xmlReader.name().toString() == "detailComment")
                {
                    AnalyseLogicCommentData(xmlReader, pObjectModel);
                }
                else
                {
                    //donothing
                }
            }
        }

        xmlFile.close();
    }
    else
    {
        return false;
    }

    return true;
}


/*! \fn CLogicPropertyWgt::AnalyseLogicItemData(QXmlStreamReader &xmlReader, std::shared_ptr<CLogicObjectModel> pObjectModel)
*********************************************************************************************************
** \brief CLogicPropertyWgt::AnalyseLogicItemData(QXmlStreamReader &xmlReader, std::shared_ptr<CLogicObjectModel> pObjectModel)
** \details ��ȡ�߼�������
** \param  
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogicXmlReader::AnalyseLogicItemData(QXmlStreamReader &xmlReader, std::shared_ptr<CLogicObjectModel> pObjectModel)
{
    // �������
    int nCreateNum = xmlReader.attributes().value("createNum").toInt();
    //ִ�б��
    int nExecNum = xmlReader.attributes().value("execNum").toInt();
    //Ŀ������
    int nItemTypeNum = xmlReader.attributes().value("itemTypeNum").toInt();
    //en/eno
    bool bEnFlag = xmlReader.attributes().value("exFlag").toInt() == 1 ? true : false;

    std::shared_ptr<CBlockProperty> tBlock = pObjectModel->CreateBlockProperty(nCreateNum);
    tBlock->SetExecNum(nExecNum);
    tBlock->SetItemType(nItemTypeNum);
    tBlock->SetEnoShowFlag(bEnFlag);

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "rect")
            {
                //����
                QString strRect = xmlReader.readElementText();
                if (strRect.split(",").count() != 4)
                {
                    break;
                }

                tBlock->SetBlockRect(QRectF(strRect.split(",").at(0).toDouble(), strRect.split(",").at(1).toDouble()
                    , strRect.split(",").at(2).toDouble(), strRect.split(",").at(3).toDouble()));
            }
            else if (xmlReader.name().toString() == "itemcomment")
            {
                tBlock->SetComment(xmlReader.readElementText());
            }
            else if (xmlReader.name().toString() == "INPUTBIND")
            {
				//�����Ϣ  ����
				std::shared_ptr<CBinProperty> tPinInfo = tBlock->CreatePinProperty(xmlReader.attributes().value("PinName").toString());
				//Ԫ������
				tPinInfo->SetPinType(2);
				//pin index
				tPinInfo->SetPinIndex(xmlReader.attributes().value("PinIndex").toInt());
				//���
				tPinInfo->CreateCellPro()->SetCellName(xmlReader.attributes().value("BindValue").toString());

            }
        }

        if (xmlReader.isEndElement())
        {
            if (xmlReader.name().toString() == "logicItem")
            {
                break;
            }
        }
    }
}

/*! \fn CLogicPropertyWgt::AnalyseLogicLineData(QXmlStreamReader &xmlReader, std::shared_ptr<CLogicObjectModel> pObjectModel)
*********************************************************************************************************
** \brief CLogicPropertyWgt::AnalyseLogicLineData(QXmlStreamReader &xmlReader, std::shared_ptr<CLogicObjectModel> pObjectModel)
** \details ��ȡֱ�߶�������
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogicXmlReader::AnalyseLogicLineData(QXmlStreamReader &xmlReader, std::shared_ptr<CLogicObjectModel> pObjectModel)
{
    xmlReader.readNext();

    std::shared_ptr<CLineProperty> tLine = pObjectModel->CreateLineProperty();

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "item")
            {
                tLine->SetBeginItemIndex(xmlReader.attributes().value("beginitem").toInt());
                tLine->SetEndItemIndex(xmlReader.attributes().value("enditem").toInt());
                tLine->SetBeginPinIndex(xmlReader.attributes().value("beginIndex").toInt());
                tLine->SetEndPinIndex(xmlReader.attributes().value("endIndex").toInt());
            }
            if (xmlReader.name().toString() == "point")
            {
                QString strPoint = xmlReader.readElementText();

                tLine->SetPoints(strPoint.split(",").at(0).toDouble(), strPoint.split(",").at(1).toDouble());
            }
        }

        if (xmlReader.isEndElement())
        {
            if (xmlReader.name().toString() == "linerounter")
            {
                break;
            }
        }
    }

}

/*! \fn CLogicPropertyWgt::AnalyseLogicCommentData(QXmlStreamReader &xmlReader, std::shared_ptr<CLogicObjectModel> pObjectModel)
*********************************************************************************************************
** \brief CLogicPropertyWgt::AnalyseLogicCommentData(QXmlStreamReader &xmlReader, std::shared_ptr<CLogicObjectModel> pObjectModel)
** \details ��ȡע����Ϣ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CLogicXmlReader::AnalyseLogicCommentData(QXmlStreamReader &xmlReader, std::shared_ptr<CLogicObjectModel> pObjectModel)
{
    std::shared_ptr<CCommentProperty> tCommemt = pObjectModel->CreateCommentProperty();

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "pos")
            {
                QString strPos = xmlReader.readElementText();
                tCommemt->SetCommentPos(QPointF(strPos.split(",").at(0).toDouble(), strPos.split(",").at(1).toDouble()));
            }
            else if (xmlReader.name().toString() == "comment")
            {
                tCommemt->SetComment(xmlReader.readElementText());
            }
            else if (xmlReader.name().toString() == "rect")
            {
                QString strRect = xmlReader.readElementText();
                tCommemt->SetRect(QRectF(strRect.split(",").at(0).toDouble(), strRect.split(",").at(1).toDouble(),
                    strRect.split(",").at(2).toDouble(), strRect.split(",").at(3).toDouble()));
            }
        }

        if (xmlReader.isEndElement())
        {
            if (xmlReader.name().toString() == "detailComment")
            {
                break;
            }
        }
    }
}
