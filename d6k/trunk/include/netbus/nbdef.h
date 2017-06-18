/*! @file
<PRE>
********************************************************************************
模块名       :  内部网络总线数据结构定义
文件名       :
文件实现功能 :
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author  LiJin
*  @version 1.0
*  @date    2015.09.1
*/
#ifndef NETBUS_DEF_H
#define NETBUS_DEF_H 

#include "../datatypes.h"

#include <string>

extern "C"
{ 

#define  NAME_SIZE  (512+4)  //!TODO 后面统一放到一个公用文件

#define GET_MSG_TIMEOUT		10   // 获取消息超时时间(秒)

	//端口号
#define SERVER_PORT		95271 // 主服务器端口号
#define FES_PORT		95275 // 前置端口号
#define CLIENT_PORT		95277 // 客户端端口号

#define NETBUS_INLINE inline	// 内敛

#define START_WAIT_TIME			3 // 配置主备后，启动等待时间

#define HEARTBEAT_TIMEOUT		5 // 主备切换超时时间

#define NODE_ALIVE_TIME			5 // 节点存活超时时间

#define HOST_NAME_SIZE			20 // 心跳报文主机名长度

#define CHECK_HOST_SLAVE		1 // 检查主备
#define CHECK_NODE_NET_STATE	2 // 检查整个网络节点状态

#define HEARTBEAT_TIMER 1*1000


	// 报文特征码
	const unsigned char  ORIGIN_FLAG = 0x5a;    // 表示开始

	enum : unsigned int
	{
		MAIN  = 1,		 // 主
		SLAVE = 2,		 // 从
	};

	// 工作网络
	enum :int
	{
		NET_A = 0, // A网
		NET_B = 1, // B网
	};

	// 消息级别(数字越大优先级越高)
	enum : int32u
	{
		GENERAL_MSG = 1,		// 普通消息级别,可用于消息全局上送.

		URGENT_MSG = 100,		// 紧急消息级别,可用于变化上送.

		MATTER_MSG = 1000,		// 事项消息级别,用于事项传输.

		CONTROL_MSG = 10000		// 控制消息类型,用于遥控、遥调.
	};

 	/*! \struct  NODE_CONFIG
	*   \brief 系统节点定义 */
	struct NODE_CONFIG
	{
		int32u OccNo;							//!  节点在数据库中的ID
		int32u SlaveOccNo;
		int32u NodeType;						//!  节点类型
		int32u NetAIPAddr;                      //! IP address of net1, filled by cfg. 
		int32u NetBIPAddr;                      //! IP address of net2, filled by cfg. 

		char chIP[2][NAME_SIZE + 1];		    //! AB网IP

		int32u NetAIpV6Addr[4];                 //! IPV6 的A网地址
		int32u NetBIpV6Addr[4];                 //! IPV6 的B网地址

	    int8u IsCheckTime;						//！ 是否对时
		int8u IsDefaultHost;					//！ 是否默认为主
		
		char HostName[NAME_SIZE + 1];		    //！ 主机名称
		char TagName[NAME_SIZE + 1];			//！ 别名
		char GrpName[NAME_SIZE + 1];            //!  组名
		// 实时值
		int32u HostSlave;						//！ 主从状态
		int8u CardStatus[2];					//！ AB网状态

		unsigned long long SwitchTime;			//！ 准备切换基准时间
		unsigned long long TimeNetA;			//！ A网基准时间
		unsigned long long TimeNetB;			//！ B网基准时间
	} ;

	typedef struct NODE_CONFIG CNodeConfig;
//	using CNodeConfig = struct  NODE_CONFIG;

	/*! \struct  NET_CONFIG
	*   \brief 网络节点定义 */
	struct NET_CONFIG
	{
		int32u MyNodeOccNo;                 //! 本节点的排行号
		bool IsDoubleNet;					//! 是否双网
		bool bCheckTime;					//! 是否对时
		int32u CheckTimeNodeOccNo;	        //! 对时节点排行号
		int32u NodeCount;					//! 节点数
		CNodeConfig* pNodeConfig;			//! 节点表指针
	} ;
//	typedef struct NET_CONFIG CNetConfig;
//	using CNetConfig = struct NET_CONFIG;

	const int32u MAX_EMSG_L = (399 * 1024);

	enum EMSG_PATH
	{
		TO_SERVER = 1,      //!< 发送到服务器
		TO_FES = 2,         //!< 发送到前置
		TO_MASTER_FES = 3,
		TO_CLIENT = 4,
		TO_PEER = 5,        //!< 发送到伙伴，对侧冗余节点
		TO_SELF = 6,		
	};
	/*! \struct  EMSG_BUF
	*   \brief 以太网报文 */
	struct EMSG_BUF_HEAD
	{
		int32u  MsgPath;        //! 消息路线

		int32u  MsgType;        //! 消息类型
		int32u  SrcOccNo;       //! 源节点的OccNo
		int32u  DestOccNo;		//! 目的地OccNo
		int32u  MsgDataSize;    //! MsgData 中的有效长度
		int32u  FuncCode;
		int32u  SeqNum;         //! 序号，依次累加	
	};

	/*! \struct  EMSG_BUF
	*   \brief 以太网报文 */
	struct EMSG_BUF
	{
		int32u  MsgPath;        //! 消息路线

		int32u  MsgType;        //! 消息类型
		int32u  SrcOccNo;       //! 源节点的OccNo
		int32u  DestOccNo;		//! 目的地OccNo
		int32u  MsgDataSize;    //! MsgData 中的有效长度
		int32u  FuncCode;
		int32u  SeqNum;         //! 序号，依次累加

		//void* data;
		char BuffData[MAX_EMSG_L];
		//unsigned char MsgData[MAX_EMSG_L]; 
	};

	const int32u EMSG_BUF_HEAD_SIZE = sizeof(EMSG_BUF_HEAD);

	const int32u EMSG_BUF_SIZE = sizeof(EMSG_BUF);

	//传送功能码
	enum class FunCode:int
	{
		//数据上送信息
		 M_AI_NA_1 = 1,          //AI信息
		 M_AI_TA_1 = 2,          //带时标AI信息
		 M_AI_ND_1 = 3,          //不带品质的AI
		 M_DI_NA_1 = 4,          //DI信息
		 M_DI_TA_1 = 5,          //带时标DI信息
		 M_DI_ND_1 = 6,          //不带品质的DI
		
		 //变化上送信息
		 M_AI_NC_1 = 7,          //正常变化上送AI
		 M_AI_SC_1 = 8,          //异常变化上送AI（订阅数据，告警数据）
		 M_DI_NC_1 = 9,          //正常变化上送DI
		 M_DI_SC_1 = 10,         //异常变化上送DI（订阅数据，告警数据）

		 //统计数据上送
		 M_AI_SD_1 = 11,	     //AI统计数据上送
		 M_DI_SD_1 = 12,         //DI统计数据上送

		 //告警数据上送
		 M_AI_AM_1 = 13,         //AI告警数据上送
		 M_DI_AM_1 = 14,         //DI告警数据上送

		 //事件上送
		 M_AI_AE_1 = 15,         //AI事件上送
		 M_DI_AE_1 = 16,         //DI事件上送
	};

	//传送原因
	enum  MSG_TYPE
	{
		COT_PERCYC = 1 ,        //数据周期上送
		COT_SPONT  = 2 ,        //数据突发上送
		COT_ALARM  = 3 ,        //告警信息上送
		COT_EVENT  = 4 ,        //事件信息上送
		COT_SUB    = 5 ,        //订阅信息上送

		COT_SETVAL = 6 ,        //设值数据下发
		COT_REDATA = 7 ,        //请求数据
	};

	

	/************************************** 心跳 **************************************/
	enum
	{
		HEARTBEAT_CODE = 0x6f,	// 心跳特征码
	};

	static const unsigned int g_nSize = 9 + HOST_NAME_SIZE + 1 + 2;

	/*#组播（多播）地址
	#224.0.0.0 -- 224.0.0.255本地保留，给知名协议使用。其中224.0.0.1是本地所有主机接受，224.0.0.2是本网所有路由器接收。
	#224.0.1.0 -- 238.255.255.255预留组播地址，多播地址应从此范围内选择。
	#239.0.0.0 -- 239.255.255.255私用组播地址。
	#232.0.0.0 -- 232.255.255.255特定源组播。*/
	#define MULTICAST_ADDR		"234.5.6.8"

	// #广播端口
	#define DGRAM_PORT_A			59527	 // A网心跳端口
	#define DGRAM_PORT_B			69527	 // B网心跳端口
}



/** @}*/
#endif // NETBUS_DEF_H