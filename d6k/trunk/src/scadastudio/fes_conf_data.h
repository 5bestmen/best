#ifndef FES_CONFIG_DATA_H
#define FES_CONFIG_DATA_H

#include "datatypes.h"
#include "scadastudio/define.h"
#include "scadastudiodefine.h"
#include "base_data.h"
#include "scale_data.h"
#include "alarm_data.h"
#include "variable_data.h"

#include <vector>
#include <QString>
#include <QMap>
#include <unordered_map>
#include <set>

#define  STRING_PAD  sizeof(size_t)

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
	class CFesData;
	class CForwardingAnalogInputData;
	class CForwardingDigitalInputData;
	class CForwardingAnalogOutputData;
	class CForwardingDigitalOutputData;
	class CForwardingControlGearData;
	class CForwardingKWHData;
	class CForwardingChannelGroup;
	class CForwardingChannelData;
	
	class CAnalogInput;
	class CDigitalInput;
	class CAnalogOutput;
	class CDigitalOutput;
	class CDevice;
	class CChannelData;
	class CChannelGroup;

	class CFesData :public CBaseData
	{
	public:
		CFesData(IMainModuleInterface *pCore);
		~CFesData();

		//CFesData();

		virtual void Save();

		void Clear();

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag()const
		{
			return m_bModifyFlag;
		}

		//校验数据
		bool CheckData();

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		bool LoadFesData(QXmlStreamReader &reader);
		bool ReadChannel(QXmlStreamReader &reader, Config::CChannelGroup *pChannelGroup);
		bool ReadChannelGroupNode(QXmlStreamReader &reader, Config::CChannelGroup *pChannelGroup);
		bool ReadC(QXmlStreamReader &reader, Config::CChannelGroup *pChannelGroup);
	

		bool ReadTransform(QXmlStreamReader &reader);
		bool ReadLinear(QXmlStreamReader &reader, CTransformGroup *pGroup);
		bool ReadL(QXmlStreamReader &reader, CTransformGroup *pGroup);
		bool ReadLinearGroupNode(QXmlStreamReader &reader, CTransformGroup *pGroup);
		bool ReadNonlinear(QXmlStreamReader &reader, CTransformGroup *pGroup);
		bool ReadNonLinearGroupNode(QXmlStreamReader &reader, CTransformGroup *pGroup);
		bool ReadNl(QXmlStreamReader &reader, CTransformGroup *pGroup);
		bool Readnl(QXmlStreamReader &reader, Config::CNonlinearTransform *pData);
		bool ReadP(QXmlStreamReader &reader, Config::CNonlinearTransform *pData);

		bool ReadAlarm(QXmlStreamReader &reader);
		bool ReadAnalogGroupNode(QXmlStreamReader &reader, CAnalogAlarmGroup *pGroup);
		bool ReadAINAlarm(QXmlStreamReader &reader, CAnalogAlarmGroup *pGroup);
		bool ReadAa(QXmlStreamReader &reader, Config::CAnalogAlarm *pAlarm);
		bool Readaa(QXmlStreamReader &reader, Config::CAnalogAlarm *pAlarm);

		bool ReadDigitalGroupNode(QXmlStreamReader &reader, CDigitalAlarmGroup *pGroup);
		bool ReadDINAlarm(QXmlStreamReader &reader, CDigitalAlarmGroup *pGroup);
		bool ReadDa(QXmlStreamReader &reader, Config::CDigitalAlarm *pDigital);
		bool Readda(QXmlStreamReader &reader, Config::CDigitalAlarm *pDigital);

		//读转发通道数据
		bool ReadForwardingChannel(QXmlStreamReader &reader, Config::CForwardingChannelGroup *pChannelGroup);
		bool ReadForwardingC(QXmlStreamReader &reader, Config::CForwardingChannelGroup *pChannelGroup);
		bool ReadForwardingChannelGroupNode(QXmlStreamReader &reader, Config::CForwardingChannelGroup *pChannelGroup);

		bool SaveFesData(const QString &szRoot, int nOccNo, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveChannel(QXmlStreamWriter &writer, Config::CChannelGroup *pChannelGroup, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveChannelChildNode(QXmlStreamWriter &writer, Config::CChannelGroup *pChannelGroup, int &nOccNoChannel, int &nOccNoDevice, int &nAIOccNo, int &nDIOccNo
			, int &nAOOccNo, int &nDOOccNo, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

		bool SaveForwardingChannel(QXmlStreamWriter &writer, Config::CForwardingChannelGroup *pChannelGroup, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveForwardingChannelChildNode(QXmlStreamWriter &writer, Config::CForwardingChannelGroup *pChannelGroup, int &nOccNoChannel, int &nAIOccNo, int &nDIOccNo
			, int &nAOOccNo, int &nCGOccNo, int &nDOOccNo, int& nKwhOccNo, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);


		bool SaveTransform(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveLinearTransformChildNode(QXmlStreamWriter &writer, Config::CTransformGroup *pGroup, int &nOccNo
			, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveLinear(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveNonLinearTransformChildNode(QXmlStreamWriter &writer, Config::CTransformGroup *pGroup, int &nOccNo
			, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveNonlinear(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

		bool SaveAlarm(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveAnalogAlarm(QXmlStreamWriter &writer, CAnalogAlarmGroup *pGroup, int &nAanalogAlarmOccNo, int &nAanalogAlarmLimitOccNo
			, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveDigtalAlarm(QXmlStreamWriter &writer, CDigitalAlarmGroup *pGroup, int &nDigitalAlarmOccNo, int &nDigitalAlarmLimitOccNo
			, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

		//变量
		bool SaveVariable(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveSystemVariable(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveUserVariable(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveUserVariableChildNode(QXmlStreamWriter &writer, Config::CUserVariableGroup *pGroup, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool ReadVariable(QXmlStreamReader &reader);
		bool ReadSystemVariable(QXmlStreamReader &reader);
		bool ReadUserVariableGroup(QXmlStreamReader &reader, Config::CUserVariableGroup *pContainer);
	
		CChannelData *AddNewChannelItem(CChannelGroup *pChannelGroup);
		bool DeleteFesHashItem(int nIDD, const QString &strChannelTagnname);
		//bool CheckModifyChannelTagNameExist(const QString &strChannelTagName);
		//m_arrChannelTagNameMap
		bool ModifyFesHashMap(int32u nIDD, const QString &strChannelTagName, const QString &strLastChannelTagName);



		CDevice *AddNewDeviceItem();

		CAnalogInput *AddNewAnalogInput();
		CDigitalInput *AddNewDigitalInput();
		CAnalogOutput *AddNewAnalogOutput();
		CDigitalOutput *AddNewDigitalOutput();

		CTransform *AddNewLinearTransform();
		CTransform *AddNewNonLinearTransform();

		CAnalogAlarm *AddNewAnalogAlarm();
		CAinAlarmLimit *AddNewAinAlarmLimit();

		CDigitalAlarm *AddNewDigitalAlarm();
		CDinAlarmLimit *AddNewDigitalAlarmLimit();

		CUserVariable *AddUserVariable();

		CForwardingChannelData *AddNewForwardingChannelItem(CForwardingChannelGroup *pGroup);
		CForwardingAnalogInputData *AddNewForwardingAIItem();
		CForwardingDigitalInputData *AddNewForwardingDIItem();
		CForwardingDigitalOutputData *AddNewForwardingDOItem();
		CForwardingAnalogOutputData *AddNewForwardingAOItem();
		CForwardingControlGearData *AddNewForwardingCGItem();
		CForwardingKWHData *AddNewForwardingKWHItem();

		bool PushFesTagNameHashMap(int nIDD, const QString &strTagName, CBaseData *pData);
		bool CheckTagNameIsExist(const QString &strTagName);
		int32u GetArrCount(int nIDD)
		{
			Q_ASSERT(!(nIDD < 0 || nIDD > FES_HASH_TYPE_NUM::MAX_IDD_NUM));
			if (nIDD < 0 || nIDD > FES_HASH_TYPE_NUM::MAX_IDD_NUM)
			{
				return -1;
			}
			
			return (int32u)m_arrMaps[nIDD].size();
		}

		CBaseData *GetData(QString &strTagName, int nType);
		CBaseData *GetDataFromAllData(QString &strTagName, int &nType);
		bool GetUserVariableRelatedSourceOccNo(const QString &strTagName, const QString &strRelatedTagName, QString &strOutput);


		//note 关联关系

		bool AddAIRelationScaleArr(std::string &tagName, CAnalogInput *pAnalogInput);
		bool ModifyAIsRelationScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type);
		bool ModifyAIsRelationScaleDescArr(std::string &tagName, std::string &desc);
		//修改模拟量转换 AIs表里的关联转换跟着修改
		bool ModifyAIsScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type);
		bool DeleteAIsScaleArr(const std::string &szTagName);
		bool DeleteAIsRelationScaleArr(const std::string &szTagName, CAnalogInput *pAnalogInput);

		bool AddAIRelationAlarmArr(std::string &tagName, CAnalogInput *pAnalogInput);
		bool ModifyAIsRelationAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, Config::CAnalogInput *pAnalogInput);
		bool ModifyAIsRelationAlarmDescArr(std::string &tagName, std::string &desc);
		//修改模拟量告警 DIs表里的关联告警跟着修改
		bool ModifyAIsAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type);
		bool DeleteAIsAlarmArr(const std::string &szTagName);
		bool DeleteAIsRelationAlarmArr(const std::string &szTagName, CAnalogInput *pAnalogInput);

		//fes di关联次遥信doubledi
		bool AddDIRelationDoubleDIArr(const std::string &szTagName, CDigitalInput *pDigitalInput);
		bool ModifyDIRelationDoubleDIArr(const std::string &szTagName, const std::string &szLastTagName, CDigitalInput *pDigitalInput);
		bool ModifyDIDoubleDIArr(const std::string &szTagName, const std::string &szLastTagName);
		bool DeleteDIRelationDoubleDIArr(const std::string &szTagName, CDigitalInput *pDigitalInput);
		bool DeleteDIDoubleDIArr(const std::string &szTagName, CDigitalInput *pDigitalInput);


		bool AddDIRelationAlarmArr(std::string &tagName, CDigitalInput *pDigitalInput);
		bool ModifyDIsRelationAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, Config::CDigitalInput *pDigitalInput);
		bool ModifyDIsRelationAlarmDescArr(std::string &tagName, std::string &desc);
		bool ModifyDIsAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type);
		bool DeleteDIsAlarmArr(const std::string &szTagName);
		bool DeleteDIsRelationAlarmArr(const std::string &szTagName, CDigitalInput *pDigitalInput);

		bool AddAORelationScaleArr(std::string &tagName, CAnalogOutput *pAnalogOutput);
		bool ModifyAOsRelationScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, Config::CAnalogOutput *pAnalogOutput);
		//修改模拟量遥控
		bool ModifyAOsScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type);
		bool ModifyAOsRelationScaleDescArr(std::string &tagName, std::string &desc);
		bool DeleteAOsScaleArr(const std::string &szTagName);
		bool DeleteAOsRelationScaleArr(const std::string &szTagName, CAnalogOutput *pAnalogOutput);


		//fes do AssociatedDI关联 关联遥信点名 关联遥信：主要用于遥控成功与否的判断。
		bool AddDORelationAssociatedDIArr(const std::string &szTagName, CDigitalOutput *pDigitalOutput);
		bool ModifyDORelationAssociatedDIArr(const std::string &szTagName, const std::string &lastTagName, CDigitalOutput *pDigitalOutput);
		//前置DI修改，前置DO AssociatedDI关联此遥信，那么关联关系修改 1对多关系
		bool ModifyDOAssociatedDIArr(const std::string &szTagName, const std::string &szLastTagName);
		bool DeleteDOsRelationAssociatedDIArr(const std::string &szTagName, CDigitalOutput *pDigitalOutput);
		bool DeleteDOsAssociatedDIArr(const std::string &szTagName);

		//fes do BlockingSignalTag关联(di tagname和用户变量bool类型变量)
		bool AddDORelationBlockingSignalTagArr(const std::string &szTagName, CDigitalOutput *pDigitalOutput);
		bool ModifyDORelationBlockingSignalTagArr(const std::string &szTagName, const std::string &szLastTagName, CDigitalOutput *pDigitalOutput);
		bool ModifyDOBlockingSignalTagArr(const std::string &szTagName, const std::string &szLastTagName);
		bool DeleteDOsRelationBlockingSignalTagArr(const std::string &szTagName, CDigitalOutput *pDigitalOutput);
		bool DeleteDOsBlockingSignalTagArr(const std::string &szTagName);

		bool AddUserVariableRelationScaleArr(std::string &tagName, CUserVariable *pVairalbe);
		bool ModifyUserVariableRelationScaleArr(CUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName, int type);
		bool ModifyUserVariableScaleArr(const std::string &szTagName, const  std::string &szLastTagName, const  std::string &szDesc, int type);
		bool DeleteUserVariableScaleArr(const std::string &tagName);
		bool DeleteUserVariableRelationScaleArr(const std::string &szTagName, CUserVariable *pVairalbe);

		//用户变量关联告警
		bool AddUserVariableRelationAlarmArr(std::string &tagName, CUserVariable *pVairalbe);
		bool ModifyUserVariableRelationAlarmArr(CUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName);
		bool ModifyUserVariableAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type);
		bool DeleteUserVariableAlarmArr(const std::string &szTagName);
		bool DeleteUserVariableRelationAlarmArr(std::string &tagName, CUserVariable *pVairalbe);

		//用户变量绑定关联源tagname
		bool AddUserVariableRelationSourceTagNameArr(std::string &tagName, CUserVariable *pVairalbe);
		bool ModifyUserVariableRelationSourceTagNameArr(CUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName);
		bool ModifyUserVariableSourceTagNameArr(std::string &tagName, std::string &lastTagName);
		bool DeleteUserVariableSourceTagNameArr(std::string &tagName);
		bool DeleteUserVariableRelationSourceTagNameArr(std::string &tagName, CUserVariable *pVairalbe);

		//转发通道
		bool AddForwardingAIRelationScaleArr(std::string &tagName, CForwardingAnalogInputData *pAnalogInput);
		bool ModifyForwardingAIsRelationScaleArr(CForwardingAnalogInputData *pData, std::string &tagName, std::string &lastTagName, std::string &desc, int type);
		bool ModifyForwardingAIsScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type);
		bool DeleteForwardingAIsScaleArr(const std::string &szTagName);
		bool DeleteForwardingAIRelationScaleArr(std::string &tagName, CForwardingAnalogInputData *pAnalogInput);

		bool AddForwardingAORelationScaleArr(std::string &tagName, CForwardingAnalogOutputData *pAnalogOutput);
		bool ModifyForwardingAOsRelationScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, Config::CForwardingAnalogOutputData *pAnalogOutput);
		bool ModifyForwardingAOsScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type);
		bool DeleteForwardingAOsScaleArr(const std::string &szTagName);
		bool DeleteForwardingAORelationScaleArr(std::string &tagName, CForwardingAnalogOutputData *pAnalogOutput);

		//转发通道SourceTagName关联 转发通道的sourcetagname只能从本前置里选点
		bool AddForwardingAIRelationSourceTagNameArr(std::string &tagName, CForwardingAnalogInputData *pAnalogInput);
		bool ModifyForwardingAIRelationSourceTagNameArr(std::string &tagName, std::string &lastTagName, CForwardingAnalogInputData *pAnalogInput);
		bool ModifyForwardingAISourceTagNameArr(std::string &tagName, std::string &lastTagName);
		bool DeleteForwardingAISourceTagNameArr(std::string &tagName);
		bool DeleteForwardingAIRelationSourceTagNameArr(std::string &tagName, CForwardingAnalogInputData *pAnalogInput);

		bool AddForwardingDIRelationSourceTagNameArr(std::string &tagName, CForwardingDigitalInputData *pDigitalInput);
		bool ModifyForwardingDIRelationSourceTagNameArr(std::string &tagName, std::string &lastTagName, CForwardingDigitalInputData *pDigitalInput);
		bool ModifyForwardingDISourceTagNameArr(std::string &tagName, std::string &lastTagName);
		bool DeleteForwardingDISourceTagNameArr(std::string &tagName);
		bool DeleteForwardingDIRelationSourceTagNameArr(std::string &tagName, CForwardingDigitalInputData *pDigitalInput);

		bool AddForwardingAORelationSourceTagNameArr(std::string &tagName, CForwardingAnalogOutputData *pAnalogOutput);
		bool ModifyForwardingAORelationSourceTagNameArr(std::string &tagName, std::string &lastTagName, CForwardingAnalogOutputData *pAnalogOutput);
		bool ModifyForwardingAOSourceTagNameArr(std::string &tagName, std::string &lastTagName);
		bool DeleteForwardingAOSourceTagNameArr(const std::string &szTagName);
		bool DeleteForwardingAORelationSourceTagNameArr(std::string &tagName, CForwardingAnalogOutputData *pAnalogOutput);

		bool AddForwardingDORelationSourceTagNameArr(std::string &tagName, CForwardingDigitalOutputData *pDigitalOutput);
		bool ModifyForwardingDORelationSourceTagNameArr(std::string &tagName, std::string &lastTagName, CForwardingDigitalOutputData *pDigitalOutput);
		bool ModifyForwardingDOSourceTagNameArr(std::string &tagName, std::string &lastTagName);
		bool DeleteForwardingDOSourceTagNameArr(const std::string &szTagName);
		bool DeleteForwardingDORelationSourceTagNameArr(std::string &tagName, CForwardingDigitalOutputData *pDigitalOutput);


		//删除前置装置AI 删除AI里关联的关系、关联AI的关系
		bool DeleteFesDeviceAICombineRelaton(const std::string &szTagName, CAnalogInput *pAnalogInput);
		bool DeleteFesDeviceDICombineRelaton(const std::string &szTagName, CDigitalInput *pDigitalInput);
		bool DeleteFesDeviceAOCombineRelaton(const std::string &szTagName, CAnalogOutput *pAnalogOutput);
		bool DeleteFesDeviceDOCombineRelaton(const std::string &szTagName, CDigitalOutput *pDigitalOutput);
	
		bool DeleteFesAnalogAlarmCombineRelaton(const std::string &szTagName, CAnalogAlarm *pAnalogAlarm);
		bool DeleteFesDigitalAlarmCombineRelaton(const std::string &szTagName, CDigitalAlarm *pDigitalAlarm);
		
		bool DeleteFesScaleCombineRelaton(const std::string &szTagName);

		bool DeleteFesUserVaraibleCombineRelaton(const std::string &szTagName, CUserVariable *pUser);

		//转发
		bool DeleteFesForwardingAICombineRelaton(const std::string &szTagName, CForwardingAnalogInputData *pAnalogInput);
		bool DeleteFesForwardingDICombineRelaton(const std::string &szTagName, CForwardingDigitalInputData *pDigialInput);
		bool DeleteFesForwardingAOCombineRelaton(const std::string &szTagName, CForwardingAnalogOutputData *pAnalogOutput);
		bool DeleteFesForwardingDOCombineRelaton(const std::string &szTagName, CForwardingDigitalOutputData *pDigitalOutput);


		//node tagname修改，node关联的前置的用户变量的sourcetagname关联关系也跟着修改
		bool ChangeNodeTagNameNodifyFesUserVaraible(const std::string &szNodetagName, const std::string &szNodelastTagName);

		bool LogToFile(const char* filename, const char* log);
private:
		void InitSystemVariable();

	public:
		//int32u m_nOccNo;				//大排行号
		//QString m_szTagName;		//主键
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		QString m_strDescription;	//描述

		uint m_nID;

		QString m_strGroupName;		//分组名称
		QString m_strExtention;		//扩展参数

		QString m_strFesVersion;	//前置版本

		//bool m_bModifiedFlag;			//修改标志

	public:
		//std::vector <CChannelData *> m_arrChannels;					//通道vec
		//std::vector<CLinearTransform *> m_arrLinears;		//线性转换vec
		//std::vector<CNonlinearTransform *> m_arrNonLinears;	//分线性转换vec

		//std::vector<CTransform *> m_arrTransforms;			//转换vec

		//std::vector<CAnalogAlarm *> m_arrAIAlarms;			//模拟量告警vec
		std::vector<CDigitalAlarm *> m_arrDIAlarms;			//数字量告警vec

		std::vector<CSystemVariable *> m_arrSystemVariable;	//系统变量
		//std::vector<CUserVariableGroup *> m_arrUserVariableContainer;	//用户变量

		using TagNameMap = std::unordered_map<std::string, CBaseData *>;

		TagNameMap m_arrMaps[FES_HASH_TYPE_NUM::MAX_IDD_NUM];


		//std::vector<Config::CFesData *> m_arrFes;
		//std::vector<Config::CChannelData *> m_vecChannel;
		//std::vector<Config::CLinearTransform *> m_arrLinear;
		//std::vector<Config::CNonlinearTransform *> m_arrNonLinear;
		//std::vector<Config::CAnalogAlarm *> m_arrAIAlarm;
		//std::vector<Config::CDigtalAlarm *> m_arrDIAlarm;

		//前置状态
		//FES_PROJECT_INFO *m_pFesState;
		//root通道组
		CChannelGroup *m_pChannelGroup;
		//root转换组 管理转换组和转换项
		CTransformGroup *m_pLinearTransformGroup;
		CTransformGroup *m_pNonLinearTransformGroup;
		//root 告警
		CAnalogAlarmGroup *m_pAnalogGroup;
		CDigitalAlarmGroup *m_pDigitalGroup;
		//用户变量
		CUserVariableGroup *m_pUserVariableGroup;	//用户变量

		CForwardingChannelGroup *m_pForwardingChannelGroup;
	private:
		int32u m_nOccNo;				//大排行号
		IMainModuleInterface *m_pCore;
		//AI关联的转换
		std::unordered_map<std::string, std::vector<CAnalogInput *>> m_arrScaleToAIMap;
		std::unordered_map<std::string, std::vector<CAnalogInput *>> m_arrAlarmToAIMap;
		std::unordered_map<std::string, std::vector<CDigitalInput *>> m_arrAlarmToDIMap;
		std::unordered_map<std::string, std::vector<CAnalogOutput *>> m_arrScaleToAOMap;

		//DI关联的次遥信
		std::unordered_map<std::string, std::vector<CDigitalInput *>> m_arrDIToDoubleDIMap;

		//DO关联的关联遥信点名
		//AssociatedDI 关联遥信
		std::unordered_map<std::string, std::vector<CDigitalOutput *>> m_arrDOToAssociatedDIMap;
		//BlockingSignalTag 闭锁开入
		std::unordered_map<std::string, std::vector<CDigitalOutput *>> m_arrDOToBlockingSignalTagMap;

		//用户变量关联转换
		std::unordered_map<std::string, std::vector<CUserVariable *>> m_arrScaleToUserVariableMap;
		//用户变量关联模拟量告警hash
		std::unordered_map<std::string, std::vector<CUserVariable *>> m_arrAlarmToUserVariableMap;
		//用户变量关联源tagname hash
		std::unordered_map<std::string, std::vector<CUserVariable *>> m_arrSourceTagNameToUserVariableMap;


		//转发AI关联的转换
		std::unordered_map<std::string, std::vector<CForwardingAnalogInputData *>> m_arrScaleToForwardingAIMap;
		std::unordered_map<std::string, std::vector<CForwardingAnalogOutputData *>> m_arrScaleToForwardingAOMap;

		//转发AI关联的fes ai tagname
		std::unordered_map<std::string, std::vector<CForwardingAnalogInputData *>> m_arrAIToForwardingAIMap;
		std::unordered_map<std::string, std::vector<CForwardingDigitalInputData *>> m_arrDIToForwardingDIMap;
		std::unordered_map<std::string, std::vector<CForwardingAnalogOutputData *>> m_arrAOToForwardingAOMap;
		std::unordered_map<std::string, std::vector<CForwardingDigitalOutputData *>> m_arrDOToForwardingDOMap;
	};

	class CFesGroup
	{
	public:
		CFesGroup();
		~CFesGroup();

		bool CheckModifyGroupNameExist(CFesGroup *pFesGroup, QString strGroupName);

		// 创建组
		CFesGroup* CreateGroup();

		//修改组
		bool ModifyGroup(const QString &strTagName, const QString &strLastTagName);

		//删除组
		bool DeletGroup(CFesGroup *pFesGroup, const QString &strTagGroup);
		bool DeletItem(CFesData *pFes, const QString &strTagGroup);

		//创建前置item
		CFesData *CreateFesItem();

		//修改项
		bool ModifyItem(const QString &strTagName, const QString &strLastTagName);

		QString m_strGroup;

		//组包含的组
		std::unordered_map<std::string, CFesGroup *> m_arrFesGroup;
		std::unordered_map<std::string, CFesData *> m_arrFesItem;
	};

	class CFesConfigData
	{
	public:
		CFesConfigData();
		~CFesConfigData();
	public:
		enum SAVE_TYPE
		{
			TO_LOCAL_FILE,
			TO_SQLITE,
			TO_MSSQL,
			TO_MYSQL,
		};

		enum NODE_TYPE
		{
			SERVER = 1, FES, WORKSTATION
		};
	public:
		bool SaveData(int nSaveType)
		{
			switch (nSaveType)
			{
			case TO_LOCAL_FILE:
				SaveDataToFile();
			default:
				break;
			}
		}

		bool LoadFesFile(CFesData *pFes, const QString &filePath);
		//bool SaveFesFiles(QDomDocument *pXml, QDomElement &e, const QString &szRoot, IMainModuleInterface *pCore);

		bool SaveProjectFesNode(QDomDocument *pXml, QDomElement &e, const QString &szRoot, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveChildNode(QDomDocument *pXml, QDomElement &e, CFesGroup *pFesGroup);
		bool SaveChildItem(CFesGroup *pFesGroup, const QString &szRoot, int nOccNo, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		
		bool LoadProjectFesNode(QDomElement &e, const QString &szRoot);
		bool LoadChildGroup(CFesGroup *pFesGroup, QDomElement &e, const QString &szRoot);
		bool LoadChildItem(CFesData *pFes, const QString &szRoot);

		CFesData *AddNewFesData(CFesGroup *pFesGroup);

		bool DeleteHashFesItem(std::string &szFesTag);

		//CFesGroup *AddFesGroup();
//		CFesGroup *AddFesGroup(CFesGroup *pFesGroup);

		bool CheckModifyFesTagNameExist(const QString &strFesTagName);

		std::unordered_map<std::string, CFesData *> &GetFecArr()
		{
			return m_arrHashTagNameMap;
		}

		bool ModifyFesHashMap(const QString &strFesTagName, const QString &strLastFesTagName)
		{
			auto it = m_arrHashTagNameMap.find(strLastFesTagName.toStdString());
			if (it != m_arrHashTagNameMap.end())
			{
				// Swap value from oldKey to newKey, note that a default constructed value 
				// is created by operator[] if 'm' does not contain newKey.
				std::swap(m_arrHashTagNameMap[strFesTagName.toStdString()], it->second);
				// Erase old key-value from map
				m_arrHashTagNameMap.erase(it);

				return true;
			}

			return false;
		}

		void LoadFesGroupMap(std::unordered_map<std::string, CFesGroup *> &arrFesGroup, const QAbstractItemModel *pModel, const QModelIndex &top, int nType);

		void Clear();
		void Create();

		bool LogToFile(const char* filename, const char* log);

		CFesData *GetFesData(std::string &szTagName)
		{
			auto it = m_arrHashTagNameMap.find(szTagName);
			if (it != m_arrHashTagNameMap.end())
			{
				return it->second;
			}

			return nullptr;
		}
	private:
		bool SaveDataToFile()
		{
			//auto i = m_arrNodes.begin();
			//for (; i != m_arrNodes.end(); ++i)
			//{
			//	//	i->
			//}
		}
		public:


		//std::vector< CNode* > m_arrNodes;

		CFesGroup *m_pFesGroup;

		std::vector<CFesData*> m_arrFesDatas;

		//所有前置map hash前置,查找快速 统计个数,添加前置
		std::unordered_map<std::string, CFesData *> m_arrHashTagNameMap;

		//std::unordered_map<std::string, CFesGroup *> m_arrFesGroupMap;
	};
}

#endif
