#ifndef LOGICCONTAINTER_H
#define LOGICCONTAINTER_H
#include <vector>
#include <memory>
#include <QString>

class CLogicObjectModel;


class CTaskContainer
{
public:
    CTaskContainer(){};
    ~CTaskContainer()
    {
        m_segModels.clear();
        m_srModels.clear();
    }

    void SetCycleType(int nType)
    {
        m_CycleType = nType;
    }

    int GetCycleType()
    {
        return m_CycleType;
    }

    void SetMsec(int nMsec)
    {
        m_nMsec = nMsec;
    }

    int GetMsec()
    {
        return m_nMsec;
    }

    void SetComment(const QString &strComment)
    {
        m_strComment = strComment;
    }

    const QString &GetComment()
    {
        return m_strComment;
    }


    // ����seg��Ŀ
    std::shared_ptr<CLogicObjectModel> CreateSegModel(const QString & szName);
    // ɾ��seg��Ŀ
    bool RemoveSegModel(const QString & szName);
    //��ȡseg����
    std::vector<std::shared_ptr<CLogicObjectModel>> GetSegModels();

    // ����sr��Ŀ
    std::shared_ptr<CLogicObjectModel> CreateSrModel(const QString & szName);
    // ɾ��sr��Ŀ
    bool RemoveSrModel(const QString & szName);
    //��ȡsr����
    std::vector<std::shared_ptr<CLogicObjectModel>> GetSrModels();


private:
    //seg
    std::vector<std::shared_ptr<CLogicObjectModel>> m_segModels;
    //sr
    std::vector<std::shared_ptr<CLogicObjectModel>> m_srModels;
    // 1:period     2:cycle
    int m_CycleType;
    //
    int m_nMsec;
    //
    QString m_strComment;
};

class CNodeLogicContainer
{
public:
    CNodeLogicContainer();
    explicit CNodeLogicContainer(const QString & szName);

    ~CNodeLogicContainer();

    const QString & GetName()
    {
        return m_strTagname;
    }

    void SetName(const QString &strName)
    {
        m_strTagname = strName;
    }
    //����mast
    std::shared_ptr<CTaskContainer> CreateMastContainer();
    //����fast
    std::shared_ptr<CTaskContainer> CreateFastContainer();
    //����aux
    std::shared_ptr<CTaskContainer> CreateAuxContainer();

    std::shared_ptr<CTaskContainer> GetMastContainer()
    {
        return m_mastModels;
    }

    std::shared_ptr<CTaskContainer> GetFastContainer()
    {
        return m_fastModels;
    }

    std::shared_ptr<CTaskContainer> GetAuxContainer()
    {
        return m_auxModels;
    }


private:
    //tagname
    QString m_strTagname;
    //mast
    std::shared_ptr<CTaskContainer> m_mastModels;
    //fast
    std::shared_ptr<CTaskContainer> m_fastModels;
    //aux
    std::shared_ptr<CTaskContainer> m_auxModels;
};

class CLogicContainter
{
public:
    CLogicContainter();
    ~CLogicContainter()
    {
        m_NodeContainers.clear();
    }
    void SetName(const QString &strName)
    {
        m_strName = strName;
    }

    const QString& GetName()
    {
        return m_strName;
    }

    void SetProtectFlag(bool bFlag)
    {
        m_bProtectedFlag = bFlag;
    }

    bool GetProtectFlag()
    {
        return m_bProtectedFlag;
    }

    void SetPassword(const QString &strPasswd)
    {
        m_strPasswd = strPasswd;
    }

    const QString &GetPassword()
    {
        return m_strPasswd;
    }

    void SetComment(const QString &strComment)
    {
        m_strComment = strComment;
    }

    const QString &GetComment()
    {
        return m_strComment;
    }


    // �����ڵ�
    std::shared_ptr<CNodeLogicContainer> CreateNodeContainer(const QString & szName);
    // ɾ���ڵ�
    bool RemoveNodeContainer(const QString & szName);
    //��ȡ����
    std::vector<std::shared_ptr<CNodeLogicContainer>> GetNodeContainers();

private:
    //name
    QString m_strName;
    //�Ƿ񱣻�
    bool m_bProtectedFlag;
    //����   ��ʱ������TODO
    QString m_strPasswd;
    //ע��
    QString m_strComment;
    //task
    std::vector<std::shared_ptr<CNodeLogicContainer>> m_NodeContainers;
    
};

#endif // LOGICCONTAINTER_H
