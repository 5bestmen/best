/*! @file node.h
<PRE>
********************************************************************************
模块名       :
文件名       :  node.h
文件实现功能 :  系统的节点定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :   
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   系统的节点定义
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.16
*******************************************************************************/
#ifndef FES_CHANNEL_H
#define FES_CHANNEL_H

#include "datatypes.h"

struct CHANNEL;

class CFesChannel
{
public:
	CFesChannel();
	explicit CFesChannel(CHANNEL *pChannel);
	~CFesChannel();

public:

private:
	int32u m_nOccNo;

};

#endif // NODE_H


/** @}*/

