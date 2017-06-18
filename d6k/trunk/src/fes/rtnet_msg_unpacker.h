/*! @file rtnet_msg_packer.h
<PRE>
********************************************************************************
模块名       :
文件名       :  rtnet_msg_packer.h
文件实现功能 :  网络总线模块->数据打包器，从实时数据库中获取数据并组织成报文
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   数据打包器
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

