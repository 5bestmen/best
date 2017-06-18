
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
    QString m_strName;                  // 名称
    QString m_strDiscribe;              // 描述
    int m_nType;                        // 类型
    bool m_bRWAttr;                     // 读写属性
    QString m_strRelationPoint;         // 关联测点

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

    QString m_strObjectName;            // 对象名

    // 静态属性
    QString m_strGuid;                  // GUID信息
    QString m_strState;                 // 设备状态（比如，线路的状态有：运行、停运，发电机：发电、停机、空载、空转、调相）(静态部分)
    QString m_strRelationWarning;       // 关联告警(静态部分)
    QString m_strRelationChildEquip;    // 关联子设备(静态部分)
    QString m_strRelationPicture;       // 关联图元(静态部分)
    QString m_strRelationControl;       // 关联的控制程序：脚本、顺控等等(静态部分)

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

    // 创建组
    CObjectGroup* CreateGroup(const QString& strGroupName);

    // 创建模型
    CObjectItem* CreateItem(const QString& strItemName = "");

    // 添加Group
    bool AddObjectGroup(const QString& strGroupNameKey, CObjectGroup* pObjItem);

    // 获取有效组名
    QString GetGroupName(const QString& strGroupName);

    // 添加item
    bool AddObjectItem(const QString& strGroupNameKey, const CObjectItem* pObjItem);

    // 获取有效item名
    QString GetItemName(const QString& strItemName);

public:
    QString m_szGroupName; // 组名
    QMap<QString, CObjectItem*>    m_mapItemInfo;   // 对象Item信息(key : item名)
    QMap<QString, CObjectGroup*>   m_mapGrps;       // 对象组信息(key : group名)
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

    // 获取组信息
    CObjectGroup* GetGroupInfo(const int nCurrntType) const;

public:
    QString m_szGroupName; // 组名

    CObjectGroup*   m_pMgrGrps;       // 对象组信息

    //CObjectItem*    m_pItemMgrInfo;   // 对象Item信息
};

#endif // DATA_INFO_H
