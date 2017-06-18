

#ifndef ASIO_UNPACKER_H_
#define ASIO_UNPACKER_H_

#include <boost/array.hpp>
#include <boost/container/list.hpp>

//#include "asio_base.h"

#ifdef ASIO_HUGE_MSG
#define ASIO_HEAD_TYPE	uint32_t
#define ASIO_HEAD_N2H	ntohl
#else
#define ASIO_HEAD_TYPE	uint16_t
#define ASIO_HEAD_N2H	ntohs
#endif
#define ASIO_HEAD_LEN	(sizeof(ASIO_HEAD_TYPE))

namespace ASIO
{
	// ������ӿ�
	template<typename MsgType>
	class IUnpacker
	{
	public:
		typedef MsgType msg_type;
		typedef const msg_type msg_ctype;
		typedef boost::container::list<msg_type> container_type;

	protected:
		virtual ~IUnpacker()
		{
		}

	public:
		//���ý����״̬��
		virtual void reset_state() = 0;
		//tcp_socket�յ��ʵ�����֮��completion_condition����0������ñ��ӿڣ��Ա���unpacker���Խ�����Ϣ��
		//�ɹ�������������Ϣͨ��msg_can���ء�
		virtual bool parse_msg(size_t bytes_transferred, container_type& msg_can) = 0;
		//���������ݽ��չ����У��ж��Ƿ�һ����������Ϣ�Ѿ��յ����ǾͿ��Խ����ˣ����������ֱ����asioʹ�á�
		//��С��������Э��ͽ�����������������ƿ���ͻ����������֮�ϣ��Ƽ���Э���ʽ�ǣ����ȣ����ݡ�
		//�Ƽ���completion_condition��ʵ�־���Ĭ�ϵ�unpacker��ʵ�֡�
		virtual size_t completion_condition(const boost::system::error_code& ec, size_t bytes_transferred) = 0;
		//ÿ�η����첽����async_read��֮ǰ�������prepare_next_recv���Եõ�һ�����õ�buffer����������ʹ�á�
		virtual boost::asio::mutable_buffers_1 prepare_next_recv() = 0;

		template<typename _Can, typename _Predicate>
		void do_something_to_all(_Can& __can, const _Predicate& __pred)
		{
			for (auto& item : __can)
				__pred(item);
		}
	};

	template<typename MsgType>
	class udp_msg : public MsgType
	{
	public:
		boost::asio::ip::udp::endpoint peer_addr;

		udp_msg() 
		{
		}

		udp_msg(const boost::asio::ip::udp::endpoint& _peer_addr, MsgType&& msg) : MsgType(std::move(msg)), peer_addr(_peer_addr) 
		{
		}

		void swap(udp_msg& other)
		{
			std::swap(peer_addr, other.peer_addr); 
			MsgType::swap(other); 
		}
		void swap(boost::asio::ip::udp::endpoint& addr, MsgType&& tmp_msg)
		{ 
			std::swap(peer_addr, addr); 
			MsgType::swap(tmp_msg); 
		}
	};

	template<typename MsgType>
	class i_udp_unpacker
	{
	public:
		using msg_type = MsgType;
		using msg_ctype = const msg_type;
		using container_type = boost::container::list<udp_msg<msg_type>>;
		//typedef MsgType msg_type;
		//typedef const msg_type msg_ctype;
		//typedef boost::container::list<udp_msg<msg_type>> container_type;

	protected:
		virtual ~i_udp_unpacker() 
		{
		}
	public:
		virtual void reset_state() 
		{
		}
		//�����������udp��˵��һ�ν��վ���һ�������������Ƿ���һ������������ճ�����⡣
		virtual msg_type parse_msg(size_t bytes_transferred) = 0;
		//����һ��asio��Ҫ��buffer������ο�i_unpacker��
		virtual boost::asio::mutable_buffers_1 prepare_next_recv() = 0;
	};
	//Ĭ�ϵĽ�������������Ҫ����Ϣ��ʽ��Ĭ�Ͻ��������Ϣ��ʽ����ͬ����ô��Ȼ�ģ�����Ҫʵ���Լ��Ľ������
	//Ĭ�Ͻ��������Ϣ��ʽ�ǣ����ȣ����ֽڣ�������Ϣ���ݣ�������֧�ֵ���Ϣ����Ϊ1��(65535 - 2)�������ͻ��������ƣ�
	//��ο�i_packer��
	class CUnpacker : public IUnpacker<std::string>
	{
	public:
		CUnpacker() 
		{
			reset_state();
		}
		//current msg's total length, -1 means don't know
		size_t current_msg_length() const
		{
			return m_nCurMsgLen; 
		} 

		/*! \fn bool parse_msg_helper(size_t nBytesTransferred, boost::container::list<std::pair<const char*, size_t>>& lstMsg)
		********************************************************************************************************* 
		** \brief ASIO::CUnpacker::parse_msg_helper 
		** \details 
		** \param nBytesTransferred 
		** \param * 
		** \param lstMsg ����ֵ
		** \return bool 
		** \author LiJin 
		** \date 2016��7��8�� 
		** \note 
		********************************************************************************************************/
		bool parse_msg_helper(size_t nBytesTransferred, boost::container::list<std::pair<const char*, size_t>>& lstMsg)
		{
			//length + msg
			m_nRemainLen += nBytesTransferred;
			assert(m_nRemainLen <= ASIO_MSG_BUFFER_SIZE);

			auto pNext = std::begin(m_arrRawBuff);
			bool bUnpackOk = true;
			// ճ��
			while (bUnpackOk) //considering stick package problem, we need a loop
			{
				if ((size_t)-1 != m_nCurMsgLen)
				{
					if (m_nCurMsgLen > ASIO_MSG_BUFFER_SIZE || m_nCurMsgLen <= ASIO_HEAD_LEN)
						bUnpackOk = false;
					else if (m_nRemainLen >= m_nCurMsgLen) //one msg received
					{
						// �����ı�����
						lstMsg.push_back(std::make_pair(std::next(pNext, ASIO_HEAD_LEN), m_nCurMsgLen - ASIO_HEAD_LEN));
						m_nRemainLen -= m_nCurMsgLen;
						std::advance(pNext, m_nCurMsgLen);
						m_nCurMsgLen = -1;
					}
					else
						break;
				}
				else if (m_nRemainLen >= ASIO_HEAD_LEN) //the msg's head been received, stick package found
				{
					ASIO_HEAD_TYPE head;
					memcpy(&head, pNext, ASIO_HEAD_LEN);
					m_nCurMsgLen = ASIO_HEAD_N2H(head);
				}
				else
					break;
			}


			if (pNext == std::begin(m_arrRawBuff)) //we should have at least got one msg.
				bUnpackOk = false;

			return bUnpackOk;
		}

	public:
		virtual void reset_state() 
		{
			m_nCurMsgLen = -1;
			m_nRemainLen = 0;
		}

		virtual bool parse_msg(size_t bytes_transferred, container_type& msg_can)
		{
			boost::container::list<std::pair<const char*, size_t>> msg_pos_can;
			bool bUnpackOk = parse_msg_helper(bytes_transferred, msg_pos_can);
			do_something_to_all(msg_pos_can, [&msg_can](decltype( *std::begin(msg_pos_can)) & item) 
			{
				msg_can.resize(msg_can.size() + 1);
				msg_can.back().assign(item.first, item.second);
			});

			if (bUnpackOk && m_nRemainLen > 0)
			{
				auto pnext = std::next(msg_pos_can.back().first, msg_pos_can.back().second);
				memcpy(std::begin(m_arrRawBuff), pnext, m_nRemainLen); //left behind unparsed data
			}

			//if unpacking failed, successfully parsed msgs will still returned via msg_can(stick package), please note.
			return bUnpackOk;
		}

		//a return value of 0 indicates that the read operation is complete. a non-zero value indicates the maximum number
		//of bytes to be read on the next call to the stream's async_read_some function. ---boost::asio::async_read
		//read as many as possible to reduce asynchronous call-back, and don't forget to handle stick package carefully in parse_msg function.
		virtual size_t completion_condition(const boost::system::error_code& ec, size_t bytes_transferred)
		{
			if (ec)
				return 0;

			auto data_len = m_nRemainLen + bytes_transferred;
			assert(data_len <= ASIO_MSG_BUFFER_SIZE);

			if ((size_t)-1 == m_nCurMsgLen && data_len >= ASIO_HEAD_LEN) //the msg's head been received
			{
				ASIO_HEAD_TYPE head;
				memcpy(&head, std::begin(m_arrRawBuff), ASIO_HEAD_LEN);
				m_nCurMsgLen = ASIO_HEAD_N2H(head);
				if (m_nCurMsgLen > ASIO_MSG_BUFFER_SIZE || m_nCurMsgLen <= ASIO_HEAD_LEN) //invalid msg, stop reading
					return 0;
			}

			return data_len >= m_nCurMsgLen ? 0 : boost::asio::detail::default_max_transfer_size;
			//read as many as possible except that we have already got an entire msg
		}

		virtual boost::asio::mutable_buffers_1 prepare_next_recv()
		{
			assert(m_nRemainLen < ASIO_MSG_BUFFER_SIZE);
			return boost::asio::buffer(boost::asio::buffer(m_arrRawBuff) + m_nRemainLen);
		}

	protected:
		boost::array<char, ASIO_MSG_BUFFER_SIZE> m_arrRawBuff;
		size_t m_nCurMsgLen; //-1 means head has not received, so doesn't know the whole msg length.
		size_t m_nRemainLen; //half-baked msg
	};
	// Ĭ�ϵ�udp���������������ͬpacker��
	class udp_unpacker : public i_udp_unpacker<std::string>
	{
	public:
		virtual msg_type parse_msg(size_t bytes_transferred)
		{ 
			assert(bytes_transferred <= ASIO_MSG_BUFFER_SIZE);
			return msg_type(raw_buff.data(), bytes_transferred);
		}
		virtual boost::asio::mutable_buffers_1 prepare_next_recv()
		{ 
			return boost::asio::buffer(raw_buff); 
		}

	protected:
		boost::array<char, ASIO_MSG_BUFFER_SIZE> raw_buff;
	};

	class replaceable_unpacker : public IUnpacker<replaceable_buffer>
	{
	public:
		virtual void reset_state()
		{ 
			unpacker_.reset_state();
		}
		virtual bool parse_msg(size_t bytes_transferred, container_type& msg_can)
		{
			CUnpacker::container_type tmp_can;
			auto unpack_ok = unpacker_.parse_msg(bytes_transferred, tmp_can);
			do_something_to_all(tmp_can, [&msg_can](decltype(*std::begin(tmp_can))& item) {
				auto com = boost::make_shared<buffer>();
				com->swap(item);
				msg_can.resize(msg_can.size() + 1);
				msg_can.back().raw_buffer(com);
			});

			//if unpacking failed, successfully parsed msgs will still returned via msg_can(stick package), please note.
			return unpack_ok;
		}

		virtual size_t completion_condition(const boost::system::error_code& ec, size_t bytes_transferred) { return unpacker_.completion_condition(ec, bytes_transferred); }
		virtual boost::asio::mutable_buffers_1 prepare_next_recv() 
		{
			return unpacker_.prepare_next_recv();
		}

	protected:
		CUnpacker unpacker_;
	};

	class replaceable_udp_unpacker : public i_udp_unpacker<replaceable_buffer>
	{
	public:
		virtual msg_type parse_msg(size_t bytes_transferred)
		{
			assert(bytes_transferred <= ASIO_MSG_BUFFER_SIZE);
			auto com = boost::make_shared<buffer>();
			com->assign(raw_buff.data(), bytes_transferred);
			return msg_type(com);
		}
		virtual boost::asio::mutable_buffers_1 prepare_next_recv() 
		{
			return boost::asio::buffer(raw_buff);
		}

	protected:
		boost::array<char, ASIO_MSG_BUFFER_SIZE> raw_buff;
	};
	//����������unpacker�Ĳ�֮ͬ�����ڣ�������Ҫһ���̶���С�Ļ��棬�����Ƚ��հ�ͷ���ٸ��ݰ�ͷ�õ���Ϣ�ĳ�����Ϣ��
	//Ȼ����䣨new���ʵ��Ļ��棬����������д����֮��һ����������Ϣ�ͽ�������ˣ�����ʡ������Ϣ�Ŀ�������Ҳ��һ��������
	//����ÿ��ֻ�ܽ���һ����Ϣ��ÿ����Ϣ��Ҫ����parse_msg���Ρ�
	//��������Ϣ��С��Ƶ��������ճ������ô��unpackerЧ�ʸ��ߣ���Ϊ��һ�ο��Խ��ն��������unbuffered_unpacker����һ��
	//�ô���unpacker�޷�����ģ��Ǿ��ǣ���������Ϣ���̷ǳ����룬������Ϣ�󲿷���1K�������ٵ�����ֻ��һ����Ϣ��1M��
	//��ô���е�unpacker���ò�����һ������1M�Ļ������ڽ�����Ϣ��������ڴ�ľ޴��˷ѡ�
	//this CUnpacker demonstrate how to forbid memory copy while parsing msgs.
	class unbuffered_unpacker : public IUnpacker<inflexible_buffer>
	{
	public:
		unbuffered_unpacker() 
		{ 
			reset_state(); 
		}
		size_t current_msg_length() const
		{ 
			return raw_buff.size(); 
		} //current msg's total length(not include the head), 0 means don't know

	public:
		virtual void reset_state()
		{ 
			raw_buff.detach(); step = 0; 
		}
		virtual bool parse_msg(size_t bytes_transferred, container_type& msg_can)
		{
			if (0 == step) //the head been received
			{
				assert(!raw_buff.empty());
				step = 1;
			}
			else if (1 == step) //the body been received
			{
				assert(!raw_buff.empty());
				if (bytes_transferred != raw_buff.size())
					return false;

				msg_can.resize(msg_can.size() + 1);
				msg_can.back().swap(raw_buff);
				step = 0;
			}

			return -1 != step;
		}

		//a return value of 0 indicates that the read operation is complete. a non-zero value indicates the maximum number
		//of bytes to be read on the next call to the stream's async_read_some function. ---boost::asio::async_read
		virtual size_t completion_condition(const boost::system::error_code& ec, size_t bytes_transferred)
		{
			if (ec)
				return 0;

			if (0 == step) //want the head
			{
				assert(raw_buff.empty());

				if (bytes_transferred < ASIO_HEAD_LEN)
					return boost::asio::detail::default_max_transfer_size;

				assert(ASIO_HEAD_LEN == bytes_transferred);
				auto cur_msg_len = ASIO_HEAD_N2H(head) - ASIO_HEAD_LEN;
				if (cur_msg_len > ASIO_MSG_BUFFER_SIZE - ASIO_HEAD_LEN) //invalid msg, stop reading
					step = -1;
				else
					raw_buff.attach(new char[cur_msg_len], cur_msg_len);
			}
			else if (1 == step) //want the body
			{
				assert(!raw_buff.empty());
				return boost::asio::detail::default_max_transfer_size;
			}
			else
				assert(false);

			return 0;
		}

		virtual boost::asio::mutable_buffers_1 prepare_next_recv()
		{ 
			return raw_buff.empty() ? boost::asio::buffer((char*)&head, ASIO_HEAD_LEN) : boost::asio::buffer(raw_buff.data(), raw_buff.size()); 
		}

	private:
		ASIO_HEAD_TYPE head;
		//please note that we don't have a fixed size array with maximum size any more(like the default CUnpacker).
		//this is very useful if you have very few but very large msgs, fox example:
		//you have a very large msg(1M size), but all others are very small, if you use a fixed size array to hold msgs in the unpackers,
		//all the unpackers must have an array with at least 1M size, each st_socket will have a CUnpacker, this will cause your application occupy very large memory but with very low utilization ratio.
		//this unbuffered_unpacker will resolve above problem, and with another benefit: no memory replication needed any more.
		msg_type raw_buff;
		int step; //-1-error format, 0-want the head, 1-want the body
	};

	class fixed_length_unpacker : public IUnpacker<std::string>
	{
	public:
		fixed_length_unpacker()
		{ 
			reset_state(); 
		}

		void fixed_length(size_t fixed_length)
		{ 
			assert(0 < fixed_length && fixed_length <= ASIO_MSG_BUFFER_SIZE);
			_fixed_length = fixed_length; 
		}
		size_t fixed_length() const 
		{
			return _fixed_length;
		}
	public:
		virtual void reset_state() 
		{ 
			remain_len = 0;
		}
		virtual bool parse_msg(size_t bytes_transferred, container_type& msg_can)
		{
			//length + msg
			remain_len += bytes_transferred;
			assert(remain_len <= ASIO_MSG_BUFFER_SIZE);

			auto pnext = std::begin(raw_buff);
			while (remain_len >= _fixed_length) //considering stick package problem, we need a loop
			{
				msg_can.resize(msg_can.size() + 1);
				msg_can.back().assign(pnext, _fixed_length);
				remain_len -= _fixed_length;
				std::advance(pnext, _fixed_length);
			}

			if (pnext == std::begin(raw_buff)) //we should have at least got one msg.
				return false;
			else if (remain_len > 0) //left behind unparsed msg
				memcpy(std::begin(raw_buff), pnext, remain_len);

			return true;
		}

		//a return value of 0 indicates that the read operation is complete. a non-zero value indicates the maximum number
		//of bytes to be read on the next call to the stream's async_read_some function. ---boost::asio::async_read
		//read as many as possible to reduce asynchronous call-back(st_tcp_socket_base::recv_handler), and don't forget to handle
		//stick package carefully in parse_msg function.
		virtual size_t completion_condition(const boost::system::error_code& ec, size_t bytes_transferred)
		{
			if (ec)
				return 0;

			auto data_len = remain_len + bytes_transferred;
			assert(data_len <= ASIO_MSG_BUFFER_SIZE);

			return data_len >= _fixed_length ? 0 : boost::asio::detail::default_max_transfer_size;
			//read as many as possible except that we have already got an entire msg
		}

		virtual boost::asio::mutable_buffers_1 prepare_next_recv()
		{
			assert(remain_len < ASIO_MSG_BUFFER_SIZE);
			return boost::asio::buffer(boost::asio::buffer(raw_buff) + remain_len);
		}

	private:
		boost::array<char, ASIO_MSG_BUFFER_SIZE> raw_buff;
		size_t _fixed_length;
		size_t remain_len; //half-baked msg
	};

	class prefix_suffix_unpacker : public IUnpacker<std::string>
	{
	public:
		prefix_suffix_unpacker()
		{ 
			reset_state();
		}

		void prefix_suffix(const std::string& prefix, const std::string& suffix)
		{ 
			assert(!suffix.empty() && prefix.size() + suffix.size() < ASIO_MSG_BUFFER_SIZE); 
			_prefix = prefix;
			_suffix = suffix;
		}
		const std::string& prefix() const 
		{ 
			return _prefix;
		}
		const std::string& suffix() const 
		{
			return _suffix;
		}

		size_t peek_msg(size_t data_len, const char* buff)
		{
			assert(nullptr != buff);

			if ((size_t)-1 == first_msg_len && data_len >= _prefix.size())
			{
				if (0 != memcmp(_prefix.data(), buff, _prefix.size()))
					return 0; //invalid msg, stop reading
				else
					first_msg_len = 0; //prefix been checked.
			}

			auto min_len = _prefix.size() + _suffix.size();
			if (data_len > min_len)
			{
				auto end = (const char*)memmem(std::next(buff, _prefix.size()), data_len - _prefix.size(), _suffix.data(), _suffix.size());
				if (nullptr != end)
				{
					first_msg_len = std::distance(buff, end) + _suffix.size(); //got a msg
					return 0;
				}
				else if (data_len >= ASIO_MSG_BUFFER_SIZE)
					return 0; //invalid msg, stop reading
			}

			return boost::asio::detail::default_max_transfer_size; //read as many as possible
		}

		//like strstr, except support \0 in the middle of mem and sub_mem
		static const void* memmem(const void* mem, size_t len, const void* sub_mem, size_t sub_len)
		{
			if (nullptr != mem && nullptr != sub_mem && sub_len <= len)
			{
				auto valid_len = len - sub_len;
				for (size_t i = 0; i <= valid_len; ++i, mem = (const char*)mem + 1)
					if (0 == memcmp(mem, sub_mem, sub_len))
						return mem;
			}

			return nullptr;
		}

	public:
		virtual void reset_state() 
		{
			first_msg_len = -1; 
			remain_len = 0;
		}
		virtual bool parse_msg(size_t bytes_transferred, container_type& msg_can)
		{
			//length + msg
			remain_len += bytes_transferred;
			assert(remain_len <= ASIO_MSG_BUFFER_SIZE);

			auto min_len = _prefix.size() + _suffix.size();
			auto unpack_ok = true;
			auto pnext = std::begin(raw_buff);
			while ((size_t)-1 != first_msg_len && 0 != first_msg_len)
			{
				assert(first_msg_len > min_len);
				auto msg_len = first_msg_len - min_len;

				msg_can.resize(msg_can.size() + 1);
				msg_can.back().assign(std::next(pnext, _prefix.size()), msg_len);
				remain_len -= first_msg_len;
				std::advance(pnext, first_msg_len);
				first_msg_len = -1;

				if (boost::asio::detail::default_max_transfer_size == peek_msg(remain_len, pnext))
					break;
				else if ((size_t)-1 == first_msg_len)
					unpack_ok = false;
			}

			if (pnext == std::begin(raw_buff)) //we should have at least got one msg.
				return false;
			else if (unpack_ok && remain_len > 0)
				memcpy(std::begin(raw_buff), pnext, remain_len); //left behind unparsed msg

			//if unpacking failed, successfully parsed msgs will still returned via msg_can(stick package), please note.
			return unpack_ok;
		}

		//a return value of 0 indicates that the read operation is complete. a non-zero value indicates the maximum number
		//of bytes to be read on the next call to the stream's async_read_some function. ---boost::asio::async_read
		//read as many as possible to reduce asynchronous call-back(st_tcp_socket_base::recv_handler), and don't forget to handle
		//stick package carefully in parse_msg function.
		virtual size_t completion_condition(const boost::system::error_code& ec, size_t bytes_transferred)
		{
			if (ec)
				return 0;

			auto data_len = remain_len + bytes_transferred;
			assert(data_len <= ASIO_MSG_BUFFER_SIZE);

			return peek_msg(data_len, std::begin(raw_buff));
		}

		virtual boost::asio::mutable_buffers_1 prepare_next_recv()
		{
			assert(remain_len < ASIO_MSG_BUFFER_SIZE);
			return boost::asio::buffer(boost::asio::buffer(raw_buff) + remain_len);
		}

	private:
		boost::array<char, ASIO_MSG_BUFFER_SIZE> raw_buff;
		std::string _prefix, _suffix;
		size_t first_msg_len;
		size_t remain_len; //half-baked msg
	};

} //namespace

#endif /* ASIO_UNPACKER_H_ */
