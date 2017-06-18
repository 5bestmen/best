#include <QXmlStreamReader>
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamWriter>

#include "xmldataanalyse.h"


CXmlDataAnalyse::CXmlDataAnalyse(CModelContainer *pModelContainer,QObject *parent)
    : QObject(parent)
{
    Q_ASSERT(pModelContainer);
    if (pModelContainer == nullptr)
    {
        return;
    }

    m_pDataContainer = pModelContainer;
}

CXmlDataAnalyse::~CXmlDataAnalyse()
{

}

void CXmlDataAnalyse::AnalyseTreeItems(const QString &strFilename)
{
    if (strFilename.isEmpty())
    {
        QMessageBox::warning(0, tr("Warning"), tr("read Config file is null,please check!"));
        return;
    }

    m_strFilename = strFilename;

    QFile fPluginFile(strFilename);
    if (fPluginFile.open(QFile::ReadOnly | QFile::Text))
    {
        AnalyseTreeXmlData(&fPluginFile);
        fPluginFile.close();
    }
    else
    {
        QMessageBox::warning(0, tr("warning"), tr("open file %1 error!").arg(strFilename));
        return;
    }
}

void CXmlDataAnalyse::AnalyseTreeXmlData(QIODevice *pDevice)
{
    QXmlStreamReader xmlReader(pDevice);

    while (!xmlReader.atEnd() && (!xmlReader.hasError()))
    {
        //遍历所有配置
        xmlReader.readNext();
        if (xmlReader.isStartElement())
        {
            //qDebug() << "xmlReader:"<<xmlReader.name().toString();
            if (xmlReader.name().toString() == "s")
            {
                //
                std::shared_ptr<CModelScene> pScene = m_pDataContainer->CreateModelScene(xmlReader.attributes().value("Name").toString());

                AnalyseSceneChildData(xmlReader,pScene);
            }
        }
    }
}


void CXmlDataAnalyse::AnalyseSceneChildData(QXmlStreamReader &xmlReader, std::shared_ptr<CModelScene> pScene)
{
    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "e")
            {
                //场景下面是组
                //组
                std::shared_ptr<CModelGroup> tGroup = pScene->CreateModelGroup(xmlReader.attributes().value("Name").toString());
                AnalyseGroupChildData(xmlReader,tGroup);
            }
            else if (xmlReader.name().toString() == "m")
            {
                //场景下面直接是模型
                std::shared_ptr<CObjectModelInfo> tModel = pScene->CreateObjectModel(xmlReader.attributes().value("Name").toString());
                tModel->SetModelId(xmlReader.attributes().value("Guid").toString());
                tModel->SetTagName(xmlReader.attributes().value("TagName").toString());

                AnalyseModelChildData(xmlReader, tModel);
            }
        }

        if (xmlReader.isEndElement() && xmlReader.name().toString() == "s")
        {
            //一个场景解析结束
            break;
        }

    }
}

void CXmlDataAnalyse::AnalyseGroupChildData(QXmlStreamReader &xmlReader, std::shared_ptr<CModelGroup> pGroup)
{
    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "m")
            {
                //组下面的模型
                std::shared_ptr<CObjectModelInfo> pModel = pGroup->CreateObjectModel(xmlReader.attributes().value("Name").toString());
                pModel->SetModelId(xmlReader.attributes().value("Guid").toString());
                pModel->SetTagName(xmlReader.attributes().value("TagName").toString());

                AnalyseModelChildData(xmlReader, pModel);
            }

        }

        if (xmlReader.isEndElement() && xmlReader.name().toString() == "e")
        {
            //一个场景解析结束
            break;
        }

    }
}

void CXmlDataAnalyse::AnalyseModelChildData(QXmlStreamReader &xmlReader, std::shared_ptr<CObjectModelInfo> pModel)
{
    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "Static")
            {
                //模型下面静态数据
                std::shared_ptr<CStaticProperty> tStatic = pModel->CreateStaticProperty(xmlReader.attributes().value("Name").toString());
                tStatic->SetDescription(xmlReader.attributes().value("Destriber").toString());
                tStatic->SetSzValue(xmlReader.attributes().value("Value").toString());
            }
            else if (xmlReader.name().toString() == "Dynamic")
            {
                //动态属性
                std::shared_ptr<CDynamicProperty> tDynamic = pModel->CreateDynamicProperty(xmlReader.attributes().value("Name").toString());

                tDynamic->SetDescription(xmlReader.attributes().value("Destriber").toString());
                tDynamic->SetDataType(xmlReader.attributes().value("DataType").toInt());
                tDynamic->SetReadOnlyFlag(xmlReader.attributes().value("ReadWrite").toInt()==0 ? false:true);
            }
        }

        if (xmlReader.isEndElement() && xmlReader.name().toString() == "m")
        {
            //一个场景解析结束
            break;
        }

    }
}

void CXmlDataAnalyse::SaveDataToFile()
{
    if (m_strFilename.isEmpty())
    {
        return;
    }

    QFile xmlFile(m_strFilename);

    if (xmlFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QXmlStreamWriter writer(&xmlFile);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("scene");
        //组装数据体
        WriteApplicationInfo(writer);

        writer.writeEndElement();
        writer.writeEndDocument();
        xmlFile.close();
    }
    else
    {
        QMessageBox::warning(0, tr("Warning"),tr("Save Error,File cannot open"));
    }

}

void CXmlDataAnalyse::WriteApplicationInfo(QXmlStreamWriter &xmlWriter)
{

    std::vector<std::shared_ptr<CModelScene>> vModelScene = m_pDataContainer->GetModelScene();

    std::vector<std::shared_ptr<CModelScene>>::iterator it = vModelScene.begin();

    while (it != vModelScene.end())
    {
        xmlWriter.writeStartElement("s");
        xmlWriter.writeAttribute("Name",(*it)->GetName());

        std::vector<std::shared_ptr<CModelGroup>> groupModel = (*it)->GetModelGrps();
        std::vector<std::shared_ptr<CModelGroup>>::iterator itGroup = groupModel.begin();

        while (itGroup != groupModel.end())
        {
            //遍历 所有group
            xmlWriter.writeStartElement("e");
            xmlWriter.writeAttribute("Name", (*itGroup)->GetName());
            //group  下 model
            std::vector<std::shared_ptr<CObjectModelInfo>> tModel = (*itGroup)->GetModels();
            std::vector<std::shared_ptr<CObjectModelInfo>>::iterator itModel = tModel.begin();
            while (itModel != tModel.end())
            {
                //遍历model
                xmlWriter.writeStartElement("m");
                xmlWriter.writeAttribute("Name", (*itModel)->GetName());
                xmlWriter.writeAttribute("TagName", (*itModel)->GetTagName());
                xmlWriter.writeAttribute("Guid", (*itModel)->GetGuid());
                //属性
                std::vector<std::shared_ptr<CStaticProperty>> tStatic = (*itModel)->GetStaticProperty();
                std::vector<std::shared_ptr<CStaticProperty>>::iterator itStatic = tStatic.begin();

                while (itStatic != tStatic.end())
                {
                    xmlWriter.writeStartElement("Static");
                    xmlWriter.writeAttribute("Name", (*itStatic)->GetTagName());
                    xmlWriter.writeAttribute("Destriber", (*itStatic)->GetDescription());
                    xmlWriter.writeAttribute("Value", (*itStatic)->GetSzValue());
                    xmlWriter.writeEndElement();

                    itStatic++;
                }


                std::vector<std::shared_ptr<CDynamicProperty>> tDynamic = (*itModel)->GetDynamicProperty();
                std::vector<std::shared_ptr<CDynamicProperty>>::iterator itDynamic = tDynamic.begin();
                while (itDynamic != tDynamic.end())
                {
                    xmlWriter.writeStartElement("Dynamic");
                    xmlWriter.writeAttribute("Name", (*itDynamic)->GetTagName());
                    xmlWriter.writeAttribute("Destriber", (*itDynamic)->GetDescription());
                    xmlWriter.writeAttribute("ReadWrite", (*itDynamic)->GetReadOnlyFlag() ? "1" : "0");
                    xmlWriter.writeEndElement();
                    itDynamic++;
                }
                //属性end
                xmlWriter.writeEndElement();

                itModel++;
            }


            //gooup 下
            xmlWriter.writeEndElement();
            itGroup++;
        }

        std::vector<std::shared_ptr<CObjectModelInfo>> myMode = (*it)->GetModels();
        std::vector<std::shared_ptr<CObjectModelInfo>>::iterator itModel = myMode.begin();

        while (itModel != myMode.end())
        {
            //遍历 model
            //遍历model
            xmlWriter.writeStartElement("m");
            xmlWriter.writeAttribute("Name", (*itModel)->GetName());
            xmlWriter.writeAttribute("TagName", (*itModel)->GetTagName());
            xmlWriter.writeAttribute("Guid", (*itModel)->GetGuid());
            //属性
            std::vector<std::shared_ptr<CStaticProperty>> tStatic = (*itModel)->GetStaticProperty();
            std::vector<std::shared_ptr<CStaticProperty>>::iterator itStatic = tStatic.begin();

            while (itStatic != tStatic.end())
            {
                xmlWriter.writeStartElement("Static");
                xmlWriter.writeAttribute("Name", (*itStatic)->GetTagName());
                xmlWriter.writeAttribute("Destriber", (*itStatic)->GetDescription());
                xmlWriter.writeAttribute("Value", (*itStatic)->GetSzValue());
                xmlWriter.writeEndElement();

                itStatic++;
            }


            std::vector<std::shared_ptr<CDynamicProperty>> tDynamic = (*itModel)->GetDynamicProperty();
            std::vector<std::shared_ptr<CDynamicProperty>>::iterator itDynamic = tDynamic.begin();
            while (itDynamic != tDynamic.end())
            {
                xmlWriter.writeStartElement("Dynamic");
                xmlWriter.writeAttribute("Name", (*itDynamic)->GetTagName());
                xmlWriter.writeAttribute("Destriber", (*itDynamic)->GetDescription());
                xmlWriter.writeAttribute("ReadWrite", (*itDynamic)->GetReadOnlyFlag() ? "1" : "0");
                xmlWriter.writeEndElement();
                itDynamic++;
            }
            //属性end
            xmlWriter.writeEndElement();

            itModel++;
        }

        xmlWriter.writeEndElement();

        it++;
    }

}
