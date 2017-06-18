
#ifndef _STRPOOL_API_H
#define _STRPOOL_API_H

#include <string>

#include "tinyxml/tinyxml.h"

#include "datatypes.h"
#include "public.h"


#ifdef STRPOOL_LIB
# define STRPOOLAPI __declspec(dllexport)
#else
# define STRPOOLAPI __declspec(dllimport)
#endif

namespace StrPool
{
	
	extern "C"
	{
		STRPOOLAPI KRESULT CreateStringPool(TiXmlElement* pStringElement);
		STRPOOLAPI KRESULT DestroyStringPool();
		STRPOOLAPI KRESULT OpenStringPool();
		STRPOOLAPI KRESULT CloseStringPool();
				
		STRPOOLAPI KRESULT SetActiveStringPool(int32u lang = LANG_EN);
		STRPOOLAPI KRESULT LoadStringFromPool(std::string& str,int32u strOccNo, int32u lang = LANG_NONE);
	}
}


#endif // _STRPOOL_API_H

