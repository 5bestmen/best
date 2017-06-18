/*! @file singleton.h
<PRE>
********************************************************************************
ģ����       :  ����ģʽ
�ļ���       :  singleton.h
�ļ�ʵ�ֹ��� :  ����ģʽ
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author LiJin
*  @version 1.0
*  @date    2015.11.15
*/

//#include "at_exit_manager.h"

#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>

#define DISALLOW_COPY(classname)                        \
    classname(const classname&) = delete;               \
    classname& operator=(const classname&) = delete

#define DISALLOW_MOVE(classname)                        \
    classname(classname&&) = delete;                    \
    classname& operator=(classname&&) = delete

#define DEFAULT_COPY(classname)                         \
    classname(const classname&) = default;              \
    classname& operator=(const classname&) = default;

#define DEFAULT_MOVE(classname)                         \
    classname(classname&&) = default;                   \
    classname& operator=(classname&&) = default;

template<typename T>
struct DefaultSingletonTraits
{
	static T* Create()
	{
		return new T();
	}

	static void Destroy(T* pInstance)
	{
		delete pInstance;
	}

	static const bool kDestroyAtExit = true;
};

template<typename T>
struct LeakySingletonTraits : public DefaultSingletonTraits<T>
{
	static const bool kDestroyAtExit = false;
};

template<typename T, typename Traits = DefaultSingletonTraits<T>>
class Singleton
{
public:
	Singleton() = delete;

	~Singleton() = delete;

	DISALLOW_COPY(Singleton);

	DISALLOW_MOVE(Singleton);

	static T* instance()
	{
		std::call_once(m_OnceFlag, &Singleton::Initialize);
		return m_pInstance;
	}

private:
	static void Initialize()
	{
		m_pInstance = Traits::Create();
		if (Traits::kDestroyAtExit) 
		{
			std::atexit([]()
				 			{
				 				Traits::Destroy(m_pInstance);
				 			});
		}
	}

private:
	static T* m_pInstance;
	static std::once_flag m_OnceFlag;
};

template<typename T, typename Traits = DefaultSingletonTraits<T>>
T* Singleton<T, Traits>::m_pInstance = nullptr;

template<typename T, typename Traits = DefaultSingletonTraits<T>>
std::once_flag Singleton<T, Traits>::m_OnceFlag;

#endif // SINGLETON_H


/** @}*/

