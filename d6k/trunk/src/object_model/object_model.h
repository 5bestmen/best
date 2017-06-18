
#ifndef OBJECT_MODEL_H
#define OBJECT_MODEL_H

#include <vector>
#include <memory>
#include <QString>

#include <QUuid>


class CObjectProperty
{
public:
	CObjectProperty()
	{

	}
	virtual ~CObjectProperty()
	{

	}
public:
	const QString & GetTagName()const
	{
		return m_szTagName;
	}

    void SetTagName(const QString &strTagName)
    {
        m_szTagName = strTagName;
    }

    const QString & GetDescription() const
    {
        return m_szDescription;
    }

    void SetDescription(const QString &strDesc)
    {
        m_szDescription = strDesc;
    }

private:
	QString m_szTagName;      // ����
	QString m_szDescription;  // ����
};

/*! \class  CObjectStateInfo
*   \brief  �����״̬��Ϣ
*/
class CObjectStateInfo
{
public:
	CObjectStateInfo()
	{

	}
	~CObjectStateInfo()
	{
		m_arrValues.clear();
	}
public:
	/*! \class  STATE_VALUE
	*   \brief  �����״ֵ̬��������
	*/
	struct STATE_VALUE
	{
		double  Value;
		QString Descrtiption;
	};



private:
	QString m_szTagName;      // ����
	QString m_szDescription;  // ����

	std::vector<std::shared_ptr<STATE_VALUE>> m_arrValues;

};

/*! \class  CObjectCtrl
*   \brief  �豸���������Ϣ
*/
class CObjectCtrlInfo
{
public:
	CObjectCtrlInfo();
	~CObjectCtrlInfo();
private:

private:
	QString m_szTagName;      // ����
	QString m_szDescription;  // �������������ʾ��ͼ�ν����ϵĿ������
};


class CStaticProperty : public CObjectProperty
{
public:
	CStaticProperty()
	{
	}

    explicit CStaticProperty(const QString & szName)
    {
        SetTagName(szName);
    };

    const QString &GetSzValue()
    {
        return m_szValue;
    }

    void SetSzValue(const QString &strSzValue)
    {
        m_szValue = strSzValue;
    }

	virtual ~CStaticProperty()
	{
	}

private:
	QString  m_szValue;
};
/*! \class  CDynamicProperty
*   \brief  ��̬���Զ���
*/
class CDynamicProperty : public CObjectProperty
{
public:
	CDynamicProperty()
	{

	}
    explicit CDynamicProperty(const QString & szName)
    {
        SetTagName(szName);
    };

	virtual ~CDynamicProperty()
	{
	}

    void SetReadOnlyFlag(bool bFalg)
    {
        m_bReadWrite = bFalg;
    }

    bool GetReadOnlyFlag()
    {
        return m_bReadWrite;
    }

    int GetDataType()
    {
        return m_nDataType;
    }

    void SetDataType(int nDatType)
    {
        m_nDataType = nDatType;
    }

public:

private:
	bool m_bReadWrite;          // ture:ֻ��
	int	m_nDataType;            // �������ͣ����Σ������͵�


};

/*! \class  CObjectModelInfo
*   \brief  �豸 ����ģ�Ͷ���
*/
class  CObjectModelInfo
{
public:
	CObjectModelInfo();
	explicit CObjectModelInfo(const QString & szName);
	~CObjectModelInfo();

public:
	const QString & GetName()const
	{
		return m_szName;
	}

	void SetName(const QString & szName)
	{
		m_szName = szName;
	}

	const QString & GetTagName()const
	{
		return m_szTagName;
	}

	void SetTagName(const QString & szName)
	{
		m_szTagName = szName;
	}

    void ClearDynamic()
    {
        m_arrDyncProperties.clear();
    }

    void CearStatic()
    {
        m_arrStaticProperties.clear();
    }

    QString GetGuid()
    {
        return m_ModelId.toString();
    }

    //��ȡ 
    std::vector<std::shared_ptr<CDynamicProperty>> GetDynamicProperty();
    //
    std::vector<std::shared_ptr<CStaticProperty>> GetStaticProperty();
	// ������̬����
	std::shared_ptr<CDynamicProperty> CreateDynamicProperty(const QString & szTagName);
	// ������̬����
	std::shared_ptr<CStaticProperty> CreateStaticProperty(const QString & szTagName);
    //����guid
    void SetModelId(const QString &strGuid);
private:
	//! ��̬����
	std::vector<std::shared_ptr<CDynamicProperty>> m_arrDyncProperties;
	//! ��̬����
	std::vector<std::shared_ptr<CStaticProperty>> m_arrStaticProperties;
	//! ���豸
	std::vector<std::shared_ptr<CObjectModelInfo>> m_arrSubObjects;

	//! ������Ϊ���豸ʱ������tagname
	QString m_szTagName;

	//! �豸����
	QString m_szName;
	//! ģ�͵�Ψһ��ʶ��
	QUuid  m_ModelId;
};
 

#endif // OBJECT_MODEL_H
