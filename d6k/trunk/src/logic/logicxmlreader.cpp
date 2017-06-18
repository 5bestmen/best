/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :  logicxmlreader.cpp
文件实现功能 :  读取数据 创建数据模型
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明> 逻辑编辑
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

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
** \details 读取配置文件  创建数据模型
** \param  fileName:配置文件名称   pObjectModel：数据模型
** \return
** \author ww
** \date 2017年1月4日
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
** \details 读取逻辑块数据
** \param  
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CLogicXmlReader::AnalyseLogicItemData(QXmlStreamReader &xmlReader, std::shared_ptr<CLogicObjectModel> pObjectModel)
{
    // 创建编号
    int nCreateNum = xmlReader.attributes().value("createNum").toInt();
    //执行编号
    int nExecNum = xmlReader.attributes().value("execNum").toInt();
    //目标类型
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
                //处理
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
				//绑点信息  名称
				std::shared_ptr<CBinProperty> tPinInfo = tBlock->CreatePinProperty(xmlReader.attributes().value("PinName").toString());
				//元件类型
				tPinInfo->SetPinType(2);
				//pin index
				tPinInfo->SetPinIndex(xmlReader.attributes().value("PinIndex").toInt());
				//绑点
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
** \details 读取直线对象数据
** \param
** \return
** \author ww
** \date 2017年1月4日
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
** \details 读取注释信息
** \param
** \return
** \author ww
** \date 2017年1月4日
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
