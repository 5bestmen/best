 

#ifndef ASIO_PACKER_H_
#define ASIO_PACKER_H_

#include <string>

#include <boost/smart_ptr.hpp>
#include "asio_buffer.h"

//#include "st_asio_wrapper_base.h"

#ifdef ASIO_HUGE_MSG
#define ASIO_HEAD_TYPE	uint32_t
#define ASIO_HEAD_H2N	htonl
#else
#define ASIO_HEAD_TYPE	uint16_t
#define ASIO_HEAD_H2N	htons
#endif
#define ASIO_HEAD_LEN	(sizeof(ASIO_HEAD_TYPE))
//the size of the buffer used when receiving msg, must equal to or larger than the biggest msg size,
//the bigger this buffer is, the more msgs can be received in one time if there are enough msgs buffered in the SOCKET.
//every unpackers have a fixed buffer with this size, every st_tcp_sockets have an unpacker, so, this size is not the bigger the better.
//if you customized the packer and unpacker, the above principle maybe not right anymore, it should depends on your implementations.
#ifndef ASIO_MSG_BUFFER_SIZE
#define ASIO_MSG_BUFFER_SIZE	4000
#endif

namespace ASIO
{
	//�������helper�࣬Ŀǰֻ��һ�������������ж�Ҫ�������Ϣ���ڴ�����Ƿ񳬹�����󳤶�
	class CPackerHelper
	{
	public:
		//return (size_t) -1 means length exceeded the ASIO_MSG_BUFFER_SIZE
		static size_t msg_size_check(size_t pre_len, const char* const pstr[], const size_t len[], size_t num)
		{
			if (nullptr == pstr || nullptr == len)
				return -1;

			auto total_len = pre_len;
			auto last_total_len = total_len;
			for (size_t i = 0; i < num; ++i)
			{
				if (nullptr != pstr[i])
				{
					total_len += len[i];
					if (last_total_len > total_len || total_len > ASIO_MSG_BUFFER_SIZE) //overflow
					{
					//	unified_out::error_out("pack msg error: length exceeded the ASIO_MSG_BUFFER_SIZE!");
						return -1;
					}
					last_total_len = total_len;
				}
			}

			return total_len;
		}
	};

	template<typename MsgType>
	class IPacker
	{
	public:
		typedef MsgType msg_type;
		typedef const msg_type msg_ctype;

	protected:
		virtual ~IPacker() 
		{

		}
	public:
		virtual void reset_state()
		{

		}
		//������������ʹ���ߵ���send_msg, broadcast_msg, post_msg�������nativeΪfalse���ñ�������
		//���ʹ�ö�����send_native_msg, broadcast_native_msg, post_native_msg�������nativeΪtrue���ñ�������
		virtual msg_type pack_msg(const char* const pstr[], const size_t len[], size_t num, bool native = false) = 0;
		//��һ������������У��õ�ԭʼ���ݡ�
		virtual char* raw_data(msg_type& msg) const
		{
			std::ignore = msg;
			return nullptr;
		}
		virtual const char* raw_data(msg_ctype& msg) const
		{
			std::ignore = msg;
			return nullptr;
		}
		virtual size_t raw_data_len(msg_ctype& msg) const
		{ 
			std::ignore = msg;
			return 0; 
		}

		msg_type pack_msg(const char* pstr, size_t len, bool native = false)
		{ 
			return pack_msg(&pstr, &len, 1, native);
		}

		msg_type pack_msg(const std::string& str, bool native = false)
		{
			return pack_msg(str.data(), str.size(), native);
		}
	};
	//Ĭ�ϵĴ�������������Ҫ����Ϣ��ʽ��Ĭ�ϴ��������Ϣ��ʽ����ͬ����ô��Ȼ�ģ�����Ҫʵ���Լ��Ĵ������
	//	Ĭ�ϴ��������Ϣ��ʽ�ǣ����ȣ����ֽڣ�������Ϣ���ݣ�������֧�ֵ���Ϣ����Ϊ1��(65535 - 2)�������ͻ��������ƣ�
	//	���Զ���HUGE_MSG�꣬��ô����֧�ֵ�4G���ȡ�
	//	ע�⣬����Ϣ��ռ�������񲻵��Ĵ��ڴ棬���������Ϣ����Ϊ1M������������󣩣���ô����ÿ��st_tcp_socket����һ���������
	//	ÿ���������������һ������1M��С�Ľ��ջ��棬��ô1024��st_tcp_socket�����ǽ���������ռ��1G�ڴ棬�ٿ���Ĭ�ϵĽ��շ��ͻ���
	//	�ֱ�������1024����Ϣ����ôÿ��st_tcp_socket�ڽ��շ��ͻ��涼��������£���ռ��2 * 1024 * 1M = 2G�ڴ棬��ô1024��st_tcp_socket
	//	�ͽ�ռ��2T�ڴ档�����Ƽ��������ǣ���������Ϣ�ܳ�����ôһ��Ҫ������С���շ��ͻ���ĳ��ȣ���������Ϊ1����Ϣ��
	class CPacker : public IPacker<std::string>
	{
	public:
		static size_t get_max_msg_size()
		{ 
			return ASIO_MSG_BUFFER_SIZE - ASIO_HEAD_LEN; 
		}

		using IPacker<msg_type>::pack_msg;
		virtual msg_type pack_msg(const char* const pstr[], const size_t len[], size_t num, bool native = false)
		{
			msg_type msg;
			auto pre_len = native ? 0 : ASIO_HEAD_LEN;
			auto total_len = CPackerHelper::msg_size_check(pre_len, pstr, len, num);
			if ((size_t)-1 == total_len)
				return msg;
			else if (total_len > pre_len)
			{
				if (!native)
				{
					auto head_len = (ASIO_HEAD_TYPE)total_len;
					if (total_len != head_len)
					{
					//	unified_out::error_out("pack msg error: length exceeded the header's range!");
						return msg;
					}

					head_len = ASIO_HEAD_H2N(head_len);
					msg.reserve(total_len);
					msg.append((const char*)&head_len, ASIO_HEAD_LEN);
				}
				else
					msg.reserve(total_len);

				for (size_t i = 0; i < num; ++i)
					if (nullptr != pstr[i])
						msg.append(pstr[i], len[i]);
			} //if (total_len > pre_len)
			return msg;
		}

		virtual char* raw_data(msg_type& msg) const
		{ 
			return const_cast<char*>(std::next(msg.data(), ASIO_HEAD_LEN));
		}

		virtual const char* raw_data(msg_ctype& msg) const 
		{ 
			return std::next(msg.data(), ASIO_HEAD_LEN);
		}

		virtual size_t raw_data_len(msg_ctype& msg) const 
		{ 
			return msg.size() - ASIO_HEAD_LEN;
		}
	};
	//������������ʱ�滻������Ļ������԰�������������ΪĬ�ϴ������������������replaceable_buffer��������replaceable_buffer����
	//	������һ��i_bufferָ�룬����ֻҪ��ʵ����i_buffer�ӿڵĶ��󣬶��ܸ���replaceable_buffer��������ο�replaceable_buffer��i_buffer�Ķ��塣
	class CReplaceablePacker : public IPacker<replaceable_buffer>
	{
	public:
		using IPacker<msg_type>::pack_msg;
		virtual msg_type pack_msg(const char* const pstr[], const size_t len[], size_t num, bool native = false)
		{
			CPacker p;
			auto msg = p.pack_msg(pstr, len, num, native);
			auto com = boost::make_shared<buffer>();
			com->swap(msg);

			return msg_type(com);
		}

		virtual char* raw_data(msg_type& msg) const
		{ 
			return const_cast<char*>(std::next(msg.data(), ASIO_HEAD_LEN));
		}

		virtual const char* raw_data(msg_ctype& msg) const 
		{
			return std::next(msg.data(), ASIO_HEAD_LEN);
		}

		virtual size_t raw_data_len(msg_ctype& msg) const 
		{
			return msg.size() - ASIO_HEAD_LEN; 
		}
	};
	//���̶�ͷ�͹̶�β�Ĵ������ͷ����Ϊ�գ�β����Ϊ�ա�
	class CPrefixSuffixPacker : public IPacker<std::string>
	{
	public:
		void prefix_suffix(const std::string& prefix, const std::string& suffix)
		{
			assert(!suffix.empty() && prefix.size() + suffix.size() < ASIO_MSG_BUFFER_SIZE);
			m_szPrefix = prefix;
			m_szSuffix = suffix; 
		}

		const std::string& prefix() const 
		{
			return m_szPrefix;
		}

		const std::string& suffix() const 
		{
			return m_szSuffix;
		}

	public:
		using IPacker<msg_type>::pack_msg;
		virtual msg_type pack_msg(const char* const pstr[], const size_t len[], size_t num, bool native = false)
		{
			msg_type msg;
			auto pre_len = native ? 0 : m_szPrefix.size() + m_szSuffix.size();
			auto total_len = CPackerHelper::msg_size_check(pre_len, pstr, len, num);
			if ((size_t)-1 == total_len)
				return msg;
			else if (total_len > pre_len)
			{
				msg.reserve(total_len);
				if (!native)
					msg.append(m_szPrefix);
				for (size_t i = 0; i < num; ++i)
					if (nullptr != pstr[i])
						msg.append(pstr[i], len[i]);
				if (!native)
					msg.append(m_szSuffix);
			} //if (total_len > pre_len)

			return msg;
		}

		virtual char* raw_data(msg_type& msg) const
		{
			return const_cast<char*>(msg.data());
		}
		virtual const char* raw_data(msg_ctype& msg) const
		{ 
			return msg.data(); 
		}
		virtual size_t raw_data_len(msg_ctype& msg) const 
		{ 
			return msg.size(); 
		}

	private:
		std::string m_szPrefix, m_szSuffix;
	};

} //namespace

#endif /* ASIO_PACKER_H_ */
