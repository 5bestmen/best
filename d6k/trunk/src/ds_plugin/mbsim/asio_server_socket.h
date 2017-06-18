

#ifndef ASIO_SERVER_SOCKET_H_
#define ASIO_SERVER_SOCKET_H_

#include "service_pump.h"
#include "asio_tcp_socket.h"

namespace ASIO
{
	class CAsioServerInterface
	{
	public:
		virtual CServicePump& GetServicePump() = 0;
		virtual const CServicePump& GetServicePump() const = 0;

		//关闭一个连接时，st_server_socket调用这个方法，i_server的实现者（st_server就是一个i_server的实现者）一般来说应该把这个连接从自己的
		//客户端对象池里面删除，调用这个方法的最佳时期是在on_recv_error里面。
		//注意，client_ptr看起来是一个st_timer，其实它的类型是st_server_base中的Socket模板参数，用st_timer完全是为了不让i_server带一个类似于
		//Socket的模板参数，带上之后会有模板参数的循环依赖的问题，如果哪位同仁可以解决此问题，麻烦告诉我一声。
		//i_server的实现者在使用client_ptr参数之前，会转换成Socket类型，请看st_server的实现。
		virtual bool del_client(const boost::shared_ptr<CAsioTimer>& client_ptr) = 0;
	};

	template<typename Packer = ASIO_DEFAULT_PACKER, typename Unpacker = ASIO_DEFAULT_UNPACKER, typename ServerInterface = CAsioServerInterface, typename Socket = boost::asio::ip::tcp::socket>
	class CServerSocketBase : public CTcpSocketBase<Socket, Packer, Unpacker>, public boost::enable_shared_from_this< CServerSocketBase<Packer, Unpacker, ServerInterface, Socket> >
	{
	public:
		static const unsigned char TIMER_BEGIN = CTcpSocketBase<Socket, Packer, Unpacker>::TIMER_END;
		static const unsigned char TIMER_ASYNC_CLOSE = TIMER_BEGIN;
		static const unsigned char TIMER_END = TIMER_BEGIN + 10;

		CServerSocketBase(ServerInterface& svr) : CTcpSocketBase<Socket, Packer, Unpacker>(svr.GetServicePump()), m_ServerRef(svr)
		{

		}

		template<typename Arg>
		CServerSocketBase(ServerInterface& svr, Arg& arg) : CTcpSocketBase<Socket, Packer, Unpacker>(svr.GetServicePump(), arg), m_ServerRef(svr)
		{

		}

		//reset all, be ensure that there's no any operations performed on this socket when invoke it
		//please note, when reuse this socket, st_object_pool will invoke reset(), child must re-write it to initialize all member variables,
		//and then do not forget to invoke CServerSocketBase::reset() to initialize father's member variables
		//重置所有，st_object_pool在重用时会调用。st_server_socket的子类可重写它以重置自己的状态，记得最后需要调用本类的reset。
		virtual void Reset()
		{
			CTcpSocketBase<Socket, Packer, Unpacker>::Reset();
		}

		void disconnect()
		{
			force_close();
		}
		//强制退出————调用父类的同名函数。
		void force_close()
		{
			if (1 != m_nCloseState)
				show_info("server link:", "been closed.");

			CTcpSocketBase<Socket, Packer, Unpacker>::force_close();
		}

		void graceful_close(bool bSync = true)
		{
			if (!is_closing())
				show_info("server link:", "been closing gracefully.");

			if (CTcpSocketBase<Socket, Packer, Unpacker>::graceful_close(bSync))
				SetTimer(TIMER_ASYNC_CLOSE, 10, boost::bind(&CServerSocketBase::async_close_handler, this, _1, ASIO_GRACEFUL_CLOSE_MAX_DURATION * 100));
		}

		void show_info(const char* head, const char* tail) const
		{
			boost::system::error_code ec;
			auto ep = GetLowestLayer().remote_endpoint(ec);
			if (!ec)
				unified_out::info_out("%s %s:%hu %s", head, ep.address().to_string().c_str(), ep.port(), tail);
		}

		void show_info(const char* head, const char* tail, const boost::system::error_code& ec) const
		{
			boost::system::error_code ec2;
			auto ep = GetLowestLayer().remote_endpoint(ec2);
			if (!ec2)
				unified_out::info_out("%s %s:%hu %s (%d %s)", head, ep.address().to_string().c_str(), ep.port(), tail, ec.value(), ec.message().data());
		}

	protected:
		virtual bool DoStart()
		{
			if (!IsStopped())
			{
				DoRecvMsg();
				return true;
			}

			return false;
		}
		//解包错误，默认的行为是关闭连接，可以重写以自定义行为。
		virtual void OnUnpackError()
		{
			unified_out::error_out("can not unpack msg.");
			force_close();
		}
		//do not forget to force_close this socket(in del_client(), there's a force_close() invocation)
		virtual void OnRecvError(const boost::system::error_code& ec)
		{
			show_info("server link:", "broken/closed", ec);

#ifdef ASIO_NOT_REUSE_ADDRESS
			force_close();
#else
			m_ServerRef.del_client(boost::dynamic_pointer_cast<CAsioTimer>( shared_from_this()));
#endif

			m_nCloseState = 0;
		}

	private:
		bool async_close_handler(unsigned int id, size_t loop_num)
		{
			assert(TIMER_ASYNC_CLOSE == id);

			if (2 == m_nCloseState)
			{
				--loop_num;
				if (loop_num > 0)
				{
					UpdateTimerInfo(id, 10, boost::bind(&CServerSocketBase::async_close_handler, this, _1, loop_num));
					return true;
				}
				else
				{
					unified_out::info_out("failed to graceful close within %d seconds", ASIO_GRACEFUL_CLOSE_MAX_DURATION);
					force_close();
				}
			}

			return false;
		}

	protected:
		//用于操控Server，Server在创建（其实是st_object_pool创建，st_server是其子类）st_server_socket的时候，会把自己的引用通过构造函数传入。
		ServerInterface& m_ServerRef;
	};
	//typedef CServerSocketBase<> st_server_socket;
	using CServerSocket = CServerSocketBase<>;
} //namespace

#endif /* ASIO_SERVER_SOCKET_H_ */
