

#ifndef RLY103_UNPACKER_H
#define RLY103_UNPACKER_H


#include "asio_unpacker.h"


class CRly103Unpacker : public ASIO::IUnpacker<std::string>
{
public:
	CRly103Unpacker()
	{
		reset_state();
	}
public:
	virtual void reset_state()
	{
		m_nCurMsgLen = -1;
		m_nRemainLen = 0;
		m_bFindHead = false;
	}

	/*! \fn bool parse_msg_helper(size_t nBytesTransferred, boost::container::list<std::pair<const char*, size_t>>& lstMsg)
	*********************************************************************************************************
	** \brief ASIO::CUnpacker::parse_msg_helper
	** \details
	** \param nBytesTransferred  �Ѿ�����ĳ���
	** \param lstMsg ����ֵ
	** \return bool
	** \author LiJin
	** \date 2016��7��11��
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
		assert(m_nRemainLen <= ASIO_MSG_BUFFER_SIZE);
		bool bUnpackOk = true;
		auto itBuff = std::begin(m_arrRawBuff);

		while (bUnpackOk)
		{
			if (m_nCurMsgLen != (size_t)(-1))
			{// �Ѿ��б���
				if (m_nCurMsgLen > ASIO_MSG_BUFFER_SIZE )
				{// ����ˣ����� ����ͷû������ȫ
					bUnpackOk = false;
					return false;
				}
				if ( m_nRemainLen >= m_nCurMsgLen )
				{// һ�������ı����Ѿ��յ�
					// �����ı�����
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
			else if (m_nRemainLen >= 3 && m_nCurMsgLen == (size_t)(-1))
			{// �ս��������ˣ������ǵ�һ���յı���
				// �ұ���ͷ
				while (true)
				{
					if ( *itBuff == 0x68 )
					{// �ҵ�����ͷ
						m_bFindHead = true;
						// �յ����ĳ���
						if (m_nRemainLen >= 3)
						{
							auto itBuffCopy = itBuff;
							std::advance(itBuffCopy, 1);
							unsigned char nLen1 = static_cast <unsigned char>(*itBuffCopy);
							std::advance(itBuffCopy, 1);
							unsigned char nLen2 = static_cast <unsigned char>(*itBuffCopy);
							// APDU ����
							m_nCurMsgLen = nLen1 + nLen2*256 + 3;

							if (m_nCurMsgLen > ASIO_MSG_BUFFER_SIZE)
							{// ���ȷǷ� ��ⱨ�����ϣ�
								bUnpackOk = false;
								reset_state();
								return false;
							}
						}
						else
						{// �ҵ�����ͷ������û���ҵ����ĳ��ȣ������´δ���
							bUnpackOk = false;
							return false;
						}
						break;
					}	
					else 
					{// û���ҵ�����ͷ������ǰŲһ��
						if (m_nRemainLen == 0)
						{// �ҵ�ͷ��û���ˡ�
							bUnpackOk = false;
							reset_state();
							return false;
						}
						if (m_nRemainLen > 0)
						{
							m_nRemainLen--;
							Q_ASSERT(itBuff != m_arrRawBuff.end());
							if (itBuff == m_arrRawBuff.end())
							{
								bUnpackOk = false;
								reset_state();
								return false;
							}
							std::advance(itBuff,1);
						}
					}
				}

			}
		}
		//we should have at least got one msg. ʲô��û�д���
		if (itBuff == std::begin(m_arrRawBuff)) 
			bUnpackOk = false;

		return bUnpackOk;
	}

	virtual bool parse_msg(size_t nBytesTransferred, container_type& msg_can)
	{
		boost::container::list< std::pair<const char*, size_t> > lstMsgPosCan; // �յ��ı���
		bool bUnpackOk = parse_msg_helper(nBytesTransferred, lstMsgPosCan);

		if (lstMsgPosCan.empty() == false)
		{// �յ�һ�������ı���
			auto func_ass = [&msg_can](std::pair<const char*, size_t> & item)
			{
				msg_can.resize(msg_can.size() + 1);
				msg_can.back().assign(item.first, item.second);
			};
			// ���յ��ı��� ���Ƶ� msg_can ��
			do_something_to_all(lstMsgPosCan, func_ass);
		}

		if (bUnpackOk && m_nRemainLen > 0)
		{// �ƶ� ճ���������һ�ⱨ�ĵĲ��� ��buff��ͷ����
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
		if (ec)
		{
			return 0;
		}

		if (nBytesTransferred == 0)
		{// û���յ���������
			return boost::asio::detail::default_max_transfer_size;
		}

		size_t nDataLen = m_nRemainLen + nBytesTransferred;
		assert(nDataLen <= ASIO_MSG_BUFFER_SIZE);
		if ((size_t)-1 == m_nCurMsgLen && nDataLen >= 3)
		{//���յ����ģ�
			auto itBuffCopy = std::begin(m_arrRawBuff);
			if (*itBuffCopy == 0x68)
			{				
				std::advance(itBuffCopy, 1);
				unsigned char nLen1 = static_cast <unsigned char>(*itBuffCopy);
				std::advance(itBuffCopy, 1);
				unsigned char nLen2 = static_cast <unsigned char>(*itBuffCopy);
				// APDU ����
				m_nCurMsgLen = nLen1 + nLen2 * 256 + 3;
				
				if (m_nCurMsgLen > ASIO_MSG_BUFFER_SIZE || m_nCurMsgLen < 15) //invalid msg, stop reading
					return 0;
			}
		}
		//read as many as possible except that we have already got an entire msg
		return nDataLen >= m_nCurMsgLen ? 0 : boost::asio::detail::default_max_transfer_size;
	}
	virtual boost::asio::mutable_buffers_1 prepare_next_recv()
	{
		assert(m_nRemainLen < ASIO_MSG_BUFFER_SIZE);
		return boost::asio::buffer(boost::asio::buffer(m_arrRawBuff) + m_nRemainLen);
	}
protected:
	boost::array<char, ASIO_MSG_BUFFER_SIZE> m_arrRawBuff;

	bool m_bFindHead;    // �ҵ�����ͷ
	size_t m_nCurMsgLen; //��һ�ⱨ�ĵ������ĳ��� .-1 means head has not received, so doesn't know the whole msg length.
	size_t m_nRemainLen; //half-baked msg
};



#endif /* RLY103_UNPACKER_H */
