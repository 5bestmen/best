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
*  @brief  基本定义
*  @author LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/
#ifndef D_GRAPH_FOLDER_H
#define D_GRAPH_FOLDER_H
  
#include <vector>
#include <memory>

class CGraphFile;
class CGraphFolder
{
public:
	CGraphFolder();
	~CGraphFolder();
public:
	// 图形文件
	std::vector <std::shared_ptr<CGraphFile>> m_arrGraphs;
public:
	std::string & GetFolderName()  
	{
		return m_szGraphFolderName;
	}
	void SetFolderName(const std::string &szName);
	//  添加一个图形
	bool AddGraph(CGraphFile *pPic);

	//  新建图形文件
	CGraphFile* CreateGraph(const std::string& szName);
	//! 删除指定的图形文件
	bool DeleteGraph(CGraphFile *pPic);

	bool DeleteGraph(const std::string & szName);

private:
	// 画面目录名称 
	std::string m_szGraphFolderName;

};




#endif // D_GRAPH_FOLDER_H

/** @}*/

