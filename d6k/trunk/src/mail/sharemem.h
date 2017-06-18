/*! @file sharemem.h
<PRE>
********************************************************************************
模块名       :
文件名       :  sharemem.h
文件实现功能 :  共享内存
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief     共享内存
*  @author    LiJin
*  @version   1.0
*  @date      2016.04.24 
*******************************************************************************/
#ifndef SHARE_MEM_H
#define SHARE_MEM_H
 
#include <memory>
  
class QSharedMemory;

/*! \class CShareMem  sharemem.h  "sharemem.h"
*  \brief 共享内存.
*/
class CShareMem
{
public:
	CShareMem();
	~CShareMem();
public:
	// 创建共享内存
	void *CreateShareMem(const char *szMemName,int nSize);
	// 获取
	void *GetShareMem(const char * szMemName);

	bool Lock();
	bool UnLock();
private:
	std::shared_ptr<QSharedMemory> m_pShm;
};
 
 

#endif // SHARE_MEM_H

/** @}*/

