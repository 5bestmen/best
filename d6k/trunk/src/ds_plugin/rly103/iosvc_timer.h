

#ifndef IOSVC_TIMER_H
#define IOSVC_TIMER_H

//#include <set>
#include <functional>
#include <memory>


#include <boost/container/set.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp> 
//#include <boost/thread.hpp>

//timers are identified by id.
//for the same timer in the same CIoSvcTimer, set_timer and stop_timer are not thread safe, please pay special attention.
//to resolve this defect, we must add a mutex member variable to timer_info, it's not worth
//
//suppose you have more than one service thread(see st_service_pump for service thread number control), then:
//same st_timer, same timer, on_timer is called sequentially
//same st_timer, different timer, on_timer is called concurrently
//different st_timer, on_timer is always called concurrently
class CIoSvcTimer
{
public:
	CIoSvcTimer(boost::asio::io_service& io_svc) : m_IoSvcRef(io_svc)
	{
	}
	~CIoSvcTimer()
	{
	}

protected:
	//定时器数据结构。
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
		// 在定时器到达后，call_back被回调，并根据返回值决定是否继续这个定时器（true即继续），同一个定时器，call_back的调用是顺序的。
		std::function<bool(unsigned int)> call_back;
		std::shared_ptr<boost::asio::deadline_timer> timer;

		bool operator <(const timer_info& other) const
		{
			return id < other.id;
		}
	};

	// 继承者的定义器ID必须从父类的TIMER_END开始，然后最好也定义一个自己的TIMER_END，如果你这个类可能会被继承的话。
	static const unsigned int TIMER_END = 0; //user timer's id must begin from parent class' TIMER_END
public:
	using ObjectType = timer_info;
	using object_ctype = const ObjectType;
	using ContainerType = boost::container::set<ObjectType>;

	// 开启定时器，定时器以id分区，如果定时器已经存在，则重新开始记时。这个函数其实就是以start为true调用update_timer_info而已。
	void SetTimer(unsigned int id, size_t milliseconds, std::function<bool(unsigned int)>&& call_back)
	{
		UpdateTimerInfo(id, milliseconds, std::move(call_back), true);
	}
	void SetTimer(unsigned int id, size_t milliseconds, const std::function<bool(unsigned int)>& call_back)
	{
		UpdateTimerInfo(id, milliseconds, call_back, true);
	}
	// 更新timer，如果start为true，更新完了之后马上会开启或者重新开启这个timer。
	// 注意，对同一个timer里面的同一个定时器操作并不是线程安全的。
	void UpdateTimerInfo(unsigned int nID, size_t nMilliseconds, std::function<bool(unsigned int)>&& fnCallBack, bool bStart = false)
	{
		ObjectType ti = { nID };

		m_TimersMutex.lock_upgrade();

		ContainerType::iterator iter = m_setTimers.find(ti);

		if (iter == std::end(m_setTimers))
		{
			m_TimersMutex.unlock_upgrade_and_lock();
			iter = m_setTimers.insert(ti).first;
			m_TimersMutex.unlock();
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

	// 查找定义器。
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
	// 开启一个已经存在的定义器。注意stop_timer之后，定义器还是存在的，只是未启动。
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
	// 停止定时器，如果定时时间已经到达，且已经被post到io_server的队列中，则on_timer仍然会在未来某个时候被回调，这是asio的设计。
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

	template<typename _Predicate>
	void do_something_to_all(const _Predicate& __pred)
	{
		boost::shared_lock<boost::shared_mutex> lock(m_TimersMutex);
		for (auto& item : m_setTimers)
			__pred(item);
	}

	// 停止所有定时器。
	void StopAllTimer()
	{
		do_something_to_all([this](ObjectType& item) { StopTimer(item); });
	}
protected:
	template<typename F>
	inline F&& make_handler_error(F&& f) const
	{
		return std::move(f);
	}

	// 内部使用的helper函数，真正的开启定时器（boost::asio::deadline_timer::async_wait）。
	void StartTimer(object_ctype& ti)
	{
		ti.timer->expires_from_now(boost::posix_time::milliseconds(ti.milliseconds));
		//return true from call_back to continue the timer, or the timer will stop
		ti.timer->async_wait(
			make_handler_error([this, &ti](const boost::system::error_code& ec)
		{
			if (!ec && ti.call_back(ti.id) && CIoSvcTimer::ObjectType::TIMER_OK == ti.status)
				StartTimer(ti);
		})
			);

	}
	// 内部使用的helper函数，真正的结束定时器（调用boost::asio::deadline_timer::cancel）。
	void StopTimer(ObjectType& ti)
	{
		boost::system::error_code ec;
		ti.timer->cancel(ec);
		ti.status = ObjectType::TIMER_CANCELED;
	}

	ContainerType m_setTimers;
	boost::shared_mutex m_TimersMutex;
private:
	boost::asio::io_service& m_IoSvcRef;
};


#endif /* ifndef IOSVC_TIMER_H */

