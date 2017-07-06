#ifndef CONFIG_DATA_H
#define CONFIG_DATA_H


#include "datatypes.h"
#include "scadastudio/define.h"
#include "scadastudiodefine.h"
#include "base_data.h"
#include "scale_data.h"
#include "alarm_data.h"
#include "variable_data.h"
#include "fes_conf_data.h"
#include "feschannel_forwardingchannel_data.h"

#include <vector>
#include <QString>
#include <QMap>
#include <unordered_map>
#include <set>



//class IMainWindow;
class FES_PROJECT_INFO; 
class IMainModuleInterface;
class QXmlStreamReader;
class QDomDocument;
class QDomElement;
class QXmlStreamWriter;
class QAbstractItemModel;
class QModelIndex;

namespace Config
{
	//节点服务 节点机器上运行哪些服务
	struct NODE_SERVICE_ROLE
	{
		NODE_SERVICE_ROLE()
		{
			Index = -1;
			Name = "";
			AppType = 0;
			LoadType = 0;
			Argument = "";
			Description = "";
			Option = "";
		}

		int Index;
		QString Name;
		int AppType;
		int LoadType;
		int Prority;
		QString Argument;
		QString Description;
		QString Option;
	};

	class CNodeStandardTemplateServiceRole
	{
	public:
		CNodeStandardTemplateServiceRole();

		~CNodeStandardTemplateServiceRole();

		QString m_strName;
		QString m_strDesc;

		std::vector<NODE_SERVICE_ROLE *> m_arrServiceRole;
	};
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//节点模块
	class CNode : public CBaseData
	{
	public:
		CNode(IMainModuleInterface *pCore);
		~CNode();

		virtual void Save()
		{

		}

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag()const
		{
			return m_bModifyFlag;
		}

		virtual int32u GetOccNo() const
		{
			return 0;
		}

		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];						//节点名
		QString m_strHostName;						//主机名
		int32u m_nOccNo;							//分组序号
		int32u m_nBlockNo;						//组内序号
		int32u m_nNodeType;							//节点类型	
		QString m_strNetwork_IP_A;					//A网IP地址
		QString m_strNetwork_IP_B;					//B网IP地址
		bool m_bIsTimeSource;						//是否是时间源
		QString m_strProgram;						//与逻辑程序进行关联
		QString m_strConfig;						//前置节点关联前置配置
													//后台节点(server和workstation)关联的SCADA变量
		int32u m_nSlaveOccNo;							//分组序号
		
		QString m_strGroup;							

		QString m_strService;						//节点运行服务配置

		std::vector<NODE_SERVICE_ROLE *> m_arrNodeServiceRole;

	private:
		//int32u m_nOccNo;
		//QString m_szTagName;
		IMainModuleInterface *m_pCore;
	private:
		//	std::vector<C>
		//	std::vector <CChannelData *> m_arrChannels;
	};

	//只有一对
	class CNodeServer : public CBaseData
	{
	public:
		CNodeServer(IMainModuleInterface *pCore);
		~CNodeServer();

		virtual void Save()
		{

		}

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag()const
		{
			return m_bModifyFlag;
		}

		virtual int32u GetOccNo() const
		{
			return 0;
		}

		bool AddNode(CNode *pNode);

		bool SaveData(QXmlStreamWriter &writer, int &nOccNo, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);



	public:
		std::vector<CNode *> m_arrNodes;
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];			//节点组名
	private:
		IMainModuleInterface *m_pCore;
	};

	class CNodeGroup
	{
	public:
		CNodeGroup();
		~CNodeGroup();

		void GetNodeTagNameByFesConfigTagName(const QString &strFesTagName, QString &strNodeTagName);
		void GetFesConfigTagNameByNodeTagName(const QString &strNodeTagName, QString &strFesTagName);
		//bool CheckModifyGroupNameExist(CFesGroup *pFesGroup, QString strGroupName);

		//// 创建组
		//CFesGroup* CreateGroup();

		//修改组
		bool ModifyGroup(const QString &strTagName, CNodeGroup *pGroup);

		////删除组
		bool DeletGroup(CNodeGroup *pGroup);
		bool DeleteItem(CNode *pItem);

		////创建前置item
		//CFesData *CreateFesItem();

		//修改项
		bool ModifyItem(const QString &strTagName, CNode *pNode);

		QString m_strGroup;

		//组包含的组
		std::vector<CNodeGroup *> m_arrGroup;
		std::vector<CNode *> m_arrItem;
	};

	class CNodeConfigData
	{
	public:
		CNodeConfigData();
		~CNodeConfigData();

		bool LoadNodeService();
		bool LoadScadaModule(QXmlStreamReader &xml);
		bool LoadSchemeModule(QXmlStreamReader &xml);
		bool LoadSchemeModuleMember(QXmlStreamReader &xml, Config::CNodeStandardTemplateServiceRole *pService);

		bool LoadNodeFile(QString &strFile);
		bool LoadNodeFesNode(QXmlStreamReader &xml, Config::CNodeGroup *pFes);
		bool LoadNodeFesGroupNode(QXmlStreamReader &xml, Config::CNodeGroup *pFesGroup);

		bool LoadNodeServerNode(QXmlStreamReader &xml);
		bool LoadNodeServerAndWorkstatonServiceMember(QXmlStreamReader &xml, Config::CNode *pNode);

		bool LoadNodeWorkstationNode(QXmlStreamReader &xml, Config::CNodeGroup *pWorkstation);
		bool LoadNodeWorkstationGroupNode(QXmlStreamReader &xml, Config::CNodeGroup *pWorkstation);

		bool AddNodeData(CNodeServer *pNode);

		bool SaveNodeFiles(QDomDocument *pXml, QDomElement &e, const QString &szRoot, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

		bool SaveChildNode(QXmlStreamWriter &writer, Config::CNodeGroup *pGroup, int &nOccNo, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveWorkstationChildNode(QXmlStreamWriter &writer, Config::CNodeGroup *pGroup, int &nOccNo, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

		void Create();
		void Clear();

		CNode *AddNodeItem(CNodeGroup *pGroup);
		CNodeGroup *AddNodeGroup(CNodeGroup *pGroup);
		
		//server添加节点
		CNode *AddServerNodeItem();
		bool DeleteHashItem(QString strTagName);
		bool PushTagNameToHash(QString strTagName, CNode *pDatapData);
		bool PushGroupTagNameToHash(QString strTagName);

		bool CheckGroupTagNameIsExist(QString strTagName);
		bool ModifyGroupHashMap(const QString &strTagName, const QString &strLastTagName);
		bool DeleteHashGroup(QString strTagName);

		bool CheckTagNameIsExist(QString strTagName);
		bool ModifyItemHashMap(const QString &strTagName, const QString &strLastTagName);


		bool ChangeNodeFesTagNameNotifyFes(QString &strTagName, QString &strLastTagName);
		bool ChangeNodeServerTagNameNotifyScada(QString &strTagName, QString &strLastTagName);

		//节点三种类型
		CNodeServer * m_pNodesServer;
		CNodeGroup *m_pFesGroup;
		CNodeGroup *m_pWorkstationGroup;

		std::vector<NODE_SERVICE_ROLE *> m_arrServerService;
		std::vector<CNodeStandardTemplateServiceRole *> m_arrStandTemplateService;
	private:
		std::unordered_map<std::string, CNode *> m_arrHashItem;
		std::unordered_map<std::string, std::string> m_arrHashGroup;


	};
};



#endif // CONFIG_DATA_H