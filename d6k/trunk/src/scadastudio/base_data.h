#ifndef BASE_CONFIG_DATA_H
#define BASE_CONFIG_DATA_H

#include "datatypes.h"

#include <vector>
#include <qstring.h>

#define  STRING_PAD  sizeof(size_t)

namespace Config
{
	struct  COMM_PARA
	{
		//int Baud;				//波特率
		//QString Com;			//COM
		//QString DeviceAddr;		//设备地址
		//QString IP;
		//int32s Port;
		QString DriverName;		//驱动名称
		QString m_strDriverParam;		//驱动参数

		int Medium;				//通讯媒介
								//0 - 以太网
								//1 - 串口
								//2 - 现场总线
								//3 - 其他

		int DriveType;			//驱动类型
								//0 - 进程内
								//1 - 进程外
	};

	class CBaseData
	{
	public:
		CBaseData();
		virtual ~CBaseData();
	public:
		virtual void Save() = 0;

		virtual void SetModifyFlag(bool bFlag) = 0;

		virtual bool GetModifyFlag() const = 0;
		// 对输入的名字进行校验检查
		bool CheckTagName(std::string & szTagName) const;

		virtual int32u GetOccNo() const = 0;

		bool LogToFile(const char* pFilename, const char* pLog);
	public:

	protected:
		bool LogString(char *pszMoudleName, char *szLogTxt, int nLevel);
	protected:
		bool m_bModifyFlag;
	};
}

#endif
