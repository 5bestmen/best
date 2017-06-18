
#ifndef _MODEL_INFO_H_
#define _MODEL_INFO_H_

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
    CContentInfo() : m_bReadWrite(false), m_nDataType(-1), m_strName(""), m_strDescribe("")
    {
    }

    bool m_bReadWrite;          // ture:只读
    int	m_nDataType;            // 数据类型，整形，浮点型等
    std::string m_strName;      // 名称
    std::string m_strDescribe;  // 标识
};

class CModelInfo
{
public:
    // Guid
    QString m_strGuid;
    // key:行号
    QMap<int, CContentInfo> m_mapContentInfo;
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
    bool SaveToFile(const QString& strPath, const QString& strFileName, const QString& strGuid);

    bool WriteInfo(QXmlStreamWriter& writer);

    void AddAttr(int nRow, CContentInfo& pAttr)
    {
        // 保存新增的数据
        m_arrAttrs.m_mapContentInfo.insert(nRow, pAttr);
    }

    int GetAttrSize()
    {
        return m_arrAttrs.m_mapContentInfo.size();
    }

public:

    
    CModelInfo m_arrAttrs;
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

/*class CModelInfo
{
public:
    CModelInfo();
    ~CModelInfo();

private:
    QString strName;

};*/

struct SGroupAndModel
{
    QMap<QString, QMap<QString, QString> > mapEquipName; // 组

    QMap<QString, QString> mapModelName; // 型号
};

// 设备模型库
class CModelLib
{
public:

    CModelLib();

    ~CModelLib();

    // 获取场景下面的型号
    CEquipmentModel* GetEquipModel(const QString& strSceneName, const QString& strModelName);

    // 获取组下面的型号
    CEquipmentModel* GetEquipModel(const QString& strSceneName, const QString& strGroupName, const QString& strModelName);

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
   
public: // 删除树形和view内存结构

    // 删除场景
    bool DelSceneItem(const QString& strSceneName);

    // 删除场景下面的组
    bool DelSceneGroupItem(const QString& strSceneName, const QString& strGroupName);

    // 删除场景下面的型号
    bool DelSceneModelNoItem(const QString& strSceneName, const QString& strModelNoName);

    // 删除组下面的型号
    bool DelGroupModelNoItem(const QString& strSceneName, const QString& strGroupName, const QString& strModelNoName);

public: // 加载信息

    // 从modeltool.xml加载信息
    bool LoadTreeDataFromFile(const QString& szPath);

    // 读取场景
    bool ReadScene(QDomElement& root);

    // 读取设备头
    bool ReadEquipment(QDomElement& dElement);

    // 读取设备内容
    bool ReadEquipmentInfo(QDomElement& dElement);

    // 读取型号
    bool ReadModel(QDomElement& dElement, QMap<QString, QString>& mapFileName);

    // 读取单个型号
    bool ReadSingleModel(QDomElement& dElement);

    // 读取型号内容
    bool ReadContentInfo(QDomElement& dElement, QMap<QString, QString>& mapFileName);

    // 从文件中加载信息到view
    bool LoadViewDataFromFile(const QString& szPath);

    // 加载view信息
    bool LoadModelInfo(const QString& strPathName);

    // 加载view信息
    bool LoadViewInfo(QDomElement& root);

    // 当前模式 (场景 or 组)
    int m_nCurrentMode;

    QString m_strSceneName;
    QString m_strGroupName;
    QString m_strModelName;

public: // 写xml

    // 保存树形结构到文件
    bool SaveToFile(const QString& szPath);

    // 写入场景信息
    void WriteSceneToConfig(QXmlStreamWriter& writer);

    // 写入设备组信息
    void WriteEquipInfo(const SGroupAndModel& sGroupModel, QXmlStreamWriter& writer);

    // 写入型号信息
    void WriteModelToConfig(const QMap<QString, QString>& mapFileName, QXmlStreamWriter& writer);

public: // 修改树型结构名

    // 修改场景名
    bool ModifyScene(const QString& strSceneName, const QString& strNewName);

    // 修改场景下面的组名
    bool ModifySceneGroup(const QString& strSceneName, const QString& strGroupName, const QString& strNewName);

    // 修改场景下面的型号
    bool ModifySceneModelNo(const QString& strSceneName, const QString& strModelName, const QString& strNewName);

    // 修改组下面的型号
    bool ModifyGroupModelNo(const QString& strSceneName, const QString& strGroupName, const QString& strModelName, const QString& strNewName);

public: // 新增时,名字check

    // check场景
    QString CheckCreateSceneName(bool bInsertFlag = false);

    // check组
    QString CheckCreateGroupName(const QString& strSceneName, bool bInsertFlag = false);

    // check场景下面的型号
    QString CheckCreateModelNoName(const QString& strSceneName, bool bInsertFlag = false);

    // check场景下面的组下面的型号
    QString CheckCreateModelNoName(const QString& strSceneName, const QString& strGroupName, bool bInsertFlag = false);

public: // modeltool.xml结构

    // 目录和文件的对应关系(设备和型号对应关系)
    QMap<QString, QSet<QString> > m_mapEquipName;

    SGroupAndModel m_sGroupModel;

    // key:场景名  value:目录和文件的对应关系(设备和型号对应关系)
    QMap<QString, SGroupAndModel> m_mapScene;

public: // 树型结构和模型结构的对应关系

    // key:场景
    QMap<QString, CEquipmentGroupAndModel> m_arrModels;

};

#endif // _MODEL_INFO_H_
