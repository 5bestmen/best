/*! @file his_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  his_svc.h
�ļ�ʵ�ֹ��� :  ��ʷ�洢���񣬶�ʱ�洢ʵʱ���ݵ�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��ʷ�洢����
*  @author  LiJin
*  @version 1.0
*  @date    2017.03.24
*******************************************************************************/
#ifndef HIS_SVC_H
#define HIS_SVC_H

#include "base_module.h"

class CHisSvc : public CBaseModule
{
public:
	CHisSvc();
	virtual ~CHisSvc();
public:
	virtual bool Initialize();
	virtual void Run();
	virtual void Shutdown();
protected:
	virtual void MainLoop();

};



 
 


#endif // HIS_SVC_H



/** @}*/

