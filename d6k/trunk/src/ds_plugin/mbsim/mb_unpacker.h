

#ifndef MBE_UNPACKER_H
#define MBE_UNPACKER_H


#include "asio_unpacker.h"
#include "mbtcp_def.h"

 
#define MB_BUFF_SIZE 512

class CMBEUnpacker : public ASIO::IUnpacker<std::string>
{
public:
	CMBEUnpacker()
	{
		reset_state();
	}
public:
	virtual void reset_state()
	{
		m_nCurMsgLen = -1;
		m_nRemainLen = 0;
		m_bFindHead = false;

		for (auto i = m_arrRawBuff.begin(); i != m_arrRawBuff.end(); ++i)
		{
			*i = 0;
		}
	}
	/*! \fn bool parse_msg_helper(size_t nBytesTransferred, boost::container::list<std::pair<const char*, size_t>>& lstMsg)
	*********************************************************************************************************
	** \brief ASIO::CUnpacker::parse_msg_helper
	** \details
	** \param nBytesTransferred  已经传输的长度
	** \param lstMsg 返回值
	** \return bool
	** \author LiJin
	** \date 2016年7月11日
	** \note
	********************************************************************************************************/
	bool parse_msg_helper(size_t nBytesTransferred, boost::container::list<std::pair<const char*, size_t>>& lstMsg)
	{
		assert(nBytesTransferred > 0);
		if (nBytesTransferred == 0)
		{
			return false;
		}
		//length + msg
		m_nRemainLen += nBytesTransferred;
		assert(m_nRemainLen <= MB_BUFF_SIZE);
		bool bUnpackOk = true;
		auto itBuff = std::begin(m_arrRawBuff);

		while (bUnpackOk)
		{
			if (m_nCurMsgLen != (size_t)(-1))
			{// 已经有报文
				if (m_nCurMsgLen > MB_TCP_BUF_SIZE)
				{// 溢出了
					bUnpackOk = false;
					return false;
				}
				if (m_nRemainLen >= m_nCurMsgLen)
				{// 一封完整的报文已经收到
					// 真正的报文体
					lstMsg.push_back(std::make_pair(itBuff, m_nCurMsgLen));
					m_nRemainLen -= m_nCurMsgLen;
					std::advance(itBuff, m_nCurMsgLen);
					m_nCurMsgLen = -1;
					bUnpackOk = true;
					break;
				}
				else
				{
					assert(false);
				}
			}
			else if (m_nRemainLen >= MB_TCP_FUNC && m_nCurMsgLen == (size_t)(-1))
			{// 收进来报文了，而且是第一次收的报文
				// 找报文头
			  
				if (m_nRemainLen >= MB_TCP_FUNC)
				{
					auto itBuffCopy = itBuff;
					std::advance(itBuffCopy, 4);
					unsigned char nLen1 = static_cast <unsigned char>(*itBuffCopy);
					std::advance(itBuffCopy, 1);
					unsigned char nLen2 = static_cast <unsigned char>(*itBuffCopy);
					// APDU 长度
					m_nCurMsgLen = nLen1 * 256 + nLen2 + MB_TCP_UID;

					if (m_nCurMsgLen > MB_TCP_BUF_SIZE)
					{// 长度非法 这封报文作废？
						bUnpackOk = false;
						reset_state();
						return false;
					}
				}				
			}
		}
		//we should have at least got one msg. 什么都没有处理到
		if (itBuff == std::begin(m_arrRawBuff))
			bUnpackOk = false;

		return bUnpackOk;
	}

	virtual bool parse_msg(size_t nBytesTransferred, container_type& msg_can)
	{
		boost::container::list< std::pair<const char*, size_t> > lstMsgPosCan; // 收到的报文
		bool bUnpackOk = parse_msg_helper(nBytesTransferred, lstMsgPosCan);

		if (lstMsgPosCan.empty() == false)
		{// 收到一封完整的报文
			auto func_ass = [&msg_can](std::pair<const char*, size_t> & item)
			{
				msg_can.resize(msg_can.size() + 1);
				msg_can.back().assign(item.first, item.second);
			};
			// 把收到的报文 复制到 msg_can 中
			do_something_to_all(lstMsgPosCan, func_ass);
		}

		if (bUnpackOk && m_nRemainLen > 0)
		{// 移动 粘包引起的下一封报文的部分 到buff的头部分
			auto itBuff = std::next(lstMsgPosCan.back().first, lstMsgPosCan.back().second);
			memcpy(std::begin(m_arrRawBuff), itBuff, m_nRemainLen); //left behind unparsed data
		}

		return bUnpackOk;
	}
	//a return value of 0 indicates that the read operation is complete. a non-zero value indicates the maximum number
	//of bytes to be read on the next call to the stream's async_read_some function. ---boost::asio::async_read
	//read as many as possible to reduce asynchronous call-back, and don't forget to handle stick package carefully in parse_msg function.
	virtual size_t completion_condition(const boost::system::error_code& ec, size_t nBytesTransferred)
	{
		/* If we have received the MBAP header we can analyze it and calculate
		* the number of bytes left to complete the current request. If complete
		* notify the protocol stack.
		*/
		if (ec)
		{
			return 0;
		}

		if (nBytesTransferred == 0)
		{// 没有收到，继续收
			return boost::asio::detail::default_max_transfer_size;
		}

		size_t nDataLen = m_nRemainLen + nBytesTransferred;
		assert(nDataLen <= MB_BUFF_SIZE);
		if ((size_t)-1 == m_nCurMsgLen && nDataLen > MB_TCP_FUNC)
		{//刚收到报文，
			auto itBuffCopy = std::begin(m_arrRawBuff);
		 
			std::advance(itBuffCopy, 4);
			unsigned char nLen1 = static_cast <unsigned char>(*itBuffCopy);
			std::advance(itBuffCopy, 1);
			unsigned char nLen2 = static_cast <unsigned char>(*itBuffCopy);
			// 报文 长度
			m_nCurMsgLen = nLen1 * 256 + nLen2 + MB_TCP_UID;

			if (m_nCurMsgLen > MB_TCP_BUF_SIZE || m_nCurMsgLen <= MB_TCP_UID) //invalid msg, stop reading
				return 0;		 
		}
		//read as many as possible except that we have already got an entire msg
		return nDataLen >= m_nCurMsgLen ? 0 : boost::asio::detail::default_max_transfer_size;
	}
	virtual boost::asio::mutable_buffers_1 prepare_next_recv()
	{
		assert(m_nRemainLen < MB_BUFF_SIZE);
		return boost::asio::buffer(boost::asio::buffer(m_arrRawBuff) + m_nRemainLen);
	}
protected:
	boost::array<char, MB_BUFF_SIZE> m_arrRawBuff;

	bool m_bFindHead;    // 找到报文头
	size_t m_nCurMsgLen; // 这一封报文的完整的长度 .-1 means head has not received, so doesn't know the whole msg length.
	size_t m_nRemainLen; // half-baked msg
};

#endif /* MBE_UNPACKER_H */
