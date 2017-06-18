

#ifndef _MISC_H
#define _MISC_H

#include <string>

namespace Utils
{
	//��������ʽ�����
	std::string FormatBuffer(char* pBuff,int nMsgLen);

	//ʱ���ʽ�����
	enum
	{
		TIME_FULL,//������ʱ����
		TIME_DATE,//������
		TIME_TIME,//ʱ����
	};
	std::string FormatTimestamp(int nType=TIME_TIME);

	//�ַ���ƥ��(�����ִ�Сд)
	bool IsEqualIgnoreCase(std::string lhs,std::string rhs);

	//ʮ�������ַ���תΪ����
	int HexToInt(char* strHex);

	//��ȡ��ǰʱ��(��)
	unsigned long GetTickSecond(void);

	//ACE sleep ��װ
	void USleep(int nMillisecond);

	//��ȡ��ǰ����ID
	int GetProcessID();

	//��ȡ��ǰ������
	std::string GetProcessName();

	//��ȡ��ǰ�߳�ID
	int GetThreadID();

	//BKDRHash
	unsigned int BKDRHash(char *str);

	//��ȡ�ļ���С
	int GetFileSize(char* filename);

	//д��־���ı��ļ�
	void Log2File(char* filename,char* log,bool IsAppend,int nMaxSizeInMB=30);

	//�������ĸ��ֽ�ת��Ϊfloatֵ
	float CharBitsToFloat(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4);
	//EG:float data = CharBitsToFloat( int(pDataBuf[4*i+2]), int(pDataBuf[4*i+3]), int(pDataBuf[4*i]), int(pDataBuf[4*i+1]) ) ;
}

#endif // _MISC_H

