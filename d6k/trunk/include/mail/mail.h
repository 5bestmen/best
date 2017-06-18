/*! @file mail.h
<PRE>
********************************************************************************
模块名       :  邮件模块
文件名       :  mail.h
文件实现功能 :  邮件模块接口定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   邮件模块接口定义
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.15
*******************************************************************************/

#ifndef _MAIL_API_H
#define _MAIL_API_H

#include "../datatypes.h"
#include <QtCore/QtGlobal>

#ifdef MAIL_LIB
# define MAILAPI Q_DECL_EXPORT
#else
# define MAILAPI Q_DECL_IMPORT
#endif
 

extern "C"
{
	enum
	{
		INVALID_MAILBOX_ID = 0,
		MAIL_MAX_SIZE = 1024,	//每封邮件最大字节数
	};

	enum EVT_TYPE
	{
		MSG_EVT0 = 0X00,  //!< 无类型
		MSG_EVT_ANA = 0X01,  //!< 模拟量越复限告警
		MSG_EVT_DIGI = 0X02,  //!< 开关量
		MSG_EVT_DIAG = 0X04,  //!< 自诊断类告警
		MSG_EVT_OPER = 0X08,  //!< 操作类告警
		MSG_EVT_SETVAL = 0X10,  //!< 设值
		MSG_EVT6 = 0X20,        //!< SOE事件
		MSG_EVT7 = 0X40,        //!< 遥控事件
		MSG_EVT8 = 0X80,
		MSG_EVT9 = 0X0100,
		MSG_EVT10 = 0X0200,
		MSG_EVT11 = 0X0400,
		MSG_EVT12 = 0X0800,
		MSG_EVT13 = 0X1000,
		MSG_EVT14 = 0X2000,
		MSG_EVT15 = 0X4000,
		MSG_EVT16 = 0X8000,
		MSG_EVT_ALL = MSG_EVT_ANA | MSG_EVT_DIGI
		| MSG_EVT_DIAG | MSG_EVT_OPER
		| MSG_EVT_SETVAL | MSG_EVT6
		| MSG_EVT7 | MSG_EVT8
		| MSG_EVT9 | MSG_EVT10
		| MSG_EVT11 | MSG_EVT12
		| MSG_EVT13 | MSG_EVT14
		| MSG_EVT15 | MSG_EVT16,
		MSG_EVT_NUM = 16,  //!< 16种消息类型
	};

	struct DMSG
	{
		int32u   SenderID;    //! 发送者ID
		int32u   RecverID;    //! 接收者ID
		int32u   SubID;       //! 订阅者ID
		int32u   Type;        //! 消息类型
		int32u	 Size;		  //! 邮件长度
		int8u	 Buff[MAIL_MAX_SIZE];  //! 邮件正文
	}; 
	/*! \fn MAILAPI bool CreatePostOffice(const char*pszPredicate, size_t nMailBoxNum)
	********************************************************************************************************* 
	** \brief CreatePostOffice 
	** \details 主程序在启动之初创建邮局
	** \param pszPredicate 前缀，比如：SCADA或者FES
	** \param nMailBoxNum 邮箱的数量，通常是驱动数量+转发数量+系统内置
	** \return MAILAPI bool 
	** \author LiJin 
	** \date 2016年10月8日 
	** \note 不同的前缀之间是无法进行通信的，邮件的通信只能在该邮局内进行。就是说，SCADA的邮件只能在SCADA中
	流转，FES只能在前置内部分发。FES与SCADA之间不能进行邮件的流转。
	********************************************************************************************************/
	MAILAPI bool CreatePostOffice(const char*pszPredicate, size_t nMailBoxNum);
	MAILAPI void DestroyPostOffice(const char *pszPredicate);	 
	/*! \fn MAILAPI bool CreateMailBox(const char * pszPredicate, const char *szMailBoxName, int nNum, int nSize,int *pID)
	********************************************************************************************************* 
	** \brief CreateMailBox 
	** \details 创建邮箱，
	** \param pszPredicate 前缀，比如：SCADA或者FES
	** \param szMailBoxName 系统在启动时按照每个邮箱的tagname创建，驱动、转发等以tagname作为邮箱名字.
	** \param nNum 邮箱缓存数量
	** \param nSize 每封邮件最大尺寸
	** \param pID 返回值，邮箱的ID
	** \return MAILAPI bool 
	** \author LiJin 
	** \date 2016年10月8日 
	** \note 
	********************************************************************************************************/
	MAILAPI bool CreateMailBox(const char * pszPredicate, const char *szMailBoxName, int nNum,int *pID);
	MAILAPI bool DestroyMailBox(const char *pszPredicate, const char *szMailBoxName);

	// 打开邮件，只有进程外模块才需要调用，如果不是进程外，可以不调用
	MAILAPI bool OpenPostOffice(const char*pszPredicate);
	// 关闭邮寄
	MAILAPI void ClosePostOffice(const char *pszPredicate);

	// 由客户程序调用，打开邮箱 ，通常由fesapi.dll\scadaapi.dll 针对逻辑做二次封装
	MAILAPI bool OpenMailBox(const char *pszPredicate, const char *szMailBoxName, int *pID);
	//针对已经获取mailID的邮箱系统 直接打开mailbox
	MAILAPI bool OpenMailBoxByID(const char* pszPredicate,int nID);

	MAILAPI bool CloseMailBox(const char *pszPredicate, const char *szMailBoxName);
	//针对已经获取mailID的邮箱系统 直接关闭mailbox
	MAILAPI bool CloseMailBoxByID(const char* pszPredicate, int nID);
	/*! \fn MAILAPI bool SubscribeMail(const char *pszPredicate, const char *szSelfMailBoxName, int32u nType)
	********************************************************************************************************* 
	** \brief SubscribeMail 
	** \details 某个邮箱用户订阅某种类型的邮件
	** \param pszPredicate  前缀，比如：SCADA或者FES
	** \param szSelfMailBoxName 订阅者的邮箱名字
	** \param nType 订阅类型，注意是按位计算
	** \return MAILAPI bool 
	** \author LiJin 
	** \date 2016年10月8日 
	** \note 某个邮箱订阅了某种类型的邮件后，该类型的邮件会自动发送到订阅者的邮箱中
	********************************************************************************************************/
	MAILAPI bool SubscribeMail(const char *pszPredicate, const char *szSelfMailBoxName, int32u nType);

	// 每个邮箱都有一个ID，这个ID由系统启动之初分配好
	MAILAPI int QueryMailBoxID(const char * pszPredicate, const char * szMailBoxName);
  
	MAILAPI bool SendRawMail(const char *pszPredicate, const char * szSendMailBoxName, const char * szRecvMailBoxName, const unsigned char* pMail, int nLength, unsigned int nWaitTimeMs);
	MAILAPI bool SendMail(const char *pszPredicate, DMSG *pMsg, unsigned int nWaitTimeMs);

//	MAILAPI bool RecvMail(const char *pszPredicate,int nRecvMailBoxID, unsigned char* pMail, int nLength, int nWaitTime = 10);
	MAILAPI bool RecvMail(const char *pszPredicate, DMSG *pMsg, unsigned int nWaitTimeMs);
}


#endif // _MAIL_API_H

/** @}*/

