

#ifndef ASIO_BASE_H
#define ASIO_BASE_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>


namespace ASIO
{
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

	template<typename _Can>
	bool splice_helper(_Can& dest_can, _Can& src_can, size_t max_size = ASIO_MAX_MSG_NUM)
	{
		auto size = dest_can.size();
		if (size < max_size) //dest_can can hold more items.
		{
			size = max_size - size; //maximum items this time can handle
			auto begin_iter = std::begin(src_can), end_iter = std::end(src_can);
			if (src_can.size() > size) //some items left behind
			{
				auto left_num = src_can.size() - size;
				end_iter = left_num > size ? std::next(begin_iter, size) : std::prev(end_iter, left_num); //find the minimum movement
			}
			else
				size = src_can.size();
			//use size to avoid std::distance() call, so, size must correct
			dest_can.splice(std::end(dest_can), src_can, begin_iter, end_iter, size);

			return size > 0;
		}

		return false;
	}
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

#include <sstream>

#ifndef ASIO_UNIFIED_OUT_BUF_NUM
#define ASIO_UNIFIED_OUT_BUF_NUM	2048
#endif

	class CAsioLogFormater
	{
	public:
		static void all_out(const char* head, char* buff, size_t buff_len, const char* fmt, va_list& ap)
		{
			assert(nullptr != buff && buff_len > 0);

			std::stringstream os;
			os.rdbuf()->pubsetbuf(buff, buff_len);

			if (nullptr != head)
				os << '[' << head << "] ";

			char time_buff[64];
			auto now = time(nullptr);
#ifdef _MSC_VER
			ctime_s(time_buff, sizeof(time_buff), &now);
#else
			ctime_r(&now, time_buff);
#endif
			auto len = strlen(time_buff);
			assert(len > 0);
			if ('\n' == *std::next(time_buff, --len))
				*std::next(time_buff, len) = '\0';

			os << time_buff << " -> ";

#if defined _MSC_VER || (defined __unix__ && !defined __linux__)
			os.rdbuf()->sgetn(buff, buff_len);
#endif
			len = (size_t)os.tellp();
			if (len >= buff_len)
				*std::next(buff, buff_len - 1) = '\0';
			else
#if BOOST_WORKAROUND(BOOST_MSVC, >= 1400) && !defined(UNDER_CE)
				vsnprintf_s(std::next(buff, len), buff_len - len, _TRUNCATE, fmt, ap);
#else
				vsnprintf(std::next(buff, len), buff_len - len, fmt, ap);
#endif
		}
	};

#define all_out_helper(head, buff, buff_len) va_list ap; va_start(ap, fmt); CAsioLogFormater::all_out(head, buff, buff_len, fmt, ap); va_end(ap)
#define all_out_helper2(head) char output_buff[ASIO_UNIFIED_OUT_BUF_NUM]; all_out_helper(head, output_buff, sizeof(output_buff)); puts(output_buff)

#ifndef ASIO_CUSTOM_LOG
	class unified_out
	{
	public:
#ifdef  ASIO_NO_UNIFIED_OUT
		static void fatal_out(const char* fmt, ...) {}
		static void error_out(const char* fmt, ...) {}
		static void warning_out(const char* fmt, ...) {}
		static void info_out(const char* fmt, ...) {}
		static void debug_out(const char* fmt, ...) {}
#else
		static void fatal_out(const char* fmt, ...)
		{
			all_out_helper2(nullptr); 
		}
		static void error_out(const char* fmt, ...) 
		{
			all_out_helper2(nullptr); 
		}
		static void warning_out(const char* fmt, ...)
		{ 
			all_out_helper2(nullptr);
		}
		static void info_out(const char* fmt, ...) 
		{
			all_out_helper2(nullptr); 
		}
		static void debug_out(const char* fmt, ...)
		{
			all_out_helper2(nullptr);
		}
#endif
	};
#endif

} //namespace

#endif /* ASIO_BASE_H */
