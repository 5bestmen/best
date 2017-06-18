#pragma once

#include "datatypes.h" 
#include "fesapi/fesdatadef.h"
#include "fesapi/fes_magicmem.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

class CShareMem;
struct StringUnit;

//×Ö·û´®³Ø
class CStringPool
{
public:
	CStringPool();
	~CStringPool();	
public:
	bool Initialize(const char* pszProjName, int32u nMode);
	void Run();
	void Shutdown();
public:
	bool GetDescByOccno(int nOccNo,  bool bIsTranslate, char* pBuff);
	size_t GetPoolCount()
	{
		return m_MapStrings.size();
	}
public:
	bool GetStringPoolByOccNo(int32u nOccNo,StringUnit* pStringUnit);	
protected:
	void   LogMsg(const char * pszText, int nLevel);
private:
	int m_nStringsCount;
	HEAD_STRING * m_pStringHead;
	std::shared_ptr<CShareMem> m_pMem;	
	std::unordered_map < int32u, StringUnit* > m_MapStrings;
private:
	bool m_bInitialized;

};

