/*! @file graph_folder.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  graph_folder.h
�ļ�ʵ�ֹ��� :  ͼ���ļ�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��������
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/
#ifndef D_GRAPH_FOLDER_H
#define D_GRAPH_FOLDER_H
  
#include <vector>
#include <memory>

#include "graphbasedata.h"

class QString;

class QDomDocument;
class QDomElement;

class CGraphFile;
class CGraphFolder : public CGraphBaseData
{
public:
	CGraphFolder();
	~CGraphFolder();
public:
	// �ӽڵ�
	std::vector <std::shared_ptr<CGraphFile>> m_arrGraphs;

public:
	std::string & GetFolderName()  
	{
		return m_szGraphFolderName;
	}
	void SetFolderName(const std::string &szName)
	{
		m_szGraphFolderName = szName;
	}
	//  ���һ��ͼ��
	bool AddGraph(CGraphFile *pPic);

	//  �½�ͼ���ļ�
	CGraphFile* CreateGraph(const std::string& szName);
	//�½�Ŀ¼
	//  �½�ͼ��Ŀ¼
	CGraphBaseData* CreateGraphFolder(const std::string& szName);

	//! ɾ��ָ����ͼ���ļ�
	bool DeleteGraph(CGraphFile *pPic);

	bool DeleteGraph(const std::string & szName);

	//! ����ͼ���ļ�
	bool SaveFiles(const QString & szRootPath) const;

	bool SaveProject(QDomDocument *pXml, QDomElement *pParent, const QString & szRoot);

	std::size_t GetGraphCount()
	{
		return m_arrGraphs.size();
	}

	const std::vector <CGraphBaseData*> GetGraphData()
	{
		return m_arrGraphBaseData;
	}

	std::string RecommendFileName();
	std::string RecommendFolderName();
	bool IsFileNameValid(const std::string& szName);
	bool IsFileFolderNameValid(const std::string& szName);

	void DeleteBaseData(CGraphBaseData* pBaseData);


private:
	// ����Ŀ¼���� 
	std::string m_szGraphFolderName;
	//

	//�����ļ� 
	std::vector <CGraphBaseData*> m_arrGraphBaseData;

};




#endif // D_GRAPH_FOLDER_H

/** @}*/

