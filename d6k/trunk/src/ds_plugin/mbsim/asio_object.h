

#ifndef ASIO_OBJECT_H
#define ASIO_OBJECT_H
 
#include "asio_base.h"
  
namespace ASIO
{
	class CAsioObject
	{
	protected:
		CAsioObject(boost::asio::io_service& io_svc) : m_IoSvcRef(io_svc) 
		{ 
			Reset();
		}
		virtual ~CAsioObject()
		{
		}

	public:
		bool IsStopped() const 
		{
			return m_IoSvcRef.stopped();
		}

		boost::asio::io_service& GetIoService()
		{
			return m_IoSvcRef; 
		}
		const boost::asio::io_service& GetIoService() const
		{ 
			return m_IoSvcRef;
		}
#ifdef ASIO_ENHANCED_STABILITY
		template<typename CallbackHandler>
		void post(const CallbackHandler& handler) {auto unused( async_call_indicator); m_IoSvcRef.post([=]() {handler();});}

		template<typename CallbackHandler>
		void post(CallbackHandler&& handler) {auto unused( async_call_indicator); m_IoSvcRef.post([=]() {handler();});}

		bool IsAsyncCalling() const { return !async_call_indicator.unique(); }

		template<typename CallbackHandler>
		std::function<void(const boost::system::error_code&)> make_handler_error( CallbackHandler && handler) const
		{
			boost::shared_ptr<char>unused(async_call_indicator); return [=](const boost::system::error_code& ec) {handler(ec); };
		}
		template<typename CallbackHandler>
		std::function<void(const boost::system::error_code&)> make_handler_error(const CallbackHandler & handler) const
		{
			boost::shared_ptr<char>unused(async_call_indicator); return [=](const boost::system::error_code& ec) {handler(ec); };
		}
		template<typename CallbackHandler>
		std::function<void(const boost::system::error_code&, size_t)> make_handler_error_size(CallbackHandler && handler) const
		{
			boost::shared_ptr<char>unused(async_call_indicator); return [=](const boost::system::error_code& ec, size_t bytes_transferred) {handler(ec, bytes_transferred); };
		}
		template<typename CallbackHandler>
		std::function<void(const boost::system::error_code&, size_t)> make_handler_error_size( CallbackHandler & handler) const
		{
			boost::shared_ptr<char>unused(async_call_indicator); return [=](const boost::system::error_code& ec, size_t bytes_transferred) {handler(ec, bytes_transferred); };
		}
protected:
		void Reset() { async_call_indicator = boost::make_shared<char>('\0'); }

protected:
		boost::shared_ptr<char> async_call_indicator;
#else
		template<typename CallbackHandler>
		void post(const CallbackHandler& handler)
		{
			m_IoSvcRef.post(handler);
		}
		template<typename CallbackHandler>
		void post(CallbackHandler&& handler)
		{
			m_IoSvcRef.post(std::move(handler));
		}
		bool IsAsyncCalling() const 
		{
			return false; 
		}

		template<typename F>
		inline F&& make_handler_error(F&& f) const
		{
			return std::move(f); 
		}

		template<typename F>
		inline const F& make_handler_error(const F& f) const 
		{ 
			return f; 
		}

		template<typename F>
		inline F&& make_handler_error_size(F&& f) const
		{
			return std::move(f); 
		}
		template<typename F>
		inline const F& make_handler_error_size(const F& f) const
		{ 
			return f;
		}
	protected:
		void Reset()
		{
		}
#endif

	protected:
		boost::asio::io_service& m_IoSvcRef;
	};

} //namespace

#endif /* ASIO_OBJECT_H */
