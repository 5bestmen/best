#ifndef SVC_PUMP_H
#define SVC_PUMP_H 

#include <list>

#include <boost/asio.hpp>
//#include <boost/container/list.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
 
namespace ASIO
{
#define SERVICE_THREAD_NUM 8

   // 对io_service的包装，用于启动st_asio_wrapper里面的service（实现了i_service的所有对象）。
	class CServicePump : public boost::asio::io_service
	{
	public:
		class CServiceInterface
		{
		protected:
			CServiceInterface(CServicePump& svc_pump) : m_ServicePumpRef(svc_pump), m_bStarted(false), m_nId(0), m_pUserData(nullptr) 
			{
				svc_pump.add(this); 
			}
			virtual ~CServiceInterface() 
			{

			}
		public:
			//for the same i_service, StartService and StopService are not thread safe,
			//to resolve this defect, we must add a mutex member variable to i_service, it's not worth
			// 启动service（调用init），如果service已经启动，则调用没有任何作用。
			void StartService()
			{
				if (!m_bStarted) 
					m_bStarted = Init(); 
			}
			void StopService()
			{ 
				if (m_bStarted)
					Uninit(); 
				m_bStarted = false; 
			}

			bool IsStarted() const 
			{ 
				return m_bStarted;
			}

			void SetID(int id)
			{ 
				m_nId = id; 
			}
			int GetID() const
			{
				return m_nId; 
			}
			void SetUserData(void* data_)
			{ 
				m_pUserData = data_; 
			}
			void* GetUserData() const 
			{
				return m_pUserData; 
			}

			CServicePump& GetServicePump()
			{ 
				return m_ServicePumpRef; 
			}
			const CServicePump& GetServicePump() const
			{
				return m_ServicePumpRef; 
			}
		protected:
			virtual bool Init() = 0;
			virtual void Uninit() = 0;
		protected:
			CServicePump& m_ServicePumpRef;
		private:
			bool  m_bStarted;
			int   m_nId;
			void* m_pUserData; //magic m_pUserData, you can use it in any way
		};

	public:
	//	typedef CServiceInterface* object_type;
		using ObjectType = CServiceInterface *;
		typedef const ObjectType object_ctype;
		//typedef boost::container::list<ObjectType> container_type;
		typedef std::list<ObjectType> container_type;

		CServicePump() : m_bStarted(false) 
		{
		}

		ObjectType find(int id)
		{
			boost::shared_lock<boost::shared_mutex> lock(service_can_mutex);
			auto iter = std::find_if(std::begin(m_lstServices), std::end(m_lstServices), [id](object_ctype& item) {return id == item->GetID(); });
			return iter == std::end(m_lstServices) ? nullptr : *iter;
		}

		void remove(ObjectType i_service_)
		{
			assert(nullptr != i_service_);

			boost::unique_lock<boost::shared_mutex> lock(service_can_mutex);
			m_lstServices.remove(i_service_);
			lock.unlock();

			stop_and_free(i_service_);
		}

		void remove(int id)
		{
			boost::unique_lock<boost::shared_mutex> lock(service_can_mutex);
			auto iter = std::find_if(std::begin(m_lstServices), std::end(m_lstServices), [id](object_ctype& item) {return id == item->GetID(); });
			if (iter != std::end(m_lstServices))
			{
				auto i_service_ = *iter;
				m_lstServices.erase(iter);
				lock.unlock();

				stop_and_free(i_service_);
			}
		}

		void clear()
		{
			container_type temp_service_can;

			boost::unique_lock<boost::shared_mutex> lock(service_can_mutex);
			temp_service_can.splice(std::end(temp_service_can), m_lstServices);
			lock.unlock();

			do_something_to_all(temp_service_can, [this](ObjectType& item) { stop_and_free(item); });
		}
		// 启动／停止service（调用end_service），thread_num是线程数量（用于调用io_service::run）。
		void StartService(int thread_num = SERVICE_THREAD_NUM) 
		{ 
			if (!IsServiceStarted()) 
				do_service(thread_num); 
		}
		//stop the service, must be invoked explicitly when the service need to stop, for example, close the application
		void StopService()
		{
			if (IsServiceStarted())
			{
				EndService();
				WaitService();
			}
		}
		// 如果某个service是在前面那个start_service之后才添加的，则调用这个启动它，否则功能完全同前面那个start_service，这也是为什么带了
		//thread_num这个看似无关的参数。
		//if you add a service after start_service, use this to start it
		void StartService(ObjectType i_service_, int nThreadNum = SERVICE_THREAD_NUM)
		{
			assert(nullptr != i_service_);

			if (IsServiceStarted())
				i_service_->StartService();
			else
				StartService(nThreadNum);
		}
		// 停止指定的service
		void StopService(ObjectType i_service_)
		{
			assert(nullptr != i_service_);
			i_service_->StopService(); 
		}

		//this function works like StartService except that it will block until all services run out 
		void RunService(int thread_num = SERVICE_THREAD_NUM)
		{
			if (!IsServiceStarted())
			{
				do_service(thread_num - 1);

				boost::system::error_code ec;
				run(ec);

				WaitService();
			}
		}
		//stop the service, must be invoked explicitly when the service need to stop, for example, close the application
		//only for service pump started by 'RunService', this function will return immediately,
		//only the return from 'RunService' means service pump ended.
		void EndService()
		{
			if (IsServiceStarted())
				do_something_to_all(m_lstServices, service_can_mutex, [](ObjectType& item) {item->StopService(); });
		}

		bool IsRunning() const 
		{
			return !stopped();
		}
		bool IsServiceStarted() const
		{
			return m_bStarted; 
		}
		void add_service_thread(int thread_num)
		{
			for (auto i = 0; i < thread_num; ++i)
				m_SvcTask.create_thread([this]() {boost::system::error_code ec; run(ec); }); 
		}
	protected:
		void do_service(int thread_num)
		{
			m_bStarted = true;
		//	unified_out::info_out("service pump started.");

			reset(); //this is needed when restart service
			do_something_to_all(m_lstServices, service_can_mutex,[](ObjectType& item) {item->StartService(); });
			add_service_thread(thread_num);
		}
		void WaitService()
		{ 
			m_SvcTask.join_all(); 
			//unified_out::info_out("service pump end.");
			m_bStarted = false;
		}

		void stop_and_free(ObjectType i_svc)
		{
			assert(nullptr != i_svc);

			i_svc->StopService();
			free(i_svc);
		}

		//if needed, rewrite this to free the service
		virtual void free(ObjectType i_service_) 
		{
			std::ignore = i_service_;
		} 

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

	//	DO_SOMETHING_TO_ALL_MUTEX(m_lstServices, service_can_mutex)
	//	DO_SOMETHING_TO_ONE_MUTEX(m_lstServices, service_can_mutex)

	private:
		void add(ObjectType i_service_)
		{
			assert(nullptr != i_service_);

			boost::unique_lock<boost::shared_mutex> lock(service_can_mutex);
			m_lstServices.push_back(i_service_);
		}
	protected:
		container_type m_lstServices;
		boost::shared_mutex service_can_mutex;
		boost::thread_group m_SvcTask;
		bool m_bStarted;
	};
}
 

#endif // SVC_PUMP_H
