/*! @file hb_svc.h
<PRE>
********************************************************************************
模块名       :
文件名       :  msg_cache.h
文件实现功能 :  冗余消息存储和操作
作者         :  ZhuYongjin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   冗余消息存储和操作
*  @author  ZhuYongjin
*  @version 1.0
*  @date    2016.10.24
*******************************************************************************/
#ifndef MSG_CACHE_H
#define MSG_CACHE_H

#include "netbus/nbdef.h"

class CMsgCache
{
public:
	CMsgCache();	

public:

	char* GetDataBuff();
	size_t  GetDataLen();

	size_t GetCurByteLength();
	size_t GetNeedByteLength();

	void CopyData(const char* pBuff, size_t nLen);
	bool IsEmpty();
	void Clear();

private:
	char m_buf[MAX_EMSG_L + 1];
	size_t  m_nCurPos;
};

#endif // MSG_CACHE_H
