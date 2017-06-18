/*! @file mailimpl.h
<PRE>
********************************************************************************
ģ����       :  �ʼ�ģ��
�ļ���       :  mailimpl.h
�ļ�ʵ�ֹ��� :  �ʼ�ģ��ӿڶ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �ʼ�ģ��ӿڶ���
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.15
*******************************************************************************/


#ifndef _MAIL_GRP_H
#define _MAIL_GRP_H

#include <memory> 
#include <set>
#include <unordered_map> 
#include "queue_impl.h"
#include "maildefine.h"

#include <boost/interprocess/sync/named_mutex.hpp>

/*! \class  CMailBoxGrp
*   \brief ������ FES\SCADA */
class CMailBoxGrp
{
public:
	CMailBoxGrp();
	~CMailBoxGrp();

	// ��ʼ��ʱ����ϵͳ�ж��ٸ�������Ҫ������ȷ����̬�����ĳߴ�
	explicit CMailBoxGrp(const char * szPredicate);
public:
	std::string & GetName()
	{
		return m_szPrediateName;
	}
	// ���������Ҫ��Ϊ����ģ�齨������
	bool Create(int nMailBoxID);
	bool Create(int nMailBoxID,size_t nNum, size_t nSize);
	bool Destroy(int nMailBoxID); 

	// ��ģ���
	bool Open(int nMailBoxID);
	bool Close(int nMailBoxID);

	bool SendMail(DMSG *pMsg, unsigned int nWaitTime = 10);
	bool RecvMail(int nRecvMailBoxID, unsigned char* pMail, size_t nBufLen, size_t&nRecvdLen,unsigned int nWaitTime = 10);
	bool RecvMail(DMSG *pMsg, unsigned int nWaitTime = 10);
	bool TrySendMail(int nRecvMailBoxID, const DMSG* pMail);
protected:
	bool SendMail(int nRecvMailBoxID, const DMSG* pMail);
private:
	std::string m_szPrediateName;
	// id ����
	std::unordered_map< int, std::shared_ptr<CQueue> > m_mapMailQueue;

	std::shared_ptr<boost::interprocess::named_mutex>  m_pQueMux;
};

 

#endif // _MAIL_GRP_H
/** @}*/
