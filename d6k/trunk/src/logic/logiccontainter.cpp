/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  logiccontainter.cpp
�ļ�ʵ�ֹ��� :  ����ģ��
����         :  ww
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��> �߼��༭
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
** \details �����µĽڵ�
** \param  �ڵ�����
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
std::shared_ptr<CNodeLogicContainer> CLogicContainter::CreateNodeContainer(const QString & szName)
{
    Q_ASSERT(szName.isEmpty() == false);
    if (szName.isEmpty() == true)
    {
        return nullptr;
    }
    // ȷ��û������
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
** \details ɾ��ָ��Ľڵ�����
** \param  �ڵ�����
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
bool CLogicContainter::RemoveNodeContainer(const QString & szName)
{
    Q_ASSERT(szName.isEmpty() == false);
    if (szName.isEmpty() == true)
    {
        return false;
    }
    // �����ֲ���
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
** \details ��ȡ���еĽڵ���Ϣ
** \param  
** \return
** \author ww
** \date 2017��1��4��
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
** \details ����mastģ��
** \param
** \return
** \author ww
** \date 2017��1��4��
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
** \details ����fastģ��
** \param
** \return
** \author ww
** \date 2017��1��4��
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
** \details ����auxģ��
** \param
** \return
** \author ww
** \date 2017��1��4��
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
** \details ����segģ��
** \param  seg����
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
std::shared_ptr<CLogicObjectModel> CTaskContainer::CreateSegModel(const QString & szName)
{
    Q_ASSERT(szName.isEmpty() == false);
    if (szName.isEmpty() == true)
    {
        return nullptr;
    }
    // ȷ��û������
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
** \details ����segģ��
** \param  seg����
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
bool CTaskContainer::RemoveSegModel(const QString & szName)
{
    Q_ASSERT(szName.isEmpty() == false);
    if (szName.isEmpty() == true)
    {
        return false;
    }
    // �����ֲ���
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
** \details ����srģ��
** \param  sr����
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
std::shared_ptr<CLogicObjectModel> CTaskContainer::CreateSrModel(const QString & szName)
{
    Q_ASSERT(szName.isEmpty() == false);
    if (szName.isEmpty() == true)
    {
        return nullptr;
    }
    // ȷ��û������
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
** \details ɾ��srģ��
** \param  sr����
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
bool CTaskContainer::RemoveSrModel(const QString & szName)
{
    Q_ASSERT(szName.isEmpty() == false);
    if (szName.isEmpty() == true)
    {
        return false;
    }
    // �����ֲ���
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
