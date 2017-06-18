#include <QFile>
#include "logicxmlwriter.h"

CLogicXmlWriter::CLogicXmlWriter()
{

}

CLogicXmlWriter::~CLogicXmlWriter()
{

}

bool CLogicXmlWriter::UpdateXmlFile(const QString &strFilename, const QList<ITEMINFO>&lstItems, const QList<LINEROUNTER>&lstLines, const QList<COMMENTDATA>&lstComment)
{
    QFile xmlFile(strFilename);

    if (xmlFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QXmlStreamWriter writer(&xmlFile);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("graph");
        //组装数据体
        if (!UpdateItemData(writer, lstItems))
        {
            return false;
        }

        if (!UpdateLineData(writer,lstLines))
        {
            return false;
        }

        if (!UpdataCommentData(writer,lstComment))
        {
            return false;
        }
        
        writer.writeEndElement();
        xmlFile.close();
    }
    else
    {
        return false;
    }
    
    return true;
}

bool CLogicXmlWriter::UpdateItemData(QXmlStreamWriter &xmlWriter, const QList<ITEMINFO>&lstItems)
{
    foreach(ITEMINFO itemInfo,lstItems)
    {
        xmlWriter.writeStartElement("logicItem");
        xmlWriter.writeAttribute("createNum", QString::number(itemInfo.nCreateNum));
        xmlWriter.writeAttribute("execNum", QString::number(itemInfo.nExecNum));
        xmlWriter.writeAttribute("itemTypeNum",QString::number(itemInfo.nItemType));
        xmlWriter.writeAttribute("exFlag",QString::number(itemInfo.bEx?1:0));

        xmlWriter.writeStartElement("rect");
        QString strRect = QString::number(itemInfo.itemRect.x()) + "," + QString::number(itemInfo.itemRect.y())
            + "," + QString::number(itemInfo.itemRect.width()) + "," + QString::number(itemInfo.itemRect.height());
        xmlWriter.writeCharacters(strRect);
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("itemcomment");
        xmlWriter.writeCharacters(itemInfo.strItemComment);
        xmlWriter.writeEndElement();


        for (int i = 0; i < itemInfo.lstInInfo.count(); i++)
        {
            QString strNodeName = "input";;
            QString strPinIndex = QString::number(itemInfo.lstInInfo.at(i).nIndex);
            QString strPinName = itemInfo.lstInInfo.at(i).strPinName;
            QString strCellType = itemInfo.lstInInfo.at(i).sCellInfo.strInputType;
            
            QString strCellChangeAddr = itemInfo.lstInInfo.at(i).sCellInfo.strChangeAddr;
            QString strCellSwitchType = QString::number(itemInfo.lstInInfo.at(i).sCellInfo.nStatus);
            QString strDestrib = itemInfo.lstInInfo.at(i).strComment;
            xmlWriter.writeStartElement(strNodeName);
            xmlWriter.writeAttribute("PinNumber", strPinIndex);
            xmlWriter.writeAttribute("CellType", strCellType);
            xmlWriter.writeAttribute("CellChangeAddr", strCellChangeAddr);
            xmlWriter.writeAttribute("CellSwitchtype", strCellSwitchType);
            xmlWriter.writeAttribute("describe", strDestrib);
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();
    }
    return true;
}

bool CLogicXmlWriter::UpdateLineData(QXmlStreamWriter &xmlWriter, const QList<LINEROUNTER>&lstLines)
{
    foreach(LINEROUNTER lineItem,lstLines)
    {
        xmlWriter.writeStartElement("linerounter");

        xmlWriter.writeStartElement("item");
        xmlWriter.writeAttribute("beginitem", lineItem.strBgeItem);
        xmlWriter.writeAttribute("enditem",lineItem.strEndItem);
        xmlWriter.writeAttribute("beginIndex", lineItem.strBeginIndex);
        xmlWriter.writeAttribute("endIndex", lineItem.strEndIndex);
        xmlWriter.writeEndElement();

        foreach(QPointF pfItem,lineItem.lstAllPoints)
        {
            QString strPointf = QString::number(pfItem.x()) + "," + QString::number(pfItem.y());
            xmlWriter.writeStartElement("point");
            xmlWriter.writeCharacters(strPointf);
            xmlWriter.writeEndElement();
        }

        xmlWriter.writeEndElement();
    }
    return true;
}

bool CLogicXmlWriter::UpdataCommentData(QXmlStreamWriter &xmlWriter, const QList<COMMENTDATA>&lstLines)
{
    for each (COMMENTDATA commentItem in lstLines)
    {
        xmlWriter.writeStartElement("detailComment");

        xmlWriter.writeStartElement("pos");
        QString strPos = QString::number(commentItem.pfPos.x()) + "," + QString::number(commentItem.pfPos.y());
        xmlWriter.writeCharacters(strPos);
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("comment");
        xmlWriter.writeCharacters(commentItem.strComment);
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("rect");
        QString strRect = QString::number(commentItem.rfRect.x()) + "," + QString::number(commentItem.rfRect.y()) + "," +
            QString::number(commentItem.rfRect.width()) + "," + QString::number(commentItem.rfRect.height());
        xmlWriter.writeCharacters(strRect);
        xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();
    }
    return true;
}
