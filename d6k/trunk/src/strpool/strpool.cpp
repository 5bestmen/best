
#include "strpool/strpool.h"

#include "strpoolimpl.h"


namespace StrPool
{
	KRESULT CreateStringPool(TiXmlElement* pStringElement)
	{
		if (g_StrPoolImpl.CreateStringPool(pStringElement))
			return K_TRUE;

		return K_FALSE;
	}
	KRESULT DestroyStringPool()
	{
		if (g_StrPoolImpl.DestroyStringPool())
			return K_TRUE;

		return K_FALSE;
	}
	KRESULT OpenStringPool()
	{
		if (g_StrPoolImpl.OpenStringPool())
			return K_TRUE;

		return K_FALSE;
	}
	KRESULT CloseStringPool()
	{
		if (g_StrPoolImpl.CloseStringPool())
			return K_TRUE;

		return K_FALSE;
	}

	KRESULT SetActiveStringPool(int32u lang)
	{
		if (g_StrPoolImpl.SetActiveStringPool(lang))
			return K_TRUE;

		return K_FALSE;
	}
	KRESULT LoadStringFromPool(std::string& str, int32u strOccNo, int32u lang)
	{
		if (g_StrPoolImpl.LoadStringFromPool(str, strOccNo, lang))
			return K_TRUE;

		return K_FALSE;
	}
}
