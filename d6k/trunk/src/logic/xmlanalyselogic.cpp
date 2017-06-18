/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  xmlanalyselogic.cpp
�ļ�ʵ�ֹ��� :  ���������ļ�
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
#include <QMessageBox>
#include <QDomDocument>
#include <QDomElement>
#include "xmlanalyselogic.h"
#include "logiccontainter.h"
#include "logicmodel.h"
#include "logicxmlreader.h"

/*! \fn CXmlAnalyseLogic::CXmlAnalyseLogic(QObject *parent)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::CXmlAnalyseLogic(QObject *parent)
** \details ���캯��
** \param  ����
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
CXmlAnalyseLogic::CXmlAnalyseLogic(QObject *parent)
    : QObject(parent)
{
    m_pObjectInfo = nullptr;
}

CXmlAnalyseLogic::~CXmlAnalyseLogic()
{
    delete m_pObjectInfo;
}

/*! \fn CXmlAnalyseLogic::SaveLogicObj(const QString &strFilename, CLogicContainter *pLogicContainter)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::SaveLogicObj(const QString &strFilename, CLogicContainter *pLogicContainter)
** \details ���湤���ļ�
** \param  
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
// void CXmlAnalyseLogic::SaveLogicObj(const QString &strFilename, CLogicContainter *pLogicContainter)
// {
//     QFile xmlFile(strFilename);
// 
//     if (xmlFile.open(QIODevice::WriteOnly | QIODevice::Text))
//     {
//         QXmlStreamWriter writer(&xmlFile);
//         writer.setAutoFormatting(true);
//         writer.writeStartDocument();
//         writer.writeStartElement("LogicConifg");
//         //��װ��������
//         SaveProAttr(pLogicContainter, writer);
// 
//         writer.writeEndElement();
//         writer.writeEndDocument();
//         xmlFile.close();
//     }
//     else
//     {
//         QMessageBox::warning(0, tr("Warning"), tr("save logic info Error!"));
//     }
// }

/*! \fn CXmlAnalyseLogic::SaveProAttr(CLogicContainter *pLogicContainter, QXmlStreamWriter &xmlWriter)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::SaveProAttr(CLogicContainter *pLogicContainter, QXmlStreamWriter &xmlWriter)
** \details ���湤�̵�����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
// void CXmlAnalyseLogic::SaveProAttr(CLogicContainter *pLogicContainter, QXmlStreamWriter &xmlWriter)
// {
//     Q_ASSERT(pLogicContainter);
//     if (pLogicContainter == nullptr)
//     {
//         return;
//     }
// 
//     xmlWriter.writeStartElement("LogicPro");
// 
//     xmlWriter.writeAttribute("Name", pLogicContainter->GetName());
//     xmlWriter.writeAttribute("Protect", pLogicContainter->GetProtectFlag() ? "1" : "0");
//     xmlWriter.writeAttribute("password", pLogicContainter->GetPassword());
//     xmlWriter.writeAttribute("comment", pLogicContainter->GetComment());
// 
//     std::vector<std::shared_ptr<CNodeLogicContainer>> tNodeContaninter = pLogicContainter->GetNodeContainers();
//     for each (std::shared_ptr<CNodeLogicContainer> NodeContainer in tNodeContaninter)
//     {
//         SaveNodeAttr(NodeContainer, xmlWriter);
//     }
// 
//     xmlWriter.writeEndElement();
// }

/*! \fn CXmlAnalyseLogic::SaveMastAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::SaveMastAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter)
** \details ����mast����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
// void CXmlAnalyseLogic::SaveMastAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter)
// {
//     xmlWriter.writeStartElement("Mast");
// 
//     xmlWriter.writeAttribute("RepeatType", QString::number(pTaskContaner->GetCycleType()));
//     xmlWriter.writeAttribute("MSec", QString::number(pTaskContaner->GetMsec()));
//     xmlWriter.writeAttribute("comment", pTaskContaner->GetComment());
// 
//     for each (std::shared_ptr<CLogicObjectModel> pObjectModel in pTaskContaner->GetSegModels())
//     {
//         xmlWriter.writeStartElement("Seg");
//         xmlWriter.writeCharacters(pObjectModel->GetName());
//         xmlWriter.writeEndElement();
//     }
// 
//     for each (std::shared_ptr<CLogicObjectModel> pObjectModel in pTaskContaner->GetSrModels())
//     {
//         xmlWriter.writeStartElement("SrSeg");
//         xmlWriter.writeCharacters(pObjectModel->GetName());
//         xmlWriter.writeEndElement();
//     }
// 
//     xmlWriter.writeEndElement();
// }

/*! \fn CXmlAnalyseLogic::SaveFastAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::SaveFastAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter)
** \details ����fast����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
// void CXmlAnalyseLogic::SaveFastAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter)
// {
//     xmlWriter.writeStartElement("Fast");
// 
//     xmlWriter.writeAttribute("RepeatType", QString::number(pTaskContaner->GetCycleType()));
//     xmlWriter.writeAttribute("MSec", QString::number(pTaskContaner->GetMsec()));
//     xmlWriter.writeAttribute("comment", pTaskContaner->GetComment());
// 
//     for each (std::shared_ptr<CLogicObjectModel> pObjectModel in pTaskContaner->GetSegModels())
//     {
//         xmlWriter.writeStartElement("Seg");
//         xmlWriter.writeCharacters(pObjectModel->GetName());
//         xmlWriter.writeEndElement();
//     }
// 
//     for each (std::shared_ptr<CLogicObjectModel> pObjectModel in pTaskContaner->GetSrModels())
//     {
//         xmlWriter.writeStartElement("SrSeg");
//         xmlWriter.writeCharacters(pObjectModel->GetName());
//         xmlWriter.writeEndElement();
//     }
// 
//     xmlWriter.writeEndElement();
// 
// }

/*! \fn CXmlAnalyseLogic::SaveAuxAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::SaveAuxAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter)
** \details ����aux����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
// void CXmlAnalyseLogic::SaveAuxAttr(std::shared_ptr<CTaskContainer> pTaskContaner, QXmlStreamWriter &xmlWriter)
// {
//     xmlWriter.writeStartElement("Aux");
// 
//     xmlWriter.writeAttribute("RepeatType", QString::number(pTaskContaner->GetCycleType()));
//     xmlWriter.writeAttribute("MSec", QString::number(pTaskContaner->GetMsec()));
//     xmlWriter.writeAttribute("comment", pTaskContaner->GetComment());
// 
//     for each (std::shared_ptr<CLogicObjectModel> pObjectModel in pTaskContaner->GetSegModels())
//     {
//         xmlWriter.writeStartElement("Seg");
//         xmlWriter.writeCharacters(pObjectModel->GetName());
//         xmlWriter.writeEndElement();
//     }
// 
//     for each (std::shared_ptr<CLogicObjectModel> pObjectModel in pTaskContaner->GetSrModels())
//     {
//         xmlWriter.writeStartElement("SrSeg");
//         xmlWriter.writeCharacters(pObjectModel->GetName());
//         xmlWriter.writeEndElement();
//     }
// 
//     xmlWriter.writeEndElement();
// 
// }

/*! \fn CXmlAnalyseLogic::SaveNodeAttr(std::shared_ptr<CNodeLogicContainer> pNodeContainer, QXmlStreamWriter &xmlWriter)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::SaveNodeAttr(std::shared_ptr<CNodeLogicContainer> pNodeContainer, QXmlStreamWriter &xmlWriter)
** \details ����node�ڵ�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
// void CXmlAnalyseLogic::SaveNodeAttr(std::shared_ptr<CNodeLogicContainer> pNodeContainer, QXmlStreamWriter &xmlWriter)
// {
//     xmlWriter.writeStartElement("LogicNode");
// 
//     xmlWriter.writeAttribute("TagName", pNodeContainer->GetName());
// 
//     std::shared_ptr<CTaskContainer> tMastContaner = pNodeContainer->GetMastContainer();
//     SaveMastAttr(tMastContaner, xmlWriter);
// 
//     std::shared_ptr<CTaskContainer> tFastContaner = pNodeContainer->GetFastContainer();
//     SaveFastAttr(tFastContaner, xmlWriter);
// 
//     std::shared_ptr<CTaskContainer> tAuxContaner = pNodeContainer->GetAuxContainer();
//     SaveAuxAttr(tAuxContaner, xmlWriter);
// 
// 
//     xmlWriter.writeEndElement();
// 
// }

/*! \fn CXmlAnalyseLogic::OpenLogicObj(const QString &strFilePath, CLogicContainter *pLogicContainter)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::OpenLogicObj(const QString &strFilePath, CLogicContainter *pLogicContainter)
** \details �������ļ�  ���ݶ�ȡģ����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
bool CXmlAnalyseLogic::OpenLogicObj(QDomDocument *pXml, QDomElement *pRoot, const QString &strFilePath, CLogicContainter *pLogicContainter)
{
    //�����ļ�����Ϣд��ģ����
    Q_ASSERT(pLogicContainter);

    if (pLogicContainter == nullptr)
    {
        return false;
    }
    //�Ƿ����logic���ݱ�ʶ
    bool bLogicExistFlag = false;
    //����·��
    m_strLogicPath = strFilePath;

    QDomNode projectNode = pXml->firstChild();

    while (!projectNode.isNull())
    {
        QDomElement tProjElement = projectNode.toElement();
        if (tProjElement.tagName() == "Project")
        {
            QDomNode LogicNode = tProjElement.firstChild();

            while (!LogicNode.isNull())
            {
                QDomElement LogicElement = LogicNode.toElement(); // try to convert the node to an element.
                if (!LogicElement.isNull())
                {
                    QString strTmp = LogicElement.tagName();// the node really is an element.

                    if (strTmp.compare("Logic") == 0)
                    {
                        bLogicExistFlag = true;
                        //logic���Լ���
                        pLogicContainter->SetName(LogicElement.attribute("Name"));
                        pLogicContainter->SetComment(LogicElement.attribute("comment"));
                        pLogicContainter->SetPassword(LogicElement.attribute("password"));
                        pLogicContainter->SetProtectFlag(LogicElement.attribute("Protect") == "0" ? false : true);


                        QDomNode tagNode = LogicElement.firstChild();

                        std::shared_ptr<CNodeLogicContainer> tNodeContainer = nullptr;

                        while (!tagNode.isNull())
                        {
                            QDomElement tagElement = tagNode.toElement(); // try to convert the node to an element.

                            strTmp = tagElement.tagName();
                            if (strTmp.compare("LogicNode") == 0)
                            {
                                QString strTagName = tagElement.attribute("TagName");
                                tNodeContainer = pLogicContainter->CreateNodeContainer(strTagName);

                                AnalyseNodeObj(strTagName, tagNode, tNodeContainer);
                            }

                            tagNode = tagNode.nextSibling();
                        }
                    }
                }

                LogicNode = LogicNode.nextSibling();
            }
        }


        projectNode = projectNode.nextSibling();
    }

    return bLogicExistFlag;
    
}

/*! \fn CXmlAnalyseLogic::AnalyseLogicObj(QIODevice *pDevice, CLogicContainter *pLogicContainter)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::AnalyseLogicObj(QIODevice *pDevice, CLogicContainter *pLogicContainter)
** \details ��ȡlogic����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
// bool CXmlAnalyseLogic::AnalyseLogicObj(QIODevice *pDevice, CLogicContainter *pLogicContainter)
// {
//     Q_ASSERT(pLogicContainter);
// 
//     if (pLogicContainter == nullptr)
//     {
//         return false;
//     }
// 
//     QXmlStreamReader xmlReader(pDevice);
// 
//     while (!xmlReader.atEnd() && (!xmlReader.hasError()))
//     {
//         //������������
//         xmlReader.readNext();
//         if (xmlReader.isStartElement())
//         {
//             if (xmlReader.name().toString() == "LogicPro")
//             {
//                 pLogicContainter->SetName(xmlReader.attributes().value("Name").toString());
//                 pLogicContainter->SetComment(xmlReader.attributes().value("comment").toString());
//                 pLogicContainter->SetPassword(xmlReader.attributes().value("password").toString());
//                 pLogicContainter->SetProtectFlag(xmlReader.attributes().value("Protect").toString() == "0" ? false : true);
// 
//                 AnalyseNodeAttr(xmlReader,pLogicContainter);
//             }
// 
//         }
//     }
// 
//     return true;
// }

/*! \fn CXmlAnalyseLogic::AnalyseNodeAttr(QXmlStreamReader &xmlReader, CLogicContainter *pLogicContainter)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::AnalyseNodeAttr(QXmlStreamReader &xmlReader, CLogicContainter *pLogicContainter)
** \details ��ȡnode����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
// void CXmlAnalyseLogic::AnalyseNodeAttr(QXmlStreamReader &xmlReader, CLogicContainter *pLogicContainter)
// {
// 
//     Q_ASSERT(pLogicContainter);
// 
//     if (pLogicContainter == nullptr)
//     {
//         return;
//     }
// 
//     std::shared_ptr<CNodeLogicContainer> tNodeContainer = nullptr;
// 
//     while (!xmlReader.atEnd() && (!xmlReader.hasError()))
//     {
//         //������������
//         xmlReader.readNext();
// 
// 
//         if (xmlReader.isStartElement())
//         {
//             if (xmlReader.name().toString() == "LogicNode")
//             {
//                 tNodeContainer = pLogicContainter->CreateNodeContainer(xmlReader.attributes().value("TagName").toString());
//                 AnalyseNodeObj(xmlReader,tNodeContainer);
// 
//             }
// 
//         }
// 
//         if (xmlReader.isEndElement())
//         {
//             if (xmlReader.name().toString() == "LogicPro")
//             {
//                 return;
//             }
//         }
//     }
// }

//�����ڵ�
/*! \fn CXmlAnalyseLogic::AnalyseNodeObj(QXmlStreamReader &xmlReader, std::shared_ptr<CNodeLogicContainer> tNodeContainer)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::AnalyseNodeObj(QXmlStreamReader &xmlReader, std::shared_ptr<CNodeLogicContainer> tNodeContainer)
** \details �����ڵ�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CXmlAnalyseLogic::AnalyseNodeObj(const QString &strTagName, QDomNode &NodeDom, std::shared_ptr<CNodeLogicContainer> tNodeContainer)
{
    Q_ASSERT(tNodeContainer);
    if (tNodeContainer == nullptr)
    {
        return;
    }

    QDomNode objectNode = NodeDom.firstChild();

    QString strTmp = "";

    while (!objectNode.isNull())
    {
        QDomElement ObjectElement = objectNode.toElement(); // try to convert the node to an element.

        strTmp = ObjectElement.tagName();

        if (strTmp.compare("Mast") == 0)
        {
            //����mast�ڵ�����
            AnalyseMastObj(strTagName, ObjectElement, tNodeContainer);
        }
        else if (strTmp.compare("Fast") == 0)
        {
            //����fast�ڵ�����
            AnalyseFastObj(strTagName, ObjectElement, tNodeContainer);
        }
        else if (strTmp.compare("Aux") == 0)
        {
            //����aux�ڵ�����
            AnalyseAuxObj(strTagName, ObjectElement, tNodeContainer);
        }

        objectNode = objectNode.nextSibling();
    }

}

/*! \fn CXmlAnalyseLogic::AnalyseMastObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::AnalyseMastObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer)
** \details ����mast�ڵ�����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CXmlAnalyseLogic::AnalyseMastObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer)
{
    std::shared_ptr<CTaskContainer> tMastContaner = tNodeContainer->CreateMastContainer();
    tMastContaner->SetComment(ObjectElement.attribute("comment"));
    tMastContaner->SetCycleType(ObjectElement.attribute("RepeatType").toInt());
    tMastContaner->SetMsec(ObjectElement.attribute("MSec").toInt());


    QDomNode segOrSrNode = ObjectElement.firstChild();

    QString strTmp = "";

    while (!segOrSrNode.isNull())
    {
        QDomElement segOrSrElement = segOrSrNode.toElement(); // try to convert the node to an element.

        strTmp = segOrSrElement.tagName();
        if (strTmp.compare("Seg") == 0)
        {
            QString strSegName = segOrSrElement.text();
            std::shared_ptr<CLogicObjectModel> tObjectSegModel = tMastContaner->CreateSegModel(strSegName);

            strSegName = m_strLogicPath + "/logic/" + strTagName + "/mast/seg/" + strSegName + ".xml";
            //�򿪶�Ӧ��ͼ���ļ�
            m_pObjectInfo->OpenLogicXmlFile(strSegName, tObjectSegModel);

        }
        else if (strTmp.compare("SrSeg") == 0)
        {
            QString strSegName = segOrSrElement.text();
            std::shared_ptr<CLogicObjectModel> tObjectSegModel = tMastContaner->CreateSrModel(strSegName);
            //�򿪶�Ӧ��ͼ���ļ�
            strSegName = m_strLogicPath + "/logic/" + strTagName + "/mast/sr/" + strSegName + ".xml";
            m_pObjectInfo->OpenLogicXmlFile(strSegName, tObjectSegModel);

        }

        segOrSrNode = segOrSrNode.nextSibling();
    }
}

/*! \fn CXmlAnalyseLogic::AnalyseFastObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::AnalyseFastObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer)
** \details ����fast�ڵ�����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CXmlAnalyseLogic::AnalyseFastObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer)
{
    std::shared_ptr<CTaskContainer> tFastContaner = tNodeContainer->CreateFastContainer();
    tFastContaner->SetComment(ObjectElement.attribute("comment"));
    tFastContaner->SetCycleType(ObjectElement.attribute("RepeatType").toInt());
    tFastContaner->SetMsec(ObjectElement.attribute("MSec").toInt());


    QDomNode segOrSrNode = ObjectElement.firstChild();

    QString strTmp = "";

    while (!segOrSrNode.isNull())
    {
        QDomElement segOrSrElement = segOrSrNode.toElement(); // try to convert the node to an element.

        strTmp = segOrSrElement.tagName();
        if (strTmp.compare("Seg") == 0)
        {
            QString strSegName = segOrSrElement.text();
            std::shared_ptr<CLogicObjectModel> tObjectSegModel = tFastContaner->CreateSegModel(strSegName);
            //������Ӧ���߼�ͼ�����ļ�
            strSegName = m_strLogicPath + "/logic/" + strTagName + "/fast/seg/" + strSegName + ".xml";
            m_pObjectInfo->OpenLogicXmlFile(strSegName, tObjectSegModel);

        }
        else if (strTmp.compare("SrSeg") == 0)
        {
            QString strSegName = segOrSrElement.text();
            std::shared_ptr<CLogicObjectModel> tObjectSegModel = tFastContaner->CreateSrModel(strSegName);
            //������Ӧ���߼�ͼ�����ļ�
            strSegName = m_strLogicPath + "/logic/" + strTagName + "/fast/sr/" + strSegName + ".xml";
            m_pObjectInfo->OpenLogicXmlFile(strSegName, tObjectSegModel);

        }

        segOrSrNode = segOrSrNode.nextSibling();
    }
}

/*! \fn CXmlAnalyseLogic::AnalyseAuxObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::AnalyseAuxObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer)
** \details ����aux�ڵ�����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CXmlAnalyseLogic::AnalyseAuxObj(const QString &strTagName, QDomElement &ObjectElement, std::shared_ptr<CNodeLogicContainer> tNodeContainer)
{
    std::shared_ptr<CTaskContainer> tAuxContaner = tNodeContainer->CreateAuxContainer();
    tAuxContaner->SetComment(ObjectElement.attribute("comment"));
    tAuxContaner->SetCycleType(ObjectElement.attribute("RepeatType").toInt());
    tAuxContaner->SetMsec(ObjectElement.attribute("MSec").toInt());


    QDomNode segOrSrNode = ObjectElement.firstChild();

    QString strTmp = "";

    while (!segOrSrNode.isNull())
    {
        QDomElement segOrSrElement = segOrSrNode.toElement(); // try to convert the node to an element.

        strTmp = segOrSrElement.tagName();
        if (strTmp.compare("Seg") == 0)
        {
            QString strSegName = segOrSrElement.text();
            std::shared_ptr<CLogicObjectModel> tObjectSegModel = tAuxContaner->CreateSegModel(strSegName);
            //������Ӧ��ͼ���ļ�
            strSegName = m_strLogicPath + "/logic/" + strTagName + "/aux0/seg/" + strSegName + ".xml";
            m_pObjectInfo->OpenLogicXmlFile(strSegName, tObjectSegModel);

        }
        else if (strTmp.compare("SrSeg") == 0)
        {
            QString strSegName = segOrSrElement.text();
            std::shared_ptr<CLogicObjectModel> tObjectSegModel = tAuxContaner->CreateSrModel(strSegName);
            //������Ӧ��ͼ���ļ�
            strSegName = m_strLogicPath + "/logic/" + strTagName + "/aux0/sr/" + strSegName + ".xml";
            m_pObjectInfo->OpenLogicXmlFile(strSegName, tObjectSegModel);

        }

        segOrSrNode = segOrSrNode.nextSibling();
    }
}

/*! \fn CXmlAnalyseLogic::AnalyseMastAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tMastContaner, const QString strNode)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::AnalyseMastAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tMastContaner, const QString strNode)
** \details ��ȡmast����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
// void CXmlAnalyseLogic::AnalyseMastAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tMastContaner, const QString strNode)
// {
//     Q_ASSERT(tMastContaner);
// 
//     if (tMastContaner == nullptr)
//     {
//         return;
//     }
// 
//     while (!xmlReader.atEnd() && (!xmlReader.hasError()))
//     {
//         //������������
//         xmlReader.readNext();
// 
// 
//         if (xmlReader.isStartElement())
//         {
//             if (xmlReader.name().toString() == "Seg")
//             {
//                 QString strSegName = xmlReader.readElementText();
//                 std::shared_ptr<CLogicObjectModel> tObjectSegModel = tMastContaner->CreateSegModel(strSegName);
// 
//                 strSegName = m_strLogicPath + "/logic_config/" + strNode + "/mast/seg/" + strSegName + ".xml";
//                 m_pObjectInfo->OpenLogicXmlFile(strSegName, tObjectSegModel);
//             }
//             else if (xmlReader.name().toString() == "SrSeg")
//             {
//                 QString strSrName = xmlReader.readElementText();
//                 std::shared_ptr<CLogicObjectModel> tObjectSegModel = tMastContaner->CreateSrModel(strSrName);
// 
//                 strSrName = m_strLogicPath + "/logic_config/" + strNode + "/mast/sr/" + strSrName + ".xml";
//                 m_pObjectInfo->OpenLogicXmlFile(strSrName, tObjectSegModel);
//             }
// 
//         }
// 
//         if (xmlReader.isEndElement())
//         {
//             if (xmlReader.name().toString() == "Mast")
//             {
//                 return;
//             }
//         }
//     }
// }

/*! \fn CXmlAnalyseLogic::AnalyseFastAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tFastContaner, const QString strNode)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::AnalyseFastAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tFastContaner, const QString strNode)
** \details ��ȡfast����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
// void CXmlAnalyseLogic::AnalyseFastAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tFastContaner, const QString strNode)
// {
//     Q_ASSERT(tFastContaner);
// 
//     if (tFastContaner == nullptr)
//     {
//         return;
//     }
// 
//     while (!xmlReader.atEnd() && (!xmlReader.hasError()))
//     {
//         //������������
//         xmlReader.readNext();
// 
// 
//         if (xmlReader.isStartElement())
//         {
//             if (xmlReader.name().toString() == "Seg")
//             {
//                 QString strSegName = xmlReader.readElementText();
//                 std::shared_ptr<CLogicObjectModel> tObjectSegModel = tFastContaner->CreateSegModel(strSegName);
// 
//                 strSegName = m_strLogicPath + "/logic_config/" + strNode + "/fast/seg/" + strSegName + ".xml";
//                 m_pObjectInfo->OpenLogicXmlFile(strSegName, tObjectSegModel);
//             }
//             else if (xmlReader.name().toString() == "SrSeg")
//             {
//                 QString strSrName = xmlReader.readElementText();
//                 std::shared_ptr<CLogicObjectModel> tObjectSegModel = tFastContaner->CreateSrModel(strSrName);
// 
//                 strSrName = m_strLogicPath + "/logic_config/" + strNode + "/fast/sr/" + strSrName + ".xml";
//                 m_pObjectInfo->OpenLogicXmlFile(strSrName, tObjectSegModel);
//             }
// 
//         }
// 
//         if (xmlReader.isEndElement())
//         {
//             if (xmlReader.name().toString() == "Fast")
//             {
//                 return;
//             }
//         }
//     }
// }

/*! \fn CXmlAnalyseLogic::AnalyseAuxAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tAuxContaner, const QString strNode)
*********************************************************************************************************
** \brief CXmlAnalyseLogic::AnalyseAuxAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tAuxContaner, const QString strNode)
** \details ��ȡaux����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
// void CXmlAnalyseLogic::AnalyseAuxAttr(QXmlStreamReader &xmlReader, std::shared_ptr<CTaskContainer> tAuxContaner, const QString strNode)
// {
//     Q_ASSERT(tAuxContaner);
// 
//     if (tAuxContaner == nullptr)
//     {
//         return;
//     }
// 
//     while (!xmlReader.atEnd() && (!xmlReader.hasError()))
//     {
//         //������������
//         xmlReader.readNext();
// 
// 
//         if (xmlReader.isStartElement())
//         {
//             if (xmlReader.name().toString() == "Seg")
//             {
//                 QString strSegName = xmlReader.readElementText();
//                 std::shared_ptr<CLogicObjectModel> tObjectSegModel = tAuxContaner->CreateSegModel(strSegName);
// 
//                 strSegName = m_strLogicPath + "/logic_config/" + strNode + "/aux0/seg/" + strSegName + ".xml";
//                 m_pObjectInfo->OpenLogicXmlFile(strSegName, tObjectSegModel);
//             }
//             else if (xmlReader.name().toString() == "SrSeg")
//             {
//                 QString strSrName = xmlReader.readElementText();
//                 std::shared_ptr<CLogicObjectModel> tObjectSegModel = tAuxContaner->CreateSrModel(strSrName);
// 
//                 strSrName = m_strLogicPath + "/logic_config/" + strNode + "/aux0/sr/" + strSrName + ".xml";
//                 m_pObjectInfo->OpenLogicXmlFile(strSrName, tObjectSegModel);
//             }
// 
//         }
// 
//         if (xmlReader.isEndElement())
//         {
//             if (xmlReader.name().toString() == "Aux")
//             {
//                 return;
//             }
//         }
//     }
// }
