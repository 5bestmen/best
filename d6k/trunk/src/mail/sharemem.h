/*! @file sharemem.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  sharemem.h
�ļ�ʵ�ֹ��� :  �����ڴ�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief     �����ڴ�
*  @author    LiJin
*  @version   1.0
*  @date      2016.04.24 
*******************************************************************************/
#ifndef SHARE_MEM_H
#define SHARE_MEM_H
 
#include <memory>
  
class QSharedMemory;

/*! \class CShareMem  sharemem.h  "sharemem.h"
*  \brief �����ڴ�.
*/
class CShareMem
{
public:
	CShareMem();
	~CShareMem();
public:
	// ���������ڴ�
	void *CreateShareMem(const char *szMemName,int nSize);
	// ��ȡ
	void *GetShareMem(const char * szMemName);

	bool Lock();
	bool UnLock();
private:
	std::shared_ptr<QSharedMemory> m_pShm;
};
 
 

#endif // SHARE_MEM_H

/** @}*/

