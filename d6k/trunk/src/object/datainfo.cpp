
#include "datainfo.h"
#include "objectdefine.h"

bool CObjectGroup::AddObjectGroup(const QString& strGroupNameKey, CObjectGroup* pObjGroup)
{
    if (strGroupNameKey.isEmpty() || pObjGroup == nullptr)
    {
        return false;
    }

    auto iter = m_mapGrps.find(strGroupNameKey);
    if (iter == m_mapGrps.end())
    {
        m_mapGrps[strGroupNameKey] = pObjGroup;
    }

    return true;
}

QString CObjectGroup::GetGroupName(const QString& strGroupName)
{
    if (strGroupName.isEmpty())
    {
        return "";
    }

    int nIndex = 0;

    QString strName = strGroupName;
    while (1)
    {
        if (0 == nIndex)
        {
            strName = strGroupName;
        }
        else
        {
            strName = QString("%1%2%3%4").arg(strGroupName).arg(NAME_LEFT_BRACKETS).arg(nIndex).arg(NAME_RIGHT_BRACKETS);
        }

        auto iter = m_mapGrps.find(strName);
        if (iter == m_mapGrps.end())
        {
            return std::move(strName);
        }

        nIndex++;
    }
}

bool CObjectGroup::AddObjectItem(const QString& strGroupNameKey, const CObjectItem* pObjItem)
{
    if (strGroupNameKey.isEmpty() || pObjItem == nullptr)
    {
        return false;
    }

    auto iter = m_mapItemInfo.find(strGroupNameKey);
    if (iter == m_mapItemInfo.end())
    {
        m_mapItemInfo[strGroupNameKey] = const_cast<CObjectItem*>(pObjItem);

        return true;
    }

    if (m_mapItemInfo[strGroupNameKey] == nullptr)
    {
        m_mapItemInfo[strGroupNameKey] = const_cast<CObjectItem*>(pObjItem);

        return true;
    }
    return false;
}

// 获取有效item名
QString CObjectGroup::GetItemName(const QString& strItemName)
{
    if (strItemName.isEmpty())
    {
        return "";
    }

    int nIndex = 0;

    QString strName = strItemName;
    while (1)
    {
        if (0 == nIndex)
        {
            strName = strItemName;
        }
        else
        {
            strName = QString("%1%2%3%4").arg(strItemName).arg(NAME_LEFT_BRACKETS).arg(nIndex).arg(NAME_RIGHT_BRACKETS);
        }

        auto iter = m_mapItemInfo.find(strName);
        if (iter == m_mapItemInfo.end())
        {
            return std::move(strName);
        }

        nIndex++;
    }
}

CObjectGroup* CObjectGroup::CreateGroup(const QString& strGroupName)
{
    auto pObjGrp = new CObjectGroup();

    return pObjGrp;
}

CObjectItem* CObjectGroup::CreateItem(const QString& strItemName)
{
    auto pObjItem = new CObjectItem();

    return pObjItem;
}



CObjectGroup* CObjectMgr::GetGroupInfo(const int nCurrntType) const
{
    //m_mapItemMgrInfo

    return nullptr;
}






void CObjectItem::SetData()
{

}
