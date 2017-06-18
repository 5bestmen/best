
#include "log/log.h"

#include "logimpl.h"

#include <vector>

namespace Log
{
	KRESULT CreateLog(char* szLogName)
	{
		if(g_LogImpl.CreateLog(szLogName))
			return K_TRUE;

		return K_FALSE;
	}


	KRESULT DestroyLog()
	{
		if(g_LogImpl.DestroyLog())
			return K_TRUE;

		return K_FALSE;
	}


	KRESULT OpenLog(char* szLogName)
	{
		if(g_LogImpl.OpenLog(szLogName))
			return K_TRUE;

		return K_FALSE;
	}


	KRESULT CloseLog()
	{
		if(g_LogImpl.CloseLog())
			return K_TRUE;

		return K_FALSE;
	}

	KRESULT LogMsg(char * pszMoudleName,char *szLogTxt,int nLevel)
	{
		if(g_LogImpl.LogMsg(pszMoudleName,szLogTxt,nLevel))
			return K_TRUE;

		return K_FALSE;
	}
}
