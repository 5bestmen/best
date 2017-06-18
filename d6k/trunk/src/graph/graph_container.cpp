/*! @file graph_container.cpp
<PRE>
********************************************************************************
模块名       :
文件名       : graph_container.cpp
文件实现功能 : 图形容器
作者         :
版本         :
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   图形容器.
*  @author  LiJin
*  @version 1.0
*  @date    2016.02.26
*******************************************************************************/

#include "graph_container.h"
#include "graph_folder.h"
#include "graph_file.h"
#include "stl_util-inl.h"

#include <sstream>
#include <QtGlobal>
#include <QDomElement> 
#include <QDomDocument> 


CGraphContainer::CGraphContainer()
{

}

CGraphContainer::~CGraphContainer()
{
	Clear();
}

void CGraphContainer::Clear()
{
	STLDeleteElements(&m_arrGraphBaseData);
}

CGraphFile* CGraphContainer::CreateGraph(const std::string& szName)
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

/*! \fn std::string  CGraphContainer::RecommendFileName( )
*********************************************************************************************************
** \brief 给新画面推荐一个名称
** \details
** \return std::string 
** \author LiJin
** \date 2010年9月4日
** \note
********************************************************************************************************/
std::string CGraphContainer::RecommendFileName()
{
	std::string  szNewName =  "";

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
		szNewName = QObject::tr("Graph_file").toLocal8Bit().data() + QString::number(i).toStdString();	//	szNewName.Format(_T("画面%d"), i);
		if (IsFileNameValid(szNewName))
		{
			return szNewName;
		}
	}

	Q_ASSERT(false);
	return ("");
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
std::string CGraphContainer::RecommendFolderName()
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

	std::stringstream strTemp;

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
/*! \fn bool CGraphContainer::IsFileNameValid(const std::string &szName)
*********************************************************************************************************
** \brief 验证文件名是否合法
** \details 需要再对字符串进行分析，看是否有不允许的字符出现
** \param szName
** \return bool
** \author LiJin
** \date 2010年9月6日
** \note
********************************************************************************************************/
bool CGraphContainer::IsFileNameValid(const std::string& szName)
{
	if (szName.empty() || szName.length() >= MAX_NAME_LEN)
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
/*! \fn bool CGraphContainer::IsFileFolderNameValid(const std::string &szName)
*********************************************************************************************************
** \brief 验证文件 夹 名是否合法
** \details 需要再对字符串进行分析，看是否有不允许的字符出现
** \param szName
** \return bool
** \author LiJin
** \date 2010年9月6日
** \note
********************************************************************************************************/
bool CGraphContainer::IsFileFolderNameValid(const std::string& szName)
{
	if (szName.empty() || szName.length() >= MAX_NAME_LEN)
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
/*! \fn  bool CGraphContainer::IsFileNameValid(CGraphFile* pGraph,const std::string& szNewName)
*********************************************************************************************************
** \brief   CGraphContainer::IsFileNameValid
** \details 验证指定的图形新文件名是否合法
** \param   pGraph
** \param   szNewName
** \return bool
** \author LiJin
** \date  2011年1月7日
** \note
********************************************************************************************************/
bool CGraphContainer::IsFileNameValid(CGraphFile* pGraph, const  std::string& szNewName)
{
	Q_ASSERT(pGraph);
	if (pGraph == nullptr)
		return false;

	if (szNewName.empty() || szNewName.length() >= MAX_NAME_LEN)
		return false;

	CGraphFolder *pFolder = pGraph->GetFolder();
	if (pFolder == nullptr)
	{
		auto iter = m_arrGraphBaseData.begin();
		iter == std::find(m_arrGraphBaseData.begin(), m_arrGraphBaseData.end(), pGraph);
		Q_ASSERT(iter != m_arrGraphBaseData.end());

		for (iter = m_arrGraphBaseData.begin(); iter != m_arrGraphBaseData.end(); ++iter)
		{			 
// 			if (*iter && (*iter != pPic))
// 			{// 重复
// 				if ((*iter)->m_szGraphName.Compare(szNewName) == 0)
// 				{
// 					return false;
// 				}
// 			}
		}
		return true;
	}
	else
	{
// 		auto iter = pFolder->m_arrPics.begin();
// 		iter == std::find(pFolder->m_arrPics.begin(), pFolder->m_arrPics.end(), pPic);
// 		ASSERT(iter != pFolder->m_arrPics.end());
// 
// 		for (iter = pFolder->m_arrPics.begin(); iter != pFolder->m_arrPics.end(); ++iter)
// 		{
// 			ASSERT(*iter);
// 			if (*iter && (*iter != pPic))
// 			{// 重复
// 				if ((*iter)->m_szGraphName.Compare(szNewName) == 0)
// 				{
// 					return false;
// 				}
// 			}
// 		}
		return true;
	}
}

/*! \fn CGraphFolder* CGraphContainer::CreateGraphFolder(const std::string& szName)
********************************************************************************************************* 
** \brief CGraphContainer::CreateGraphFolder 
** \details 新建一个空的文件夹
** \param szName 
** \return CGraphFolder* 
** \author LiJin 
** \date 2016年8月27日 
** \note 
********************************************************************************************************/
CGraphBaseData* CGraphContainer::CreateGraphFolder(const std::string& szName)
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

/*! \fn bool CGraphContainer::SaveFiles(const QString & szRootPath) const
********************************************************************************************************* 
** \brief CGraphContainer::SaveFiles 
** \details 保存所有的图形文件
** \param szRootPath 
** \return bool 
** \author LiJin 
** \date 2016年4月5日 
** \note 
********************************************************************************************************/
bool CGraphContainer::SaveFiles(const QString & szRootPath) 
{
	Q_UNUSED(szRootPath);
// 	bool bRet = false;
// 
// 	for (auto it_file : m_arrGraphs)
// 	{
// 		if (it_file->GetFlashFlag())
// 		{
// 			bRet |= it_file->SaveFile(szRootPath);
// 		}
// 	}
// 
// 	for (auto it_file : m_arrGraphFolders)
// 	{
// 
// 		bRet |= it_file->SaveFiles(szRootPath);
// 	}

// 	return bRet;

	return true;
}



/*! \fn bool CGraphContainer::SaveProject(QDomDocument *pXml, QDomElement *pParent, const QString & szRoot)
********************************************************************************************************* 
** \brief CGraphContainer::SaveProject 
** \details 保存图形文件
** \param pXml 
** \param pParent 
** \param szRoot 
** \return bool 
** \author LiJin 
** \date 2016年8月19日 
** \note 
********************************************************************************************************/
bool CGraphContainer::SaveProject(QDomDocument *pXml, QDomElement *pParent, const QString & szRoot)
{
	Q_ASSERT(pXml && pParent);
	if (pXml == nullptr || pParent == nullptr)
		return false;

	QDomElement eleGrp,eleFile;
	
//	 pXml->createElement("graph");
//	pRoot->appendChild(root);
    
	// 先处理 proj 工程文件（graph 部分） 
// 	for each (auto it in m_arrGraphs)
// 	{
// 		Q_ASSERT(it);
// 		if (it != nullptr)
// 		{
// 			eleFile = pXml->createElement("file");
// 			pParent->appendChild(eleFile);
// 			eleFile.setAttribute("name", QString::fromLocal8Bit(it->GetGraphName().c_str()));
// 		}
// 	}
// 	// 接着处理 proj 工程文件（group部分）
// 	for each (auto it in m_arrGraphFolders)
// 	{
// 		Q_ASSERT(it); 
// 		if (it != nullptr)
// 		{
// 			eleGrp = pXml->createElement("group");
// 			pParent->appendChild(eleGrp);
// 			eleGrp.setAttribute("name", QString::fromLocal8Bit(it->GetFolderName().c_str()));
// 			eleGrp.setAttribute("count", it->GetGraphCount());
// 		}
// 	}
	//

	for each (CGraphBaseData* var in m_arrGraphBaseData)
	{
		Q_ASSERT(var);
		if (var != nullptr)
		{
			if (var->GetItemType() ==  CGraphBaseData::BASE_TYPE_FILE)
			{
				QDomElement eleFile = pXml->createElement("file");
				pParent->appendChild(eleFile);
				eleFile.setAttribute("name", QString::fromLocal8Bit(var->GetItemName().c_str()));

				CGraphFile *pGraphFile = dynamic_cast<CGraphFile*>(var);
				Q_ASSERT(pGraphFile);
				if (pGraphFile != nullptr && pGraphFile->GetFlashFlag())
				{
					pGraphFile->SaveFile("");
				}
			}
			else if (var->GetItemType() == CGraphBaseData::BASE_TYPE_FOLDER)
			{
				QDomElement eleGrp = pXml->createElement("group");
				pParent->appendChild(eleGrp);
				eleGrp.setAttribute("name", QString::fromLocal8Bit(var->GetItemName().c_str()));
				SaveFolderPrj(var,&eleGrp,pXml);
			}
		}
	}


	QString szRootPah = szRoot + "//" + "graph";
	// 保存文件
	SaveFiles(szRootPah);


	return true;

}



bool CGraphContainer::OpenFiles(const QString & szRootPath, CGraphFile *pGraphFile)
{
	pGraphFile->LoadFile(szRootPath);
	return true;
}

void CGraphContainer::SaveFolderPrj(CGraphBaseData *pFolder, QDomElement *pDomFolder, QDomDocument *pXml)
{
	CGraphFolder *pGraphFile = dynamic_cast<CGraphFolder*>(pFolder);
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
	{
		return;
	}

	for each (CGraphBaseData* var in pGraphFile->GetGraphData())
	{
		if (var->GetItemType() == CGraphBaseData::BASE_TYPE_FILE)
		{
			//文件
			QDomElement eleFile = pXml->createElement("file");
			pDomFolder->appendChild(eleFile);
			eleFile.setAttribute("name", QString::fromLocal8Bit(var->GetItemName().c_str()));

			CGraphFile *pGraphFile = dynamic_cast<CGraphFile*>(var);
			Q_ASSERT(pGraphFile);
			if (pGraphFile != nullptr && pGraphFile->GetFlashFlag())
			{
				pGraphFile->SaveFile("");
			}

		}
		else if (var->GetItemType() == CGraphBaseData::BASE_TYPE_FOLDER)
		{
			//文件夹
			QDomElement eleGrp = pXml->createElement("group");
			pDomFolder->appendChild(eleGrp);
			eleGrp.setAttribute("name", QString::fromLocal8Bit(var->GetItemName().c_str()));
			SaveFolderPrj(var, &eleGrp, pXml);

		}
	}




}

void CGraphContainer::DeleteBaseData(CGraphBaseData* pBaseData)
{
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
