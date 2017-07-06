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
		//int Baud;				//������
		//QString Com;			//COM
		//QString DeviceAddr;		//�豸��ַ
		//QString IP;
		//int32s Port;
		QString DriverName;		//��������
		QString m_strDriverParam;		//��������

		int Medium;				//ͨѶý��
								//0 - ��̫��
								//1 - ����
								//2 - �ֳ�����
								//3 - ����

		int DriveType;			//��������
								//0 - ������
								//1 - ������
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
		// ����������ֽ���У����
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
