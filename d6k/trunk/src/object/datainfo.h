
#ifndef DATA_INFO_H
#define DATA_INFO_H

#include <QString>
#include <QVector>
#include <QMap>

#include "../include/scadastudio/imainwindow.h"

class CItemInfo
{
public:
    CItemInfo()
    {

    }
    ~CItemInfo()
    {

    }

public:
    QString m_strName;                  // ����
    QString m_strDiscribe;              // ����
    int m_nType;                        // ����
    bool m_bRWAttr;                     // ��д����
    QString m_strRelationPoint;         // �������

};

class CObjectItem
{
public:
    CObjectItem()
    {

    }

    ~CObjectItem()
    {

    }

    void SetData();

public:

    QString m_strObjectName;            // ������

    // ��̬����
    QString m_strGuid;                  // GUID��Ϣ
    QString m_strState;                 // �豸״̬�����磬��·��״̬�У����С�ͣ�ˣ�����������硢ͣ�������ء���ת�����ࣩ(��̬����)
    QString m_strRelationWarning;       // �����澯(��̬����)
    QString m_strRelationChildEquip;    // �������豸(��̬����)
    QString m_strRelationPicture;       // ����ͼԪ(��̬����)
    QString m_strRelationControl;       // �����Ŀ��Ƴ��򣺽ű���˳�صȵ�(��̬����)

    QVector<CItemInfo> m_vecTableInfo;
};

class CObjectGroup
{
public:
    CObjectGroup()
    {

    }
    ~CObjectGroup()
    {

    }

    // ������
    CObjectGroup* CreateGroup(const QString& strGroupName);

    // ����ģ��
    CObjectItem* CreateItem(const QString& strItemName = "");

    // ���Group
    bool AddObjectGroup(const QString& strGroupNameKey, CObjectGroup* pObjItem);

    // ��ȡ��Ч����
    QString GetGroupName(const QString& strGroupName);

    // ���item
    bool AddObjectItem(const QString& strGroupNameKey, const CObjectItem* pObjItem);

    // ��ȡ��Чitem��
    QString GetItemName(const QString& strItemName);

public:
    QString m_szGroupName; // ����
    QMap<QString, CObjectItem*>    m_mapItemInfo;   // ����Item��Ϣ(key : item��)
    QMap<QString, CObjectGroup*>   m_mapGrps;       // ��������Ϣ(key : group��)
};

class CObjectMgr
{
public:
    CObjectMgr()
    {
        m_pMgrGrps = nullptr;
        //m_pItemMgrInfo = nullptr;
    }
    ~CObjectMgr()
    {

    }    

    // ��ȡ����Ϣ
    CObjectGroup* GetGroupInfo(const int nCurrntType) const;

public:
    QString m_szGroupName; // ����

    CObjectGroup*   m_pMgrGrps;       // ��������Ϣ

    //CObjectItem*    m_pItemMgrInfo;   // ����Item��Ϣ
};

#endif // DATA_INFO_H
