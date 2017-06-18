

#ifndef ASIO_BUFFER_H
#define ASIO_BUFFER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

//the size of the buffer used when receiving msg, must equal to or larger than the biggest msg size,
//the bigger this buffer is, the more msgs can be received in one time if there are enough msgs buffered in the SOCKET.
//every unpackers have a fixed buffer with this size, every st_tcp_sockets have an unpacker, so, this size is not the bigger the better.
//if you customized the packer and unpacker, the above principle maybe not right anymore, it should depends on your implementations.
#ifndef ASIO_MSG_BUFFER_SIZE
#define ASIO_MSG_BUFFER_SIZE	4000
#endif
//msg send and recv buffer's maximum size (list::size()), corresponding buffers are expanded dynamicly, which means only allocate memory when needed.
#ifndef ASIO_MAX_MSG_NUM
#define ASIO_MAX_MSG_NUM		1024
#endif

#if 0
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <string>




#if defined _MSC_VER
#define ST_ASIO_SF "%Iu"
#define ST_THIS //workaround to make up the BOOST_AUTO's defect under vc2008 and compiler bugs before vc2012
#define ssize_t SSIZE_T
#else // defined __GNUC__
#define ST_ASIO_SF "%tu"
#define ST_THIS this->
#endif

#endif

namespace ASIO
{
	class i_buffer
	{
	protected:
		virtual ~i_buffer()
		{
		}
	public:
		virtual bool empty() const = 0;
		virtual size_t size() const = 0;
		virtual const char* data() const = 0;
	};

	class buffer : public std::string, public i_buffer
	{
	public:
		virtual bool empty() const
		{
			return std::string::empty();
		}
		virtual size_t size() const
		{
			return std::string::size();
		}
		virtual const char* data() const
		{
			return std::string::data();
		}
	};

	class replaceable_buffer
	{
	public:
		replaceable_buffer()
		{
		}

		replaceable_buffer(const boost::shared_ptr<i_buffer>& _buffer) : buffer(_buffer)
		{

		}

		replaceable_buffer(replaceable_buffer&& other) : buffer(other.buffer)
		{
			other.buffer.reset();
		}

		replaceable_buffer(const replaceable_buffer& other) : buffer(other.buffer)
		{
		}

		boost::shared_ptr<i_buffer> raw_buffer()
		{
			return buffer;
		}
		boost::shared_ptr<const i_buffer> raw_buffer() const
		{
			return buffer;
		}
		void raw_buffer(const boost::shared_ptr<i_buffer>& _buffer)
		{
			buffer = _buffer;
		}

		//the following five functions are needed by st_asio_wrapper, for other functions, depends on the implementation of your packer and unpacker
		bool empty() const
		{
			return !buffer || buffer->empty();
		}

		size_t size() const
		{
			return buffer ? buffer->size() : 0;
		}
		const char* data() const
		{
			return buffer ? buffer->data() : nullptr;
		}
		void swap(replaceable_buffer& other)
		{
			buffer.swap(other.buffer);
		}
		void clear()
		{
			buffer.reset();
		}

	protected:
		boost::shared_ptr<i_buffer> buffer;
	};

	//this buffer is more efficient than std::string if the memory is already allocated, because the replication been saved.
	//for example, you are sending memory-mapped files.
	class inflexible_buffer
	{
	public:
		inflexible_buffer()
		{
			do_detach();
		}

		inflexible_buffer(inflexible_buffer&& other)
		{
			do_attach(other.buff, other.len);
			other.do_detach();
		}
		~inflexible_buffer()
		{
			detach();
		}

		void assign(const char* _buff, size_t _len)
		{
			assert(_len > 0 && nullptr != _buff);
			auto _buff_ = new char[_len];
			memcpy(_buff_, _buff, _len);

			attach(_buff_, _len);
		}

		void attach(char* _buff, size_t _len)
		{
			detach(); do_attach(_buff, _len);
		}

		void detach()
		{
			delete buff;
			do_detach();
		}

		//the following five functions (char* data() is used by inflexible_unpacker, not counted) are needed by st_asio_wrapper,
		//for other functions, depends on the implementation of your packer and unpacker.
		bool empty() const
		{
			return 0 == len || nullptr == buff;
		}
		size_t size() const
		{
			return len;
		}
		const char* data() const
		{
			return buff;
		}
		char* data()
		{
			return buff;
		}
		void swap(inflexible_buffer& other)
		{
			std::swap(buff, other.buff);
			std::swap(len, other.len);
		}
		void clear()
		{
			detach();
		}

	protected:
		void do_attach(char* _buff, size_t _len)
		{
			buff = _buff;
			len = _len;
		}
		void do_detach()
		{
			buff = nullptr;
			len = 0;
		}

	protected:
		char* buff;
		size_t len;
	};

	//free functions, used to do something to any container(except map and multimap) optionally with any mutex

#if 0
	template<typename _Can, typename _Mutex, typename _Predicate>
	void do_something_to_all(_Can& __can, _Mutex& __mutex, const _Predicate& __pred)
	{ 
		boost::shared_lock<boost::shared_mutex> lock(__mutex); 
		for (auto& item : __can) 
			__pred(item); 
	}

	template<typename _Can, typename _Predicate>
	void do_something_to_all(_Can& __can, const _Predicate& __pred) 
	{
		for (auto& item : __can) 
			__pred(item);
	}

	template<typename _Can, typename _Mutex, typename _Predicate>
	void do_something_to_one(_Can& __can, _Mutex& __mutex, const _Predicate& __pred)
	{
		boost::shared_lock<boost::shared_mutex> lock(__mutex);
		for (auto iter = std::begin(__can); iter != std::end(__can); ++iter)
			if (__pred(*iter))
				break;
	}

	template<typename _Can, typename _Predicate>
	void do_something_to_one(_Can& __can, const _Predicate& __pred)
	{
		for (auto iter = std::begin(__can); iter != std::end(__can); ++iter)
			if (__pred(*iter))
				break;
	}
#endif 
#if 0
	//member functions, used to do something to any member container(except map and multimap) optionally with any member mutex
#define DO_SOMETHING_TO_ALL_MUTEX(CAN, MUTEX) DO_SOMETHING_TO_ALL_MUTEX_NAME(do_something_to_all, CAN, MUTEX)
#define DO_SOMETHING_TO_ALL(CAN) DO_SOMETHING_TO_ALL_NAME(do_something_to_all, CAN)

#if !defined _MSC_VER || _MSC_VER >= 1700
#define DO_SOMETHING_TO_ALL_MUTEX_NAME(NAME, CAN, MUTEX) \
	template<typename _Predicate> void NAME(const _Predicate& __pred) {boost::shared_lock<boost::shared_mutex> lock(MUTEX); for (auto& item : CAN) __pred(item);}

#define DO_SOMETHING_TO_ALL_NAME(NAME, CAN) \
	template<typename _Predicate> void NAME(const _Predicate& __pred) {for (auto& item : CAN) __pred(item);} \
	template<typename _Predicate> void NAME(const _Predicate& __pred) const {for (auto& item : CAN) __pred(item);}
#else

#endif

#define DO_SOMETHING_TO_ONE_MUTEX(CAN, MUTEX) DO_SOMETHING_TO_ONE_MUTEX_NAME(do_something_to_one, CAN, MUTEX)
#define DO_SOMETHING_TO_ONE(CAN) DO_SOMETHING_TO_ONE_NAME(do_something_to_one, CAN)

#define DO_SOMETHING_TO_ONE_MUTEX_NAME(NAME, CAN, MUTEX) \
template<typename _Predicate> void NAME(const _Predicate& __pred) \
		{boost::shared_lock<boost::shared_mutex> lock(MUTEX); for (auto iter = std::begin(CAN); iter != std::end(CAN); ++iter) if (__pred(*iter)) break;}

#define DO_SOMETHING_TO_ONE_NAME(NAME, CAN) \
template<typename _Predicate> void NAME(const _Predicate& __pred) {for (auto iter = std::begin(CAN); iter != std::end(CAN); ++iter) if (__pred(*iter)) break;} \
template<typename _Predicate> void NAME(const _Predicate& __pred) const {for (auto iter = std::begin(CAN); iter != std::end(CAN); ++iter) if (__pred(*iter)) break;}

	//used by both TCP and UDP
#define SAFE_SEND_MSG_CHECK \
	{ \
	if (! is_send_allowed() ||  get_io_service().stopped()) return false; \
	boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(50)); \
	}

#define GET_PENDING_MSG_NUM(FUNNAME, CAN, MUTEX) size_t FUNNAME() {boost::shared_lock<boost::shared_mutex> lock(MUTEX); return CAN.size();}
#define PEEK_FIRST_PENDING_MSG(FUNNAME, CAN, MUTEX, MSGTYPE) \
void FUNNAME(MSGTYPE& msg) \
	{ \
	msg.clear(); \
	boost::shared_lock<boost::shared_mutex> lock(MUTEX); \
	if (!CAN.empty()) \
		msg = CAN.front(); \
	}
#define POP_FIRST_PENDING_MSG(FUNNAME, CAN, MUTEX, MSGTYPE) \
void FUNNAME(MSGTYPE& msg) \
	{ \
	msg.clear(); \
	boost::unique_lock<boost::shared_mutex> lock(MUTEX); \
	if (!CAN.empty()) \
		{ \
		msg.swap(CAN.front()); \
		CAN.pop_front(); \
		} \
	}
#define POP_ALL_PENDING_MSG(FUNNAME, CAN, MUTEX, CANTYPE) \
void FUNNAME(CANTYPE& msg_list) \
	{ \
	boost::unique_lock<boost::shared_mutex> lock(MUTEX); \
	msg_list.splice(msg_list.end(), CAN); \
	}

	///////////////////////////////////////////////////
	//TCP msg sending interface
#define TCP_SEND_MSG_CALL_SWITCH(FUNNAME, TYPE) \
TYPE FUNNAME(const char* pstr, size_t len, bool can_overflow = false) {return FUNNAME(&pstr, &len, 1, can_overflow);} \
TYPE FUNNAME(const std::string& str, bool can_overflow = false) {return FUNNAME(str.data(), str.size(), can_overflow);}

#define TCP_SEND_MSG(FUNNAME, NATIVE) \
bool FUNNAME(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false) \
	{ \
	boost::unique_lock<boost::shared_mutex> lock(ST_THIS send_msg_buffer_mutex); \
	return (can_overflow || ST_THIS send_msg_buffer.size() < ASIO_MAX_MSG_NUM) ? ST_THIS do_direct_send_msg(ST_THIS packer_->pack_msg(pstr, len, num, NATIVE)) : false; \
	} \
TCP_SEND_MSG_CALL_SWITCH(FUNNAME, bool)

#define TCP_POST_MSG(FUNNAME, NATIVE) \
bool FUNNAME(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false) {return ST_THIS direct_post_msg(ST_THIS packer_->pack_msg(pstr, len, num, NATIVE), can_overflow);} \
TCP_SEND_MSG_CALL_SWITCH(FUNNAME, bool)

	//guarantee send msg successfully even if can_overflow equal to false, success at here just means putting the msg into st_tcp_socket's send buffer successfully
	//if can_overflow equal to false and the buffer is not available, will wait until it becomes available
#define TCP_SAFE_SEND_MSG(FUNNAME, SEND_FUNNAME) \
bool FUNNAME(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false) {while (!SEND_FUNNAME(pstr, len, num, can_overflow)) SAFE_SEND_MSG_CHECK return true;} \
TCP_SEND_MSG_CALL_SWITCH(FUNNAME, bool)

#define TCP_BROADCAST_MSG(FUNNAME, SEND_FUNNAME) \
void FUNNAME(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false) \
		{ST_THIS do_something_to_all([=](typename Pool::object_ctype& item) {item->SEND_FUNNAME(pstr, len, num, can_overflow);});} \
TCP_SEND_MSG_CALL_SWITCH(FUNNAME, void)
	//TCP msg sending interface
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	//UDP msg sending interface
#define UDP_SEND_MSG_CALL_SWITCH(FUNNAME, TYPE) \
TYPE FUNNAME(const boost::asio::ip::udp::endpoint& peer_addr, const char* pstr, size_t len, bool can_overflow = false) {return FUNNAME(peer_addr, &pstr, &len, 1, can_overflow);} \
TYPE FUNNAME(const boost::asio::ip::udp::endpoint& peer_addr, const std::string& str, bool can_overflow = false) {return FUNNAME(peer_addr, str.data(), str.size(), can_overflow);}

#define UDP_SEND_MSG(FUNNAME, NATIVE) \
bool FUNNAME(const boost::asio::ip::udp::endpoint& peer_addr, const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false) \
	{ \
	boost::unique_lock<boost::shared_mutex> lock(ST_THIS send_msg_buffer_mutex); \
	if (can_overflow || ST_THIS send_msg_buffer.size() < ASIO_MAX_MSG_NUM) \
		{ \
		udp_msg<typename Packer::msg_type> msg(peer_addr, ST_THIS packer_->pack_msg(pstr, len, num, NATIVE)); \
		return ST_THIS do_direct_send_msg(std::move(msg)); \
		} \
	return false; \
	} \
UDP_SEND_MSG_CALL_SWITCH(FUNNAME, bool)

#define UDP_POST_MSG(FUNNAME, NATIVE) \
bool FUNNAME(const boost::asio::ip::udp::endpoint& peer_addr, const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false) \
	{ \
	udp_msg<typename Packer::msg_type> msg(peer_addr, ST_THIS packer_->pack_msg(pstr, len, num, NATIVE)); \
	return ST_THIS direct_post_msg(std::move(msg), can_overflow); \
	} \
UDP_SEND_MSG_CALL_SWITCH(FUNNAME, bool)

	//guarantee send msg successfully even if can_overflow equal to false, success at here just means putting the msg into st_udp_socket's send buffer successfully
	//if can_overflow equal to false and the buffer is not available, will wait until it becomes available
#define UDP_SAFE_SEND_MSG(FUNNAME, SEND_FUNNAME) \
bool FUNNAME(const boost::asio::ip::udp::endpoint& peer_addr, const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false) \
		{while (!SEND_FUNNAME(peer_addr, pstr, len, num, can_overflow)) SAFE_SEND_MSG_CHECK return true;} \
UDP_SEND_MSG_CALL_SWITCH(FUNNAME, bool)
	//UDP msg sending interface
	///////////////////////////////////////////////////
#endif
} //namespace

#endif /* ST_ASIO_WRAPPER_BASE_H_ */
