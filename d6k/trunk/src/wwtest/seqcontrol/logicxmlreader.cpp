#include <QFile>
#include "logicxmlreader.h"

CLogicXmlReader::CLogicXmlReader(QObject *parent)
    : QObject(parent)
{

}

CLogicXmlReader::~CLogicXmlReader()
{

}

bool CLogicXmlReader::OpenXmlFile(const QString &fileName)
{
    m_lstItemInfos.clear();
    m_lstLineInfos.clear();

    if (fileName.isEmpty())
    {
        return false;
    }

    QFile xmlFile(fileName);

    if (xmlFile.open(QFile::ReadOnly | QFile::Text))
    {
        AnalyseXmlFile(&xmlFile);
        xmlFile.close();
    }
    else
    {
        return false;
    }

    return true;
}

void CLogicXmlReader::AnalyseXmlFile(QIODevice *pDevice)
{
    QXmlStreamReader xmlReader(pDevice);

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "logicItem")
            {
                AnalyseItemData(xmlReader);
            }
            else if (xmlReader.name().toString() == "linerounter")
            {
                AnalyseLineData(xmlReader);
            }
            else if (xmlReader.name().toString() == "detailComment")
            {
                AnalyseCommentData(xmlReader);
            }
            else
            {
                //donothing
            }
        }
    }
}

void CLogicXmlReader::AnalyseItemData(QXmlStreamReader &xmlReader)
{
    // 创建编号
    int nCreateNum = xmlReader.attributes().value("createNum").toInt();
    //执行编号
    int nExecNum = xmlReader.attributes().value("execNum").toInt();
    //目标类型
    int nItemTypeNum = xmlReader.attributes().value("itemTypeNum").toInt();
    //en/eno
    bool bEnFlag = xmlReader.attributes().value("exFlag").toInt() == 1 ? true : false;

    //读取item内容
    ITEMINFO itemInfo;
    itemInfo.nCreateNum = nCreateNum;
    itemInfo.nExecNum = nExecNum;
    itemInfo.nItemType = nItemTypeNum;
    itemInfo.bEx = bEnFlag;

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "rect")
            {
                //处理
                QString strRect = xmlReader.readElementText();
                if (strRect.split(",").count() != 4)
                {
                    break;
                }

                itemInfo.itemRect = QRectF(strRect.split(",").at(0).toDouble(), strRect.split(",").at(1).toDouble()
                                  , strRect.split(",").at(2).toDouble(), strRect.split(",").at(3).toDouble());
            }
            else if (xmlReader.name().toString() == "itemcomment")
            {
                itemInfo.strItemComment = xmlReader.readElementText();
            }
            else if (xmlReader.name().toString() == "input")
            {
                //input
                QString strLnNum = xmlReader.name().toString().remove("ln");
               
                LOGICITEMPRO lnInfo;
                //端子号
                lnInfo.nIndex = strLnNum.toInt();
                //类型
                lnInfo.sCellInfo.strInputType = xmlReader.attributes().value("type").toString();
                //元件编号
                lnInfo.sCellInfo.nElement = xmlReader.attributes().value("index").toInt();
                //变址
                lnInfo.sCellInfo.strChangeAddr = xmlReader.attributes().value("changeAddr").toString();
                //开关类型
                lnInfo.sCellInfo.nStatus = xmlReader.attributes().value("switchtype").toInt();
                //描述
                lnInfo.strComment = xmlReader.attributes().value("describe").toString();
                
                itemInfo.lstInInfo.append(lnInfo);
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

    m_lstItemInfos.append(itemInfo);
}

void CLogicXmlReader::AnalyseLineData(QXmlStreamReader &xmlReader)
{
    xmlReader.readNext();

    LINEROUNTER lineItem;

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "item")
            {
                lineItem.strBgeItem = xmlReader.attributes().value("beginitem").toString();
                lineItem.strEndItem = xmlReader.attributes().value("enditem").toString();
                lineItem.strBeginIndex = xmlReader.attributes().value("beginIndex").toString();
                lineItem.strEndIndex = xmlReader.attributes().value("endIndex").toString();
            }
            if (xmlReader.name().toString() == "point")
            {
                QString strPoint = xmlReader.readElementText();
                QPointF pfPoint = QPointF(strPoint.split(",").at(0).toDouble(), strPoint.split(",").at(1).toDouble());

                lineItem.lstAllPoints.append(pfPoint);
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

    m_lstLineInfos.append(lineItem);
}

QList<ITEMINFO> CLogicXmlReader::GetItemInfo()
{
    return m_lstItemInfos;
}

QList<LINEROUNTER> CLogicXmlReader::GetLineInfo()
{
    return m_lstLineInfos;
}

void CLogicXmlReader::AnalyseCommentData(QXmlStreamReader &xmlReader)
{

    COMMENTDATA commentInfo;

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "pos")
            {
                QString strPos = xmlReader.readElementText();
                commentInfo.pfPos = QPointF(strPos.split(",").at(0).toDouble(),strPos.split(",").at(1).toDouble());
            }
            else if (xmlReader.name().toString() == "comment")
            {
                commentInfo.strComment = xmlReader.readElementText();
            }
            else if (xmlReader.name().toString() == "rect")
            {
                QString strRect = xmlReader.readElementText();
                commentInfo.rfRect = QRectF(strRect.split(",").at(0).toDouble(), strRect.split(",").at(1).toDouble(),
                    strRect.split(",").at(2).toDouble(), strRect.split(",").at(3).toDouble());
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
    m_lstComment.append(commentInfo);
}

QList<COMMENTDATA> CLogicXmlReader::GetCommentInfo()
{
    return m_lstComment;
}
