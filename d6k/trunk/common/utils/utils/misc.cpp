
#include "misc.h"

#include <ace/Date_Time.h> 
#include <ace/OS_NS_sys_time.h>
#include <ace/OS_NS_unistd.h>
/*
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif
*/

namespace Utils
{
	std::string FormatBuffer(char* pBuff,int nMsgLen)
	{
		std::string strDebugData("");
		char szByte[10]  = {'\0'};

		for(int i = 0; i < nMsgLen; i++)
		{
			sprintf(szByte, "%02X ", (unsigned char)pBuff[i]);
			strDebugData += szByte;
		}

		return strDebugData;
	}

	std::string FormatTimestamp(int nType)
	{
		unsigned long lCurTime = (unsigned long)(ACE_OS::gettimeofday().sec()); 
		ACE_Time_Value tv2(lCurTime);

		ACE_Date_Time tvTime(tv2);

		char buf[256];

		switch(nType)
		{
		case TIME_TIME:
			sprintf(buf,"[%.2d:%.2d:%.2d]",tvTime.hour(),tvTime.minute(),tvTime.second());
			break;
		case TIME_DATE:
			sprintf(buf,"[%.4d:%.2d:%.2d]",tvTime.year(),tvTime.month(),tvTime.day());
			break;
		case TIME_FULL:
			sprintf(buf,"[%d:%d:%d %d:%d:%d:%d]",tvTime.year(),tvTime.month(),tvTime.day(),tvTime.hour(),tvTime.minute(),tvTime.second(),tvTime.microsec());
			break;
		}


		return buf;
	}

	bool IsEqualIgnoreCase(std::string lhs,std::string rhs)
	{
#ifdef WIN32
		return  (_stricmp(lhs.c_str(),rhs.c_str()) == 0 ) ?true:false;//0:相等；!0:不等
#else
		return (strcmpIgnoreCase(lhs.c_str(),rhs.c_str()) == 0 )?true:false;
#endif
	}

	int HexToInt(char* strHex)
	{
		//十六进制字符串转换为10进制数
		char *str;     
		int nValue = (int)strtol(strHex, &str, 16);//十六进制  
		return nValue;
	}

	unsigned long GetTickSecond(void)
	{
		unsigned long lCurTime = (unsigned long)(ACE_OS::gettimeofday().sec()); 
		ACE_Time_Value tv2(lCurTime);
		return tv2.sec();

		/*
		#ifdef WIN32
		#define OS_WINDOWS WIN32


		#include <windows.h>
		#endif
		#ifdef LINUX
		#include <unistd.h>
		#include <sys/time.h>
		#include <netinet/in.h>
		#endif
		#ifdef VXWORKS
		#include "vxworks.h"
		#include <tickLib.h>
		#include <sysLib.h>
		#endif

		*/
		/*
		ULONGA currentTime;
		#ifdef WIN32
		currentTime = GetTickCount();
		#endif
		#ifdef LINUX
		struct timeval current;
		gettimeofday(&current, NULL);
		currentTime = current.tv_sec * 1000 + current.tv_usec/1000;
		#endif
		#ifdef OS_VXWORKS
		ULONGA timeSecond = tickGet() / sysClkRateGet();
		ULONGA timeMilsec = tickGet() % sysClkRateGet() * 1000 / sysClkRateGet();
		currentTime = timeSecond * 1000 + timeMilsec;
		#endif
		return currentTime;
		*/
	}

	void USleep(int nMillisecond)
	{
		int nSecond = nMillisecond/1000;
		int nMilcond = nMillisecond%1000;

		ACE_OS::sleep(ACE_Time_Value(nSecond, nMilcond*1000));
	}

	int GetProcessID()
	{
		int iPid = (int)getpid();

		return iPid;
	}

	std::string GetProcessName()
	{
#ifdef WIN32
		char file_name[MAX_PATH];

		::GetModuleFileName(NULL, file_name, sizeof(file_name));

		std::string strPathName(file_name);
		int nPos = strPathName.find_last_of('\\');

		int nLength = strPathName.length();

		if(nPos>=nLength-1)
			return "";

		std::string strFileName = strPathName.substr(nPos+1,strPathName.length()-nPos-1);
		return strFileName;
#else
		return string("Not implemented!");
#endif
	}

	int GetThreadID()
	{
#ifdef WIN32
		return GetCurrentThreadId();
#else

#endif
	}

	// BKDR Hash 
	unsigned int BKDRHash(char *str)
	{
		unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
		unsigned int hash = 0;

		while (*str)
		{
			hash = hash * seed + (*str++);
		}

		return (hash & 0x7FFFFFFF);
	}

	int GetFileSize(char* filename)
	{
		FILE *fp;
		if((fp=fopen(filename,"r"))==NULL)
			return 0;
		fseek(fp,0,SEEK_END);
		int nsize = ftell(fp);    //return NULL;
		fclose(fp);

		return nsize;
	}

	void Log2File(char* filename,char* log,bool IsAppend,int nMaxSizeInMB)
	{
		FILE* fp = NULL;
		if(IsAppend)
		{
			int nSize = GetFileSize(filename);
			if(nSize > nMaxSizeInMB*1024*1024)
				fp = fopen(filename, "w");
			else
				fp=fopen(filename,"at+");
		}
		else
		{
			fp = fopen(filename, "w");
		}

		if(fp==NULL)
			return;

		std::string strMsg(log);
		strMsg += "\n";
		fwrite(strMsg.c_str(),strMsg.length(),1,fp);
		fclose(fp);
	}


	float CharBitsToFloat(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4)
	{
		int bits = (b1 << 24) + (b2 << 16) + (b3 << 8) + b4;
		/* s 为符号（sign）；e 为指数（exponent）；m 为有效位数（mantissa）*/
		int
			s = (bits >> 31) == 0 ? 1 : -1,
			e = (bits >> 23) & 0xff,
			m = (e == 0) ?
			(bits & 0x7fffff) << 1 :
			(bits & 0x7fffff) | 0x800000;
		return s * m * (float)pow(2.0, e - 150);
	}

}//end namespace UTILS
