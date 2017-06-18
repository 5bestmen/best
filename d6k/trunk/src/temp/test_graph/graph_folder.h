/*! @file graph_folder.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       : graph_folder.h
�ļ�ʵ�ֹ��� : ͼ���ļ�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  ��������
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
	// ͼ���ļ�
	std::vector <std::shared_ptr<CGraphFile>> m_arrGraphs;
public:
	std::string & GetFolderName()  
	{
		return m_szGraphFolderName;
	}
	void SetFolderName(const std::string &szName);
	//  ���һ��ͼ��
	bool AddGraph(CGraphFile *pPic);

	//  �½�ͼ���ļ�
	CGraphFile* CreateGraph(const std::string& szName);
	//! ɾ��ָ����ͼ���ļ�
	bool DeleteGraph(CGraphFile *pPic);

	bool DeleteGraph(const std::string & szName);

private:
	// ����Ŀ¼���� 
	std::string m_szGraphFolderName;

};




#endif // D_GRAPH_FOLDER_H

/** @}*/

