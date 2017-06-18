 

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
	//打包器的helper类，目前只有一个方法，就是判断要打包的消息，在打包后，是否超过了最大长度
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
		//打包函数，如果使用者调用send_msg, broadcast_msg, post_msg，则会以native为false调用本函数；
		//如果使用都调用send_native_msg, broadcast_native_msg, post_native_msg，则会以native为true调用本函数。
		virtual msg_type pack_msg(const char* const pstr[], const size_t len[], size_t num, bool native = false) = 0;
		//从一个打包器对象中，得到原始数据。
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
	//默认的打包器，如果你需要的消息格式与默认打包器的消息格式不相同，那么显然的，你需要实现自己的打包器；
	//	默认打包器的消息格式是：长度（２字节）＋　消息内容，所以其支持的消息长度为1至(65535 - 2)，如果想突破这个限制，
	//	可以定义HUGE_MSG宏，那么可以支持到4G长度。
	//	注意，大消息会占用你相像不到的大内存，比如你的消息长度为1M（看起来不算大），那么由于每个st_tcp_socket都有一个解包器，
	//	每个解包器都必须有一个至少1M大小的接收缓存，那么1024个st_tcp_socket，光是解包器缓存就占用1G内存，再考虑默认的接收发送缓存
	//	分别可最大存放1024个消息，那么每个st_tcp_socket在接收发送缓存都满的情况下，可占用2 * 1024 * 1M = 2G内存，那么1024个st_tcp_socket
	//	就将占用2T内存。所以推荐的作法是，如果你的消息很长，那么一定要尽量减小接收发送缓存的长度，比如设置为1个消息。
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
	//当你想在运行时替换打包器的话，可以把这个打包器设置为默认打包器，这个打包器返回replaceable_buffer对象，由于replaceable_buffer对象
	//	保存了一个i_buffer指针，所以只要是实现了i_buffer接口的对象，都能赋予replaceable_buffer，具体请参看replaceable_buffer及i_buffer的定义。
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
	//带固定头和固定尾的打包器，头可以为空，尾不能为空。
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
