#ifndef LOGICMODEL_H
#define LOGICMODEL_H
#include <QString>
#include <vector>
#include <memory>
#include <QRectF>
#include <QPointF>

class CBlockProperty;
//Ԫ����Ϣ 
class CCellProperty
{
public:
    void SetCellName(const QString &strName)
    {
        m_strName = strName;
    }

    const QString &GetCellName()
    {
        return m_strName;
    }


private:
    //Ԫ������
    QString m_strName;
};

//�������Ϣ
class CInPutProperty
{
public:
    CInPutProperty(){};

    ~CInPutProperty(){};
    void SetCreateNum(int nCreateNum)
    {
        m_nCreateNum = nCreateNum;
    }

    int GetCreateNum()
    {
        return m_nCreateNum;
    }

    void SetExecNum(int nExecNum)
    {
        m_nExecNum = nExecNum;
    }

    int GetExecNum()
    {
        return m_nExecNum;
    }

    void SetOutPutIndex(int nOutputindex)
    {
        m_nOutPutIndex = nOutputindex;
    }

    int GetOutputIndex()
    {
        return m_nOutPutIndex;
    }

    const QString &GetOutPinName()
    {
        return m_strOutPutPinName;
    }

    void SetOutputPinName(const QString &strPinName)
    {
        m_strOutPutPinName = strPinName;
    }
private:
    int m_nCreateNum;             //�������
    int m_nExecNum;               //ִ�б��
    int m_nOutPutIndex;           //������
    QString m_strOutPutPinName;   //���bin����
};


//����ģ��
class CBinProperty
{
public:
    CBinProperty();
    ~CBinProperty();
    explicit CBinProperty(const QString &strPinname)
    {
        m_strPinName = strPinname;
    }
    const QString& GetName()
    {
        return m_strPinName;
    }
    //

    void SetPinType(int nType)
    {
        m_nPinType = nType;
    }

    int GetPinType()
    {
        return m_nPinType;
    }

    void SetPinIndex(int nIndex)
    {
        m_nIndex = nIndex;
    }

    int GetPinIndex()
    {
        return m_nIndex;
    }

    void SetPinName(const QString &strName)
    {
        m_strPinName = strName;
    }

    void SetComment(const QString &strComment)
    {
        m_strComment = strComment;
    }

    const QString &GetComment()
    {
        return m_strComment;
    }

    std::shared_ptr<CInPutProperty> GetOtherModel()
    {
        return m_InProperty;
    }

    std::shared_ptr<CInPutProperty> CreateInModel();

    std::shared_ptr<CCellProperty> GetCellPro()
    {
        return m_CellModel;
    }

    std::shared_ptr<CCellProperty> CreateCellPro();

private:
    //��������    1:output   2:Ԫ��   3:��
    int m_nPinType;   
    //���ӱ��
    int m_nIndex;
    //��������
    QString m_strPinName;
    //�Զ������Ϣ
    std::shared_ptr<CInPutProperty>  m_InProperty;

    //ע��
    QString m_strComment;
    //Ԫ����Ϣ
    std::shared_ptr<CCellProperty>  m_CellModel;
};

//�߼���ģ��
class CBlockProperty
{
public:
    CBlockProperty();
    explicit CBlockProperty(int nCreateNum);
    ~CBlockProperty()
    {
        m_pinValues.clear();
    }
    //
    int GetCreateNum();
    //
    void SetBlockRect(const QRectF &bRect)
    {
        m_itemRect = bRect;
    }

    const QRectF & GetBlockRect()
    {
        return m_itemRect;
    }

    void SetEnoShowFlag(bool bFlag)
    {
        m_bEnoShowFlag = bFlag;
    }

    bool GetEnoShowFlag()
    {
        return m_bEnoShowFlag;
    }

    void SetComment(const QString &strComment)
    {
        m_strComment = strComment;
    }

    const QString & GetComment()
    {
        return m_strComment;
    }

    void SetExecNum(int nExeNum)
    {
        m_nExecNum = nExeNum;
    }

    int GetExecNum()
    {
        return m_nExecNum;
    }

    void SetItemType(int nType)
    {
        m_nItemType = nType;
    }

    int GetItemType()
    {
        return m_nItemType;
    }

    std::shared_ptr<CBinProperty> CreatePinProperty(const QString & szName);

    void CreatePinFromOther(std::vector<std::shared_ptr<CBinProperty>> pinValues)
    {
        m_pinValues = pinValues;
    }

    std::vector<std::shared_ptr<CBinProperty>> GetPinValues()
    {
        return m_pinValues;
    }

private:
    //�߼���λ��
    QRectF m_itemRect;
    //�Ƿ����eno
    bool m_bEnoShowFlag;
    //ע��
    QString m_strComment;
    //item�������
    int m_nCreateNum;
    //itemִ�б��
    int m_nExecNum;
    //item����
    int m_nItemType;

    std::vector<std::shared_ptr<CBinProperty>> m_pinValues;

};

//��ϵģ��
class CLineProperty
{
public:
    CLineProperty(){};
    ~CLineProperty()
    {
        m_lstAllPoints.clear();
    }

    void SetBeginItemIndex(int nBegin)
    {
        m_nBeginItemIndex = nBegin;
    }

    int GetBeginItemIndex()
    {
        return m_nBeginItemIndex;
    }

    void SetEndItemIndex(int nEnd)
    {
        m_nEndItemIndex = nEnd;
    }

    int GetEndItemIndex()
    {
        return m_nEndItemIndex;
    }

    void SetEndPinIndex(int nEndPin)
    {
        m_nEndPinIndex = nEndPin;
    }

    int GetEndPinIndex()
    {
        return m_nEndPinIndex;
    }

    void SetBeginPinIndex(int nBegPin)
    {
        m_nBeginIndex = nBegPin;
    }

    int GetBeginPinIndex()
    {
        return m_nBeginIndex;
    }

    std::vector<std::shared_ptr<QPointF>> GetAllPoints()
    {
        return m_lstAllPoints;
    }

    void SetPoints(double x,double y);
private:

    //��ʼitem�Ĵ������
    int m_nBeginItemIndex;
    //����item�Ĵ������
    int m_nEndItemIndex;
    //����item�Ķ��ӱ��
    int m_nEndPinIndex;           //ln?
    //��ʼitem�Ķ��ӱ��
    int m_nBeginIndex;         //out?
    //���еĵ㼯
    std::vector<std::shared_ptr<QPointF>> m_lstAllPoints;   //���е�ļ���

};

//ע��
class CCommentProperty
{
public:
    CCommentProperty(){};
    ~CCommentProperty(){};

    void SetCommentPos(const QPointF &pfComment)
    {
        m_pfPos = pfComment;
    }

    const QPointF &GetCommentPos()
    {
        return m_pfPos;
    }

    void SetComment(const QString &strComment)
    {
        m_strComment = strComment;
    }

    const QString &GetComment()
    {
        return m_strComment;
    }

    void SetRect(const QRectF &rect)
    {
        m_rfRect = rect;
    }

    const QRectF &GetRect()
    {
        return m_rfRect;
    }

private:
    QPointF m_pfPos;           //����λ��
    QString m_strComment;      //ע������
    QRectF  m_rfRect;          //rect
};

class  CLogicObjectModel
{
public:
    CLogicObjectModel();
    explicit CLogicObjectModel(const QString & szName);
    ~CLogicObjectModel()
    {
        ClearAllProperties();
    }

public:
    const QString & GetName()const
    {
        return m_szName;
    }

    void SetName(const QString & szName)
    {
        m_szName = szName;
    }


    //��ȡ��������
    std::vector<std::shared_ptr<CBlockProperty>> GetBlockProperty()
    {
        return m_arrBlockPropertise;
    }
    //��ȡ�߼�������
    std::vector<std::shared_ptr<CLineProperty>> GetLineProperty()
    {
        return m_arrLineProperties;
    }
    //��ȡע������
    std::vector<std::shared_ptr<CCommentProperty>> GetCommentProperty()
    {
        return m_arrCommentProperties;
    }

    //����block
    std::shared_ptr<CBlockProperty> CreateBlockProperty(int nCreateNum);
    //����line
    std::shared_ptr<CLineProperty> CreateLineProperty();
    //����ע��
    std::shared_ptr<CCommentProperty> CreateCommentProperty();
    //

    //clear
    void ClearAllProperties()
    {
        if (!m_arrBlockPropertise.empty())
        {
            m_arrBlockPropertise.clear();
        }

        if (!m_arrLineProperties.empty())
        {
            m_arrLineProperties.clear();
        }

        if (!m_arrCommentProperties.empty())
        {
            m_arrCommentProperties.clear();
        }
        
    }
private:
    //! ����
    std::vector<std::shared_ptr<CBlockProperty>> m_arrBlockPropertise;
    //! ֱ��
    std::vector<std::shared_ptr<CLineProperty>> m_arrLineProperties;
    //! ע��
    std::vector<std::shared_ptr<CCommentProperty>> m_arrCommentProperties;

    //! ������
    QString m_szName;
};


#endif // OBJECT_MODEL_H
