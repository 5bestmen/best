/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :  logiccontainter.cpp
文件实现功能 :  数据模型
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明> 逻辑编辑
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author ww
*  @version 1.0
*  @date    2017.1.4
*/
#include "logiccontainter.h"
#include "logicmodel.h"

CLogicContainter::CLogicContainter()
{

}

/*! \fn CLogicContainter::CreateNodeContainer(const QString & szName)
*********************************************************************************************************
** \brief CLogicContainter::CreateNodeContainer(const QString & szName)
** \details 创建新的节点
** \param  节点名称
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
std::shared_ptr<CNodeLogicContainer> CLogicContainter::CreateNodeContainer(const QString & szName)
{
    Q_ASSERT(szName.isEmpty() == false);
    if (szName.isEmpty() == true)
    {
        return nullptr;
    }
    // 确定没有重名
    for (auto it : m_NodeContainers)
    {
        if (szName.compare(it->GetName()) == 0)
        {
            Q_ASSERT(false);
            return nullptr;
        }
    }

    std::shared_ptr<CNodeLogicContainer> pNodeContainer = std::make_shared<CNodeLogicContainer>(szName);
    m_NodeContainers.emplace_back(pNodeContainer);

    return pNodeContainer;
}

/*! \fn CLogicContainter::RemoveNodeContainer(const QString & szName)
*********************************************************************************************************
** \brief CLogicContainter::RemoveNodeContainer(const QString & szName)
** \details 删除指点的节点名称
** \param  节点名称
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
bool CLogicContainter::RemoveNodeContainer(const QString & szName)
{
    Q_ASSERT(szName.isEmpty() == false);
    if (szName.isEmpty() == true)
    {
        return false;
    }
    // 按名字查找
    for (auto it = m_NodeContainers.begin(); it != m_NodeContainers.end();)
    {
        if (szName.compare((*it)->GetName()) == 0)
        {
            it = m_NodeContainers.erase(it);
            return true;
        }
        else
        {
            ++it;
        }
    }

    Q_ASSERT(false);
    return false;
}

/*! \fn CLogicContainter::GetNodeContainers()
*********************************************************************************************************
** \brief CLogicContainter::GetNodeContainers()
** \details 获取所有的节点信息
** \param  
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
std::vector<std::shared_ptr<CNodeLogicContainer>> CLogicContainter::GetNodeContainers()
{
    return m_NodeContainers;
}

CNodeLogicContainer::CNodeLogicContainer()
{
    m_mastModels = nullptr;
    m_fastModels = nullptr;
    m_auxModels = nullptr;
}

CNodeLogicContainer::CNodeLogicContainer(const QString & szName) :m_strTagname(szName)
{
    Q_ASSERT(szName.isEmpty() == false);
}

CNodeLogicContainer::~CNodeLogicContainer()
{

}

/*! \fn CNodeLogicContainer::CreateMastContainer()
*********************************************************************************************************
** \brief CNodeLogicContainer::CreateMastContainer()
** \details 创建mast模型
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
std::shared_ptr<CTaskContainer> CNodeLogicContainer::CreateMastContainer()
{
    Q_ASSERT(m_mastModels == nullptr);
    if (m_mastModels != nullptr)
    {
        return nullptr;
    }

    m_mastModels = std::make_shared<CTaskContainer>();

    return m_mastModels;
}

/*! \fn CNodeLogicContainer::CreateFastContainer()
*********************************************************************************************************
** \brief CNodeLogicContainer::CreateFastContainer()
** \details 创建fast模型
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
std::shared_ptr<CTaskContainer> CNodeLogicContainer::CreateFastContainer()
{
    Q_ASSERT(m_fastModels == nullptr);
    if (m_fastModels != nullptr)
    {
        return nullptr;
    }

    m_fastModels = std::make_shared<CTaskContainer>();

    return m_fastModels;
}

/*! \fn CNodeLogicContainer::CreateAuxContainer()
*********************************************************************************************************
** \brief CNodeLogicContainer::CreateAuxContainer()
** \details 创建aux模型
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
std::shared_ptr<CTaskContainer> CNodeLogicContainer::CreateAuxContainer()
{
    Q_ASSERT(m_auxModels == nullptr);
    if (m_auxModels != nullptr)
    {
        return nullptr;
    }

    m_auxModels = std::make_shared<CTaskContainer>();

    return m_auxModels;
}

/*! \fn CTaskContainer::CreateSegModel(const QString & szName)
*********************************************************************************************************
** \brief CTaskContainer::CreateSegModel(const QString & szName)
** \details 创建seg模型
** \param  seg名称
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
std::shared_ptr<CLogicObjectModel> CTaskContainer::CreateSegModel(const QString & szName)
{
    Q_ASSERT(szName.isEmpty() == false);
    if (szName.isEmpty() == true)
    {
        return nullptr;
    }
    // 确定没有重名
    for (auto it : m_segModels)
    {
        if (szName.compare(it->GetName()) == 0)
        {
            Q_ASSERT(false);
            return nullptr;
        }
    }

    std::shared_ptr<CLogicObjectModel> pObjectModel = std::make_shared<CLogicObjectModel>(szName);
    m_segModels.emplace_back(pObjectModel);

    return pObjectModel;
}

/*! \fn CTaskContainer::CreateSegModel(const QString & szName)
*********************************************************************************************************
** \brief CTaskContainer::CreateSegModel(const QString & szName)
** \details 创建seg模型
** \param  seg名称
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
bool CTaskContainer::RemoveSegModel(const QString & szName)
{
    Q_ASSERT(szName.isEmpty() == false);
    if (szName.isEmpty() == true)
    {
        return false;
    }
    // 按名字查找
    for (auto it = m_segModels.begin(); it != m_segModels.end();)
    {
        if (szName.compare((*it)->GetName()) == 0)
        {
            it = m_segModels.erase(it);
            return true;
        }
        else
        {
            ++it;
        }
    }

    Q_ASSERT(false);
    return false;
}

std::vector<std::shared_ptr<CLogicObjectModel>> CTaskContainer::GetSegModels()
{
    return m_segModels;
}

/*! \fn CTaskContainer::CreateSrModel(const QString & szName)
*********************************************************************************************************
** \brief CTaskContainer::CreateSrModel(const QString & szName)
** \details 创建sr模型
** \param  sr名称
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
std::shared_ptr<CLogicObjectModel> CTaskContainer::CreateSrModel(const QString & szName)
{
    Q_ASSERT(szName.isEmpty() == false);
    if (szName.isEmpty() == true)
    {
        return nullptr;
    }
    // 确定没有重名
    for (auto it : m_srModels)
    {
        if (szName.compare(it->GetName()) == 0)
        {
            Q_ASSERT(false);
            return nullptr;
        }
    }

    std::shared_ptr<CLogicObjectModel> pObjectModel = std::make_shared<CLogicObjectModel>(szName);
    m_srModels.emplace_back(pObjectModel);

    return pObjectModel;
}

/*! \fn CTaskContainer::RemoveSrModel(const QString & szName)
*********************************************************************************************************
** \brief CTaskContainer::RemoveSrModel(const QString & szName)
** \details 删除sr模型
** \param  sr名称
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
bool CTaskContainer::RemoveSrModel(const QString & szName)
{
    Q_ASSERT(szName.isEmpty() == false);
    if (szName.isEmpty() == true)
    {
        return false;
    }
    // 按名字查找
    for (auto it = m_srModels.begin(); it != m_srModels.end();)
    {
        if (szName.compare((*it)->GetName()) == 0)
        {
            it = m_srModels.erase(it);
            return true;
        }
        else
        {
            ++it;
        }
    }

    Q_ASSERT(false);
    return false;
}

std::vector<std::shared_ptr<CLogicObjectModel>> CTaskContainer::GetSrModels()
{
    return m_srModels;
}
