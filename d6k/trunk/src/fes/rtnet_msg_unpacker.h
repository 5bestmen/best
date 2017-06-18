/*! @file rtnet_msg_packer.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  rtnet_msg_packer.h
�ļ�ʵ�ֹ��� :  ��������ģ��->���ݴ��������ʵʱ���ݿ��л�ȡ���ݲ���֯�ɱ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ���ݴ����
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.30
*******************************************************************************/
#ifndef RT_NET_MSG_UNPACKER_H
#define RT_NET_MSG_UNPACKER_H

#include "netmsg_unpacker.h"


template<typename DBSvc>
class CRTNetMsgPacker :public INetMsgUnPacker
{
public:
	CRTNetMsgPacker(std::shared_ptr<DBSvc> & pDB) : m_pDBSvc(pDB)
	{
		assert(pDB);
	}



private:
	std::shared_ptr<DBSvc> &   m_pDBSvc;
};


#endif // RT_NET_MSG_UNPACKER_H

/** @}*/

