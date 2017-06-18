

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
public:
	using ObjectType = timer_info;
	using object_ctype = const ObjectType;
	using ContainerType = boost::container::set<ObjectType>;

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

	template<typename _Predicate>
	void do_something_to_all(const _Predicate& __pred)
	{
		boost::shared_lock<boost::shared_mutex> lock(m_TimersMutex);
		for (auto& item : m_setTimers)
			__pred(item);
	}

	// ֹͣ���ж�ʱ����
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

	// �ڲ�ʹ�õ�helper�����������Ŀ�����ʱ����boost::asio::deadline_timer::async_wait����
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
	boost::asio::io_service& m_IoSvcRef;
};


#endif /* ifndef IOSVC_TIMER_H */

