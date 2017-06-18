
#include "modelinfo.h"

#include <QDir>
#include <QTextStream>
#include <QUuid>

bool CEquipmentModel::SaveToFile(const QString& strPath, const QString& strFileName, const QString& strGuid)
{
    if (strPath.isEmpty() || strFileName.isEmpty() || strGuid.isEmpty())
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
    writer.writeAttribute("Guid", strGuid);

    // д����Ϣ
    WriteInfo(writer);

    writer.writeEndElement();

    writer.writeEndDocument();

    file.close();

    return true;
}

bool CEquipmentModel::WriteInfo(QXmlStreamWriter& writer)
{
    int nIndex = 0;
    for each (auto i in m_arrAttrs.m_mapContentInfo)
    {        
        writer.writeStartElement("m");

        writer.writeAttribute("ID", QString("%1").arg(nIndex));

        writer.writeAttribute("name", i.m_strName.c_str());

        writer.writeAttribute("desc", i.m_strDescribe.c_str());

        writer.writeAttribute("type", QString("%1").arg(i.m_nDataType));

        writer.writeAttribute("rw", QString("%1").arg(i.m_bReadWrite));

        writer.writeEndElement();

        nIndex++;
    }
    return true;
}

/************************************************************* class CModelLib ******************************************************************/

CModelLib::CModelLib()
{
    m_nCurrentMode = -1;
}

CModelLib::~CModelLib()
{

}

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
        writer.writeAttribute("Name", i);

        // д����Ϣ
        WriteEquipInfo(m_mapScene[i], writer);

        writer.writeEndElement();

        nIndex++;
    }
}

// д���豸��Ϣ
void CModelLib::WriteEquipInfo(const SGroupAndModel& sGroupModel, QXmlStreamWriter& writer)
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
    WriteModelToConfig(sGroupModel.mapModelName, writer);

    writer.writeEndElement();
}

// д���ͺ���Ϣ
void CModelLib::WriteModelToConfig(const QMap<QString, QString>& mapFileName, QXmlStreamWriter& writer)
{
    if (mapFileName.size() <= 0)
    {
        return;
    }

    writer.writeStartElement("model");

    // ����GUID
    QUuid gid = QUuid::createUuid();
    QString strGuid = gid.toString();

    int nIndex = 1;
    for each (auto i in mapFileName.keys())
    {
        writer.writeStartElement("m");

        writer.writeAttribute("ID", QString("%1").arg(nIndex));

        writer.writeAttribute("Name", i);

        writer.writeAttribute("Guid", mapFileName[i]);

        writer.writeEndElement();

        nIndex++;
    }

    writer.writeEndElement();
}

/************************************************************* �޸����ͽṹ�� **************************************************************/
// �޸ĳ�����
bool CModelLib::ModifyScene(const QString& strSceneName, const QString& strNewName)
{
    if (strSceneName.isEmpty() || strNewName.isEmpty())
    {
        return false;
    }

    QString strOldPath = M_SAVE_MODEL_FOLDER_NAME + "//" + strSceneName;
    QString strNewPath = M_SAVE_MODEL_FOLDER_NAME + "//" + strNewName;

    QDir dir;
    if (dir.exists(strOldPath))
    {
        // Ŀ¼���޸ĳɹ�������ڴ��޸ġ���ֹ��Ŀ¼���ڴ�״̬����renameʧ�ܡ�
        bool bRes = dir.rename(strOldPath, strNewPath);
        if (!bRes)
        {
            return false;
        }
    }

    // �޸����ͽṹ
    auto iterTreeScene = m_mapScene.find(strSceneName);
    if (iterTreeScene != m_mapScene.end())
    {
        m_mapScene[strNewName] = *iterTreeScene;

        m_mapScene.erase(iterTreeScene);
    }

    // �޸�view�ṹ
    auto iterViewScene = m_arrModels.find(strSceneName);
    if (iterViewScene != m_arrModels.end())
    {
        // �ڴ�ṹ�޸�
        m_arrModels[strNewName] = *iterViewScene;
        // ɾ���ɵĽṹ
        m_arrModels.erase(iterViewScene);
    }

    // �����µĽṹ��equipmentmodel.xml��
    SaveToFile("");

    return true;
}

// �޸ĳ������������
bool CModelLib::ModifySceneGroup(const QString& strSceneName, const QString& strGroupName, const QString& strNewName)
{
    if (strSceneName.isEmpty() || strGroupName.isEmpty() || strNewName.isEmpty())
    {
        return false;
    }

    QString strOldPath = M_SAVE_MODEL_FOLDER_NAME + "//" + strSceneName + "//" + strGroupName;
    QString strNewPath = M_SAVE_MODEL_FOLDER_NAME + "//" + strSceneName + "//" + strNewName;
    QDir dir;
    if (dir.exists(strOldPath))
    {
        bool bRes = dir.rename(strOldPath, strNewPath);
        if (!bRes)
        {
            return false;
        }
    }

    // �޸����ͽṹ
    auto iterTreeGroup = m_mapScene[strSceneName].mapEquipName.find(strGroupName);
    if (iterTreeGroup != m_mapScene[strSceneName].mapEquipName.end())
    {
        m_mapScene[strSceneName].mapEquipName[strNewName] = *iterTreeGroup;

        m_mapScene[strSceneName].mapEquipName.erase(iterTreeGroup);
    }

    // �޸�view�ṹ
    auto iterViewGroup = m_arrModels[strSceneName].m_mapEquipGroup.find(strGroupName);
    if (iterViewGroup != m_arrModels[strSceneName].m_mapEquipGroup.end())
    {
        // �ڴ�ṹ�޸�
        m_arrModels[strSceneName].m_mapEquipGroup[strNewName] = *iterViewGroup;
        // ɾ���ɵĽṹ
        m_arrModels[strSceneName].m_mapEquipGroup.erase(iterViewGroup);
    }

    // �����µĽṹ��equipmentmodel.xml��
    SaveToFile("");

    return true;
}

// �޸ĳ���������ͺ�
bool CModelLib::ModifySceneModelNo(const QString& strSceneName, const QString& strModelName, const QString& strNewName)
{
    if (strSceneName.isEmpty() || strModelName.isEmpty() || strNewName.isEmpty())
    {
        return false;
    }

    QString strOldPath = M_SAVE_MODEL_FOLDER_NAME + "//" + strSceneName + "//" + strModelName + M_FILE_TYPE;
    QString strNewPath = M_SAVE_MODEL_FOLDER_NAME + "//" + strSceneName + "//" + strNewName + M_FILE_TYPE;
    QFile file;
    if (file.exists(strOldPath))
    {
        bool bRes = file.rename(strOldPath, strNewPath);
        if (!bRes)
        {
            return false;
        }
    }

    // �޸����ͽṹ
    auto iterTreeModel = m_mapScene[strSceneName].mapModelName.find(strModelName);
    if (iterTreeModel != m_mapScene[strSceneName].mapModelName.end())
    {
        m_mapScene[strSceneName].mapModelName.insert(strNewName, m_mapScene[strSceneName].mapModelName[strModelName]);

        m_mapScene[strSceneName].mapModelName.erase(iterTreeModel);
    }

    // �޸�view�ṹ
    auto iterViewModel = m_arrModels[strSceneName].m_mapEquipModel.find(strModelName);
    if (iterViewModel != m_arrModels[strSceneName].m_mapEquipModel.end())
    {
        // �ڴ�ṹ�޸�
        m_arrModels[strSceneName].m_mapEquipModel[strNewName] = *iterViewModel;
        // ɾ���ɵĽṹ
        m_arrModels[strSceneName].m_mapEquipModel.erase(iterViewModel);
    }

    // �����µĽṹ��equipmentmodel.xml��
    SaveToFile("");

    return true;
}

bool CModelLib::ModifyGroupModelNo(const QString& strSceneName, const QString& strGroupName, const QString& strModelName, const QString& strNewName)
{
    if (strSceneName.isEmpty() || strGroupName.isEmpty() || strModelName.isEmpty() || strNewName.isEmpty())
    {
        return false;
    }

    QString strOldPath = M_SAVE_MODEL_FOLDER_NAME + "//" + strSceneName + "//" + strGroupName + "//" + strModelName + M_FILE_TYPE;
    QString strNewPath = M_SAVE_MODEL_FOLDER_NAME + "//" + strSceneName + "//" + strGroupName + "//" + strNewName + M_FILE_TYPE;
    QFile file;
    if (file.exists(strOldPath))
    {
        bool bRes = file.rename(strOldPath, strNewPath);
        if (!bRes)
        {
            return false;
        }
    }

    // �޸����ͽṹ
    auto& modelName = m_mapScene[strSceneName].mapEquipName[strGroupName];
    auto iterTreeModel = modelName.find(strModelName);
    if (iterTreeModel != modelName.end())
    {
        modelName.insert(strNewName, modelName[strModelName]);

        modelName.erase(iterTreeModel);
    }

    // �޸�view�ṹ
    auto iterViewModel = m_arrModels[strSceneName].m_mapEquipGroup[strGroupName].find(strModelName);
    if (iterViewModel != m_arrModels[strSceneName].m_mapEquipGroup[strGroupName].end())
    {
        // �ڴ�ṹ�޸�
        m_arrModels[strSceneName].m_mapEquipGroup[strGroupName][strNewName] = *iterViewModel;
        // ɾ���ɵĽṹ
        m_arrModels[strSceneName].m_mapEquipGroup[strGroupName].erase(iterViewModel);
    }

    // �����µĽṹ��equipmentmodel.xml��
    SaveToFile("");

    return true;
}

/************************************************************* ɾ�����κ�view�ڴ�ṹ **************************************************************/
// ɾ������
bool CModelLib::DelSceneItem(const QString& strSceneName)
{
    if (strSceneName.isEmpty())
    {
        return false;
    }

    // ɾ��modeltool.xml�ṹ
    auto iterTreeScene = m_mapScene.find(strSceneName);
    if (iterTreeScene != m_mapScene.end())
    {
        m_mapScene.erase(iterTreeScene);
    }

    // ɾ��view
    auto iterViewScene = m_arrModels.find(strSceneName);
    if (iterViewScene != m_arrModels.end())
    {
        m_arrModels.erase(iterViewScene);
    }

    return true;
}

// ɾ�������������
bool CModelLib::DelSceneGroupItem(const QString& strSceneName, const QString& strGroupName)
{
    if (strSceneName.isEmpty() || strGroupName.isEmpty())
    {
        return false;
    }

    // ɾ��equipmentmodel.xml�ṹ
    auto iterTreeScene = m_mapScene.find(strSceneName);
    if (iterTreeScene != m_mapScene.end())
    {
        auto iterTreeGroup = iterTreeScene->mapEquipName.find(strGroupName);
        if (iterTreeGroup != iterTreeScene->mapEquipName.end())
        {
            iterTreeScene->mapEquipName.erase(iterTreeGroup);
        }
    }

    // ɾ��view
    auto iterViewScene = m_arrModels.find(strSceneName);
    if (iterViewScene != m_arrModels.end())
    {
        auto iterViewGroup = iterViewScene->m_mapEquipGroup.find(strGroupName);
        if (iterViewGroup != iterViewScene->m_mapEquipGroup.end())
        {
            iterViewScene->m_mapEquipGroup.erase(iterViewGroup);
        }
    }
    
    return true;
}

// ɾ������������ͺ�
bool CModelLib::DelSceneModelNoItem(const QString& strSceneName, const QString& strModelNoName)
{
    if (strSceneName.isEmpty() || strModelNoName.isEmpty())
    {
        return false;
    }

    // ɾ��modeltool.xml�ṹ
    auto iterTreeScene = m_mapScene.find(strSceneName);
    if (iterTreeScene != m_mapScene.end())
    {
        auto iterTreeGroup = iterTreeScene->mapModelName.find(strModelNoName);
        if (iterTreeGroup != iterTreeScene->mapModelName.end())
        {
            iterTreeScene->mapModelName.erase(iterTreeGroup);
        }
    }

    // ɾ��view
    auto iterViewScene = m_arrModels.find(strSceneName);
    if (iterViewScene != m_arrModels.end())
    {
        auto iterViewGroup = iterViewScene->m_mapEquipModel.find(strModelNoName);
        if (iterViewGroup != iterViewScene->m_mapEquipModel.end())
        {
            iterViewScene->m_mapEquipModel.erase(iterViewGroup);
        }
    }

    return true;
}

// ɾ����������ͺ�
bool CModelLib::DelGroupModelNoItem(const QString& strSceneName, const QString& strGroupName, const QString& strModelNoName)
{
    if (strSceneName.isEmpty() || strGroupName.isEmpty() || strModelNoName.isEmpty())
    {
        return false;
    }

    // ɾ��equipmentmodel.xml�ṹ
    auto iterTreeScene = m_mapScene.find(strSceneName);
    if (iterTreeScene != m_mapScene.end())
    {
        auto iterTreeGroup = iterTreeScene->mapEquipName.find(strGroupName);
        if (iterTreeGroup != iterTreeScene->mapEquipName.end())
        {
            auto iterTreeModel = iterTreeGroup->find(strModelNoName);
            if (iterTreeModel != iterTreeGroup->end())
            {
                iterTreeGroup->erase(iterTreeModel);
            }
        }
    }

    // ɾ��view
    auto iterViewScene = m_arrModels.find(strSceneName);
    if (iterViewScene != m_arrModels.end())
    {
        auto iterViewGroup = iterViewScene->m_mapEquipGroup.find(strGroupName);
        if (iterViewGroup != iterViewScene->m_mapEquipGroup.end())
        {
            auto iterViewModel = iterViewGroup->find(strModelNoName);
            if (iterViewModel != iterViewGroup->end())
            {
                iterViewGroup->erase(iterViewModel);
            }
        }
    }

    return true;
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
QString CModelLib::CheckCreateGroupName(const QString& strSceneName, bool bInsertFlag/* = false*/)
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
QString CModelLib::CheckCreateModelNoName(const QString& strSceneName, bool bInsertFlag/* = false*/)
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
            auto iterModel = m_mapScene[strSceneName].mapModelName.find(strName);
            if (iterModel == m_mapScene[strSceneName].mapModelName.end())
            {
                if (bInsertFlag)
                {
                    // ����GUID
                    QUuid gid = QUuid::createUuid();
                    QString strGuid = gid.toString();

                    m_mapScene[strSceneName].mapModelName[strName] = strGuid;
                }
                return std::move(strName);
            }
        }

        nIndex++;
    }
}

// check�����������������ͺ�
QString CModelLib::CheckCreateModelNoName(const QString& strSceneName, const QString& strGroupName, bool bInsertFlag/* = false*/)
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
                    QUuid gid = QUuid::createUuid();
                    QString strGuid = gid.toString();

                    m_mapScene[strSceneName].mapEquipName[strGroupName].insert(strName, strGuid);
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
            m_sGroupModel.mapModelName.clear();

            if (!ReadEquipment(domElm))
            {
                // log
                continue;
            }

            if (m_sGroupModel.mapEquipName.size() > 0 || m_sGroupModel.mapModelName.size() > 0)
            {
                m_mapScene[strSceneName] = m_sGroupModel;
            }
            else
            {
                m_mapScene[strSceneName];
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

            if (!ReadModel(domElm, m_sGroupModel.mapEquipName[strName]))
            {
                // log
                continue;
            }

            if (m_sGroupModel.mapEquipName.size() < 0)
            { // ������û���ͺű�������
                m_sGroupModel.mapEquipName[strName];
            }
        }
        else if (node.nodeName() == "model")
        {
            QDomElement domElm = node.toElement();

            if (!ReadContentInfo(domElm, m_sGroupModel.mapModelName))
            {
                // log
                continue;
            }
        }
    }

    return true;
}

bool CModelLib::ReadModel(QDomElement& dElement, QMap<QString, QString>& mapFileName)
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

            if (!ReadContentInfo(pAIElement, mapFileName))
            {
                // log
                continue;
            }
        }
    }

    return true;
}

bool CModelLib::ReadContentInfo(QDomElement& dElement, QMap<QString, QString>& mapFileName)
{
    QDomNode dnChild;
    for (dnChild = dElement.firstChild(); !dnChild.isNull(); dnChild = dnChild.nextSiblingElement())
    {
        QDomElement dData = dnChild.toElement();

        int id = dData.attribute("ID").toInt();
        QString strName = dData.attribute("Name");
        QString strGuid = dData.attribute("Guid");

        mapFileName.insert(strName, strGuid);
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

            for each (auto m in m_mapScene[i].mapEquipName[j].keys())
            {
                m_nCurrentMode = TYPE_EQUIPMENT;
                m_strModelName = m;

                strPathName = M_SAVE_MODEL_FOLDER_NAME + "//" + i + "//" + j + "//" + m + M_FILE_TYPE;

                LoadModelInfo(strPathName);
            }

            for each (auto n in m_mapScene[i].mapModelName.keys())
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


bool CModelLib::LoadModelInfo(const QString& strPathName)
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
    QString strGuid = root.attribute("Guid");


    for (; !node.isNull(); node = node.nextSibling())
    {
        if (node.nodeName() == "m")
        {
            QDomElement domElm = node.toElement();
            if (domElm.isNull())
            {
                continue;
            }

            int nID = domElm.attribute("ID").toInt();
            QString strName = domElm.attribute("name");
            QString strDesc = domElm.attribute("desc");
            int nType = domElm.attribute("type").toInt();
            bool bReadWrite = QVariant(domElm.attribute("rw")).toBool();

            if (TYPE_SCENE == m_nCurrentMode)
            {
                CContentInfo& contentInfo = m_arrModels[m_strSceneName].m_mapEquipModel[m_strModelName].m_arrAttrs.m_mapContentInfo[nID];
                contentInfo.m_strName = strName.toStdString();
                contentInfo.m_strDescribe = strDesc.toStdString();
                contentInfo.m_nDataType = nType;
                contentInfo.m_bReadWrite = bReadWrite;
            }
            else if (TYPE_EQUIPMENT == m_nCurrentMode)
            {
                CContentInfo& contentInfo = m_arrModels[m_strSceneName].m_mapEquipGroup[m_strGroupName][m_strModelName].m_arrAttrs.m_mapContentInfo[nID];
                contentInfo.m_strName = strName.toStdString();
                contentInfo.m_strDescribe = strDesc.toStdString();
                contentInfo.m_nDataType = nType;
                contentInfo.m_bReadWrite = bReadWrite;
            }
        }
    }

    return true;
}


/************************************************************* ��ȡģ�ͽṹ **************************************************************/
// ��ȡ����������ͺ�
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

// ��ȡ��������ͺ�
CEquipmentModel* CModelLib::GetEquipModel(const QString& strSceneName, const QString& strGroupName, const QString& strModelName)
{
    if (strSceneName.isEmpty() || strGroupName.isEmpty() || strModelName.isEmpty())
    {
        return nullptr;
    }

    QMap<QString, CEquipmentGroupAndModel>::iterator iterScene = m_arrModels.find(strSceneName);
    if (iterScene != m_arrModels.end())
    {
        QMap<QString, QMap<QString, CEquipmentModel> >::const_iterator iterGroup = iterScene->m_mapEquipGroup.find(strGroupName);
        if (iterGroup != iterScene->m_mapEquipGroup.end())
        {
            return &(iterScene->m_mapEquipGroup[strGroupName][strModelName]);
        }
    }

    return nullptr;
}