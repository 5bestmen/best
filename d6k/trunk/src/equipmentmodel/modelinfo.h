
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

    bool m_bReadWrite;          // ture:ֻ��
    int	m_nDataType;            // �������ͣ����Σ������͵�
    std::string m_strName;      // ����
    std::string m_strDescribe;  // ��ʶ
};

class CModelInfo
{
public:
    // Guid
    QString m_strGuid;
    // key:�к�
    QMap<int, CContentInfo> m_mapContentInfo;
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
    bool SaveToFile(const QString& strPath, const QString& strFileName, const QString& strGuid);

    bool WriteInfo(QXmlStreamWriter& writer);

    void AddAttr(int nRow, CContentInfo& pAttr)
    {
        // ��������������
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
    // key:�ͺ�
    QMap<QString, CEquipmentModel> m_mapEquipModel;

    // key:�豸��
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
    QMap<QString, QMap<QString, QString> > mapEquipName; // ��

    QMap<QString, QString> mapModelName; // �ͺ�
};

// �豸ģ�Ϳ�
class CModelLib
{
public:

    CModelLib();

    ~CModelLib();

    // ��ȡ����������ͺ�
    CEquipmentModel* GetEquipModel(const QString& strSceneName, const QString& strModelName);

    // ��ȡ��������ͺ�
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
   
public: // ɾ�����κ�view�ڴ�ṹ

    // ɾ������
    bool DelSceneItem(const QString& strSceneName);

    // ɾ�������������
    bool DelSceneGroupItem(const QString& strSceneName, const QString& strGroupName);

    // ɾ������������ͺ�
    bool DelSceneModelNoItem(const QString& strSceneName, const QString& strModelNoName);

    // ɾ����������ͺ�
    bool DelGroupModelNoItem(const QString& strSceneName, const QString& strGroupName, const QString& strModelNoName);

public: // ������Ϣ

    // ��modeltool.xml������Ϣ
    bool LoadTreeDataFromFile(const QString& szPath);

    // ��ȡ����
    bool ReadScene(QDomElement& root);

    // ��ȡ�豸ͷ
    bool ReadEquipment(QDomElement& dElement);

    // ��ȡ�豸����
    bool ReadEquipmentInfo(QDomElement& dElement);

    // ��ȡ�ͺ�
    bool ReadModel(QDomElement& dElement, QMap<QString, QString>& mapFileName);

    // ��ȡ�����ͺ�
    bool ReadSingleModel(QDomElement& dElement);

    // ��ȡ�ͺ�����
    bool ReadContentInfo(QDomElement& dElement, QMap<QString, QString>& mapFileName);

    // ���ļ��м�����Ϣ��view
    bool LoadViewDataFromFile(const QString& szPath);

    // ����view��Ϣ
    bool LoadModelInfo(const QString& strPathName);

    // ����view��Ϣ
    bool LoadViewInfo(QDomElement& root);

    // ��ǰģʽ (���� or ��)
    int m_nCurrentMode;

    QString m_strSceneName;
    QString m_strGroupName;
    QString m_strModelName;

public: // дxml

    // �������νṹ���ļ�
    bool SaveToFile(const QString& szPath);

    // д�볡����Ϣ
    void WriteSceneToConfig(QXmlStreamWriter& writer);

    // д���豸����Ϣ
    void WriteEquipInfo(const SGroupAndModel& sGroupModel, QXmlStreamWriter& writer);

    // д���ͺ���Ϣ
    void WriteModelToConfig(const QMap<QString, QString>& mapFileName, QXmlStreamWriter& writer);

public: // �޸����ͽṹ��

    // �޸ĳ�����
    bool ModifyScene(const QString& strSceneName, const QString& strNewName);

    // �޸ĳ������������
    bool ModifySceneGroup(const QString& strSceneName, const QString& strGroupName, const QString& strNewName);

    // �޸ĳ���������ͺ�
    bool ModifySceneModelNo(const QString& strSceneName, const QString& strModelName, const QString& strNewName);

    // �޸���������ͺ�
    bool ModifyGroupModelNo(const QString& strSceneName, const QString& strGroupName, const QString& strModelName, const QString& strNewName);

public: // ����ʱ,����check

    // check����
    QString CheckCreateSceneName(bool bInsertFlag = false);

    // check��
    QString CheckCreateGroupName(const QString& strSceneName, bool bInsertFlag = false);

    // check����������ͺ�
    QString CheckCreateModelNoName(const QString& strSceneName, bool bInsertFlag = false);

    // check�����������������ͺ�
    QString CheckCreateModelNoName(const QString& strSceneName, const QString& strGroupName, bool bInsertFlag = false);

public: // modeltool.xml�ṹ

    // Ŀ¼���ļ��Ķ�Ӧ��ϵ(�豸���ͺŶ�Ӧ��ϵ)
    QMap<QString, QSet<QString> > m_mapEquipName;

    SGroupAndModel m_sGroupModel;

    // key:������  value:Ŀ¼���ļ��Ķ�Ӧ��ϵ(�豸���ͺŶ�Ӧ��ϵ)
    QMap<QString, SGroupAndModel> m_mapScene;

public: // ���ͽṹ��ģ�ͽṹ�Ķ�Ӧ��ϵ

    // key:����
    QMap<QString, CEquipmentGroupAndModel> m_arrModels;

};

#endif // _MODEL_INFO_H_
