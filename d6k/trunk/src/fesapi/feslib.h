
#ifndef FESLIB_H
#define FESLIB_H
#include "datatypes.h"
#include <memory>
#include <atomic>
 
class CMemDB;
class CTagNamePool;
class CStringPool;

class CFesApi
{
public:
	CFesApi(void);
	~CFesApi(void);
public:
	bool Initialize(const char *pszDataPath, int32u nChannelOccNO, unsigned int nMode);
	void Run();
	void Shutdown(int32u nChannelOccNO = 0 );

public:
	void SetQuit()
	{
		m_bQuit = true;
	}
	std::shared_ptr<CMemDB> &GetMemDB()
	{
		return m_pMemDB;
	}
	std::shared_ptr<CTagNamePool> &GetTagNamePool()
	{
		return m_pTagNamePool;
	}
	std::shared_ptr<CStringPool>& GetStringPool()
	{
		return m_pStringPool;
	}
private:
	std::atomic<bool> m_bQuit;

	std::atomic<int> m_nRefCount;

protected:
	std::shared_ptr<CMemDB> m_pMemDB;
	std::shared_ptr<CTagNamePool> m_pTagNamePool;
	std::shared_ptr<CStringPool> m_pStringPool;
};


#endif // FESLIB_H

