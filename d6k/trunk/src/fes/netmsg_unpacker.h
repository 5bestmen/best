/*! @file netmsg_packer.h
<PRE>
********************************************************************************
模块名       :
文件名       :  netmsg_packer.h
文件实现功能 :  以太网消息打包器
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   以太网消息打包器
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.29
*******************************************************************************/
#ifndef NET_UNPACKETAGE_H
#define NET_UNPACKETAGE_H

#include "log/log.h"
#include <vector>
#include <tuple>

#include <cassert>
#include <QObject>
#include <boost/list.hpp>

template<typename MsgType>
class INetMsgUnPacker
{
public:
	typedef MsgType msg_type;
	typedef const msg_type msg_ctype;
	typedef boost::container::list<msg_type> container_type;
protected:
	virtual ~INetMsgUnPacker()
	{

	}
public:
	// 继承类必须实现的接口
	virtual void ResetState() = 0;
	virtual bool ParseMsg(size_t nBytesTransferred, container_type& msg_can) = 0;
	virtual size_t CompletionCondition(const boost::system::error_code& ec, size_t nBytesTransferred) = 0;
//	virtual buffer_type prepare_next_recv() = 0;
};


class CNetMsgUnPacker : public INetMsgUnPacker <std::vector<unsigned char> >
{
public:
	CNetMsgUnPacker()
	{
		ResetState();
	}

	virtual void ResetState()
	{
		m_nCurMsgLen = -1; 
		m_nRemainLen = 0; 
	}
//	virtual bool ParseMsg(size_t nBytesTransferred, boost::container::list<std::pair<const char*, size_t>>& msg_can)
	virtual bool ParseMsg(size_t nBytesTransferred, container_type& msg_can)
	{
		boost::container::list<std::pair<const unsigned char*, size_t>> msg_pos_can;

		return true;
	}

	bool ParseMsg(size_t nBytesTransferred, boost::container::list<std::pair<const unsigned char*, size_t>>& msg_can)
	{
		m_nRemainLen += nBytesTransferred;

		
		return true;
	}

protected:

	size_t m_nCurMsgLen; //! -1 means head not received, so msg length is not available.
	size_t m_nRemainLen; //! half-baked msg

};


#endif // NET_UNPACKETAGE_H


/** @}*/

