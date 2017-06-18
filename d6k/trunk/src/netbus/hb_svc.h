/*! @file hb_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  hb_svc.h
�ļ�ʵ�ֹ��� :  UDP�㲥���ͣ����գ�����״̬�ж�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   UDP�㲥���ͣ����գ�����״̬�ж�
*  @author  LiJin
*  @version 1.0
*  @date    2016.10.24
*******************************************************************************/
#ifndef HB_SVC_MODULE_H
#define HB_SVC_MODULE_H
 
#include "base_module.h"

#include <string>
#include "datatypes.h"

class ACE_SOCK_Dgram_Bcast;
 
class CHeartBeatSvc : public CBaseModule
{
public: 
	CHeartBeatSvc(CNetbusSvc* pServe);
	~CHeartBeatSvc();
protected:
	CHeartBeatSvc( );

	enum :unsigned short
	{
		HB_PORT = 18764,   //!< �㲥�˿�

	};

public:
	virtual bool Initialize(const char *pszDataPath, int32u nNodeType, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown(); 
protected:
	void SendBroadMsg();

	void RecvBroadMsg();
 
protected:
	// �㲥
	std::shared_ptr<ACE_SOCK_Dgram_Bcast> m_pDgramBcast;
 
};

#endif // HB_SVC_MODULE_H


/** @}*/

