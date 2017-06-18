/*! @file graph_container.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  graph_container.h
�ļ�ʵ�ֹ��� :  ͼ������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ͼ������
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
	//  ��������Ƿ���ϵ������� �����ļ������Ƿ�����зǷ��ַ�
	static bool CheckName(const std::string &szName, bool bWarning = false);

	//  �½�ͼ���ļ�
	CGraphFile* CreateGraph(const std::string& szName);
	//  ���һ��ͼ��
	bool AddGraph(CGraphFile* pGraph);
	// ɾ��ָ����ͼ���ļ�
	void DeleteGraph(CGraphFile *pPic);
	// ɾ��һ���ļ�
	bool  DeleteGraph(const std::string & szFolderName, const std::string& szFileName);

	//  �½��ļ� �Ƽ��ļ���
	std::string RecommendFileName();
	//  ��֤�����Ƿ�Ϸ�
	bool IsFileNameValid(const std::string & szName);
	// ��֤�����Ƿ�Ϸ�
	bool CGraphContainer::IsFileNameValid(CGraphFile* pGraph, const  std::string& szNewName);

	//  �½�ͼ��Ŀ¼
	CGraphBaseData* CreateGraphFolder(const std::string& szName);


	// ����
	CGraphFile *GetGraph(const std::string &szName);
	CGraphFolder *GetGraphFolder(const std::string & szName);

	//! �Ƽ��ļ�Ŀ¼�� 
	std::string RecommendFolderName();
	//  ��֤�����Ƿ�Ϸ�
	bool IsFileFolderNameValid(const std::string & szName);

	bool ValidateFolderName(const std::string &szName);
	bool ValidateFolderName(CGraphFolder *pFolder, const std::string & szNewName);

	//! ����ͼ���ļ�
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

	// ͼ��Ŀ¼
// 	std::vector <CGraphFolder*> m_arrGraphFolders;

	// ͼ���ļ�
// 	std::vector <CGraphFile*> m_arrGraphs;

	//�����ļ� 
	std::vector <CGraphBaseData*> m_arrGraphBaseData;
};


 

 
#endif // GRAPH_CONTAINER_H
/** @}*/

