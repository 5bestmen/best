

#include "object_model.h"

#include <QDebug>

 
CObjectModelInfo::CObjectModelInfo()
{

}

CObjectModelInfo::~CObjectModelInfo()
{

}

CObjectModelInfo::CObjectModelInfo(const QString & szName) :m_szName(szName)
{
	Q_ASSERT(szName.isEmpty() == false);
	if (szName.isEmpty() == false)
	{
		m_ModelId = QUuid::createUuid();
	}

}

std::shared_ptr<CDynamicProperty> CObjectModelInfo::CreateDynamicProperty(const QString & szTagName)
{
	Q_ASSERT(szTagName.isEmpty() == false);
	if (szTagName.isEmpty() == true)
	{
		return nullptr;
	}
	// 确定没有重名
	for (auto it : m_arrDyncProperties)
	{
		if (szTagName.compare(it->GetTagName()) == 0)
		{
			Q_ASSERT(false);
			return nullptr;
		}
	}
	for (auto it : m_arrStaticProperties)
	{
		if (szTagName.compare(it->GetTagName()) == 0)
		{
			Q_ASSERT(false);
			return nullptr;
		}
	}
	std::shared_ptr<CDynamicProperty> pProp = std::make_shared<CDynamicProperty>(szTagName);
	m_arrDyncProperties.emplace_back(pProp);

	return pProp;
}

std::shared_ptr<CStaticProperty> CObjectModelInfo::CreateStaticProperty(const QString & szTagName)
{
    Q_ASSERT(szTagName.isEmpty() == false);
    if (szTagName.isEmpty() == true)
    {
        return nullptr;
    }
    // 确定没有重名
    for (auto it : m_arrStaticProperties)
    {
        if (szTagName.compare(it->GetTagName()) == 0)
        {
            Q_ASSERT(false);
            return nullptr;
        }
    }
    for (auto it : m_arrStaticProperties)
    {
        if (szTagName.compare(it->GetTagName()) == 0)
        {
            Q_ASSERT(false);
            return nullptr;
        }
    }
    std::shared_ptr<CStaticProperty> pProp = std::make_shared<CStaticProperty>(szTagName);
    m_arrStaticProperties.emplace_back(pProp);

    return pProp;
}


void CObjectModelInfo::SetModelId(const QString &strGuid)
{
    m_ModelId = strGuid;
}

std::vector<std::shared_ptr<CDynamicProperty>> CObjectModelInfo::GetDynamicProperty()
{
    return m_arrDyncProperties;
}

std::vector<std::shared_ptr<CStaticProperty>> CObjectModelInfo::GetStaticProperty()
{
    return m_arrStaticProperties;
}
