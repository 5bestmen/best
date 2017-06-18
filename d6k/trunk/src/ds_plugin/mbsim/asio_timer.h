

#ifndef ASIO_TIMER_H
#define ASIO_TIMER_H

#include <boost/container/set.hpp>
//#include <boost/function.hpp>

//#include <set>
#include <functional>
#include <memory>

#include "asio_object.h"


//If you inherit a class from class X, your own timer ids must begin from X::TIMER_END
namespace ASIO
{
	//timers are identified by id.
	//for the same timer in the same CAsioTimer, set_timer and stop_timer are not thread safe, please pay special attention.
	//to resolve this defect, we must add a mutex member variable to timer_info, it's not worth
	//
	//suppose you have more than one service thread(see st_service_pump for service thread number control), then:
	//same st_timer, same timer, on_timer is called sequentially
	//same st_timer, different timer, on_timer is called concurrently
	//different st_timer, on_timer is always called concurrently
	class CAsioTimer :public CAsioObject
	{
	protected:
		//��ʱ�����ݽṹ��
		struct timer_info
		{
			enum timer_status
			{
				TIMER_OK,
				TIMER_CANCELED
			};

			unsigned int id;
			timer_status status;
			size_t milliseconds;
			// �ڶ�ʱ�������call_back���ص��������ݷ���ֵ�����Ƿ���������ʱ����true����������ͬһ����ʱ����call_back�ĵ�����˳��ġ�
			std::function<bool(unsigned int)> call_back;
			std::shared_ptr<boost::asio::deadline_timer> timer;

			bool operator <(const timer_info& other) const
			{
				return id < other.id;
			}
		};

		// �̳��ߵĶ�����ID����Ӹ����TIMER_END��ʼ��Ȼ�����Ҳ����һ���Լ���TIMER_END��������������ܻᱻ�̳еĻ���
		static const unsigned int TIMER_END = 0; //user timer's id must begin from parent class' TIMER_END

		CAsioTimer(boost::asio::io_service& io_svc) : CAsioObject(io_svc)
		{
		}
		~CAsioTimer()
		{
		}
	public:
		//typedef timer_info object_type;
		using ObjectType = timer_info;
		using object_ctype = const ObjectType;
		//typedef const ObjectType object_ctype;
		//using ContainerType = std::set<ObjectType>;
		//typedef std::set<ObjectType> ContainerType;
		using ContainerType = boost::container::set<ObjectType>;

		//typedef boost::container::set<ObjectType> ContainerType;

		// ������ʱ������ʱ����id�����������ʱ���Ѿ����ڣ������¿�ʼ��ʱ�����������ʵ������startΪtrue����update_timer_info���ѡ�
		void SetTimer(unsigned int id, size_t milliseconds, std::function<bool(unsigned int)>&& call_back)
		{
			UpdateTimerInfo(id, milliseconds, std::move(call_back), true);
		}
		void SetTimer(unsigned int id, size_t milliseconds, const std::function<bool(unsigned int)>& call_back)
		{
			UpdateTimerInfo(id, milliseconds, call_back, true);
		}
		// ����timer�����startΪtrue����������֮�����ϻῪ���������¿������timer��
		// ע�⣬��ͬһ��timer�����ͬһ����ʱ�������������̰߳�ȫ�ġ�
		void UpdateTimerInfo(unsigned int nID, size_t nMilliseconds, std::function<bool(unsigned int)>&& fnCallBack, bool bStart = false)
		{
			ObjectType ti = { nID };

			//lock m_setTimers
			m_TimersMutex.lock_upgrade();

			auto iter = m_setTimers.find(ti);

			if (iter == std::end(m_setTimers))
			{
				m_TimersMutex.unlock_upgrade_and_lock();
				iter = m_setTimers.insert(ti).first;
				m_TimersMutex.unlock();
				//std::shared_ptr<boost::asio::deadline_timer> timer = std::make_shared<boost::asio::deadline_timer>(m_IoServiceRef);;
				iter->timer = std::make_shared<boost::asio::deadline_timer>(m_IoSvcRef);
			}
			else
				m_TimersMutex.unlock_upgrade();

			//items in m_setTimers not locked
			iter->status = ObjectType::TIMER_OK;
			iter->milliseconds = nMilliseconds;
			iter->call_back.swap(fnCallBack);

			if (bStart)
				StartTimer(*iter);
		}

		void UpdateTimerInfo(unsigned int id, size_t milliseconds, const std::function<bool(unsigned int)>& call_back, bool start = false)
		{
			UpdateTimerInfo(id, milliseconds, std::function<bool(unsigned int)>(call_back), start);
		}

		// ���Ҷ�������
		ObjectType FindTimer(unsigned int id)
		{
			ObjectType ti = { id, ObjectType::TIMER_CANCELED, 0 };

			boost::shared_lock<boost::shared_mutex> lock(m_TimersMutex);
			auto iter = m_setTimers.find(ti);
			if (iter == std::end(m_setTimers))
				return *iter;
			else
				return ti;
		}
		// ����һ���Ѿ����ڵĶ�������ע��stop_timer֮�󣬶��������Ǵ��ڵģ�ֻ��δ������
		bool StartTimer(unsigned int nID)
		{
			ObjectType ti = { nID };

			boost::shared_lock<boost::shared_mutex> lock(m_TimersMutex);
			auto iter = m_setTimers.find(ti);
			if (iter == std::end(m_setTimers))
				return false;
			lock.unlock();

			StartTimer(*iter);
			return true;
		}
		// ֹͣ��ʱ���������ʱʱ���Ѿ�������Ѿ���post��io_server�Ķ����У���on_timer��Ȼ����δ��ĳ��ʱ�򱻻ص�������asio����ơ�
		void StopTimer(unsigned int id)
		{
			ObjectType ti = { id };

			boost::shared_lock<boost::shared_mutex> lock(m_TimersMutex);
			auto iter = m_setTimers.find(ti);
			if (iter != std::end(m_setTimers))
			{
				lock.unlock();
				StopTimer(*iter);
			}
		}

		// 		template<typename _Predicate> void do_something_to_all(const _Predicate& __pred) 
		// 		{
		// 			boost::shared_lock<boost::shared_mutex> lock(MUTEX);
		// 			for (auto& item : CAN) __pred(item); 
		// 		}

		// 		template<typename _Can, typename _Mutex, typename _Predicate>
		// 		void do_something_to_all(_Can& __can, _Mutex& __mutex, const _Predicate& __pred)
		// 		{
		// 			boost::shared_lock<boost::shared_mutex> lock(__mutex);
		// 			for (auto& item : __can)
		// 				__pred(item);
		template<typename _Predicate>
		void do_something_to_all(const _Predicate& __pred)
		{
			boost::shared_lock<boost::shared_mutex> lock(m_TimersMutex);
			for (auto& item : m_setTimers)
				__pred(item);
		}

		//	DO_SOMETHING_TO_ALL_MUTEX(m_setTimers, m_TimersMutex)
		//	DO_SOMETHING_TO_ONE_MUTEX(m_setTimers, m_TimersMutex)
		// ֹͣ���ж�ʱ����
		void StopAllTimer()
		{
			//	do_something_to_all(m_setTimers, m_TimersMutex, [this](ObjectType& item) { StopTimer(item); });

			do_something_to_all([this](ObjectType& item) { StopTimer(item); });
		}
	protected:
		void Reset()
		{
			CAsioObject::Reset();
		}
		// �ڲ�ʹ�õ�helper�����������Ŀ�����ʱ����boost::asio::deadline_timer::async_wait����
		void StartTimer(object_ctype& ti)
		{
			ti.timer->expires_from_now(boost::posix_time::milliseconds(ti.milliseconds));
			//return true from call_back to continue the timer, or the timer will stop
			ti.timer->async_wait(
				make_handler_error([this, &ti](const boost::system::error_code& ec)
			{
				if (!ec && ti.call_back(ti.id) && CAsioTimer::ObjectType::TIMER_OK == ti.status)
					StartTimer(ti);
			})
				);

		}
		// �ڲ�ʹ�õ�helper�����������Ľ�����ʱ��������boost::asio::deadline_timer::cancel����
		void StopTimer(ObjectType& ti)
		{
			boost::system::error_code ec;
			ti.timer->cancel(ec);
			ti.status = ObjectType::TIMER_CANCELED;
		}

		ContainerType m_setTimers;
		boost::shared_mutex m_TimersMutex;
	private:
		using CAsioObject::m_IoSvcRef;
	};

} //namespace

#endif /* ifndef ASIO_TIMER_H */

