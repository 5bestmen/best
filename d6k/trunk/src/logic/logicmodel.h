#ifndef LOGICMODEL_H
#define LOGICMODEL_H
#include <QString>
#include <vector>
#include <memory>
#include <QRectF>
#include <QPointF>

class CBlockProperty;
//元件信息 
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
    //元件名称
    QString m_strName;
};

//输入端信息
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
    int m_nCreateNum;             //创建编号
    int m_nExecNum;               //执行编号
    int m_nOutPutIndex;           //输出编号
    QString m_strOutPutPinName;   //输出bin名称
};


//端子模块
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
    //端子类型    1:output   2:元件   3:空
    int m_nPinType;   
    //端子编号
    int m_nIndex;
    //端子名称
    QString m_strPinName;
    //对端输出信息
    std::shared_ptr<CInPutProperty>  m_InProperty;

    //注释
    QString m_strComment;
    //元件信息
    std::shared_ptr<CCellProperty>  m_CellModel;
};

//逻辑块模型
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
    //逻辑块位置
    QRectF m_itemRect;
    //是否包含eno
    bool m_bEnoShowFlag;
    //注释
    QString m_strComment;
    //item创建编号
    int m_nCreateNum;
    //item执行编号
    int m_nExecNum;
    //item类型
    int m_nItemType;

    std::vector<std::shared_ptr<CBinProperty>> m_pinValues;

};

//关系模型
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

    //起始item的创建编号
    int m_nBeginItemIndex;
    //结束item的创建编号
    int m_nEndItemIndex;
    //结束item的端子编号
    int m_nEndPinIndex;           //ln?
    //起始item的端子编号
    int m_nBeginIndex;         //out?
    //所有的点集
    std::vector<std::shared_ptr<QPointF>> m_lstAllPoints;   //所有点的集合

};

//注释
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
    QPointF m_pfPos;           //坐标位置
    QString m_strComment;      //注释内容
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


    //获取方块属性
    std::vector<std::shared_ptr<CBlockProperty>> GetBlockProperty()
    {
        return m_arrBlockPropertise;
    }
    //获取逻辑线属性
    std::vector<std::shared_ptr<CLineProperty>> GetLineProperty()
    {
        return m_arrLineProperties;
    }
    //获取注释属性
    std::vector<std::shared_ptr<CCommentProperty>> GetCommentProperty()
    {
        return m_arrCommentProperties;
    }

    //创建block
    std::shared_ptr<CBlockProperty> CreateBlockProperty(int nCreateNum);
    //创建line
    std::shared_ptr<CLineProperty> CreateLineProperty();
    //创建注释
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
    //! 方块
    std::vector<std::shared_ptr<CBlockProperty>> m_arrBlockPropertise;
    //! 直线
    std::vector<std::shared_ptr<CLineProperty>> m_arrLineProperties;
    //! 注释
    std::vector<std::shared_ptr<CCommentProperty>> m_arrCommentProperties;

    //! 段名称
    QString m_szName;
};


#endif // OBJECT_MODEL_H
