
#include "equipmentmodel.h"

#include <QDir>
#include <QTextStream>

bool CEquipmentModel::SaveToFile(const QString& strPath, const QString& strFileName)
{
    if (strPath.isEmpty() || strFileName.isEmpty())
    {
        return false;
    }

    QDir dir;
    // ģ��Ŀ¼����
    if (!dir.exists(M_SAVE_MODEL_FOLDER_NAME))
    {
        bool bRes = dir.mkdir(M_SAVE_MODEL_FOLDER_NAME);
        if (!bRes)
        {
            return false;
        }
    }

    if (!dir.exists(strPath))
    {
        bool bRes = dir.mkpath(strPath);
        if (!bRes)
        {
            return false;
        }
    }

    QString strPathName = strPath + "//" + strFileName + M_FILE_TYPE;
    QFile file(strPathName);

    bool bRes = file.open(QIODevice::WriteOnly);
    if (!bRes)
    {
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();

    writer.writeStartElement("model");

    // д����Ϣ
    WriteInfo(writer);

    writer.writeEndElement();

    writer.writeEndDocument();

    file.close();

    return true;
}

bool CEquipmentModel::WriteInfo(QXmlStreamWriter& writer)
{
    // д��ai
    writer.writeStartElement("ai");

    WriteAnalogInfo(writer);

    writer.writeEndElement();

    // д��di
    writer.writeStartElement("di");

    WriteBinaryInfo(writer);

    writer.writeEndElement();

    return true;
}

bool CEquipmentModel::WriteAnalogInfo(QXmlStreamWriter& writer)
{
    int nIndex = 0;
    for each (auto i in m_arrAttrs[DATATYPE_ANALOG])
    {        
        writer.writeStartElement("a");

        writer.writeAttribute("ID", QString("%1").arg(nIndex));

        writer.writeAttribute("desc", i.s_strDescribe.c_str());

        writer.writeAttribute("type", QString("%1").arg(i.s_nDataType));

        writer.writeAttribute("rw", QString("%1").arg(i.s_bReadWrite));

        writer.writeEndElement();

        nIndex++;
    }
    return true;
}

bool CEquipmentModel::WriteBinaryInfo(QXmlStreamWriter& writer)
{
    int nIndex = 0;
    for each (auto i in m_arrAttrs[DATATYPE_BINARY])
    {
        writer.writeStartElement("d");

        writer.writeAttribute("ID", QString("%1").arg(nIndex));

        writer.writeAttribute("desc", i.s_strDescribe.c_str());

        writer.writeAttribute("type", QString("%1").arg(i.s_nDataType));

        writer.writeAttribute("rw", QString("%1").arg(i.s_bReadWrite));

        writer.writeEndElement();

        nIndex++;
    }
    return true;
}

/*****************************************************************************************************************************************/
bool CModelLib::SaveToFile(const QString& szPath)
{
    QFile file(M_MODEL_CONFIG_NAME_XML);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();

    writer.writeStartElement("scene");


    // д����Ϣ
    WriteSceneToConfig(writer);

    writer.writeEndElement();

    writer.writeEndDocument();

    file.close();
    return true;
}

// д�볡����Ϣ
void CModelLib::WriteSceneToConfig(QXmlStreamWriter& writer)
{
    int nIndex = 1;
    for each (auto i in m_mapScene.keys())
    {
        writer.writeStartElement("s");

        writer.writeAttribute("ID", QString("%1").arg(nIndex));
        // д��ChannelNO
        writer.writeAttribute("Name", i);

        // д����Ϣ
        WriteEquipInfo(m_mapScene[i], writer);

        writer.writeEndElement();

        nIndex++;
    }
}

// д���豸��Ϣ
void CModelLib::WriteEquipInfo(SGroupAndModel& sGroupModel, QXmlStreamWriter& writer)
{
    int nIndex = 1;
    writer.writeStartElement("equip");

    // д������Ϣ
    for each (auto i in sGroupModel.mapEquipName.keys())
    {
        writer.writeStartElement("e");

        // д���豸��Ϣ
        // д��ID
        writer.writeAttribute("ID", QString("%1").arg(nIndex));
        // д������
        writer.writeAttribute("Name", i);

        // д���ͺ�
        WriteModelToConfig(sGroupModel.mapEquipName[i], writer);

        nIndex++;

        writer.writeEndElement();
    }

    // д���ͺ���Ϣ
    WriteModelToConfig(sGroupModel.setModelName, writer);

    writer.writeEndElement();
}

// д���ͺ���Ϣ
void CModelLib::WriteModelToConfig(QSet<QString>& setFileName, QXmlStreamWriter& writer)
{
    if (setFileName.size() <= 0)
    {
        return;
    }

    writer.writeStartElement("model");

    int nIndex = 1;
    for each (auto i in setFileName)
    {
        writer.writeStartElement("m");

        writer.writeAttribute("ID", QString("%1").arg(nIndex));

        writer.writeAttribute("Name", i);

        writer.writeEndElement();

        nIndex++;
    }

    writer.writeEndElement();
}

// ɾ��Ŀ¼,����1��ʾĿ¼�������ļ�
int CModelLib::DelFolderItem(QString& strFolderName)
{
    // ɾ��Ŀ¼
    /*QSet<QString>::iterator iter = m_setModelName.find(strFolderName);
    if (iter != m_setModelName.end())
    {
        m_setModelName.erase(iter);
    }

    // ɾ��Ŀ¼���ļ��Ķ�Ӧ��ϵ
    QMap<QString, QSet<QString> >::iterator iterTmp = m_mapEquipName.find(strFolderName);

    if (iterTmp != m_mapEquipName.end())
    {
        // ��ȡɾ��Ŀ¼����������ļ���
        QSet<QString>& setFileName = m_mapEquipName[strFolderName];

        // ɾ����ʾ����
        foreach(auto i, setFileName)
        {
            QMap<QString, CEquipmentModel>::iterator iterFile = m_arrModels.find(i);
            if (iterFile != m_arrModels.end())
            {
                m_arrModels.erase(iterFile);
            }
        }

        // ��ʾ����ɾ������ɾ��Ŀ¼���ļ��Ķ�Ӧ��ϵ
        m_mapEquipName.erase(iterTmp);

        return 1;
    }*/

    return 0;
}

// ɾ���ļ�item
void CModelLib::DelFileItem(QString& strFolderName, QString& strFileName)
{
    /*if (m_mapEquipName[strFolderName].size() > 0)
    {
        // ɾ����Ŀ¼��Ӧ���ļ�
        QSet<QString>::const_iterator iter = m_mapEquipName[strFolderName].find(strFileName);
        if (iter != m_mapEquipName[strFolderName].end())
        {
            m_mapEquipName[strFolderName].erase(iter);
        }
    }

    // ɾ����ʾ����
    QMap<QString, CEquipmentModel>::iterator iterFile = m_arrModels.find(strFileName);
    if (iterFile != m_arrModels.end())
    {
        m_arrModels.erase(iterFile);
    }*/
}

// check����
QString CModelLib::CheckCreateSceneName(bool bInsertFlag/* = false*/)
{
    QString strName = M_SCENE_NAME;

    int nIndex = 0;

    while (1)
    {
        if (0 == nIndex)
        {
            strName = M_SCENE_NAME;
        }
        else
        {
            strName = QString("%1%2").arg(M_SCENE_NAME).arg(nIndex);
        }

        auto iterScene = m_mapScene.find(strName);
        if (iterScene == m_mapScene.end())
        {
            if (bInsertFlag)
            {
                m_mapScene[strName];
            }
            return std::move(strName);
        }

        nIndex++;
    }
}

// check��
QString CModelLib::CheckCreateGroupName(QString& strSceneName, bool bInsertFlag/* = false*/)
{
    if (strSceneName.isEmpty())
    {
        return "";
    }

    QString strName = M_GROUP_NAME;

    int nIndex = 0;

    while (1)
    {
        if (0 == nIndex)
        {
            strName = M_GROUP_NAME;
        }
        else
        {
            strName = QString("%1%2").arg(M_GROUP_NAME).arg(nIndex);
        }

        auto iter = m_mapScene.find(strSceneName);
        if (iter == m_mapScene.end())
        {
            return "";
        }
        else
        {
            auto iterGroup = m_mapScene[strSceneName].mapEquipName.find(strName);
            if (iterGroup == m_mapScene[strSceneName].mapEquipName.end())
            {
                if (bInsertFlag)
                {
                    m_mapScene[strSceneName].mapEquipName[strName];
                }
                return std::move(strName);
            }
        }

        nIndex++;
    }
}

// check����������ͺ�
QString CModelLib::CheckCreateModelNoName(QString& strSceneName, bool bInsertFlag/* = false*/)
{
    if (strSceneName.isEmpty())
    {
        return "";
    }

    QString strName = M_MODEL_NO_NAME;

    int nIndex = 0;

    while (1)
    {
        if (0 == nIndex)
        {
            strName = M_MODEL_NO_NAME;
        }
        else
        {
            strName = QString("%1%2").arg(M_MODEL_NO_NAME).arg(nIndex);
        }

        auto iterScene = m_mapScene.find(strSceneName);
        if (iterScene == m_mapScene.end())
        {
            return "";
        }
        else
        {
            auto iterModel = m_mapScene[strSceneName].setModelName.find(strName);
            if (iterModel == m_mapScene[strSceneName].setModelName.end())
            {
                if (bInsertFlag)
                {
                    m_mapScene[strSceneName].setModelName.insert(strName);
                }
                return std::move(strName);
            }
        }

        nIndex++;
    }
}

// check�����������������ͺ�
QString CModelLib::CheckCreateModelNoName(QString& strSceneName, QString& strGroupName, bool bInsertFlag/* = false*/)
{
    if (strSceneName.isEmpty() || strGroupName.isEmpty())
    {
        return "";
    }

    QString strName = M_MODEL_NO_NAME;

    int nIndex = 0;

    while (1)
    {
        if (0 == nIndex)
        {
            strName = M_MODEL_NO_NAME;
        }
        else
        {
            strName = QString("%1%2").arg(M_MODEL_NO_NAME).arg(nIndex);
        }

        auto iterScene = m_mapScene.find(strSceneName);
        if (iterScene == m_mapScene.end())
        {
            return "";
        }
        else
        {
            auto iterGroup = m_mapScene[strSceneName].mapEquipName.find(strGroupName);
            if (iterGroup == m_mapScene[strSceneName].mapEquipName.end())
            {
                return "";
            }

            auto iterModel = m_mapScene[strSceneName].mapEquipName[strGroupName].find(strName);
            if (iterModel == m_mapScene[strSceneName].mapEquipName[strGroupName].end())
            {
                if (bInsertFlag)
                {
                    m_mapScene[strSceneName].mapEquipName[strGroupName].insert(strName);
                }
                return std::move(strName);
            }
        }

        nIndex++;
    }
}

/******************************************************* Load tree data from file **************************************************************/
bool CModelLib::LoadTreeDataFromFile(const QString& szPath)
{
    QFile file(M_MODEL_CONFIG_NAME_XML);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QDomDocument  document;

    // �����ļ�����ʱ�Ὣ������Ϊ��ʼ״̬
    if (!document.setContent(&file))
    {
        file.close();
        return false;
    }

    QDomElement root = document.documentElement();
    if (root.tagName() != "scene")
    {
        file.close();
        return false;
    }

    ReadScene(root);    

    file.close();
    return true;
}


bool CModelLib::ReadScene(QDomElement& root)
{
    QDomNode node = root.firstChild();
    for (; !node.isNull(); node = node.nextSibling())
    {
        if (node.nodeName() == "s")
        {
            QDomElement domElm = node.toElement();
            if (domElm.isNull())
            {
                continue;
            }

            // ����ID
            int nSceneID = domElm.attribute("ID").toInt();
            QString strSceneName = domElm.attribute("Name");

            m_sGroupModel.mapEquipName.clear();
            m_sGroupModel.setModelName.clear();

            if (!ReadEquipment(domElm))
            {
                // log
                continue;
            }

            if (m_sGroupModel.mapEquipName.size() > 0 || m_sGroupModel.setModelName.size() > 0)
            {
                m_mapScene[strSceneName] = m_sGroupModel;
            }
        }
    }

    return true;
}

bool CModelLib::ReadEquipment(QDomElement& dElement)
{
    QDomNode dnChild;
    for (dnChild = dElement.firstChild(); !dnChild.isNull(); dnChild = dnChild.nextSiblingElement())
    {
        QDomElement dData = dnChild.toElement();

        if (dData.nodeName() == "equip")
        {
            //QSysMesInfo *pSysInfo=new QSysMesInfo;
            QDomElement pAIElement = dData.toElement();
            if (pAIElement.isNull())
            {
                continue;
            }

            if (!ReadEquipmentInfo(pAIElement))
            {
                // log
                continue;
            }
        }
    }

    return true;
}

bool CModelLib::ReadEquipmentInfo(QDomElement& dElement)
{
    QDomNode node = dElement.firstChild();
    for (; !node.isNull(); node = node.nextSibling())
    {
        if (node.nodeName() == "e")
        {
            QDomElement domElm = node.toElement();
            if (domElm.isNull())
            {
                continue;
            }

            int equipID = domElm.attribute("ID").toInt();
            QString strName = domElm.attribute("Name");

            QSet<QString> setFileName;
            if (!ReadModel(domElm, setFileName))
            {
                // log
                continue;
            }

            if (setFileName.size() > 0)
            {
                m_sGroupModel.mapEquipName[strName] = setFileName;
            }
            else
            { // ������û���ͺű�������
                m_sGroupModel.mapEquipName[strName];
            }
        }
        else if (node.nodeName() == "model")
        {
            QDomElement domElm = node.toElement();

            QSet<QString> setFileName;
            if (!ReadContentInfo(domElm, setFileName))
            {
                // log
                continue;
            }

            if (setFileName.size() > 0)
            {
                m_sGroupModel.setModelName = setFileName;
            }
        }
    }

    return true;
}

bool CModelLib::ReadModel(QDomElement& dElement, QSet<QString>& setFileName)
{
    QDomNode dnChild;
    for (dnChild = dElement.firstChild(); !dnChild.isNull(); dnChild = dnChild.nextSiblingElement())
    {
        QDomElement dData = dnChild.toElement();

        if (dData.nodeName() == "model")
        {
            //QSysMesInfo *pSysInfo=new QSysMesInfo;
            QDomElement pAIElement = dData.toElement();
            if (pAIElement.isNull())
            {
                continue;
            }

            if (!ReadContentInfo(pAIElement, setFileName))
            {
                // log
                continue;
            }
        }
    }

    return true;
}

bool CModelLib::ReadContentInfo(QDomElement& dElement, QSet<QString>& setFileName)
{
    QDomNode dnChild;
    for (dnChild = dElement.firstChild(); !dnChild.isNull(); dnChild = dnChild.nextSiblingElement())
    {
        QDomElement dData = dnChild.toElement();

        int id = dData.attribute("ID").toInt();
        QString strName = dData.attribute("Name");

        setFileName.insert(strName);
    }

    return true;
}

/******************************************************* Load view data from file **************************************************************/
bool CModelLib::LoadViewDataFromFile(const QString& szPath)
{
    // ��ǰĿ¼��Ŀ¼�ļ���
    QString strPathName = "";

    for each (auto i in m_mapScene.keys())
    {
        m_strSceneName = i;
        for each (auto j in m_mapScene[i].mapEquipName.keys())
        {
            m_strGroupName = j;

            for each (auto m in m_mapScene[i].mapEquipName[j])
            {
                m_nCurrentMode = TYPE_EQUIPMENT;
                m_strModelName = m;

                strPathName = M_SAVE_MODEL_FOLDER_NAME + "//" + i + "//" + j + "//" + m + M_FILE_TYPE;

                LoadModelInfo(strPathName);
            }

            for each (auto n in m_mapScene[i].setModelName)
            {
                m_nCurrentMode = TYPE_SCENE;
                m_strModelName = n;

                strPathName = M_SAVE_MODEL_FOLDER_NAME + "//" + i + "//" + n + M_FILE_TYPE;

                LoadModelInfo(strPathName);
            }
        }
    }
    return true;
}


bool CModelLib::LoadModelInfo(QString& strPathName)
{
    QFile file(strPathName);
    if (!file.exists())
    {
        return false;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QDomDocument  document;

    // �����ļ�����ʱ�Ὣ������Ϊ��ʼ״̬
    if (!document.setContent(&file))
    {
        file.close();
        return false;
    }

    QDomElement root = document.documentElement();
    if (root.tagName() != "model")
    {
        file.close();
        return false;
    }

    LoadViewInfo(root);

    file.close();

    return true;
}

// ����view��Ϣ
bool CModelLib::LoadViewInfo(QDomElement& root)
{
    QDomNode node = root.firstChild();
    for (; !node.isNull(); node = node.nextSibling())
    {
        if (node.nodeName() == "ai")
        {
            QDomElement domElm = node.toElement();
            if (domElm.isNull())
            {
                continue;
            }

            if (!LoadAnalogViewData(domElm))
            {
                // log
                continue;
            }
        }
        else if (node.nodeName() == "di")
        {
            QDomElement domElm = node.toElement();
            if (domElm.isNull())
            {
                continue;
            }

            if (!LoadBinaryViewData(domElm))
            {
                // log
                continue;
            }
        }
    }

    return true;
}

bool CModelLib::LoadAnalogViewData(QDomElement& dElement)
{
    QDomNode node = dElement.firstChild();
    for (; !node.isNull(); node = node.nextSibling())
    {
        if (node.nodeName() == "a")
        {
            QDomElement domElm = node.toElement();
            if (domElm.isNull())
            {
                continue;
            }

            int nID = domElm.attribute("ID").toInt();
            QString strName = domElm.attribute("desc");
            int nType = domElm.attribute("type").toInt();
            bool bReadWrite = QVariant(domElm.attribute("rw")).toBool();

            if (TYPE_SCENE == m_nCurrentMode)
            {
                CContentInfo& contentInfo = m_arrModels[m_strSceneName].m_mapEquipModel[m_strModelName].m_arrAttrs[DATATYPE_ANALOG][nID];
                contentInfo.s_strDescribe = strName.toStdString();
                contentInfo.s_nDataType = nType;
                contentInfo.s_bReadWrite = bReadWrite;
            }
            else if (TYPE_EQUIPMENT == m_nCurrentMode)
            {
                CContentInfo& contentInfo = m_arrModels[m_strSceneName].m_mapEquipGroup[m_strGroupName][m_strModelName].m_arrAttrs[DATATYPE_ANALOG][nID];
                contentInfo.s_strDescribe = strName.toStdString();
                contentInfo.s_nDataType = nType;
                contentInfo.s_bReadWrite = bReadWrite;
            }
        }
    }

    return true;
}

// ���ؿ�������view��Ϣ
bool CModelLib::LoadBinaryViewData(QDomElement& dElement)
{
    QDomNode node = dElement.firstChild();
    for (; !node.isNull(); node = node.nextSibling())
    {
        if (node.nodeName() == "d")
        {
            QDomElement domElm = node.toElement();
            if (domElm.isNull())
            {
                continue;
            }

            int nID = domElm.attribute("ID").toInt();
            QString strName = domElm.attribute("desc");
            int nType = domElm.attribute("type").toInt();
            bool bReadWrite = QVariant(domElm.attribute("rw")).toBool();

            if (TYPE_SCENE == m_nCurrentMode)
            {
                CContentInfo& contentInfo = m_arrModels[m_strSceneName].m_mapEquipModel[m_strModelName].m_arrAttrs[DATATYPE_BINARY][nID];
                contentInfo.s_strDescribe = strName.toStdString();
                contentInfo.s_nDataType = nType;
                contentInfo.s_bReadWrite = bReadWrite;
            }
            else if (TYPE_EQUIPMENT == m_nCurrentMode)
            {
                CContentInfo& contentInfo = m_arrModels[m_strSceneName].m_mapEquipGroup[m_strGroupName][m_strModelName].m_arrAttrs[DATATYPE_BINARY][nID];
                contentInfo.s_strDescribe = strName.toStdString();
                contentInfo.s_nDataType = nType;
                contentInfo.s_bReadWrite = bReadWrite;
            }
        }
    }

    return true;
}

CEquipmentModel* CModelLib::GetEquipModel(const QString& strSceneName, const QString& strModelName)
{
    if (strSceneName.isEmpty() || strModelName.isEmpty())
    {
        return nullptr;
    }

    QMap<QString, CEquipmentGroupAndModel>::iterator iterScene = m_arrModels.find(strSceneName);
    if (iterScene != m_arrModels.end())
    {
        return &(m_arrModels[strSceneName].m_mapEquipModel[strModelName]);
    }

    return nullptr;
}