
#ifndef _STRPOOL_IMPL_H
#define _STRPOOL_IMPL_H

#include <string>
#include <map>

#include "utils/array.h"
#include "tinyxml/tinyxml.h"

#include "strdefine.h"


class CStrPoolImpl
{
public:
	CStrPoolImpl(void);
	~CStrPoolImpl(void);


public:
	bool CreateStringPool(TiXmlElement* pStringElement);
	bool DestroyStringPool();
	bool OpenStringPool();
	bool CloseStringPool();

	bool SetActiveStringPool(int lang);
	bool LoadStringFromPool(std::string& str, int nStrOccNo, int nLang);

private:
	bool InnerCreateStringPool(TiXmlElement* pLangElement,int nIndex);
	int GetElementChildCount(TiXmlElement* pElement, std::string strTag);
	int LangStringToID(std::string strLang);
private:
	int m_nActiveLang;
	
	typedef std::map<int, CArray<STRINGDATA>* >  mapStrPool;
	mapStrPool m_mapStringPool; //int:lang,CArray:StrPool
	CArray<int> m_arrayStringPoolIndex; //记录所有字符串池的langid
};

extern CStrPoolImpl g_StrPoolImpl;


#endif // _STRPOOL_IMPL_H

