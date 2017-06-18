

#ifndef _MISC_H
#define _MISC_H

#include <string>

namespace Utils
{
	//缓冲区格式化输出
	std::string FormatBuffer(char* pBuff,int nMsgLen);

	//时间格式化输出
	enum
	{
		TIME_FULL,//年月日时分秒
		TIME_DATE,//年月日
		TIME_TIME,//时分秒
	};
	std::string FormatTimestamp(int nType=TIME_TIME);

	//字符串匹配(不区分大小写)
	bool IsEqualIgnoreCase(std::string lhs,std::string rhs);

	//十六进制字符串转为整型
	int HexToInt(char* strHex);

	//获取当前时间(秒)
	unsigned long GetTickSecond(void);

	//ACE sleep 封装
	void USleep(int nMillisecond);

	//获取当前进程ID
	int GetProcessID();

	//获取当前进程名
	std::string GetProcessName();

	//获取当前线程ID
	int GetThreadID();

	//BKDRHash
	unsigned int BKDRHash(char *str);

	//获取文件大小
	int GetFileSize(char* filename);

	//写日志至文本文件
	void Log2File(char* filename,char* log,bool IsAppend,int nMaxSizeInMB=30);

	//缓冲区四个字节转换为float值
	float CharBitsToFloat(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4);
	//EG:float data = CharBitsToFloat( int(pDataBuf[4*i+2]), int(pDataBuf[4*i+3]), int(pDataBuf[4*i]), int(pDataBuf[4*i+1]) ) ;
}

#endif // _MISC_H

