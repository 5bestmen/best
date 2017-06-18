/*! @file hb_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  msg_cache.h
�ļ�ʵ�ֹ��� :  ������Ϣ�洢�Ͳ���
����         :  ZhuYongjin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ������Ϣ�洢�Ͳ���
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
