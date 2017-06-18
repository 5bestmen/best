/*! @file db_svc.h
<PRE>
********************************************************************************
模块名       :
文件名       :  db_svc.h
文件实现功能 :  实时数据库模块
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   调试代理
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#ifndef DBSVC_MODULE_H
#define DBSVC_MODULE_H

#include "base_module.h"
#include "fesconfig.h"
#include "fesapi/fes_magicmem.h"

#include <QDomDocument>
#include <QXmlStreamReader>
#include <QMap>

#include <vector>
#include <unordered_map>
#include <QtCore/QtGlobal>
#include <ace/Event.h>

struct FesInfo;
struct NODE;
struct CHANNEL;
struct DEVICE;
struct DIN;
struct AIN;
struct DOUT;
struct AOUT;
struct TRANSFORM_LINEAR;
struct TRANSFORM_NONLINEAR;
struct TRANSFORM;
struct ALARM;
struct AIN_ALARM;
struct AIN_ALARM_LIMIT;
struct DIN_ALARM;
struct DIN_ALARM_LIMIT;
struct TAG_OCCNO;
struct VARDATA;

typedef std::shared_ptr<TRANSFORM_LINEAR>    TRANSFORM_LINEAR_DEF;
typedef std::shared_ptr<TRANSFORM_NONLINEAR> TRANSFORM_NONLINEAR_DEF;
typedef std::shared_ptr<AIN_ALARM> AIN_ALARM_DEF;
typedef std::shared_ptr<DIN_ALARM> DIN_ALARM_DEF;
typedef std::shared_ptr<AIN_ALARM_LIMIT> AIN_ALARM_LIMIT_DEF;
typedef std::shared_ptr<DIN_ALARM_LIMIT> DIN_ALARM_LIMIT_DEF;
typedef std::shared_ptr<TAG_OCCNO> TAG_OCCNO_DEF;
typedef std::shared_ptr<VARDATA> VARDATA_DEF;
class CShareMem;
class CServer;
class CIoChannel;
class CFTChannel;
class CSysVarSvc;

enum XmlReaderType
{
	_DOM_XML_TYPE    =1,
	_STREAM_XML_TYPE =2,
};

class CDbSvc : public CBaseModule
{
public:
	CDbSvc(CFesSvc* pServer, const std::string&szMailBoxName,int &MailID);
	virtual ~CDbSvc(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();

	// 本节点的Node的排行号，如果是主从机，则返回默认主机
	int32u GetMyNodeOccNo()
	{
		//todo :待实现
		//return 0;
		return m_nMyNodeOccNo;
	}

	std::pair<unsigned int, NODE*> GetNodeData()
	{
		return std::make_pair(m_nNodeCount, m_pNodes);
	}

	//访问内存结构
	std::shared_ptr<CChannelMgr>&  GetChannelMgr()
	{
		return m_pChannelMgr;
	}
	//访问内存信息
	bool GetChannelByOccNo(int32u nOccNo, CHANNEL** pChannel);

	bool GetDeviceByOccNo(int32u nOccNo, DEVICE** pDev);

	bool GetAinByOccNo(int32u nOccNo, AIN** pAIN);

	bool GetDinByOccNo(int32u nOccNo, DIN** pDIN);

	bool GetAoutByOccNo(int32u nOccNo, AOUT** pAout);

	bool GetDoutByOccNo(int32u nOccNo, DOUT** pDout);

	
	// 前置内核的内部数据访问接口
	int32u GetNodeOccNo(const char *pszHostName);

	size_t GetDeviceCount(int32u nChannelOccNo);
	// 获取通道中的装置列表
	size_t GetDeviceOccNos(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);

	size_t GetAinCountInChannel(int32u nChannelOccNo);
	size_t GetDinCountInChannel(int32u nChannelOccNo);
	size_t GetAoutCountInChannel(int32u nChannelOccNo);
	size_t GetDoutCountInChannel(int32u nChannelOccNo);

	size_t GetAinCountInDevice(int32u nDeviceOccNo);
	size_t GetDinCountInDevice(int32u nDeviceOccNo);
	size_t GetAoutCountInDevice(int32u nDeviceOccNo);
	size_t GetDoutCountInDevice(int32u nDeviceOccNo);

	// 获取通道中测点的列表
	size_t GetAinOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);
	size_t GetDinOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);
	size_t GetAoutOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);
	size_t GetDoutOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);

	// 获取装置中测点的列表
	size_t GetAinOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos);
	size_t GetDinOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos);
	size_t GetAoutOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos);
	size_t GetDoutOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos);

	//访问数据长度信息
	size_t GetChannelCount() const
	{
		return m_nChannelCount;
	}
	size_t GetDeviceCount() const
	{
		return m_nDeviceCount;
	}
	size_t GetAINCount() const
	{
		return m_nAinCount;
	}
	size_t GetDINCount() const
	{
		return m_nDinCount;
	}
	size_t GetAoutCount() const
	{
		return m_nAoutCount;
	}
	size_t GetDoutCount() const
	{
		return m_nDoutCount;
	}

	// 通过序号获取模拟量指针，nIdx 从0开始
	AIN*GetAinByIndex(int32u nIdx);
	DIN*GetDinByIndex(int32u nIdx);
	AOUT*GetAoutByIndex(int32u nIdx);
	DOUT*GetDoutByIndex(int32u nIdx);

	std::string GetTagName(int32u nOccNo, int32u nDataType) const;

	size_t GetIoChannelNames(std::vector<std::string>& arrTagNames)const;
	size_t GetRemoteChannelNames(std::vector<std::string>& arrTagNames)const;

	void CreateMailBoxs();
	void DestroyMailBoxs();

	bool IsDBAlive(unsigned int nTimeout);

	int32u GetMyHostState()const;
	int8u GetMyHostScanEnable()const;
	// 检查开出的闭锁状态
	void CheckDoutBlockState(DOUT & dout);

	int32u GetNodeState(int32u nOccNo)const;

	int32s GetChannelMailBoxID(int32u nChannelNo);
	// IO 类的操作接口
	bool IoGetDinValue(int32u nOccNo, int8u &nValue, int8u &nQuality)const;
	bool IoGetAinValue(int32u nOccNo, fp64 &fValue, int8u &nQuality) const;

	// 设开出
	bool FesSetDoutValue(int32u nOccNo, int8u Value, int8u nSource);
	// 写值
	bool FesSetAoutValue(int32u nOccNo, fp64 Value, int32u nSource);

	void IoDiagAlarm(int32u nChannleNo, int32u nDeviceNo, const char* pszAlarmTxt, TIMEPAK * pTm);
	// 操作报警
	void IoOperAlarm(int32u nChannleNo, int32u nDeviceNo, const char * pszAlarmTxt, TIMEPAK * pTm);
	// 保护的自诊断
	void IoRelayDiagAlarm(int32u nChannleNo, int32u nDeviceNo, const char* pszAlarmTxt, TIMEPAK * pTm);
	// 通用的告警
	void IoAlarmMsg(int32u nChannleNo, int32u nAlarmType, const char* pszAlarmTxt, TIMEPAK * pTm);


private:
	//获取节点信息
	bool GetNodeInfo(const char* szName);
	//获取所有节点信息
	bool GetTotalNodeInfo(const char* szName);
	//获取当前前置配置信息
	bool GetFesInfo(const char* szName);
	//判断节点是否配置
	bool IsNodeAdded(const QString& str,QDomElement nEle);

private:
	QString m_szNodeName;
	int32u  m_nMyNodeOccNo;
	QString m_szCurFesInfoName;
	QVector < QString > m_VecFesFileName;
	
private:
	bool ReadXmlInfo(const char* pszPath, XmlReaderType nType);

/************以下读取方式按照流形式读取********************************/
private:
	//概要信息
	void ReadFesInfo();
	void ReadChannelInfo();
	void ReadDev(CChannelPtr pChannel);
	void ReadDevInfo(CChannelPtr pChannel);
	void ReadData(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadDataInfo(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadTransFormInfo();
	void ReadAlarmInfo();
	//数据点信息
	void ReadAinInfo(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadDinInfo(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadAoutInfo(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadDoutInfo(CDevicePtr pDev, CChannelPtr pChannel);
	//转换信息
	void ReadScale();
	void ReadLinearInfo();
	void ReadNonLinearInfo();
	void ReadNonLinearVal(TRANSFORM_NONLINEAR_DEF pTransFormNonLinear);
	//告警信息
	void ReadAlarm();
	void ReadAinAlarmInfo();
	void ReadDinAlarmInfo();
	//越限告警
	void ReadAinOfflimitInfo(AIN_ALARM_DEF pAinAlarm);
	void ReadDinOfflimitInfo(DIN_ALARM_DEF pDinAlarm);
	void SkipUnknownElement();
	//变量
	void ReadVariable();
	void ReadSystemVariable();
	void ReadUserVariable();
private:
	//流读取信息
	QXmlStreamReader m_XmlReader;

/************以下读取方式按照DOM形式读取****************************/
private:
	//概要信息
	bool ReadChannelInfo(QDomElement eleCHannel);
	bool ReadTransFormInfo(QDomElement eleDevice);
	bool ReadAlarmInfo(QDomElement eleData);
	//数据点信息
	bool ReadAinInfo(QDomElement eleData, CDevicePtr pDev, CChannelPtr pChannel);
	bool ReadDinInfo(QDomElement eleData, CDevicePtr pDev, CChannelPtr pChannel);
	bool ReadAoutInfo(QDomElement eleData, CDevicePtr pDev, CChannelPtr pChannel);
	bool ReadDoutInfo(QDomElement eleData, CDevicePtr pDev, CChannelPtr pChannel);
	//转换信息
	bool ReadLinearInfo(QDomElement eleData);
	bool ReadNonLinearInfo(QDomElement eleData);
	//告警信息
	bool ReadAinAlarmInfo(QDomElement eleData);
	bool ReadDinAlarmInfo(QDomElement eleData);
	//越限告警
	bool ReadAinOfflimitInfo(QDomElement eleData);
	bool ReadDinOfflimitInfo(QDomElement eleData);
private:
	//大节点
	QDomElement m_DomNodes;
	QDomElement m_DomChannel;
	QDomElement m_DomTransForm;
	QDomElement m_DomAlarm;
	//小节点
	QDomElement m_DomDev;
	QDomElement m_DomData;
private:
	//! 从配置文件中读取出来的信息，加载内存库后丢弃
	FesInfo_PTR           m_pFesInfo;
	std::vector<std::shared_ptr<NODE>>    m_arrTempNodes;
	std::vector<std::shared_ptr<CHANNEL>> m_arrTempChannels;
	std::vector<std::shared_ptr<DEVICE>>  m_arrTempDevices;
	std::vector<std::shared_ptr<AIN>>     m_arrTempAins;
	std::vector<std::shared_ptr<DIN>>     m_arrTempDins;
	std::vector<std::shared_ptr<AOUT>>    m_arrTempAouts;
	std::vector<std::shared_ptr<DOUT>>    m_arrTempDouts;

	std::vector<TRANSFORM_LINEAR_DEF>     m_arrTempTransFormLinears;
	std::vector<TRANSFORM_NONLINEAR_DEF>  m_arrTempTransFormNonLinears;

	std::vector<AIN_ALARM_DEF>            m_arrTempAinAlarms;
	std::vector<DIN_ALARM_DEF>            m_arrTempDinAlarms;
	std::vector<AIN_ALARM_LIMIT_DEF>      m_arrTempAinLimitAlarms;
	std::vector<DIN_ALARM_LIMIT_DEF>      m_arrTempDinLimitAlarms;
	std::shared_ptr<CChannelMgr>          m_pChannelMgr;

	std::vector<VARDATA_DEF> m_arrTempSystemVariables;
	std::vector<VARDATA_DEF> m_arrTempUserVariables;
private:
	// 内存创建
	void BuildMemDB(const char *szDBName);	
	//创建通道内存
	size_t BuildChannelDB(char* pAddress);
	//创建节点内存
	size_t BuildNodeDB(char* pAddress);
	//创建设备内存
	size_t BuildDeviceDB(char* pAddress);
	//创建AIN内存
	size_t BuildAinDB(char* pAddress);
	//创建DIN内存
	size_t BuildDinDB(char* pAddress);
	//创建AOUT内存
	size_t BuildAoutDB(char* pAddress);
	//创建DOUT内存
	size_t BuildDoutDB(char* pAddress);
	//创建告警内存
	size_t BuildAlarmDB(char* pAddress);
	//模拟量告警以及限值
	size_t BuildAinAlarmDB(char* pAddress);
	//开关量告警
	size_t BuildDinAlarmDB(char* pAddress);
	//线性变化
	size_t BuildLinearDB(char* pAddress);
	//非线性变换
	size_t BuildNonLinearDB(char* pAddress);
	//系统变量
	size_t BuildSystemVariableDB(char* pAddress);
	//用户变量
	size_t BuildUserVariableDB(char* pAddress);
	//创建变长内存
	size_t BuildExtraDB(char* pAddress);

	void OutPutShm(char* pAddress);

	void LoadDrivers();
	void StartDrivers();
	void StopDrivers();

	// 强制清空临时数据，减少内存占用
	void ClearTempArray();
private:
	QString  m_szRdbName;

	HEAD_MEM* m_pMagicMem;

	std::shared_ptr<CSysVarSvc> m_pSysSvc;

	std::shared_ptr<ACE_Event> m_pDBAliveFlag;
	ACE_Event m_DBAliveFlag;
private:
	//! 所谓的内存数据库
	std::shared_ptr<CShareMem> m_pMem;

	std::string m_szDataBaseName;

	// 共享内存中的各数据的排布
	NODE*  m_pNodes;
	unsigned int m_nNodeCount;

	CHANNEL *m_pChannels;
	unsigned int m_nChannelCount;
	//! 记住地址，方便遍历
	std::vector<CHANNEL*> m_arrChannelPtrs;
	std::vector<std::shared_ptr<CIoChannel>> m_arrIoChannels;

	DEVICE *m_pDevices;
	unsigned int m_nDeviceCount;
	std::vector<DEVICE*> m_arrDevices;

	AIN *m_pAins;
	unsigned int m_nAinCount;
	std::vector<AIN*> m_arrAins;

	DIN *m_pDins;
	unsigned int m_nDinCount;
	std::vector<DIN*> m_arrDins;

	DOUT *m_pDouts;
	unsigned int m_nDoutCount;
	std::vector<DOUT*> m_arrDouts;

	AOUT *m_pAouts;
	unsigned int m_nAoutCount;
	std::vector<AOUT*> m_arrAouts;


	TRANSFORM_LINEAR *m_pLinear;
	unsigned int m_nLinearCount;
	std::vector<TRANSFORM_LINEAR*> m_arrLinears;

	TRANSFORM_NONLINEAR *m_pNonLinear;
	unsigned int m_nNonLinearCount;
	std::vector<TRANSFORM_NONLINEAR*> m_arrNonLinears;

	AIN_ALARM  *m_pAinAlarm;
	unsigned int m_nAinAlarmCount;
	std::vector<AIN_ALARM*> m_arrAinAlarms;

	AIN_ALARM_LIMIT *m_pAinAlarmLimit;
	unsigned int m_nAinAlarmLimitCount;
	std::vector<AIN_ALARM_LIMIT*> m_arrAinAlarmLimits;

	DIN_ALARM *m_pDinAlarm;
	unsigned int m_nDinAlarmCount;
	std::vector<DIN_ALARM*> m_arrDinAlarms;

	DIN_ALARM_LIMIT *m_pDinAlarmLimit;
	unsigned int m_nDinAlarmLimitCount;
	std::vector<DIN_ALARM_LIMIT*> m_arrDinAlarmLimits;

	VARDATA * m_pSystemVariable;
	unsigned int m_nSystemVariableCount;
	std::vector<VARDATA* >m_arrSystemVariables;

	VARDATA*  m_pUserVariable;
	unsigned int m_nUserVariableCount;
	std::vector<VARDATA* >m_arrUserVariables;

	FT_CHANNEL *m_pFTChannels;
	unsigned int m_nFTChannelCount;
	//! 记住地址，方便遍历
	std::vector<CHANNEL*> m_arrFTChannelPtrs;
	std::vector<std::shared_ptr<CFTChannel>> m_arrFTChannels;

private:
	//创建TagName存储池
	void BuildTagNamePoolDB(const char* szName);

	size_t BuildTagNameDB(char* pAddress,const std::vector<TAG_OCCNO_DEF>& vec);
	
	void StartMainTask();
	void StopMainTask();

	void MainTask(void *pImpl);
private:
	//! 所谓的内存数据库
	std::shared_ptr<CShareMem> m_pTagMem;

	HEAD_MEM* m_pMagicTagMem;

	std::vector<TAG_OCCNO_DEF> m_arrNodeOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrChannelOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDeviceOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrAinOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDinOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrAoutOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDoutOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrAinAlarmOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDinAlarmOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrAinAlarmLimitOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDinAlarmLimitOccNos;	
	std::vector<TAG_OCCNO_DEF> m_arrTransLinearOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrTransNonLinearOccNos;

	std::vector<TAG_OCCNO_DEF> m_arrSyetemVarOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrUserVarOccNos;

private:
	std::shared_ptr<CShareMem> m_pStringMen;
	HEAD_STRING * m_pMagicStringMem;
	StringUnit * m_pStringUnit;
	std::vector< std::shared_ptr < StringUnit > > m_arrStrings;
	std::unordered_map<int32u, std::shared_ptr < StringUnit > >m_MapNo2String;
private:
	bool BuildStringNamePool(const char* pszPathName);
	bool LoadStringNamePool(const char* pszPatnName);
	bool ReadStringUnit(QXmlStreamReader& reader);
};

#endif // _DBSVC_MODULE_H


/** @}*/

