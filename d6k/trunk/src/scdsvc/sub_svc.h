/*! @file sub_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  sub_svc.h
�ļ�ʵ�ֹ��� :  ���ķ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  ���ķ���ά�������嵥���������嵥��ʱɨ�����ݿ⣬���ɶ��ı��ģ�Ȼ��
                ���������߷���ʱ���͸ñ��ģ����ǵ�ʵʱ���ⱨ�Ĳ�������ʽ���档
				���ң����Բ��ö��ģ�ң�ž�����Ҫ���ö���ģʽ��
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ���ķ���
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.03
*******************************************************************************/
#ifndef SUB_SVC_MODULE_H
#define SUB_SVC_MODULE_H

#include "base_module.h"

#include <memory>
  
class CSubScriptionSvc : public CBaseModule
{
public:
	CSubScriptionSvc(CScadaSvc* pServer, const std::string & szMailBoxName, int &MailID);
	virtual ~CSubScriptionSvc(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();
private: 
};

#endif // SUB_SVC_MODULE_H


/** @}*/

