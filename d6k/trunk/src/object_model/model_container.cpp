

#include "model_container.h"
#include "object_model.h"

CModelGroup::CModelGroup()
{

}

CModelGroup::~CModelGroup()
{
	m_arrModels.clear();
}

CModelGroup::CModelGroup(const QString & szName) :m_szName(szName)
{
	Q_ASSERT(szName.isEmpty() == false);
}
/*! \fn bool CModelGroup::RemoveObjectModel(const QString & szName)
********************************************************************************************************* 
** \brief CModelGroup::RemoveObjectModel 
** \details 按名字删除指定对象模型
** \param szName 
** \return bool 
** \author LiJin 
** \date 2016年12月9日 
** \note 
********************************************************************************************************/
bool CModelGroup::RemoveObjectModel(const QString & szName)
{
	Q_ASSERT(szName.isEmpty() == false);
	if (szName.isEmpty() == true)
	{
		return false;
	}
	// 按名字查找
	for (auto it = m_arrModels.begin(); it != m_arrModels.end();)
	{
		if (szName.compare((*it)->GetName()) == 0)
		{
			it = m_arrModels.erase(it);
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
/*! \fn bool CModelGroup::RenameModel(const QString & szOldName, const QString & szNewName)
********************************************************************************************************* 
** \brief CModelGroup::RenameModel 
** \details 给模型重新命名
** \param szOldName 
** \param szNewName 
** \return bool 
** \author LiJin 
** \date 2016年12月9日 
** \note 
********************************************************************************************************/
bool CModelGroup::RenameModel(const QString & szOldName, const QString & szNewName)
{
	Q_ASSERT(szOldName.isEmpty() == false);
	if (szOldName.isEmpty() == true)
	{
		return false;
	}

	Q_ASSERT(szNewName.isEmpty() == false);
	if (szNewName.isEmpty() == true)
	{
		return false;
	}

	Q_ASSERT(szOldName != szNewName);
	if (szOldName == szNewName)
		return false;

	// 按名字查找
	for (auto it = m_arrModels.begin(); it != m_arrModels.end();)
	{
		if (szNewName.compare((*it)->GetName()) == 0)
		{// 新的名字与系统中现有的有重复

			return false;
		}		 
	}

	for (auto it = m_arrModels.begin(); it != m_arrModels.end();)
	{
		if (szOldName.compare((*it)->GetName()) == 0)
		{//  
			(*it)->SetName(szNewName);
			return true;
		}
	}

	Q_ASSERT(false);
	return false;	
}


/*! \fn std::shared_ptr<CObjectModelInfo> CModelGroup::CreateObjectModel(const QString & szName)
*********************************************************************************************************
** \brief CModelGroup::CreateObjectModel
** \details 创建对象模型
** \param szName
** \return std::shared_ptr<CObjectModelInfo>
** \author LiJin
** \date 2016年12月9日
** \note
********************************************************************************************************/
std::shared_ptr<CObjectModelInfo> CModelGroup::CreateObjectModel(const QString & szName)
{
	Q_ASSERT(szName.isEmpty() == false);
	if (szName.isEmpty() == true)
	{
		return nullptr;
	}
	// 确定没有重名
	for (auto it : m_arrModels)
	{
		if (szName.compare(it->GetName()) == 0)
		{
			Q_ASSERT(false);
			return nullptr;
		}
	}

	std::shared_ptr<CObjectModelInfo> pObject = std::make_shared<CObjectModelInfo>(szName);
	m_arrModels.emplace_back(pObject);

	return pObject;
}

//获取模型
std::vector<std::shared_ptr<CObjectModelInfo>> CModelGroup::GetModels()
{
    return m_arrModels;
}


CModelScene::CModelScene()
{


}

CModelScene::CModelScene(const QString & szName) :m_szName(szName)
{
	Q_ASSERT(szName.isEmpty() == false);
}

CModelScene::~CModelScene()
{
	m_arrModelGrps.clear();
	m_arrModels.clear();
}
/*! \fn std::shared_ptr<CObjectModelInfo> CModelScene::CreateObjectModel(const QString & szName)
********************************************************************************************************* 
** \brief CModelScene::CreateObjectModel 
** \details 创建对象模型
** \param szName 
** \return std::shared_ptr<CObjectModelInfo> 
** \author LiJin 
** \date 2016年12月9日 
** \note 
********************************************************************************************************/
std::shared_ptr<CObjectModelInfo> CModelScene::CreateObjectModel(const QString & szName)
{
	Q_ASSERT(szName.isEmpty() == false);
	if (szName.isEmpty() == true)
	{
		return nullptr;
	}
	// 确定没有重名
	for (auto it : m_arrModelGrps)
	{
		if (szName.compare(it->GetName()) == 0)
		{
			Q_ASSERT(false);
			return nullptr;
		}
	}
	// 和模型名称也不要重合
	for (auto it : m_arrModels)
	{
		if (szName.compare(it->GetName()) == 0)
		{
			Q_ASSERT(false);
			return nullptr;
		}
	}

	std::shared_ptr<CObjectModelInfo> pObject = std::make_shared<CObjectModelInfo>(szName);
	m_arrModels.emplace_back(pObject);

	return pObject;
}
/*! \fn std::shared_ptr<CModelGroup> CModelScene::CreateModelGroup(const QString & szName)
********************************************************************************************************* 
** \brief CModelScene::CreateModelGroup 
** \details 创建一个模型组
** \param szName 
** \return std::shared_ptr<CModelGroup> 
** \author LiJin 
** \date 2016年12月9日 
** \note 
********************************************************************************************************/
std::shared_ptr<CModelGroup> CModelScene::CreateModelGroup(const QString & szName)
{
	Q_ASSERT(szName.isEmpty() == false);
	if (szName.isEmpty() == true)
	{
		return nullptr;
	}
	// 确定没有重名
	for (auto it : m_arrModelGrps)
	{
		if (szName.compare(it->GetName()) == 0)
		{
			Q_ASSERT(false);
			return nullptr;
		}
	}
	// 和模型名称也不要重合
	for (auto it : m_arrModels)
	{
		if (szName.compare(it->GetName()) == 0)
		{
			Q_ASSERT(false);
			return nullptr;
		}
	}

	std::shared_ptr<CModelGroup> pGrp = std::make_shared<CModelGroup>(szName);
	m_arrModelGrps.emplace_back(pGrp);

	return pGrp;
}

//获取模型
std::vector<std::shared_ptr<CObjectModelInfo>> CModelScene::GetModels()
{
    return m_arrModels;
}
//获取组
std::vector<std::shared_ptr<CModelGroup>> CModelScene::GetModelGrps()
{
    return m_arrModelGrps;
}


/*! \fn bool  CModelScene::RemoveModelGroup(const QString & szName)
********************************************************************************************************* 
** \brief CModelScene::RemoveModelGroup 
** \details 删除指定组
** \param szName 
** \return bool 
** \author LiJin 
** \date 2016年12月9日 
** \note 
********************************************************************************************************/
bool  CModelScene::RemoveModelGroup(const QString & szName)
{
	Q_ASSERT(szName.isEmpty() == false);
	if (szName.isEmpty() == true)
	{
		return false;
	}
	// 按名字查找
	for (auto it = m_arrModelGrps.begin(); it != m_arrModelGrps.end();)
	{
		if (szName.compare((*it)->GetName()) == 0)
		{
			it = m_arrModelGrps.erase(it);
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
/*! \fn bool  CModelScene::RemoveObjectModel(const QString & szName)
********************************************************************************************************* 
** \brief CModelScene::RemoveObjectModel 
** \details 删除指定对象
** \param szName 
** \return bool 
** \author LiJin 
** \date 2016年12月9日 
** \note 
********************************************************************************************************/
bool  CModelScene::RemoveObjectModel(const QString & szName)
{
	Q_ASSERT(szName.isEmpty() == false);
	if (szName.isEmpty() == true)
	{
		return false;
	}
	// 按名字查找
	for (auto it = m_arrModels.begin(); it != m_arrModels.end();)
	{
		if (szName.compare((*it)->GetName()) == 0)
		{
			it = m_arrModels.erase(it);
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

CModelContainer::CModelContainer()
{

}

CModelContainer::~CModelContainer()
{
	m_arrScenes.clear();
}

/*! \fn std::shared_ptr<CModelScene> CModelContainer::CreateModelScene(const QString & szName)
********************************************************************************************************* 
** \brief CModelContainer::CreateModelScene 
** \details 创建一个业务场景
** \param szName 
** \return std::shared_ptr<CModelScene> 
** \author LiJin 
** \date 2016年12月9日 
** \note 
********************************************************************************************************/
std::shared_ptr<CModelScene> CModelContainer::CreateModelScene(const QString & szName)
{
	Q_ASSERT(szName.isEmpty() == false);
	if (szName.isEmpty() == true)
	{
		return nullptr;
	}
	// 确定没有重名
	for (auto it : m_arrScenes)
	{
		if (szName.compare(it->GetName()) == 0)
		{
			Q_ASSERT(false);
			return nullptr;
		}
	}

	std::shared_ptr<CModelScene> pScene = std::make_shared<CModelScene>(szName);	 
	m_arrScenes.emplace_back(pScene);

	return pScene;
}
/*! \fn bool  CModelContainer::RemoveModelScene(const QString & szName)
********************************************************************************************************* 
** \brief CModelContainer::RemoveModelScene 
** \details 删除一个业务场景
** \param szName 
** \return bool 
** \author LiJin 
** \date 2016年12月9日 
** \note 
********************************************************************************************************/
bool  CModelContainer::RemoveModelScene(const QString & szName)
{
	Q_ASSERT(szName.isEmpty() == false);
	if (szName.isEmpty() == true)
	{
		return false;
	}
	// 按名字查找
 	for (auto it = m_arrScenes.begin(); it != m_arrScenes.end();)
	{
		if (szName.compare((*it)->GetName()) == 0)
		{
			it=m_arrScenes.erase(it);
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


//获取数据
std::vector<std::shared_ptr<CModelScene>> CModelContainer::GetModelScene()
{
    return m_arrScenes;
}
