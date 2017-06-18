
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
    bool s_bReadWrite; // ture:只读
    std::string s_strDescribe;
};

//  设备模型
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
        // 保存新增的数据
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
    // 第一个map key：数据类型  第二个map key:行号
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
    // key:型号
    QMap<QString, CEquipmentModel> m_mapEquipModel;

    // key:设备组
    QMap<QString, QMap<QString, CEquipmentModel> > m_mapEquipGroup;
};

// 设备模型库
class CModelLib
{
    struct SGroupAndModel
    {
        QMap<QString, QSet<QString> > mapEquipName; // 组

        QSet<QString> setModelName; // 型号
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
    
    // 删除目录,返回1表示目录下面有文件
    int DelFolderItem(QString& strFolderName);

    void DelFileItem(QString& strFolderName, QString& strFileName);

// 加载信息
public:
    // 从modeltool.xml加载信息
    bool LoadTreeDataFromFile(const QString& szPath);

    // 读取场景
    bool ReadScene(QDomElement& root);

    // 读取设备头
    bool ReadEquipment(QDomElement& dElement);

    // 读取设备内容
    bool ReadEquipmentInfo(QDomElement& dElement);

    // 读取型号
    bool ReadModel(QDomElement& dElement, QSet<QString>& setFileName);

    // 读取单个型号
    bool ReadSingleModel(QDomElement& dElement);

    // 读取型号内容
    bool ReadContentInfo(QDomElement& dElement, QSet<QString>& setFileName);

    // 从文件中加载信息到view
    bool LoadViewDataFromFile(const QString& szPath);

    // 加载view信息
    bool LoadModelInfo(QString& strPathName);

    // 加载view信息
    bool LoadViewInfo(QDomElement& root);

    // 加载模拟量view信息
    bool LoadAnalogViewData(QDomElement& dElement);

    // 加载开关量量view信息
    bool LoadBinaryViewData(QDomElement& dElement);

    // 当前模式 (场景 or 组)
    int m_nCurrentMode;

    QString m_strSceneName;
    QString m_strGroupName;
    QString m_strModelName;

public:

    // 保存树形结构到文件
    bool SaveToFile(const QString& szPath);

    // 写入场景信息
    void WriteSceneToConfig(QXmlStreamWriter& writer);

    // 写入设备组信息
    void WriteEquipInfo(SGroupAndModel& sGroupModel, QXmlStreamWriter& writer);

    // 写入型号信息
    void WriteModelToConfig(QSet<QString>& setFileName, QXmlStreamWriter& writer);

public: // 修改树型结构
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
            // 目录名修改成功后才做内存修改。防止子目录处于打开状态引起rename失败。
            bool bRes = dir.rename(strOldPath, strNewPath);
            if (!bRes)
            {
                return false;
            }
        }

        // 修改树型结构
        auto iterTreeScene = m_mapScene.find(strSceneName);
        if (iterTreeScene != m_mapScene.end())
        {
            m_mapScene[strNewName] = *iterTreeScene;

            m_mapScene.erase(iterTreeScene);
        }

        // 修改view结构
        auto iterViewScene = m_arrModels.find(strSceneName);
        if (iterViewScene != m_arrModels.end())
        {
            // 内存结构修改
            m_arrModels[strNewName] = *iterViewScene;
            // 删除旧的结构
            m_arrModels.erase(iterViewScene);
        }

        // 保存新的结构到modeltool.xml中
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

        // 修改树型结构
        auto iterTreeGroup = m_mapScene[strSceneName].mapEquipName.find(strGroupName);
        if (iterTreeGroup != m_mapScene[strSceneName].mapEquipName.end())
        {
            m_mapScene[strSceneName].mapEquipName[strNewName] = *iterTreeGroup;

            m_mapScene[strSceneName].mapEquipName.erase(iterTreeGroup);
        }

        // 修改view结构
        auto iterViewGroup = m_arrModels[strSceneName].m_mapEquipGroup.find(strGroupName);
        if (iterViewGroup != m_arrModels[strSceneName].m_mapEquipGroup.end())
        {
            // 内存结构修改
            m_arrModels[strSceneName].m_mapEquipGroup[strNewName] = *iterViewGroup;
            // 删除旧的结构
            m_arrModels[strSceneName].m_mapEquipGroup.erase(iterViewGroup);
        }

        // 保存新的结构到modeltool.xml中
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

        // 修改树型结构
        auto iterTreeModel = m_mapScene[strSceneName].setModelName.find(strModelName);
        if (iterTreeModel != m_mapScene[strSceneName].setModelName.end())
        {
            m_mapScene[strSceneName].setModelName.insert(strNewName);

            m_mapScene[strSceneName].setModelName.erase(iterTreeModel);
        }

        // 修改view结构
        auto iterViewModel = m_arrModels[strSceneName].m_mapEquipModel.find(strModelName);
        if (iterViewModel != m_arrModels[strSceneName].m_mapEquipModel.end())
        {
            // 内存结构修改
            m_arrModels[strSceneName].m_mapEquipModel[strNewName] = *iterViewModel;
            // 删除旧的结构
            m_arrModels[strSceneName].m_mapEquipModel.erase(iterViewModel);
        }

        // 保存新的结构到modeltool.xml中
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

        // 修改树型结构
        auto iterTreeModel = m_mapScene[strSceneName].mapEquipName[strGroupName].find(strModelName);
        if (iterTreeModel != m_mapScene[strSceneName].mapEquipName[strGroupName].end())
        {
            m_mapScene[strSceneName].mapEquipName[strGroupName].insert(strNewName);

            m_mapScene[strSceneName].mapEquipName[strGroupName].erase(iterTreeModel);
        }

        // 修改view结构
        auto iterViewModel = m_arrModels[strSceneName].m_mapEquipGroup[strGroupName].find(strModelName);
        if (iterViewModel != m_arrModels[strSceneName].m_mapEquipGroup[strGroupName].end())
        {
            // 内存结构修改
            m_arrModels[strSceneName].m_mapEquipGroup[strGroupName][strNewName] = *iterViewModel;
            // 删除旧的结构
            m_arrModels[strSceneName].m_mapEquipGroup[strGroupName].erase(iterViewModel);
        }

        // 保存新的结构到modeltool.xml中
        SaveToFile("");

        return true;
    }

public:

    // check场景
    QString CheckCreateSceneName(bool bInsertFlag = false);

    // check组
    QString CheckCreateGroupName(QString& strSceneName, bool bInsertFlag = false);

    // check场景下面的型号
    QString CheckCreateModelNoName(QString& strSceneName, bool bInsertFlag = false);
    // check场景下面的组下面的型号
    QString CheckCreateModelNoName(QString& strSceneName, QString& strGroupName, bool bInsertFlag = false);

public: // modeltool.xml结构

    // 目录和文件的对应关系(设备和型号对应关系)
    QMap<QString, QSet<QString> > m_mapEquipName;

    SGroupAndModel m_sGroupModel;

    // key:场景名  value:目录和文件的对应关系(设备和型号对应关系)
    QMap<QString, SGroupAndModel> m_mapScene;

public: // 树型结构和模型结构的对应关系

    // key:场景
    QMap<QString, CEquipmentGroupAndModel> m_arrModels;

private:
    // 数据类型
    int m_nSelectDataType;


private:
};




#endif
