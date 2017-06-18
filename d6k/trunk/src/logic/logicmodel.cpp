/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  logicmodel.cpp
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
** \details �����������ģ��
** \param   
** \return
** \author ww
** \date 2017��1��4��
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
** \details �����ڵ�ģ��
** \param
** \return
** \author ww
** \date 2017��1��4��
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
** \details ������������ģ��
** \param ��������
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
std::shared_ptr<CBinProperty> CBlockProperty::CreatePinProperty(const QString & szName)
{
    Q_ASSERT(szName.isEmpty() == false);
    if (szName.isEmpty() == true)
    {
        return nullptr;
    }
    // ȷ��û������
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
** \details �����߼���ģ��
** \param ��������
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
std::shared_ptr<CBlockProperty> CLogicObjectModel::CreateBlockProperty(int nCreateNum)
{
    Q_ASSERT(nCreateNum > 0);
    if (nCreateNum <= 0)
    {
        return nullptr;
    }
    // ȷ��û������
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

//����line
/*! \fn CLogicObjectModel::CreateLineProperty()
*********************************************************************************************************
** \brief CLogicObjectModel::CreateLineProperty()
** \details ����line
** \param 
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
std::shared_ptr<CLineProperty> CLogicObjectModel::CreateLineProperty()
{
    std::shared_ptr<CLineProperty> pLine = std::make_shared<CLineProperty>();
    m_arrLineProperties.emplace_back(pLine);

    return pLine;
}

//����ע��
/*! \fn CLogicObjectModel::CreateCommentProperty()
*********************************************************************************************************
** \brief CLogicObjectModel::CreateCommentProperty()
** \details ����ע��
** \param 
** \return
** \author ww
** \date 2017��1��4��
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
