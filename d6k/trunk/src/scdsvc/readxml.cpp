

#include "readxml.h"

#include <memory>

#include <QHostInfo>
#include <QFile>
#include <QXmlStreamReader>

#include "scdsvc_define.h"

template<typename T>
bool SortFuction(const T& T1, const T& T2)
{
    return T1->OccNo < T2->OccNo;
}

CReadXml::CReadXml(const QString& strProjectPath)
{
    m_strProjectPath = strProjectPath;
    m_strScadaPath = m_strProjectPath + "/scada/";
    m_strNodeInfoName = m_strProjectPath + "/node/" + "node.xml";

    m_pChannelMgr = std::make_shared<CChannelMgr>();
}

CReadXml::~CReadXml()
{

}

bool CReadXml::LoadDataFromXml()
{
    // 从配置的node.xml中获取前置工作组和工作站信息，然后根据这些信息获取fes.xml和workstation.xml配置信息。
    // todo：先直接读取fes.xml配置信息。
    // 从node.xml中读取fes配置信息。

    if (m_strNodeInfoName.isEmpty())
    {
        return false;
    }

    QDomDocument doc;

    QFile file(m_strNodeInfoName);

    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    if (!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    //读取本机名称和下面信息对应
    //QString szRealHostName = QHostInfo::localHostName();

    QDomElement domElem = doc.documentElement();

    if (domElem.tagName() != "node")
    {
        file.close();
        return false;
    }

    QDomNode nChild = domElem.firstChild();

    for (; !nChild.isNull(); nChild = nChild.nextSibling())
    {
        QDomElement de = nChild.toElement();

        //读取到前置节点组信息
        if (de.nodeName() == "nfesgroup")
        {
            // 从node.xml中获取fes信息
            if (!GetFesInfoFrom_NodeXml(de))
            {
                file.close();
                return false;
            }
        }
        else if (de.nodeName() == "nworksstation")
        {
            // workstation
            // todo:
        }
    }

    // 读取fes.xml内容
    if (!Load_FesXml())
    {
        return false;
    }
    
    file.close();
    return true;
}

bool CReadXml::GetFesInfoFrom_NodeXml(QDomElement& domElem)
{
    for (QDomNode nGrandChild = domElem.firstChild(); !nGrandChild.isNull(); nGrandChild = nGrandChild.nextSibling())
    {
        //读取到前置组信息
        QDomElement nGC = nGrandChild.toElement();
        /*if (nGC.nodeName() != "nf")
        {
            continue;
        }*/

        QString szGrp = nGC.attribute("Group");
        if (szGrp.isEmpty())
        {
            return false;
        }

        QVector<FesInfo> vecFesInfo;
        for (QDomNode nThirdChild = nGC.firstChild(); !nThirdChild.isNull(); nThirdChild = nThirdChild.nextSibling())
        {
            QDomElement de = nThirdChild.toElement();

            //QString szHostInfo = de.attribute("HostName");

            FesInfo fesInfo;
            fesInfo.strNodeName = de.attribute("NodeName");
            fesInfo.strHostName = de.attribute("HostName");
            fesInfo.strIP_A = de.attribute("Network_IP_A");
            fesInfo.strIP_B = de.attribute("Network_IP_B");
            QString strFesCfg = de.attribute("FesConfig");
            if (fesInfo.strNodeName.isEmpty() || fesInfo.strHostName.isEmpty() ||
                fesInfo.strIP_A.isEmpty())
            {
                return false;
            }

            if (!strFesCfg.isEmpty())
            {
                fesInfo.strFesCfg = m_strProjectPath + "/scada/" + strFesCfg;
                
                QFile file(fesInfo.strFesCfg);
                if (!file.exists())
                {
                    // 文件不存在不加载
                    continue;
                }

                m_setCfg.insert(fesInfo.strFesCfg);

                vecFesInfo.push_back(fesInfo);
            }
        }

        if (vecFesInfo.size() > 0)
        {
            auto iter = m_mapFesInfo.find(szGrp);
            if (iter == m_mapFesInfo.end())
            {
                m_mapFesInfo[szGrp] = vecFesInfo;
            }
        }
    }

    if (m_mapFesInfo.size() <= 0)
    {
        return false;
    }

    return true;
}

bool CReadXml::Load_FesXml()
{
    for each (auto i in m_mapFesInfo.keys())
    {
        auto iterI = m_memData.m_mapFesInfo.find(i);
        if (iterI != m_memData.m_mapFesInfo.end())
        {
            continue;
        }

        std::shared_ptr<CNodeAndMemName> nodeMem = m_memData.m_mapFesInfo[i];
        for each (auto j in m_mapFesInfo[i])
        {
            if (nodeMem == nullptr)
            {
                nodeMem = std::make_shared<CNodeAndMemName>();
                m_memData.m_mapFesInfo[i] = nodeMem;
            }
            
            std::shared_ptr<CMemData> memData = nodeMem->m_mapNodeAndShmNameInfo[j.strNodeName];
            if (memData == nullptr)
            {
                memData = std::make_shared<CMemData>();
                nodeMem->m_mapNodeAndShmNameInfo[j.strNodeName] = memData;
            }

            // 节点名
            m_strCrtNodeName.clear();
            m_vecAi.clear();
            m_strCrtNodeName = j.strNodeName;

            if (j.strFesCfg.isEmpty())
            {
                return false;
            }

            

            if (!LoadSingleFesXml(j.strFesCfg, memData))
            {
                return false;
            }
        }
    }

    return true;
}

bool CReadXml::LoadSingleFesXml(const QString& strFesPathName, std::shared_ptr<CMemData> pMemData)
{
    Q_ASSERT(pMemData);

    if (strFesPathName.isEmpty())
    {
        return false;
    }

    QFile file(strFesPathName);
    if (!file.exists())
    {
        // 文件不存在不加载
        return false;
    }

    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        return false;
    }

    m_XmlReader.setDevice(&file);
    m_XmlReader.readNext();
    while (!m_XmlReader.atEnd())
    {
        if (m_XmlReader.isStartElement())
        {
            if (m_XmlReader.name() == "fes")
            {
                ReadFesInfo(pMemData);
            }
            else
            {
                m_XmlReader.raiseError("not valid tagname");
            }
        }

        m_XmlReader.readNext();

    }

    file.close();

    if (m_strCrtNodeName.isEmpty())
    {
        return false;
    }

    // 排序
    if (pMemData->m_vecAiMemData.size() >= 2)
    {
        std::sort(pMemData->m_vecAiMemData.begin(), pMemData->m_vecAiMemData.end(), SortFuction<AIN_PTR>);
    }
    
    if (pMemData->m_vecDiMemData.size() >= 2)
    {
        std::sort(pMemData->m_vecDiMemData.begin(), pMemData->m_vecDiMemData.end(), SortFuction<DIN_PTR>);
    }

    return true;
}

void CReadXml::ReadFesInfo(std::shared_ptr<CMemData> pMemData)
{
    Q_ASSERT(pMemData);

    m_XmlReader.readNext();
    while (!m_XmlReader.atEnd())
    {
        if (m_XmlReader.isEndElement())
        {
            m_XmlReader.readNext();
            break;
        }
        else if (m_XmlReader.isStartElement())
        {
            if (m_XmlReader.name() == "channel")
            {
                ReadChannelInfo(pMemData);
            }
        }

        m_XmlReader.readNext();
    }
}

void CReadXml::ReadChannelInfo(std::shared_ptr<CMemData> pMemData)
{
    Q_ASSERT(pMemData);

    if (m_pChannelMgr == nullptr)
    {
        return;
    }

    CHANNEL_PTR pChannel = Q_NULLPTR;

    TAG_OCCNO_DEF pChannelOccNo = Q_NULLPTR;

    m_XmlReader.readNext();
    while (!m_XmlReader.atEnd())
    {
        if (m_XmlReader.isEndElement())
        {
            m_XmlReader.readNext();
            break;
        }
        else if (m_XmlReader.isStartElement())
        {
            if (m_XmlReader.name() == "c")
            {
                pChannel = std::make_shared<CHANNEL>();

                pChannelOccNo = std::make_shared<CTagNameOccNo>();
                //读取文件
                CChannelPtr ppChannel = m_pChannelMgr->CreateChannel(pChannel);

                pChannel->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

                pChannelOccNo->OccNo = pChannel->OccNo;
                std::memset(pChannelOccNo->TagName, 0, sizeof(pChannelOccNo->TagName));

                /*strncpy(pChannelOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
                    qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));

                strncpy(pChannel->ChannelName,
                    m_XmlReader.attributes().value("ChannelName").toLocal8Bit().data(),
                    qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("ChannelName").length()));
                strncpy(pChannel->DriverName,
                    m_XmlReader.attributes().value("DriverName").toLocal8Bit().data(),
                    qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("DriverName").length()));

                m_arrTempChannels.push_back(pChannel);

                m_arrChannelOccNos.push_back(pChannelOccNo);*/

                ReadDev(ppChannel, pMemData);
            }
            else
            {
                //SkipUnknownElement();
            }
        }

        m_XmlReader.readNext();

    }
}

void CReadXml::ReadDev(CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData)
{
    Q_ASSERT(pChannel);
    Q_ASSERT(pMemData);

    m_XmlReader.readNext();

    while (!m_XmlReader.atEnd())
    {
        if (m_XmlReader.isEndElement())
        {
            m_XmlReader.readNext();
            break;
        }
        else if (m_XmlReader.isStartElement())
        {
            if (m_XmlReader.name() == "device")
            {
                ReadDevInfo(pChannel, pMemData);
            }
            else
            {
                //SkipUnknownElement();
            }
        }

        m_XmlReader.readNext();
    }
}

void CReadXml::ReadDevInfo(CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData)
{
    Q_ASSERT(pChannel);
    Q_ASSERT(pMemData);

    DEVICE_PTR pDevice = Q_NULLPTR;

    TAG_OCCNO_DEF pDevOccNo = Q_NULLPTR;

    m_XmlReader.readNext();

    while (!m_XmlReader.atEnd())
    {
        if (m_XmlReader.isEndElement())
        {
            m_XmlReader.readNext();
            break;
        }
        else if (m_XmlReader.isStartElement())
        {
            if (m_XmlReader.name() == "d")
            {
                pDevice = std::make_shared<DEVICE>();

                pDevOccNo = std::make_shared<CTagNameOccNo>();

                CDevicePtr ppDevice = m_pChannelMgr->CreateDevice(pChannel, pDevice);
                //设备属性
                pDevice->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

                pDevOccNo->OccNo = pDevice->OccNo;
                std::memset(pDevOccNo->TagName, 0, sizeof(pDevOccNo->TagName));

                strncpy(pDevOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
                    qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));

                pDevice->NameOccNo = m_XmlReader.attributes().value("ID").toInt();
                pDevice->ModelOccNo = m_XmlReader.attributes().value("DeviceModelID").toInt();
                pDevice->ChannelOccNo = m_XmlReader.attributes().value("ID").toInt();

                //m_arrTempDevices.push_back(pDevice);

                //m_arrDeviceOccNos.push_back(pDevOccNo);

                ReadData(ppDevice, pChannel, pMemData);
            }
            else
            {
                //SkipUnknownElement();
            }
        }

        m_XmlReader.readNext();

    }
}

void CReadXml::ReadData(CDevicePtr pDev, CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData)
{
    Q_ASSERT(pMemData);

    m_XmlReader.readNext();

    while (!m_XmlReader.atEnd())
    {
        if (m_XmlReader.isEndElement())
        {
            m_XmlReader.readNext();
            break;
        }
        else if (m_XmlReader.isStartElement())
        {
            if (m_XmlReader.name() == "data")
            {
                ReadDataInfo(pDev, pChannel, pMemData);
            }
            else
            {
                //SkipUnknownElement();
            }
        }

        m_XmlReader.readNext();
    }
}

void CReadXml::ReadDataInfo(CDevicePtr pDev, CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData)
{
    Q_ASSERT(pMemData);

    m_XmlReader.readNext();

    while (!m_XmlReader.atEnd())
    {
        if (m_XmlReader.isStartElement())
        {
            if (m_XmlReader.name() == "AIN")
            {
                ReadAinInfo(pDev, pChannel, pMemData);
            }
            else if (m_XmlReader.name() == "DIN")
            {
                ReadDinInfo(pDev, pChannel, pMemData);
            }
            else
            {
                //SkipUnknownElement();
            }
        }

        m_XmlReader.readNext();

    }
}

void CReadXml::ReadAinInfo(CDevicePtr pDev, CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData)
{
    Q_ASSERT(pMemData);

    AIN_PTR pAin = Q_NULLPTR;

    TAG_OCCNO_DEF pAinOccNo = Q_NULLPTR;

    m_XmlReader.readNext();

    while (!m_XmlReader.atEnd())
    {
        if (m_XmlReader.isEndElement())
        {
            m_XmlReader.readNext();
            break;
        }
        else if (m_XmlReader.isStartElement())
        {
            QString strName = m_XmlReader.name().toString();
            if (strName == "ai")
            {
                pAin = std::make_shared<AIN>();

                pAinOccNo = std::make_shared<CTagNameOccNo>();

                pAin->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

                pAinOccNo->OccNo = pAin->OccNo;
                std::memset(pAinOccNo->TagName, 0, sizeof(pAinOccNo->TagName));

                strncpy(pAinOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
                    qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));

                pAin->BlockNo = m_XmlReader.attributes().value("BlockOccNo").toInt();
                //TODO
                pAin->NameOccNo = m_XmlReader.attributes().value("").toInt();
                pAin->ChannelOccNo = m_XmlReader.attributes().value("ChannelOccNo").toInt();
                pAin->DeviceOccNo = m_XmlReader.attributes().value("DeviceOccNo").toInt();
                strncpy(pAin->Address,
                    m_XmlReader.attributes().value("Address").toString().toStdString().c_str(),
                    qMin((size_t)MAX_ADDRESS_LENGTH, (size_t)m_XmlReader.attributes().value("Address").length()));
                pAin->InitValue = m_XmlReader.attributes().value("InitValue").toFloat();
                pAin->DataSource = m_XmlReader.attributes().value("DataSource").toInt();
                pAin->TransformType = m_XmlReader.attributes().value("TransformType").toInt();
                pAin->RangeL = m_XmlReader.attributes().value("RangeL").toDouble();
                pAin->RangeH = m_XmlReader.attributes().value("RangeH").toDouble();

                pAin->HighQua = m_XmlReader.attributes().value("HighQua").toDouble();
                pAin->LowQua = m_XmlReader.attributes().value("LowQua").toDouble();

                pMemData->m_vecAiMemData.push_back(pAin);

                m_XmlReader.readNext();

                if (m_XmlReader.isEndElement())
                {
                    m_XmlReader.readNext();
                }
            }
            else
            {
                //SkipUnknownElement();
            }
        }

        m_XmlReader.readNext();
    }
}

void CReadXml::ReadDinInfo(CDevicePtr pDev, CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData)
{
    Q_ASSERT(pMemData);

    DIN_PTR pDin = Q_NULLPTR;
    TAG_OCCNO_DEF pDinOccNo = Q_NULLPTR;

    m_XmlReader.readNext();
    while (!m_XmlReader.atEnd())
    {
        if (m_XmlReader.isEndElement())
        {
            m_XmlReader.readNext();
            break;
        }
        else if (m_XmlReader.isStartElement())
        {
            QString strName = m_XmlReader.name().toString();
            if (strName == "di")
            {
                pDin = std::make_shared<DIN>();

                pDin->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

                pDinOccNo = std::make_shared<CTagNameOccNo>();

                pDinOccNo->OccNo = pDin->OccNo;
                std::memset(pDinOccNo->TagName, 0, sizeof(pDinOccNo->TagName));

                strncpy(pDinOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
                    qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));


                pDin->ChannelOccNo = m_XmlReader.attributes().value("ChannelOccNo").toInt();
                pDin->DeviceOccNo = m_XmlReader.attributes().value("DeviceOccNo").toInt();
                pDin->BlockNo = m_XmlReader.attributes().value("BlockOccNo").toInt();
                strncpy(pDin->Address,
                    m_XmlReader.attributes().value("Address").toString().toStdString().c_str(),
                    qMin((size_t)MAX_ADDRESS_LENGTH, (size_t)m_XmlReader.attributes().value("Address").length()));
                pDin->Init = m_XmlReader.attributes().value("InitValue").toFloat();
                pDin->DataSource = m_XmlReader.attributes().value("DataSource").toInt();

                pMemData->m_vecDiMemData.push_back(pDin);
                
                m_XmlReader.readNext();

                if (m_XmlReader.isEndElement())
                {
                    m_XmlReader.readNext();
                }
            }
            else
            {
                //SkipUnknownElement();
            }
        }

        m_XmlReader.readNext();
    }

}





