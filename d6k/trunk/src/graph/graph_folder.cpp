/*! @file graph_folder.h
<PRE>
********************************************************************************
模块名       :
文件名       : graph_folder.h
文件实现功能 : 图形文件
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基本定义
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/
#include "graph_folder.h"
#include "graph_file.h"
#include "graph_container.h"
#include <sstream>
#include "stl_util-inl.h"


CGraphFolder::CGraphFolder()
{

}

CGraphFolder::~CGraphFolder()
{
	//	FreeMem();
	STLDeleteElements(&m_arrGraphBaseData);
}

CGraphFile* CGraphFolder::CreateGraph(const std::string& szName)
{
	Q_ASSERT(szName.empty() == false);
	if (szName.empty() == true)
		return nullptr;

	// 确定没有重名
	for (auto it : m_arrGraphBaseData)
	{
		if (szName.compare(it->GetItemName()) == 0)
		{
			Q_ASSERT(false);
			return nullptr;
		}
	}

	auto pGraphFile = new CGraphFile();
	pGraphFile->SetGraphName(szName);
	//创建对象标注类型  显示名称
	pGraphFile->SetItemType(CGraphBaseData::BASE_TYPE_FILE);
	pGraphFile->SetItemName(szName);

	m_arrGraphBaseData.push_back(pGraphFile);

	return pGraphFile;
}


CGraphBaseData* CGraphFolder::CreateGraphFolder(const std::string& szName)
{
	///////////////////
	Q_ASSERT(szName.empty() == false);
	if (szName.empty() == true)
		return nullptr;

	// 确定没有重名
	for (auto it : m_arrGraphBaseData)
	{
		if (szName.compare(it->GetItemName()) == 0)
		{
			Q_ASSERT(false);
			return nullptr;
		}
	}

	auto pGraphFile = new CGraphFolder();
	pGraphFile->SetFolderName(szName);
	//创建对象标注类型  显示名称
	pGraphFile->SetItemType(CGraphBaseData::BASE_TYPE_FOLDER);
	pGraphFile->SetItemName(szName);

	m_arrGraphBaseData.push_back(pGraphFile);

	return pGraphFile;
	//////////////////
}

std::string CGraphFolder::RecommendFileName()
{
	std::string  szNewName = QObject::tr("Graph_file").toLocal8Bit().data();

	size_t i = 0;
	for (auto it : m_arrGraphBaseData)
	{
		if (it->GetItemType() == CGraphBaseData::BASE_TYPE_FILE)
		{
			i++;
		}

	}

	i = i + 1;

	std::stringstream strTemp;

	for (; i <= CGraphContainer::MAX_GRAPH_CNT; i++)
	{
		strTemp << szNewName << i;
		szNewName = strTemp.str();
		//	szNewName.Format(_T("画面%d"), i);
		if (IsFileNameValid(szNewName))
		{
			return szNewName;
		}
	}

	Q_ASSERT(false);
	return ("");
}

bool CGraphFolder::IsFileNameValid(const std::string& szName)
{
	if (szName.empty() || szName.length() >= 128)
		return false;


	for (auto it : m_arrGraphBaseData)
	{
		if (it->GetItemType() == CGraphBaseData::BASE_TYPE_FILE)
		{
			if (it->GetItemName().compare(szName) == 0)
			{
				return false;
			}
		}

	}

	return true;
}

/*! \fn std::string CGraphContainer::RecommendFolderName()
*********************************************************************************************************
** \brief CGraphContainer::RecommendFolderName
** \details 给新组推荐一个名称
** \return std::string
** \author LiJin
** \date 2016年8月27日
** \note
********************************************************************************************************/
std::string CGraphFolder::RecommendFolderName()
{
	std::string  szNewName = "";

	size_t i = 0;
	for (auto it : m_arrGraphBaseData)
	{
		if (it->GetItemType() == CGraphBaseData::BASE_TYPE_FOLDER)
		{
			i++;
		}

	}

	i = i + 1;


	for (; i <= CGraphContainer::MAX_GRAPH_GROUP_CNT; i++)
	{
		szNewName = QObject::tr("Group").toLocal8Bit().data() + QString::number(i).toStdString();
		//	szNewName.Format(_T("画面%d"), i);
		if (IsFileFolderNameValid(szNewName))
		{
			return szNewName;
		}
	}
	Q_ASSERT(false);
	return ("");
}

bool CGraphFolder::IsFileFolderNameValid(const std::string& szName)
{
	if (szName.empty() || szName.length() >= 128)
		return false;

	for (auto it : m_arrGraphBaseData)
	{
		// 重复
		if (it->GetItemType() == CGraphBaseData::BASE_TYPE_FOLDER)
		{
			if (it->GetItemName().compare(szName) == 0)
			{
				return false;
			}
		}
	}
	return true;
}

void CGraphFolder::DeleteBaseData(CGraphBaseData* pBaseData)
{
	Q_ASSERT(pBaseData);
	if (pBaseData == nullptr)
	{
		return;
	}
	for (auto it = m_arrGraphBaseData.begin(); it != m_arrGraphBaseData.end();)
	{
		if ((*it) == pBaseData)
		{
			m_arrGraphBaseData.erase(it);
			delete pBaseData;
			return;
		}
		else
		{
			++it;
		}
	}
}

/*! \fn bool CGraphFolder::SaveFiles(const QString & szRootPath) const
********************************************************************************************************* 
** \brief CGraphFolder::SaveFiles 
** \details 保存目录下面的文件
** \param szRootPath 
** \return bool 
** \author LiJin 
** \date 2016年4月5日 
** \note 
********************************************************************************************************/
bool CGraphFolder::SaveFiles(const QString & szRootPath) const
{
	bool bRet = false;
	Q_ASSERT(m_szGraphFolderName.empty() == false);
	if (m_szGraphFolderName.empty() == true)
	{
		return false;
	}

	QString szFolderPath = szRootPath + "//";
	szFolderPath.append(m_szGraphFolderName.c_str());

	for (auto it_file : m_arrGraphs)
	{
		bRet |= it_file->SaveFile(szFolderPath);
	}

	return bRet;
}


/** @}*/

