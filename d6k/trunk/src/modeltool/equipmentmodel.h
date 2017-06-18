
#ifndef _EQUIPMENT_MODEL_H_
#define _EQUIPMENT_MODEL_H_

#include "modeldefine.h"

#include <string>

#include <QMap>
#include <QSet>
#include <QDir>
#include <QFile>
#include <QXmlStreamWriter>
#include <QtXml/QDomNode>


class QXmlStreamWriter;

class CContentInfo
{
public:
    CContentInfo() : s_nDataType(-1), s_bReadWrite(false), s_strDescribe("")
    {
    }

    int	s_nDataType;
    bool s_bReadWrite; // ture:ֻ��
    std::string s_strDescribe;
};

//  �豸ģ��
class  CEquipmentModel
{
public:
    CEquipmentModel()
    {

    }

    ~CEquipmentModel()
    {

    }

public:
    bool SaveToFile(const QString& strPath, const QString& strFileName);

    bool WriteInfo(QXmlStreamWriter& writer);

    bool WriteAnalogInfo(QXmlStreamWriter& writer);

    bool WriteBinaryInfo(QXmlStreamWriter& writer);

    void AddAttr(int nDataType, int nRow, CContentInfo& pAttr)
    {
        // ��������������
        m_arrAttrs[nDataType].insert(nRow, pAttr);
    }

    QMap<int, CContentInfo>* GetAttr(int nDataType)
    {
        QMap<int, QMap<int, CContentInfo> >::const_iterator iter = m_arrAttrs.find(nDataType);
        if (iter != m_arrAttrs.end())
        {
            return &m_arrAttrs[nDataType];
        }

        return nullptr;
    }

    int GetAttrSize(int nDataType)
    {
        return m_arrAttrs[nDataType].size();
    }

public:
    typedef QMap<int, QMap<int, CContentInfo> > M_COTENT_ATTRIBUTE;
    // ��һ��map key����������  �ڶ���map key:�к�
    M_COTENT_ATTRIBUTE m_arrAttrs;
};

class CEquipmentGroupAndModel
{
public:
    CEquipmentGroupAndModel()
    {

    }

    ~CEquipmentGroupAndModel()
    {

    }

public:
    // key:�ͺ�
    QMap<QString, CEquipmentModel> m_mapEquipModel;

    // key:�豸��
    QMap<QString, QMap<QString, CEquipmentModel> > m_mapEquipGroup;
};

// �豸ģ�Ϳ�
class CModelLib
{
    struct SGroupAndModel
    {
        QMap<QString, QSet<QString> > mapEquipName; // ��

        QSet<QString> setModelName; // �ͺ�
    };

public:

    CModelLib()
    {
        m_nCurrentMode = -1;
        m_nSelectDataType = -1;
    }

    ~CModelLib()
    {

    }

    CEquipmentModel* GetEquipModel(const QString& strSceneName, const QString& strModelName);

    CEquipmentModel* GetEquipModel(const QString& strSceneName, const QString& strGroupName, const QString& strModelName)
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

    bool FindModel(QString& strFileName)
    {
        /*QMap<QString, CEquipmentModel>::const_iterator iter = m_arrModels.find(strFileName);
        if (iter != m_arrModels.end())
        {
            return true;
        }*/

        return false;
    }

    void AddModel(QString& strFileName, CEquipmentModel& pModel)
    {
        Q_ASSERT(!strFileName.isEmpty());

        //m_arrModels.insert(strFileName, pModel);
    }

    void DelModel(CEquipmentModel* pModel)
    {
        
    }

    bool AddFileItem(QString& strFolderName, QString& strFileName)
    {
        if (strFolderName.isEmpty() || strFileName.isEmpty())
        {
            return false;
        }

        m_mapEquipName[strFolderName].insert(strFileName);

        return true;
    }
    
    // ɾ��Ŀ¼,����1��ʾĿ¼�������ļ�
    int DelFolderItem(QString& strFolderName);

    void DelFileItem(QString& strFolderName, QString& strFileName);

// ������Ϣ
public:
    // ��modeltool.xml������Ϣ
    bool LoadTreeDataFromFile(const QString& szPath);

    // ��ȡ����
    bool ReadScene(QDomElement& root);

    // ��ȡ�豸ͷ
    bool ReadEquipment(QDomElement& dElement);

    // ��ȡ�豸����
    bool ReadEquipmentInfo(QDomElement& dElement);

    // ��ȡ�ͺ�
    bool ReadModel(QDomElement& dElement, QSet<QString>& setFileName);

    // ��ȡ�����ͺ�
    bool ReadSingleModel(QDomElement& dElement);

    // ��ȡ�ͺ�����
    bool ReadContentInfo(QDomElement& dElement, QSet<QString>& setFileName);

    // ���ļ��м�����Ϣ��view
    bool LoadViewDataFromFile(const QString& szPath);

    // ����view��Ϣ
    bool LoadModelInfo(QString& strPathName);

    // ����view��Ϣ
    bool LoadViewInfo(QDomElement& root);

    // ����ģ����view��Ϣ
    bool LoadAnalogViewData(QDomElement& dElement);

    // ���ؿ�������view��Ϣ
    bool LoadBinaryViewData(QDomElement& dElement);

    // ��ǰģʽ (���� or ��)
    int m_nCurrentMode;

    QString m_strSceneName;
    QString m_strGroupName;
    QString m_strModelName;

public:

    // �������νṹ���ļ�
    bool SaveToFile(const QString& szPath);

    // д�볡����Ϣ
    void WriteSceneToConfig(QXmlStreamWriter& writer);

    // д���豸����Ϣ
    void WriteEquipInfo(SGroupAndModel& sGroupModel, QXmlStreamWriter& writer);

    // д���ͺ���Ϣ
    void WriteModelToConfig(QSet<QString>& setFileName, QXmlStreamWriter& writer);

public: // �޸����ͽṹ
    bool ModifyScene(QString& strSceneName, QString& strNewName)
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

        // �����µĽṹ��modeltool.xml��
        SaveToFile("");

        return true;
    }

    bool ModifySceneGroup(QString& strSceneName, QString& strGroupName, QString& strNewName)
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

        // �����µĽṹ��modeltool.xml��
        SaveToFile("");

        return true;
    }

    bool ModifySceneModelNo(QString& strSceneName, QString& strModelName, QString& strNewName)
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
        auto iterTreeModel = m_mapScene[strSceneName].setModelName.find(strModelName);
        if (iterTreeModel != m_mapScene[strSceneName].setModelName.end())
        {
            m_mapScene[strSceneName].setModelName.insert(strNewName);

            m_mapScene[strSceneName].setModelName.erase(iterTreeModel);
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

        // �����µĽṹ��modeltool.xml��
        SaveToFile("");

        return true;
    }

    bool ModifyGroupModelNo(QString& strSceneName, QString& strGroupName, QString& strModelName, QString& strNewName)
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
        auto iterTreeModel = m_mapScene[strSceneName].mapEquipName[strGroupName].find(strModelName);
        if (iterTreeModel != m_mapScene[strSceneName].mapEquipName[strGroupName].end())
        {
            m_mapScene[strSceneName].mapEquipName[strGroupName].insert(strNewName);

            m_mapScene[strSceneName].mapEquipName[strGroupName].erase(iterTreeModel);
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

        // �����µĽṹ��modeltool.xml��
        SaveToFile("");

        return true;
    }

public:

    // check����
    QString CheckCreateSceneName(bool bInsertFlag = false);

    // check��
    QString CheckCreateGroupName(QString& strSceneName, bool bInsertFlag = false);

    // check����������ͺ�
    QString CheckCreateModelNoName(QString& strSceneName, bool bInsertFlag = false);
    // check�����������������ͺ�
    QString CheckCreateModelNoName(QString& strSceneName, QString& strGroupName, bool bInsertFlag = false);

public: // modeltool.xml�ṹ

    // Ŀ¼���ļ��Ķ�Ӧ��ϵ(�豸���ͺŶ�Ӧ��ϵ)
    QMap<QString, QSet<QString> > m_mapEquipName;

    SGroupAndModel m_sGroupModel;

    // key:������  value:Ŀ¼���ļ��Ķ�Ӧ��ϵ(�豸���ͺŶ�Ӧ��ϵ)
    QMap<QString, SGroupAndModel> m_mapScene;

public: // ���ͽṹ��ģ�ͽṹ�Ķ�Ӧ��ϵ

    // key:����
    QMap<QString, CEquipmentGroupAndModel> m_arrModels;

private:
    // ��������
    int m_nSelectDataType;


private:
};




#endif
