#ifndef _DATA_DEF_H
#define _DATA_DEF_H
#include "fesapi/fesdatadef.h"
#include <QObject>
#include <memory>
#include <vector>

struct  NODE;
class CNodeGroup;
class CNodeGroupMgr;
typedef std::shared_ptr<NODE> NODE_DEF;
typedef std::shared_ptr<CNodeGroup> NODE_GROUP_DEF;
typedef std::shared_ptr<CNodeGroupMgr> NODE_GRP_MGR_DEF;

#define MAGIC_HEAD 0X4C49

enum  NODE_ENUM
{
	MAX_NAME_LEN=256,
	MAX_MEM_LENGTH=512,
	MAX_PAD_LENGTH=12,
};

struct NODE_MEM
{
	int OccNo;                           //节点OccNo号
	int Type;                            //节点类型
	char NodeTagName[MAX_NAME_LEN];      //节点tagname
	int  NodeOffSet;                     //节点偏移
	int  NodeSize;                       //节点内存大小
};




struct NODE_APP_MAGIC                   
{
	int MagicNum1;
	int MagicNum2;
	int nCount;                          //APP 或者 数目
	int32u nOccNo;
	int ShmSize;                         //节点偏移
};

struct NODE_SCD_MAGIC
{
	int MagicNum1;
	int MagicNum2;
	int NodeServerCount;                 //server节点数目
	int NodeFesCount;                    //fes节点数目
	int NodeClientCount;				 //client节点数目
	int AllNodeCount;                    //所有节点数目
	int ShmSize;                         //节点偏移
};

struct APP_INFO
{
	int nIndex;
	char appName[MAX_NAME_LEN];
	char loadMode[MAX_NAME_LEN];
	char agru[MAX_NAME_LEN];
	char desc[MAX_NAME_LEN];
	int32u nDescOccNo;
};

// 传送数据类型
enum
{
	//INVALID_TYPE = 0,
	AIN_TYPE = 1,
	DIN_TYPE = 2,
	MAX_DATA_TYPE = 100,
};

//最大传送字节数
enum
{
	MAX_NETBUS_PAK_LENGTH = 398 * 1024,
};

//传送原因
enum
{
	COT_NORMAL_ASYNC = 1,      //正常数据同步
	COT_INCIDENT_ENENT = 2,    //突发事件
};

class DATA_BASE
{
public:
	int m_nType;                      //!<传送数据类型
	int m_nCount;                     //!<传送数据数目
	int m_nNodeOccNo;                 //!<节点OccNo
	int m_nStartOccNo;                //!<传送数据起始地址
	int m_nTransReason;               //!<数据传送原因
	int m_nExtraLen;
};





//节点组
class CNodeGroup
{
public:
	CNodeGroup(std::string& szName)
	{
		Q_ASSERT(!szName.empty() && szName.size() > 0);
		m_szGrpName = szName;
	}
	~CNodeGroup()
	{
		m_arrNodes.clear();
	}
public:
	void AddNode(NODE_DEF pNode)
	{
		Q_ASSERT(pNode);
		m_arrNodes.push_back(pNode);
	}
	NODE_DEF GetNodeByOccNo(int nOccNo)
	{
		Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo<MAX_NODE_OCCNO);

		if (nOccNo==INVALID_OCCNO || nOccNo >=MAX_NODE_OCCNO)
		{
			return nullptr;
		}
		//先从当前节点列表中寻找
		for (auto i:m_arrNodes)
		{
			if (i->OccNo = nOccNo)
			{
				return i;
			}
		}
		//再从其子组中寻找
		for (auto i:m_arrGrps)
		{
			for (auto j:i->m_arrNodes)
			{
				if (j->OccNo==nOccNo)
				{
					return j;
				}
			}
		}
		return nullptr;
	}
	NODE_DEF GetNode(int i)
	{
		Q_ASSERT(i < m_arrNodes.size());
		if (i>=m_arrNodes.size())
		{
			return nullptr;
		}
		return m_arrNodes.at(i);
	}
	NODE_GROUP_DEF GetGrp(int i)
	{
		Q_ASSERT(i<m_arrGrps.size());
		if (i>=m_arrGrps.size())
		{
			return nullptr;
		}
		return m_arrGrps.at(i);
	}
	void AddGrp(NODE_GROUP_DEF pGrp)
	{
		Q_ASSERT(pGrp);
		m_arrGrps.push_back(pGrp);
		/*for (auto i:pGrp->m_arrNodes)
		{
			this->m_arrNodes.push_back(i);
		}*/
	}	
	//获取零散节点数目
	size_t GetNodeSize()
	{
		return m_arrNodes.size();
	}
	//获取节点组数目
	size_t GetGrpSize()
	{
		return m_arrGrps.size();
	}
	//获取当前前置下所有节点数目,一组算一个前置（冗余）
	size_t GetTotalNodeSize()
	{
		return m_arrGrps.size()+m_arrNodes.size();
	}
private:
	std::string m_szGrpName;
	std::vector <std::shared_ptr < NODE > > m_arrNodes;
	std::vector <std::shared_ptr < CNodeGroup > >m_arrGrps;
};

//工作节点组管理
class CNodeGroupMgr
{
public:
	CNodeGroupMgr()
	{
	
	}
	~CNodeGroupMgr()
	{
		
	}
public:
	void AddGroup(NODE_TYPE nodeType,NODE_GROUP_DEF pGrp)
	{
		Q_ASSERT(pGrp);
		m_MapNode.insert(std::make_pair(nodeType, pGrp));
	}
	NODE_GROUP_DEF GetGroup(NODE_TYPE nodeType)
	{
		auto it = m_MapNode.find(nodeType);
		if (it==m_MapNode.end())
		{
			return nullptr;
		}
		return it->second;
	}
private:
	std::map<NODE_TYPE,NODE_GROUP_DEF>m_MapNode;
};



#endif