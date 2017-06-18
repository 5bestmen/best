
#ifndef _THREAD_SAFE_DATA_H
#define _THREAD_SAFE_DATA_H

#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <QMultiHash>
#include <deque>   

// 共享数据区
template<typename Key, typename Value>

class CThreadSafeHash
{
public:
	CThreadSafeHash() {}

	bool contains(const Key &key) const
	{
		QReadLocker locker(&lock);
		return hash.contains(key);
	}

	int count() const
	{
		QReadLocker locker(&lock);
		return hash.count();
	}
	
	int count(const Key &key) const
	{
		QReadLocker locker(&lock);
		return hash.count(key);
	}

	bool isEmpty() const
	{
		QReadLocker locker(&lock);
		return hash.isEmpty();
	}

	void clear()
	{
		QWriteLocker locker(&lock);
		hash.clear();
	}
	
	void insert(const Key &key, const Value &value)
	{
		QWriteLocker locker(&lock);
		hash.insert(key, value);
	}

	int remove(const Key &key)
	{
		QWriteLocker locker(&lock);
		return hash.remove(key);
	}

	int remove(const Key &key, const Value &value)
	{
		QWriteLocker locker(&lock);
		return hash.remove(key, value);
	}
	
	QList<Value> values(const Key &key) const
	{
		QReadLocker locker(&lock);
		return hash.values(key);
	}

	const QList<Value> takeOne(bool *more)
	{
		Q_ASSERT(more);
		QWriteLocker locker(&lock);
		typename QMultiHash<Key, Value>::const_iterator i =
			hash.constBegin();
		if (i == hash.constEnd()) 
		{
			*more = false;
			return QList<Value>();
		}
		*more = true;
		const QList<Value> values = hash.values(i.key());
		hash.remove(i.key());
		return values;
	}

private:
	mutable QReadWriteLock lock;
	QMultiHash<Key, Value> hash;
};


template <typename  T> 
class CSafeDeque
{
public:
	CSafeDeque(){}
	typedef	typename  std::deque<T>::iterator ITERATOR;
public:
	T& front()
	{
		QWriteLocker locker(&m_rwLock);
		return m_Queue.front();
	}

	void push_back(const T& element)
	{
		QWriteLocker locker(&m_rwLock);
		m_Queue.push_back(element);
	}
	void push_front(const T& element)
	{
		QWriteLocker locker(&m_rwLock);
		m_Queue.push_front(element);
	}
	void pop_back()
	{
		QWriteLocker locker(&m_rwLock);
		m_Queue.pop_back();
	}
	void pop_front()
	{
		QWriteLocker locker(&m_rwLock);
		m_Queue.pop_front();
	}
	//删除指定元素
	void erase(typename  std::deque<T>::iterator  pos)
	{
		QWriteLocker locker(&m_rwLock);
		m_Queue.erase(pos);
	}
	//删除区间元素
	void erase(typename  std::deque<T>::iterator posBegin, typename  std::deque<T>::iterator  posEnd)
	{
		QWriteLocker locker(&m_rwLock);
		m_Queue.erase(posBegin,posEnd);
	}
	// 从头部开始删除到只剩下  nLeftCount 个元素
	void erase(size_t nLeftCount)
	{
		if (nLeftCount <= 0)
			return;

		QWriteLocker locker(&m_rwLock);
		while (m_Queue.size() >= nLeftCount)
		{
			m_Queue.pop_front();
		}
	}

	ITERATOR begin()
	{
		QReadLocker locker(&m_rwLock);
		return m_Queue.begin();
	}

	ITERATOR end()
	{
		QReadLocker locker(&locker);
		return m_Queue.end();
	}

	void clear()
	{
		QWriteLocker locker(&m_rwLock);
		m_Queue.clear();
	}

	bool empty() const
	{
		QReadLocker locker(&m_rwLock);
		return m_Queue.empty();
	}

	size_t size()
	{
		QReadLocker locker(&m_rwLock);
		return m_Queue.size();
	}

	size_t max_size()
	{
		QReadLocker locker(&m_rwLock);
		return m_Queue.max_size();
	}

	T& at(int pos)
	{
		QReadLocker locker(&m_rwLock);
		return m_Queue.at(pos);
	}
	// pos from zero?????
	void at(int pos, T& ret)
	{
		QReadLocker locker(&m_rwLock);
		if (pos >= m_Queue.size())
			return;
		ret = m_Queue.at(pos);		
	}
#if 0
	void Remove(int nPos)
	{
		QWriteLocker locker(&locker);
		T & t = m_Queue.at(nPos);
	}
#endif

private:
	mutable QReadWriteLock m_rwLock;
	std::deque<T> m_Queue;
};

#endif
