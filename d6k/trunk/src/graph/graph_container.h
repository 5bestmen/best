/*! @file graph_container.h
<PRE>
********************************************************************************
模块名       :
文件名       :  graph_container.h
文件实现功能 :  图形容器
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   图形容器
*  @author  LiJin
*  @version 1.0
*  @date    2016.02.25
*******************************************************************************/
#ifndef GRAPH_CONTAINER_H
#define GRAPH_CONTAINER_H

#include <vector>
#include <memory>
#include <QString>

class QDomDocument;
class QDomElement;

class CGraphFile;
class CGraphFolder;
class CGraphBaseData;

class CGraphContainer
{
public:
	CGraphContainer();
	~CGraphContainer();
public:
	enum 
	{
		MAX_NAME_LEN = 128,
		MAX_GRAPH_CNT = 2028,
		MAX_GRAPH_GROUP_CNT = 2028,
	};
public:
	//  检查名称是否符合点名规则 检验文件名中是否包含有非法字符
	static bool CheckName(const std::string &szName, bool bWarning = false);

	//  新建图形文件
	CGraphFile* CreateGraph(const std::string& szName);
	//  添加一个图形
	bool AddGraph(CGraphFile* pGraph);
	// 删除指定的图形文件
	void DeleteGraph(CGraphFile *pPic);
	// 删除一个文件
	bool  DeleteGraph(const std::string & szFolderName, const std::string& szFileName);

	//  新建文件 推荐文件名
	std::string RecommendFileName();
	//  验证名字是否合法
	bool IsFileNameValid(const std::string & szName);
	// 验证名字是否合法
	bool CGraphContainer::IsFileNameValid(CGraphFile* pGraph, const  std::string& szNewName);

	//  新建图形目录
	CGraphBaseData* CreateGraphFolder(const std::string& szName);


	// 查找
	CGraphFile *GetGraph(const std::string &szName);
	CGraphFolder *GetGraphFolder(const std::string & szName);

	//! 推荐文件目录名 
	std::string RecommendFolderName();
	//  验证名字是否合法
	bool IsFileFolderNameValid(const std::string & szName);

	bool ValidateFolderName(const std::string &szName);
	bool ValidateFolderName(CGraphFolder *pFolder, const std::string & szNewName);

	//! 保存图形文件
	bool SaveFiles(const QString & szRootPath) ;

	bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	//
	bool OpenFiles(const QString & szRootPath, CGraphFile *pGraphFile);

	void SaveFolderPrj(CGraphBaseData *pFolder, QDomElement *pDomFolder, QDomDocument *pXml);
	//
	void DeleteBaseData(CGraphBaseData* pBaseData);

protected:
	void Clear();
private:

	// 图形目录
// 	std::vector <CGraphFolder*> m_arrGraphFolders;

	// 图形文件
// 	std::vector <CGraphFile*> m_arrGraphs;

	//数据文件 
	std::vector <CGraphBaseData*> m_arrGraphBaseData;
};


 

 
#endif // GRAPH_CONTAINER_H
/** @}*/

