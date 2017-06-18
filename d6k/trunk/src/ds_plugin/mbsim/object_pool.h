
#ifndef ASIO_OBJECT_POOL_H_
#define ASIO_OBJECT_POOL_H_

#include <boost/atomic.hpp>
#include <boost/unordered_set.hpp>

#include "asio_timer.h"
#include "service_pump.h"

#ifndef ASIO_MAX_OBJECT_NUM
#define ASIO_MAX_OBJECT_NUM	4096
#endif


//释放（从内存中删除）已经关闭的连接的扫描频率，单位为秒。
//如果开启了对象重用，本功能不工作，但使用者仍然可以自己调用free_object来释放指定数量的已经关闭的连接。
//define ASIO_REUSE_OBJECT macro will enable object pool, all objects in temp_object_can will never be freed, but kept for reusing,
//otherwise, st_object_pool will free objects in temp_object_can automatically and periodically, ST_ASIO_FREE_OBJECT_INTERVAL means the interval, unit is second,
//see temp_object_can at the end of st_object_pool class for more details.
//please note that even if you defined ASIO_REUSE_OBJECT macro, ASIO_FREE_OBJECT_INTERVAL macro is still useful, it will make st_object_pool
//to close (just close, not free, Object must has close function which takes no parameter) objects automatically and periodically for saving SOCKET handles.
#ifndef ASIO_REUSE_OBJECT
#ifndef ASIO_FREE_OBJECT_INTERVAL
#define ASIO_FREE_OBJECT_INTERVAL	10 //seconds, validate only if ASIO_REUSE_OBJECT not defined
#endif
#endif

//自动清除（从对象池移出到临时链表）已经关闭的连接，这个功能有什么用呢？
//如果在连接断开时，你没有或者不想去从对象池中删除，那么可以让st_object_pool周期性的查找已经关闭的连接，
//然后移出对象池，这样一次遍历可以操作多条连接，对于短连接效率可能会比每条连接断开时都从对象池中移除高。
//如果每次连接断开时，你都移除了对象池（请看st_server::del_client），请务必不要定义这个宏。
// 清除已经关闭的连接的扫描频率，单位为秒。
//define this to have st_server_socket_base invoke st_object_pool::clear_all_closed_object() automatically and periodically
//this feature may influence performance when huge number of objects exist,
//so, re-write st_server_socket_base::on_recv_error and invoke st_object_pool::del_object() is recommended in long connection system
//in short connection system, you are recommended to open this feature, use ASIO_CLEAR_CLOSED_SOCKET_INTERVAL to set the interval

#define ASIO_CLEAR_CLOSED_SOCKET_INTERVAL	60 //seconds, validate only if ST_ASIO_AUTO_CLEAR_CLOSED_SOCKET defined


//after this duration, corresponding objects in temp_object_can can be freed from the heap or reused,
//you must define this macro as a value, not just define it, the value means the duration, unit is second.
//if macro ST_ASIO_ENHANCED_STABILITY been defined, this macro is useless, object's life time is always zero.
#ifndef ASIO_OBSOLETED_OBJECT_LIFE_TIME
#define ASIO_OBSOLETED_OBJECT_LIFE_TIME	5 //seconds

//after this duration, the corresponding object can be freed from the heap or be reused
#endif

namespace ASIO
{
	//对象池类，用于管理正常的和失效的连接，对象重用，定时删除已经关闭的连接等；
	//注意，只能管理由CAsioSocket派生的对象
	template<typename Object>
	class CObjectPool : public CServicePump::CServiceInterface, public CAsioTimer
	{
	public:
// 		typedef boost::shared_ptr<Object> ObjectType;
// 		typedef const ObjectType object_ctype;
		using ObjectType = boost::shared_ptr<Object>;
		using object_ctype = const ObjectType;

	protected:
		// hash表需要的对象比较类
		struct object_hasher
		{
		public:
			size_t operator()(object_ctype& object_ptr) const
			{
				return (size_t)object_ptr->GetID();
			}
			size_t operator()(uint_fast64_t id) const
			{
				return (size_t)id;
			}
		};

		struct object_equal
		{
		public:
			bool operator()(object_ctype& left, object_ctype& right) const
			{
				return left->GetID() == right->GetID();
			}
			bool operator()(uint_fast64_t id, object_ctype& right) const
			{
				return id == right->GetID();
			}
		};
	public:
		//typedef boost::unordered::unordered_set<ObjectType, object_hasher, object_equal> ContainerType;
		using ContainerType = boost::unordered::unordered_set<ObjectType, object_hasher, object_equal>;
	protected:
		// 已经关闭的连接，从对象池移出到临时链表时，将按这个结构保存，主要目的是为了记录关闭的时间（以判断是否可以释放或者重用）。
		struct invalid_object
		{
			object_ctype object_ptr;
#ifdef ST_ASIO_ENHANCED_STABILITY
			invalid_object(object_ctype& object_ptr_) : object_ptr(object_ptr_)
			{ 
				assert(object_ptr); 
			}

			bool IsTimeout() const
			{
				return true; 
			}
			bool IsTimeout(time_t now) const
			{
				return true; 
			}
#else
			const time_t kick_out_time;

			invalid_object(object_ctype& object_ptr_) : kick_out_time(time(nullptr)), object_ptr(object_ptr_)
			{
				assert(object_ptr);
			}
			// 是否超时（超时之后就可以释放或者重用object_ptr对象了）。
			bool IsTimeout() const
			{
				return IsTimeout(time(nullptr));
			}
			bool IsTimeout(time_t now) const
			{
				return kick_out_time <= now - ASIO_OBSOLETED_OBJECT_LIFE_TIME;
			}
#endif
		};

	protected:
		static const unsigned int TIMER_BEGIN = CAsioTimer::TIMER_END;
		static const unsigned int TIMER_FREE_SOCKET = TIMER_BEGIN;
		static const unsigned int TIMER_CLEAR_SOCKET = TIMER_BEGIN + 1;
		static const unsigned int TIMER_END = TIMER_BEGIN + 10;
		
		CObjectPool(CServicePump& svc_pump) : CServiceInterface(svc_pump), CAsioTimer(svc_pump), m_nCurId(static_cast<unsigned int>(-1)), m_nMaxSize(ASIO_MAX_OBJECT_NUM)
		{

		}
		//开始，根据宏开启一些定时器，比如如果未定义REUSE_OBJECT，则开启一个定时器用于定时查找已经关闭的连接。
		void Start()
		{
#ifndef ST_ASIO_FREE_OBJECT_INTERVAL
			SetTimer(TIMER_FREE_SOCKET, 1000 * ASIO_FREE_OBJECT_INTERVAL, [this](unsigned char id)->bool {assert(TIMER_FREE_SOCKET == id);  free_object(); return true; });
#endif
#ifdef ASIO_NOT_REUSE_ADDRESS
			SetTimer(TIMER_CLEAR_SOCKET, 1000 * ASIO_NOT_REUSE_ADDRESS, [this](unsigned char id)->bool {assert(TIMER_CLEAR_SOCKET == id);  clear_obsoleted_object(); return true;});
#endif
		}

		void Stop()
		{
			StopAllTimer();
		}

		bool add_object(object_ctype& object_ptr)
		{
			//	assert(object_ptr && &object_ptr->GetIoService() == &GetServicePump());
			assert(object_ptr);

			boost::unique_lock<boost::shared_mutex> lock(object_can_mutex);
			return object_can.size() < m_nMaxSize ? object_can.insert(object_ptr).second : false;
		}

		//only add object_ptr to m_lstInvalidObjects when it's in object_can, this can avoid duplicated items in m_lstInvalidObjects, because m_lstInvalidObjects is a list,
		//there's no way to check the existence of an item in a list efficiently.
		//清除一个对象（从对象池移出到临时链表，并不真正的从内存中释放该对象）。
		bool del_object(object_ctype& object_ptr)
		{
			assert(object_ptr);

			boost::unique_lock<boost::shared_mutex> lock(object_can_mutex);
			bool bExist = object_can.erase(object_ptr) > 0;
			lock.unlock();

			if (bExist)
			{
				boost::unique_lock<boost::shared_mutex> lock( invalid_object_can_mutex);
				m_lstInvalidObjects.push_back(object_ptr);
			}

			return bExist;
		}
		virtual void on_create(object_ctype& object_ptr)
		{
			std::ignore = object_ptr;
		}
		void init_object(object_ctype& object_ptr)
		{
			if (object_ptr)
			{
				object_ptr->SetID(++m_nCurId);
				on_create(object_ptr);
			}
			//	else
			//		unified_out::error_out("create object failed!");
		}
		//每次创建（包括重用）一个对象之后，会调用这个函数，使用者可以利用这个机会（重写这个虚函数），在对象池级别
		//对新创建的对象做一些必要的设置（最典型的就是设置对象的ID，不过这个工作已经在object_pool::post_create里面做了，
		//所以如果要做你自己的设置，必须在重写的post_create函数中调用父类的post_create函数）。
		virtual void post_create(object_ctype& object_ptr)
		{
			if (object_ptr)
				object_ptr->SetID(++m_nCurId);
		}

#ifdef ASIO_REUSE_OBJECT
		ObjectType reuse_object()
		{
			boost::unique_lock<boost::shared_mutex> lock( invalid_object_can_mutex);
			//objects are order by time, so we don't have to go through all items in temp_object_can
			for (auto iter = std::begin(m_lstInvalidObjects); iter != std::end(m_lstInvalidObjects) && iter->IsTimeout(); ++iter)
				if (iter->object_ptr.unique() && iter->object_ptr->IsObsoleted())
				{
					auto object_ptr(std::move(iter->object_ptr));
					m_lstInvalidObjects.erase(iter);
					lock.unlock();

					object_ptr->reset();
					return object_ptr;
				}

			return ObjectType();
		}
		template<typename Arg>
		ObjectType create_object(Arg& arg)
		{
			auto object_ptr = reuse_object();
			if (!object_ptr)
				object_ptr = boost::make_shared<Object>(arg);

			init_object(object_ptr);
			return object_ptr;
		}

		template<typename Arg1, typename Arg2>
		object_type create_object(Arg1& arg1, Arg2& arg2)
		{
			auto object_ptr = reuse_object();
			if (!object_ptr)
				object_ptr = boost::make_shared<Object>(arg1, arg2);

			init_object(object_ptr);
			return object_ptr;
		}
#else
		template<typename Arg>
		ObjectType create_object(Arg& arg)
		{
			auto object_ptr = boost::make_shared<Object>(arg);
			init_object(object_ptr);
			return object_ptr;
		}

		template<typename Arg1, typename Arg2>
		ObjectType create_object(Arg1& arg1, Arg2& arg2)
		{
			auto object_ptr = boost::make_shared<Object>(arg1, arg2);
			init_object(object_ptr);
			return object_ptr;
		}
#endif
		ObjectType create_object()
		{
			return create_object(m_ServicePumpRef);
		}
	public:
		//to configure unordered_set(for example, set factor or reserved size), not locked the mutex, so must be called before service_pump starting up.
		ContainerType& container()
		{
			return object_can;
		}

		size_t max_size() const
		{
			return m_nMaxSize;
		}
		void max_size(size_t _max_size)
		{
			m_nMaxSize = _max_size;
		}

		size_t size()
		{
			boost::shared_lock<boost::shared_mutex> lock(object_can_mutex);
			return object_can.size();
		}

		size_t invalid_object_size()
		{
			boost::shared_lock<boost::shared_mutex> lock( invalid_object_can_mutex);
			return m_lstInvalidObjects.size();
		}
		// 根据id查找有效对象。
		ObjectType find(uint_fast64_t id)
		{
			boost::shared_lock<boost::shared_mutex> lock(object_can_mutex);
			auto iter = object_can.find(id, object_hasher(), object_equal());
			return iter != std::end(object_can) ? *iter : ObjectType();
		}
		//this method has linear complexity, please note.
		ObjectType at(size_t index)
		{
			boost::shared_lock<boost::shared_mutex> lock(object_can_mutex);
			assert(index < object_can.size());
			return index < object_can.size() ? *(std::next(std::begin(object_can), index)) : ObjectType();
		}

		//this method has linear complexity, please note.
		// 获取指定位置的无效对象，位置序号从0开始。
		ObjectType invalid_object_at(size_t index)
		{
			boost::shared_lock<boost::shared_mutex> lock( invalid_object_can_mutex);
			assert(index < m_lstInvalidObjects.size());
			return index < m_lstInvalidObjects.size() ? std::next(std::begin(m_lstInvalidObjects), index)->object_ptr : ObjectType();
		}
	
		// 根据id查找无效对象。this method has linear complexity, please note.
		ObjectType invalid_object_find(uint_fast64_t id)
		{
			boost::shared_lock<boost::shared_mutex> lock( invalid_object_can_mutex);
			for (auto iter = std::begin(m_lstInvalidObjects); iter != std::end(m_lstInvalidObjects); ++iter)
				if (id == iter->object_ptr->id())
					return iter->object_ptr;
			return ObjectType();
		}

		// 根据id查找无效对象，并且从容器中删除。this method has linear complexity, please note.
		ObjectType invalid_object_pop(uint_fast64_t id)
		{
			boost::shared_lock<boost::shared_mutex> lock( invalid_object_can_mutex);
			for (auto iter = std::begin(m_lstInvalidObjects); iter != std::end(m_lstInvalidObjects); ++iter)
				if (id == iter->object_ptr->id())
				{
					auto object_ptr = iter->object_ptr;
					m_lstInvalidObjects.erase(iter);
					return object_ptr;
				}
			return ObjectType();
		}
		//列出所有有效对象。
		void list_all_object()
		{
			do_something_to_all( [](object_ctype& item) {item->show_info("", ""); } );
		}

		//Kick out obsoleted objects
		//Consider the following assumption:
		//1.You didn't invoke del_object in on_recv_error or other places.
		//2.For some reason(I haven't met yet), on_recv_error not been invoked
		//st_object_pool will automatically invoke this function if ASIO_NOT_REUSE_ADDRESS been defined
		// 删除对象池里面的所有无效对象（移出到临时链表）。
		size_t clear_obsoleted_object()
		{
			ContainerType objects;

			boost::unique_lock<boost::shared_mutex> lock(object_can_mutex);
			for (auto iter = std::begin(object_can); iter != std::end(object_can);)
				if ((*iter).unique() && (*iter)->obsoleted())
				{
#ifdef ST_ASIO_ENHANCED_STABILITY
					(*iter)->close();
#endif
					objects.insert(*iter);
					iter = object_can.erase(iter);
				}
				else
					++iter;
			lock.unlock();

			auto size = objects.size();
			if (0 != size)
			{
				boost::unique_lock<boost::shared_mutex> lock( invalid_object_can_mutex);
				m_lstInvalidObjects.insert(std::end(m_lstInvalidObjects), std::begin(objects), std::end(objects));
			}

			return size;
		}

		//free or close a specific number of objects
		//if you used object pool(define ASIO_REUSE_OBJECT), you can manually call this function to free some objects after the object pool(get_closed_object_size())
		// goes big enough for memory saving(because the objects in m_lstInvalidObjects are waiting for reusing and will never be freed),
		// you can also define ASIO_FREE_OBJECT_INTERVAL to let st_object_pool to call this function automatically and periodically, but objects will only be closed.
		//if you don't used object pool, st_object_pool will invoke this function automatically and periodically, so you don't need to invoke this function exactly
		//return affected object number, if just_close equal to true, then closed objects will be treated as unaffected.
		// 释放指定数量的无效对象，如果对象重用开启，则无效对象永远不会释放（而是等待被重用），在某些情况下，你可能不需要
		//这么多等待被重用的无效对象，可以用这个函数来释放一些无效对象。
#ifdef ASIO_REUSE_OBJECT
		size_t free_object(size_t num = -1, bool just_close = true)
#else
		size_t free_object(size_t num = -1, bool just_close = false)
#endif
		{
			size_t num_affected = 0;

			boost::unique_lock<boost::shared_mutex> lock( invalid_object_can_mutex);
			//objects are order by time, so we don't have to go through all items in m_lstInvalidObjects
			for (auto iter = std::begin(m_lstInvalidObjects); num > 0 && iter != std::end(m_lstInvalidObjects) && iter->IsTimeout();)
			{
				if (iter->object_ptr.unique() && iter->object_ptr->obsoleted())
				{
					--num;
					if (just_close)
					{
						if (iter->object_ptr->close())
							++num_affected;
						++iter;
					}
					else
					{
						++num_affected;
						iter = m_lstInvalidObjects.erase(iter);
					}
				}
				else
					++iter;
			}

			return num_affected;
		}
		// #define DO_SOMETHING_TO_ALL_MUTEX(CAN, MUTEX) DO_SOMETHING_TO_ALL_MUTEX_NAME(do_something_to_all, CAN, MUTEX)
		// #define DO_SOMETHING_TO_ALL_MUTEX_NAME(NAME, CAN, MUTEX) \
		// 	template<typename _Predicate> void NAME(const _Predicate& __pred) {boost::shared_lock<boost::shared_mutex> lock(MUTEX); for (auto& item : CAN) __pred(item);}

		// 		DO_SOMETHING_TO_ALL_MUTEX(object_can, object_can_mutex)
		//		DO_SOMETHING_TO_ONE_MUTEX(object_can, object_can_mutex)

		template<typename _Predicate>
		void do_something_to_all(const _Predicate& __pred)
		{
			boost::shared_lock<boost::shared_mutex> lock(object_can_mutex);
			for (auto& item : object_can)
				__pred(item);
		}

	protected:
		// 当前已经分配到哪个id了，用于为每一个通过st_object_pool::create_object创建的对象分配一个唯一的id。
		boost::atomic_uint_fast64_t m_nCurId;

		// 存放有效对象（对象池）。
		ContainerType object_can;
		boost::shared_mutex object_can_mutex;
		size_t m_nMaxSize;

		//because all objects are dynamic created and stored in object_can, maybe when receiving error occur
		//(you are recommended to delete the object from object_can, for example via st_server_base::del_client), some other asynchronous calls are still queued in boost::asio::io_service,
		//and will be dequeued in the future, we must guarantee these objects not be freed from the heap or reused, so we move these objects from object_can to m_lstInvalidObjects,
		//and free them from the heap or reuse them in the near future, see ASIO_OBSOLETED_OBJECT_LIFE_TIME macro for more details.
		//if ASIO_NOT_REUSE_ADDRESS been defined, clear_obsoleted_object() will be invoked automatically and periodically to move all closed objects into m_lstInvalidObjects.
		boost::container::list<invalid_object> m_lstInvalidObjects;
		boost::shared_mutex  invalid_object_can_mutex; // 存放无效对象（临时链表）。
	};

} //namespace

#endif /* ASIO_WRAPPER_OBJECT_POOL_H_ */
