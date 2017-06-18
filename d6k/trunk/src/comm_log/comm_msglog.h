
#ifndef COMM_MSG_LOG_H
#define COMM_MSG_LOG_H
 
#include <string>
#include <QDateTime>
#include <vector>

#include "public.h"
 
 

class CCommMsgLog
{
public:
	CCommMsgLog()
	{
		m_nIndex = 0;

	 	m_nThreadId = 0;
		m_nProcessId = 0; 
	}
	
	CCommMsgLog(struct COMM_MSG *pLog);
	CCommMsgLog(CCommMsgLog *pCommLog);

	CCommMsgLog& operator= (const CCommMsgLog &);

	~CCommMsgLog()
	{

	}
public:
	// ���л�
	size_t SaveToBuffer(unsigned char *pInBuff,size_t nInBuffLen)const;
	bool  LoadFromBuffer(const char *pInBuff, size_t nInBuffLen);

// 	void SaveToLogData(LOG_DATA  *pData) const;
// 	bool LoadFromLogData(const LOG_DATA *pData);

	// ���ݳ��ȣ�����ͬ��ͷ�����ȵ���Ϣ��
	size_t GetMsgSize() const
	{
		size_t nSyncHeadLen = sizeof(size_t);
		size_t nNextLen = sizeof(size_t);
		
		return GetDataSize() + nSyncHeadLen + nNextLen;
	}
private:
	// ��Ч���ݳ��ȣ�������ͬ��ͷ�����ȵ���Ϣ��
	size_t GetDataSize() const;

public:
	size_t m_nIndex;

	size_t   m_nThreadId;
	size_t   m_nProcessId;

	unsigned int m_nMsgType;       //! ������Ϣ������ ���磺���ڷ��ͣ�UDP���գ�TCP����...
	std::string  m_szMsgSubType;   //! ������ �ַ�������������˵����������֡���ٻ�������RECV��SEND����ʹ��������ʹ��

	QDateTime m_tmLog;

	std::string m_szModuleName;     //! ģ������
	std::string m_szProtocalName;   //! Э�����ƣ�����Ϊ��
	std::string m_szDescription;    //! ����ⱨ�ĵ�����������Ϊ��


	std::string m_szDest;   //! �������̫��������Ŀ�����IP:�˿ںţ�����Ǵ��ڣ�������Ĵ��ں�
	std::string m_szSrc;    //! �������̫�����������IP:�˿ںţ�����Ǵ��ڣ�������Ĵ��ں�

	std::vector<unsigned char> m_arrContent; //! ��������
};
 
 

#endif // COMM_MSG_LOG_H

