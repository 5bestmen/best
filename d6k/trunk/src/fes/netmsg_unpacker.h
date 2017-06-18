/*! @file netmsg_packer.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  netmsg_packer.h
�ļ�ʵ�ֹ��� :  ��̫����Ϣ�����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��̫����Ϣ�����
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
	// �̳������ʵ�ֵĽӿ�
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

