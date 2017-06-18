/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :  logicmodel.cpp
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
#include "logicmodel.h"

CBinProperty::CBinProperty()
{

}

CBinProperty::~CBinProperty()
{

}

/*! \fn CBinProperty::CreateInModel()
*********************************************************************************************************
** \brief CBinProperty::CreateInModel()
** \details 创建输入端子模型
** \param   
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
std::shared_ptr<CInPutProperty> CBinProperty::CreateInModel()
{
    Q_ASSERT(m_InProperty == nullptr);

    if (m_InProperty != nullptr)
    {
        return nullptr;
    }

    m_InProperty = std::make_shared<CInPutProperty>();

    return m_InProperty;
}

/*! \fn CBinProperty::CreateCellPro()
*********************************************************************************************************
** \brief CBinProperty::CreateCellPro()
** \details 创建节点模型
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
std::shared_ptr<CCellProperty> CBinProperty::CreateCellPro()
{
    Q_ASSERT(m_CellModel == nullptr);

    if (m_CellModel != nullptr)
    {
        return nullptr;
    }

    m_CellModel = std::make_shared<CCellProperty>();

    return m_CellModel;
}



CBlockProperty::CBlockProperty()
{
}

CBlockProperty::CBlockProperty(int nCreateNum) :m_nCreateNum(nCreateNum)
{
}

/*! \fn CBlockProperty::CreatePinProperty(const QString & szName)
*********************************************************************************************************
** \brief CBlockProperty::CreatePinProperty(const QString & szName)
** \details 创建端子属性模型
** \param 端子名称
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
std::shared_ptr<CBinProperty> CBlockProperty::CreatePinProperty(const QString & szName)
{
    Q_ASSERT(szName.isEmpty() == false);
    if (szName.isEmpty() == true)
    {
        return nullptr;
    }
    // 确定没有重名
    for (auto it : m_pinValues)
    {
        if (szName.compare(it->GetName()) == 0)
        {
            return it;
        }
    }

    std::shared_ptr<CBinProperty> pPinProperty = std::make_shared<CBinProperty>(szName);
    m_pinValues.emplace_back(pPinProperty);

    return pPinProperty;
}

int CBlockProperty::GetCreateNum()
{
    return m_nCreateNum;
}


CLogicObjectModel::CLogicObjectModel()
{

}

CLogicObjectModel::CLogicObjectModel(const QString & szName) :m_szName(szName)
{
    Q_ASSERT(szName.isEmpty() == false);
}

/*! \fn CLogicObjectModel::CreateBlockProperty(int nCreateNum)
*********************************************************************************************************
** \brief CLogicObjectModel::CreateBlockProperty(int nCreateNum)
** \details 创建逻辑块模型
** \param 端子名称
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
std::shared_ptr<CBlockProperty> CLogicObjectModel::CreateBlockProperty(int nCreateNum)
{
    Q_ASSERT(nCreateNum > 0);
    if (nCreateNum <= 0)
    {
        return nullptr;
    }
    // 确定没有重名
    for (auto it : m_arrBlockPropertise)
    {
        if (nCreateNum == it->GetCreateNum())
        {
            Q_ASSERT(false);
            return nullptr;
        }
    }

    std::shared_ptr<CBlockProperty> pBlock = std::make_shared<CBlockProperty>(nCreateNum);
    m_arrBlockPropertise.emplace_back(pBlock);

    return pBlock;
}

//创建line
/*! \fn CLogicObjectModel::CreateLineProperty()
*********************************************************************************************************
** \brief CLogicObjectModel::CreateLineProperty()
** \details 创建line
** \param 
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
std::shared_ptr<CLineProperty> CLogicObjectModel::CreateLineProperty()
{
    std::shared_ptr<CLineProperty> pLine = std::make_shared<CLineProperty>();
    m_arrLineProperties.emplace_back(pLine);

    return pLine;
}

//创建注释
/*! \fn CLogicObjectModel::CreateCommentProperty()
*********************************************************************************************************
** \brief CLogicObjectModel::CreateCommentProperty()
** \details 创建注释
** \param 
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
std::shared_ptr<CCommentProperty> CLogicObjectModel::CreateCommentProperty()
{
    std::shared_ptr<CCommentProperty> pComment = std::make_shared<CCommentProperty>();
    m_arrCommentProperties.emplace_back(pComment);

    return pComment;

}

void CLineProperty::SetPoints(double x, double y)
{
    std::shared_ptr<QPointF> pPoint = std::make_shared<QPointF>(x,y);
    m_lstAllPoints.emplace_back(pPoint);
}
