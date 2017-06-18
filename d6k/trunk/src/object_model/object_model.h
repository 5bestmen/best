
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
	QString m_szTagName;      // 名称
	QString m_szDescription;  // 描述
};

/*! \class  CObjectStateInfo
*   \brief  对象的状态信息
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
	*   \brief  对象的状态值及其描述
	*/
	struct STATE_VALUE
	{
		double  Value;
		QString Descrtiption;
	};



private:
	QString m_szTagName;      // 名称
	QString m_szDescription;  // 描述

	std::vector<std::shared_ptr<STATE_VALUE>> m_arrValues;

};

/*! \class  CObjectCtrl
*   \brief  设备控制输出信息
*/
class CObjectCtrlInfo
{
public:
	CObjectCtrlInfo();
	~CObjectCtrlInfo();
private:

private:
	QString m_szTagName;      // 名称
	QString m_szDescription;  // 描述，这个会显示在图形界面上的控制面板
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
*   \brief  动态属性定义
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
	bool m_bReadWrite;          // ture:只读
	int	m_nDataType;            // 数据类型，整形，浮点型等


};

/*! \class  CObjectModelInfo
*   \brief  设备 对象模型定义
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

    //获取 
    std::vector<std::shared_ptr<CDynamicProperty>> GetDynamicProperty();
    //
    std::vector<std::shared_ptr<CStaticProperty>> GetStaticProperty();
	// 创建动态属性
	std::shared_ptr<CDynamicProperty> CreateDynamicProperty(const QString & szTagName);
	// 创建静态属性
	std::shared_ptr<CStaticProperty> CreateStaticProperty(const QString & szTagName);
    //设置guid
    void SetModelId(const QString &strGuid);
private:
	//! 动态属性
	std::vector<std::shared_ptr<CDynamicProperty>> m_arrDyncProperties;
	//! 静态属性
	std::vector<std::shared_ptr<CStaticProperty>> m_arrStaticProperties;
	//! 子设备
	std::vector<std::shared_ptr<CObjectModelInfo>> m_arrSubObjects;

	//! 仅在作为子设备时，才用tagname
	QString m_szTagName;

	//! 设备名称
	QString m_szName;
	//! 模型的唯一标识符
	QUuid  m_ModelId;
};
 

#endif // OBJECT_MODEL_H
